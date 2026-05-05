/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stmaire <stmaire@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/30 16:16:13 by stephanie         #+#    #+#             */
/*   Updated: 2026/05/05 09:18:57 by stmaire          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void    *monitor_routine(void *arg)
{
	(void)arg;
	return (NULL);
	//doit tourner en permanence, mais sans consommer 100% du CPU (petit usleep).
}
int     is_burn_out(t_data *data)
{
	(void)data;
	return (0);
	// Vérifie le flag sécurisé
}
int     check_all_alive(t_data *data)
{

	(void)data;
	return (0);
	// Le monitor vérifie chaque last_compilation_time
}
