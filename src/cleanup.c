/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stephanie <stephanie@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/30 12:35:06 by stephanie         #+#    #+#             */
/*   Updated: 2026/05/06 17:49:12 by stephanie        ###   ########.fr       */
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
			// if (data->dongles[i].wait_queue.heap)
            // {
            //     free(data->dongles[i].wait_queue.heap);
            //     data->dongles[i].wait_queue.heap = NULL;
            // }
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
	{
		pthread_join(data->thread_monitoring, NULL);
		data->thread_monitoring = 0; //En mettant l'ID à 0 après le join, on évite
		// de tenter de rejoindre deux fois le même thread (ce qui fait crasher certains systèmes).
	}
	if (data->coders)
	{
		i = 0;
		while (i < data->parsed_args.number_of_coders)
		{
			if (data->coders[i].thread_id)
			{
				pthread_join(data->coders[i].thread_id, NULL);
				data->coders[i].thread_id = 0;
			}
			i++;
		}
	}
}
