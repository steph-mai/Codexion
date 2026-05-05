/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stmaire <stmaire@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/30 15:20:20 by stephanie         #+#    #+#             */
/*   Updated: 2026/05/05 16:13:33 by stmaire          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	start_simulation(t_data *data)
{
	size_t	i;

	i = 0;
	data->start_time = get_time_ms();
	data->is_simulation_running = 1;
	while (i < data->parsed_args.number_of_coders)
	{
		data->coders[i].last_compilation_time = data->start_time;
		if (pthread_create(&data->coders[i].thread_id,
				NULL, coder_routine, &data->coders[i]) != 0)
		{
			set_simulation_stop(data);
			return (print_error("Failed to create coders threads."));
		}
		i++;
	}
	if (pthread_create(&data->thread_monitoring,
			NULL, monitor_routine, data) != 0)
	{
		set_simulation_stop(data);
		return (print_error("Failed to create monitoring thread"));
	}
	return (1);
}

void	set_simulation_stop(t_data *data)
{
	pthread_mutex_lock(&data->simulation_over_mutex);
	if (data->is_simulation_running != 0)
		data->is_simulation_running = 0;
	pthread_mutex_unlock(&data->simulation_over_mutex);
}

int	get_simulation_status(t_data *data)
{
	int	running;

	pthread_mutex_lock(&data->simulation_over_mutex);
	running = data->is_simulation_running;
	pthread_mutex_unlock(&data->simulation_over_mutex);
	return (running);
}
