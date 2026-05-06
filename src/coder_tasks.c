/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_tasks.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stephanie <stephanie@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/06 11:45:09 by stmaire           #+#    #+#             */
/*   Updated: 2026/05/06 17:22:48 by stephanie        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "codexion.h"

int take_available_dongles(t_coder *coder)
{
    t_dongle *first;
    t_dongle *second;

    if (coder->left_dongle->id < coder->right_dongle->id)
    {
        first = coder->left_dongle;
        second = coder->right_dongle;
    }
    else
    {
        first = coder->right_dongle;
        second = coder->left_dongle;
    }
	if (!lock_mutex_and_take_dongles(coder, first, second))
    	return (0);
	return (1);
}

int lock_mutex_and_take_dongles(t_coder *coder, t_dongle *first, t_dongle *second)
{
    // Prendre le premier dongle (gère l'attente et le cooldown)
    if (!secure_wait_for_dongle(coder, first))
        return (0);
    print_status(coder, "has taken a dongle");

    // Cas spécifique du codeur seul (évite le double lock sur le même mutex)
    if (first == second)
    {
        smart_sleep(coder->data->parsed_args.time_to_burnout, coder->data);
        pthread_mutex_unlock(&first->dongle_mutex);
        return (0);
    }

    // Prendre le deuxième dongle
    if (!secure_wait_for_dongle(coder, second))
    {
        first->is_available = 1;
        pthread_cond_broadcast(&first->dongle_cond);
        pthread_mutex_unlock(&first->dongle_mutex);
        return (0);
    }
    print_status(coder, "has taken a dongle");
    return (1);
}

int secure_wait_for_dongle(t_coder *coder, t_dongle *dongle)
{
    struct timespec ts;
    long long       target_ms;

    pthread_mutex_lock(&dongle->dongle_mutex);
    // TANT QUE le dongle est utilisé OU que le cooldown n'est pas fini
    while (!dongle->is_available || get_timestamp(coder->data) < dongle->available_at)
    {
        if (!get_simulation_status(coder->data))
        {
            pthread_mutex_unlock(&dongle->dongle_mutex);
            return (0);
        }
        if (!dongle->is_available)
        {
            // Cas 1 : Le dongle est occupé par un voisin -> Attente simple
            pthread_cond_wait(&dongle->dongle_cond, &dongle->dongle_mutex);
        }
        else
        {
            // Cas 2 : Le dongle est libre mais encore trop chaud -> Attente chronométrée
            target_ms = coder->data->start_time + dongle->available_at;
            ts.tv_sec = target_ms / 1000;
            ts.tv_nsec = (target_ms % 1000) * 1000000;
            // On s'endort jusqu'à l'heure cible (le mutex est relâché pendant ce temps)
            pthread_cond_timedwait(&dongle->dongle_cond, &dongle->dongle_mutex, &ts);
        }
    }
    // Sortie de boucle : On a le mutex, il est libre et il est froid !
    dongle->is_available = 0;
    // On garde le mutex verrouillé pour que personne d'autre ne le prenne
    return (1);
}

// int secure_wait_for_dongle(t_coder *coder, t_dongle *dongle)
// {
//     struct timespec ts;
//     long long       target_ms;

//     pthread_mutex_lock(&dongle->dongle_mutex);
//     while (1)
//     {
//         // 1. Condition de sortie (Mort ou Fin de simulation)
//         if (!get_simulation_status(coder->data))
//         {
//             pthread_mutex_unlock(&dongle->dongle_mutex);
//             return (0);
//         }

//         // 2. Gestion de la disponibilité physique (Occupé ou trop chaud)
//         if (!dongle->is_available || get_timestamp(coder->data) < dongle->available_at)
//         {
//             if (!dongle->is_available)
//                 pthread_cond_wait(&dongle->dongle_cond, &dongle->dongle_mutex);
//             else
//             {
//                 target_ms = coder->data->start_time + dongle->available_at;
//                 ts.tv_sec = target_ms / 1000;
//                 ts.tv_nsec = (target_ms % 1000) * 1000000;
//                 pthread_cond_timedwait(&dongle->dongle_cond, &dongle->dongle_mutex, &ts);
//             }
//             continue; // Une fois réveillé, on réévalue tout depuis le début
//         }

//         // 3. --- LOGIQUE DE PRIORITÉ (PSEUDO-CODE) ---
//         // Le dongle est libre et froid, mais ai-je le droit de le prendre ?

//         if (coder->data->parsed_args.scheduling_type == EDF)
//         {
//             /*
//             if (un_voisin_est_plus_proche_du_burnout_que_moi)
//             {
//                 // On relâche la priorité et on attend un signal
//                 pthread_cond_wait(&dongle->dongle_cond, &dongle->dongle_mutex);
//                 continue;
//             }
//             */
//         }
//         else if (coder->data->parsed_args.scheduling_type == FIFO)
//         {
//             /*
//             if (un_voisin_attend_ce_dongle_depuis_plus_longtemps_que_moi)
//             {
//                 pthread_cond_wait(&dongle->dongle_cond, &dongle->dongle_mutex);
//                 continue;
//             }
//             */
//         }

//         // 4. Si on arrive ici, on a passé tous les filtres !
//         break;
//     }

//     dongle->is_available = 0;
//     return (1);
// }

void compilation_work(t_coder *coder)
{
	print_status(coder, "is compiling");
	pthread_mutex_lock(&coder->data->simulation_over_mutex);
	coder->last_compilation_time = get_time_ms();
	coder->achieved_compilations_nb++;
	pthread_mutex_unlock(&coder->data->simulation_over_mutex);
	smart_sleep(coder->data->parsed_args.time_to_compile, coder->data);
}

void put_dongles_away(t_coder *coder)
{
    long long current_time;

    current_time = get_timestamp(coder->data);

    // On traite les deux dongles.
    // Ils sont déjà verrouillés depuis take_available_dongles.

    // Dongle Gauche
    coder->left_dongle->available_at = current_time + coder->data->parsed_args.dongle_cooldown;
    coder->left_dongle->is_available = 1;

    // Dongle Droit
    coder->right_dongle->available_at = current_time + coder->data->parsed_args.dongle_cooldown;
    coder->right_dongle->is_available = 1;

    // Optionnel : Un seul printf protégé pour les deux releases si tu veux gagner en performance
    pthread_mutex_lock(&coder->data->print_mutex);
    if (get_simulation_status(coder->data))
    {
        printf("coder %zu dongle %zu ready at %lld\n", coder->id, coder->left_dongle->id, coder->left_dongle->available_at);
        printf("coder %zu dongle %zu ready at %lld\n", coder->id, coder->right_dongle->id, coder->right_dongle->available_at);
    }
    pthread_mutex_unlock(&coder->data->print_mutex);

    // Signal et Unlock
    pthread_cond_broadcast(&coder->left_dongle->dongle_cond);
    pthread_mutex_unlock(&coder->left_dongle->dongle_mutex);

    pthread_cond_broadcast(&coder->right_dongle->dongle_cond);
    pthread_mutex_unlock(&coder->right_dongle->dongle_mutex);
}

int complete_tasks(t_coder *coder)
{
	print_status(coder, "is debugging");
	smart_sleep(coder->data->parsed_args.time_to_debug, coder->data);
	print_status(coder, "is refactoring");
	smart_sleep(coder->data->parsed_args.time_to_refactor, coder->data);
	return (get_simulation_status(coder->data));
}

