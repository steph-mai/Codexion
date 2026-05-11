/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stmaire <stmaire@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/30 12:27:22 by stephanie         #+#    #+#             */
/*   Updated: 2026/05/11 14:57:48 by stmaire          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	init_wait_queue(t_data *data, size_t i)
{
	data->dongles[i].wait_queue.node = malloc(sizeof(t_node)
			* data->parsed_args.number_of_coders);
	if (!(data->dongles[i].wait_queue.node))
	{
		cleanup_initialized_dongles(data, i);
		return (print_error("Malloc failed for wait queue"));
	}
	memset(data->dongles[i].wait_queue.node,
		0, sizeof(t_node) * data->parsed_args.number_of_coders);
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
		data->dongles[i].is_available = 1;
		data->dongles[i].is_unused = 1;
		if (!init_wait_queue(data, i))
			return (0);
		if (pthread_mutex_init(&data->dongles[i].dongle_mutex, NULL) != 0
			|| pthread_cond_init(&data->dongles[i].dongle_cond, NULL) != 0)
		{
			cleanup_initialized_dongles(data, i + 1);
			return (0);
		}
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
		return (free_everything_and_return(data));
	if (!init_data_dongles(data))
		return (free_everything_and_return(data));
	if (!init_data_coders(data))
		return (free_everything_and_return(data));
	return (1);
}
