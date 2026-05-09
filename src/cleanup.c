/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stephanie <stephanie@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/30 12:35:06 by stephanie         #+#    #+#             */
/*   Updated: 2026/05/09 15:56:26 by stephanie        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void cleanup_initialized_dongles(t_data *data, size_t count)
{
    size_t i;

    i = 0;
    if (!data->dongles)
        return;
    while (i < count)
    {
        // On ne free que si le pointeur n'est pas NULL
        if (data->dongles[i].wait_queue.node)
            free(data->dongles[i].wait_queue.node);
        pthread_mutex_destroy(&data->dongles[i].dongle_mutex);
        pthread_cond_destroy(&data->dongles[i].dongle_cond);
        i++;
    }
    free(data->dongles);
    data->dongles = NULL;
}

int free_everything_and_return(t_data *data)
{
	if (!data)
		return(0);
	cleanup_initialized_dongles(data, data->parsed_args.number_of_coders);
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

