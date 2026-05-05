/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stmaire <stmaire@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/30 14:00:17 by stephanie         #+#    #+#             */
/*   Updated: 2026/05/05 09:17:16 by stmaire          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "codexion.h"

void *coder_routine(void *arg)
{
	(void)arg;
	return (NULL);
	// t_coder *coder = (t_coder *)arg;
	// while (1) // tant que les fonctions de travail renvoient 1
	// {
	// 	//le coder prend les cles
	// 	// il travaille
	// 	// compil_done += 1
	// 	//il repose les cles
	// 	//il refacto/debug
	// 	// 1. Vérifier avant de commencer
	// 	// if (is_simulation_over(coder->data))
	// 	// 	return (0);

	// 	// // 2. Tenter de prendre les dongles (logique du scheduler)
	// 	// // Si on doit attendre :
	// 	// while (dongles_not_available)
	// 	// {
	// 	// 	// On vérifie la mort/fin de travail pendant l'attente
	// 	// 	if (is_simulation_over(coder->data))
	// 	// 		return (0);

	// 	// 	// Attente passive ou active...
	// 	// }

    // return (1); // On a les dongles et la simu tourne encore
	// }
	// return (NULL);
}

int take_available_dongles(t_coder *coder)
{
	(void)coder;
	return (0);
	//return 1 si erreur = si la simulation s 'arrete
	//pendant que le codeur attend les clés
	//!!!attention blocage si chaque coder prend la clé de droite,
	//car la table est circulaire et chacun en a une en main et attend
	// si le coder est obligé de prendre en 1er la clé avec la + petit ID, pas de blocage
	// cf...dessin
}
void compilation_work(t_coder *coder)
{
	(void)coder;
	//return 1 si erreur
	// ... fait le travail ...
    // if (is_simulation_over(coder->data)) =
	// si burnout déclaré pendant la compilation
    //     return (0); //  = Stop
    // return (1); //
}
void put_dongles_away(t_coder *coder)
{
	(void)coder;
}
int complete_tasks(t_coder *coder)
{
	(void)coder;
	return(0);
	//debug and refacto
}
