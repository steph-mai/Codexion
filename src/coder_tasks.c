/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_tasks.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stephanie <stephanie@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/06 11:45:09 by stmaire           #+#    #+#             */
/*   Updated: 2026/05/07 17:47:07 by stephanie        ###   ########.fr       */
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
    if (!secure_wait_for_dongle(coder, first))
        return (0);
    print_status(coder, "has taken a dongle");
    if (first == second)
    {
        smart_sleep(coder->data->parsed_args.time_to_burnout, coder->data);
        pthread_mutex_unlock(&first->dongle_mutex);
        return (0);
    }
    if (!secure_wait_for_dongle(coder, second))
    {
        pthread_mutex_lock(&first->dongle_mutex);
		first->is_unused = 1;
		pthread_cond_broadcast(&first->dongle_cond);
		pthread_mutex_unlock(&first->dongle_mutex);
		return (0);
    }
    print_status(coder, "has taken a dongle");
    return (1);
}

int secure_wait_for_dongle(t_coder *coder, t_dongle *dongle)
{
    struct          timespec ts;
    long long       target_ms;
    t_node          new_in_wait_list;
    int             is_priority;

    new_in_wait_list.coder_id = coder->id;
    new_in_wait_list.arrival_time = get_time_ms();
    if (coder->data->parsed_args.scheduler == SCHEDULER_EDF)
        new_in_wait_list.priority_marker = coder->last_compilation_time + coder->data->parsed_args.time_to_burnout;
    else
        new_in_wait_list.priority_marker = new_in_wait_list.arrival_time;

    pthread_mutex_lock(&dongle->dongle_mutex);
    push_in_wait_queue(&dongle->wait_queue, new_in_wait_list);
    while (1)
    {
        if (!get_simulation_status(coder->data))
        {
            pthread_mutex_unlock(&dongle->dongle_mutex);
            remove_from_wait_queue(&dongle->wait_queue, coder->id);
            return (0);
        }
        is_priority = (get_first_in_wait_queue(&dongle->wait_queue) == coder->id);
        dongle->is_available = (dongle->is_unused && get_timestamp(coder->data) >= dongle->available_at);
        if (is_priority && dongle->is_available)
            break;
        if (is_priority && dongle->is_unused && !dongle->is_available)
        {
            target_ms = coder->data->start_time + dongle->available_at;
            ts.tv_sec = target_ms / 1000;
            ts.tv_nsec = (target_ms % 1000) * 1000000;
            pthread_cond_timedwait(&dongle->dongle_cond, &dongle->dongle_mutex, &ts);
        }
        else
        {
            pthread_cond_wait(&dongle->dongle_cond, &dongle->dongle_mutex);
        }
    }
    wait_queue_pop(&dongle->wait_queue);
    dongle->is_available = 0;
    dongle->is_unused = 0;
	pthread_mutex_unlock(&dongle->dongle_mutex);
    return (1);
}

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
    coder->left_dongle->available_at = current_time + coder->data->parsed_args.dongle_cooldown;
    coder->left_dongle->is_unused = 1;
    coder->right_dongle->available_at = current_time + coder->data->parsed_args.dongle_cooldown;
    coder->right_dongle->is_unused = 1;
    pthread_mutex_lock(&coder->data->print_mutex);
    if (get_simulation_status(coder->data))
    {
        printf("coder %zu dongle %zu ready at %lld\n", coder->id, coder->left_dongle->id, coder->left_dongle->available_at);
        printf("coder %zu dongle %zu ready at %lld\n", coder->id, coder->right_dongle->id, coder->right_dongle->available_at);
    }
    pthread_mutex_unlock(&coder->data->print_mutex);
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

