/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stephanie <stephanie@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/30 12:35:06 by stephanie         #+#    #+#             */
/*   Updated: 2026/04/30 15:27:38 by stephanie        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int free_everything_and_return(t_data *data)
{
	// fonction qui free tout
	// 	Détruire les mutex des dongles : Une boucle while pour faire pthread_mutex_destroy et pthread_cond_destroy sur chaque dongle.

	// Libérer le tableau de dongles : free(data->dongles).

	// Détruire les mutex globaux : pthread_mutex_destroy(&data->write_mutex), etc..

	// Mettre les pointeurs à NULL :  pour éviter les "double free".
	// interdit de détruire un mutex verrouillé!  verifier que
	// les threads sont terminés (avec pthread_join) ou que les mutex
	//sont deverouilles avant de nettoyer.
}
void cleanup_simulation(t_data *data)
{
	//pour s'assurer que le programme ne  sarrete pas tt qu'un thread est un train de travailler.
}
