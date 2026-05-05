/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stmaire <stmaire@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/30 12:35:06 by stephanie         #+#    #+#             */
/*   Updated: 2026/05/05 15:20:41 by stmaire          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int free_everything_and_return(t_data *data)
{
	size_t	i;

	if (!data)
		return(0);
	if(data->dongles)
	{
		i = 0;
		while (i < data->parsed_args.number_of_coders)
		{
			pthread_cond_destroy(&data->dongles[i].dongle_cond);
			pthread_mutex_destroy(&data->dongles[i].dongle_mutex);
			i++;
		}
		free(data->dongles);
		data->dongles = NULL;
	}
		if(data->coders)
	{
		free(data->coders);
		data->coders = NULL;
	}
	pthread_mutex_destroy(&data->print_mutex);
	pthread_mutex_destroy(&data->simulation_over_mutex);
	return (0);
}

void cleanup_simulation(t_data *data)
{
	size_t	i;

	if (!data)
		return;
	if (data->thread_monitoring)
		pthread_join(data->thread_monitoring, NULL);
	if (data->coders)
	{
		i = 0;
		while (i < data->parsed_args.number_of_coders)
		{
			if (data->coders[i].thread_id)
				pthread_join(data->coders[i].thread_id, NULL);
			i++;
		}
	}
}
