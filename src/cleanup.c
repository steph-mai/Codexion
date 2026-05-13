/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stephanie <stephanie@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/12 15:34:18 by stmaire           #+#    #+#             */
/*   Updated: 2026/05/13 16:20:26 by stephanie        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	cleanup_simulation(t_data *data)
{
	size_t	i;

	if (!data)
		return ;
	if (data->thread_monitoring)
	{
		pthread_join(data->thread_monitoring, NULL);
		data->thread_monitoring = 0;
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

int	free_everything_and_return(t_data *data)
{
	if (!data)
		return (0);
	if (data->dongles)
	{
		free(data->dongles);
		data->dongles = NULL;
	}
	if (data->coders)
	{
		free(data->coders);
		data->coders = NULL;
	}
	if (data->wait_queue.node)
	{
		free(data->wait_queue.node);
		data->wait_queue.node = NULL;
	}
	pthread_mutex_destroy(&data->print_mutex);
	pthread_mutex_destroy(&data->simulation_mutex);
	pthread_mutex_destroy(&data->queue_mutex);
	pthread_cond_destroy(&data->queue_cond);
	return (0);
}
