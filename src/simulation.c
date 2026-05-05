/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stmaire <stmaire@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/30 15:20:20 by stephanie         #+#    #+#             */
/*   Updated: 2026/05/05 09:05:24 by stmaire          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	start_simulation(t_data *data)
{
	(void)data;
	return (1);
   //fn  initialise le chronomètre global (start_time),
   //active le flag de fonctionnement de la simulation,
   //procède à la création du thread de surveillance (Monitor)
   //et de l'ensemble des threads Codeurs.
   //vérifie le succès de chaque appel à pthread_create.
   //En cas d'échec de création d'un thread en cours de route,
   //doit signaler l'arrêt immédiat pour ne pas laisser de threads orphelins.
}

void set_simulation_stop(t_data *data)
{
	(void)data;
	//Son rôle est de modifier de manière sécurisée (sous protection de mutex)
	// le flag d'état de la simulation dans la structure globale
	//pour indiquer que celle-ci doit s'arrêter.
	//Elle est appelée soit par le Monitor (en cas de burnout ou de travail fini),
	//soit par la fonction de lancement si un thread ne parvient pas à démarrer.
	//Elle permet d'assurer que tous les threads lisant ce flag s'arrêteront de concert.
	//action :Écriture : Elle change la valeur de is_simulation_running de 1 à 0.
	// ecrite pat Le Monitor (si quelqu'un meurt) ou le Main (si un thread rate).
	//ecrite Une seule fois (le moment où tout bascule).
}

