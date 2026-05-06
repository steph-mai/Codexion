/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stmaire <stmaire@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/06 11:43:35 by stmaire           #+#    #+#             */
/*   Updated: 2026/05/06 12:00:31 by stmaire          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "codexion.h"

void	*coder_routine(void *arg)
{
	t_coder *coder = (t_coder *)arg;

	if (coder->id % 2 == 0)
		usleep(15000);
	while (get_simulation_status(coder->data))
	{
		if(!take_available_dongles(coder))
			break;
		compilation_work(coder);
		put_dongles_away(coder);
		if(!complete_tasks(coder))
			break;
		smart_sleep(1, coder->data);
	}
	return (NULL);
}
