/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stmaire <stmaire@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/05 16:17:51 by stmaire           #+#    #+#             */
/*   Updated: 2026/05/05 18:05:58 by stmaire          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	checked_is_burned_out(t_data *data, t_coder *coder) // args à verifier
{
	long long	current_time;

	current_time = get_time_ms();
	if (current_time - coder-> last_compilation_time >= data->parsed_args.time_to_burnout)
	{
		set_simulation_stop(data);
		pthread_mutex_lock(&data->print_mutex);
		printf("%lld %zu burned out\n", current_time - data->start_time, coder->id);
		pthread_mutex_unlock(&data->print_mutex);
		return (1);
	}
	return (0);
}

static int	checked_is_all_finished(t_data *data)
{
	size_t	i;

	i = 0;
	while (i < data->parsed_args.number_of_coders)
	{
		if (data->coders[i].achieved_compilations_nb < data->parsed_args.number_of_compiles_required)
			return (0);
		i++;
	}
	set_simulation_stop(data);
	return (1);
}

void    *monitor_routine(void *arg)
{
	t_data	*data;
	size_t	i;

	data = (t_data *)arg;
	usleep(1000);
	while (get_simulation_status(data))
	{
		i = 0;
		while (i < data->parsed_args.number_of_coders)
		{
			if (checked_is_burned_out(data, &data->coders[i]))
				return (NULL);
			i++;
		}
		if (checked_is_all_finished(data))
			return (NULL);
		usleep(500);
	}
	return (NULL);
}
