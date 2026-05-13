/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stephanie <stephanie@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/12 15:33:55 by stmaire           #+#    #+#             */
/*   Updated: 2026/05/13 16:19:21 by stephanie        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

#include "codexion.h"

static int	init_global_wait_queue(t_data *data)
{
	data->wait_queue.node = malloc(sizeof(t_node)
			* data->parsed_args.number_of_coders);
	if (!(data->wait_queue.node))
		return (print_error("Malloc failed for global wait queue"));
	memset(data->wait_queue.node, 0,
		sizeof(t_node) * data->parsed_args.number_of_coders);
	data->wait_queue.size = 0;
	if (pthread_mutex_init(&data->queue_mutex, NULL) != 0
		|| pthread_cond_init(&data->queue_cond, NULL) != 0)
		return (print_error("Mutex/Cond init failed for global queue"));
	return (1);
}

static int	init_data_dongles(t_data *data)
{
	size_t	i;

	data->dongles = malloc(sizeof(t_dongle)
			* data->parsed_args.number_of_coders);
	if (!(data->dongles))
		return (print_error("Malloc failed for dongles"));
	memset(data->dongles, 0, sizeof(t_dongle)
		* data->parsed_args.number_of_coders);
	i = 0;
	while (i < data->parsed_args.number_of_coders)
	{
		data->dongles[i].id = i + 1;
		data->dongles[i].is_unused = 1;
		data->dongles[i].available_at = 0;
		i++;
	}
	return (1);
}

static int	init_data_coders(t_data *data)
{
	size_t	i;

	data->coders = malloc(sizeof(t_coder) * data->parsed_args.number_of_coders);
	if (!(data->coders))
		return (print_error("Malloc failed for coders"));
	memset(data->coders, 0, sizeof(t_coder)
		* data->parsed_args.number_of_coders);
	i = 0;
	while (i < data->parsed_args.number_of_coders)
	{
		data->coders[i].id = i + 1;
		data->coders[i].data = data;
		data->coders[i].last_compilation_time = 0;
		data->coders[i].left_dongle = &data->dongles[i];
		data->coders[i].right_dongle
			= &data->dongles[(i + 1) % data->parsed_args.number_of_coders];
		i++;
	}
	return (1);
}

int	init_data(t_data *data)
{
	if (pthread_mutex_init(&data->print_mutex, NULL) != 0
		|| pthread_mutex_init(&data->simulation_mutex, NULL) != 0)
		return (0);
	if (!init_data_dongles(data))
		return (0);
	if (!init_data_coders(data))
		return (0);
	if (!init_global_wait_queue(data))
		return (0);
	return (1);
}
