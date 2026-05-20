/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stmaire <stmaire@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/06 11:43:35 by stmaire           #+#    #+#             */
/*   Updated: 2026/05/20 11:46:09 by stmaire          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	compilation_work(t_coder *coder)
{
	pthread_mutex_lock(&coder->data->simulation_mutex);
	coder->last_compilation_time = get_time_ms();
	pthread_mutex_unlock(&coder->data->simulation_mutex);
	print_status(coder, "is compiling");
	active_sleep(coder->data->parsed_args.time_to_compile, coder->data);
	pthread_mutex_lock(&coder->data->simulation_mutex);
	coder->achieved_compilations_nb++;
	pthread_mutex_unlock(&coder->data->simulation_mutex);
}

static int	complete_tasks(t_coder *coder)
{
	print_status(coder, "is debugging");
	active_sleep(coder->data->parsed_args.time_to_debug, coder->data);
	print_status(coder, "is refactoring");
	active_sleep(coder->data->parsed_args.time_to_refactor, coder->data);
	return (get_simulation_status(coder->data));
}

void	*coder_routine(void *arg)
{
	t_coder	*coder;

	coder = (t_coder *)arg;
	if (coder->data->parsed_args.number_of_coders == 1)
	{
		print_status(coder, "has taken a dongle");
		active_sleep(coder->data->parsed_args.time_to_burnout
			+ 10, coder->data);
		return (NULL);
	}
	if (coder->id % 2 == 0)
		usleep(1000);
	while (get_simulation_status(coder->data))
	{
		if (!identify_and_take_dongles(coder))
			break ;
		compilation_work(coder);
		put_dongles_away(coder);
		if (!complete_tasks(coder))
			break ;
		active_sleep(1, coder->data);
	}
	return (NULL);
}
