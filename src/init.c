/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stmaire <stmaire@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/30 12:27:22 by stephanie         #+#    #+#             */
/*   Updated: 2026/05/05 15:33:33 by stmaire          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "codexion.h"

int init_data(t_data *data)
{
	if (pthread_mutex_init(&data->print_mutex, NULL) != 0)
		return(print_error("Failed to init print_mutex"));
	if (pthread_mutex_init(&data->simulation_over_mutex, NULL) != 0)
		return(print_error("Failed to init simulation_over_mutex"));
	if(!init_data_dongles(data))
		return(print_error("Failed to init dongles"));
	if(!init_data_coders(data))
		return(print_error("Failed to init coders"));
	return (1);
}

static int init_data_dongles(t_data *data)
{
	size_t	i;

	i = 0;
	data->dongles = malloc(sizeof(t_dongle) * data->parsed_args.number_of_coders);
	if(!(data->dongles))
		return(print_error("Malloc failed for simulation data (dongles)"));

	while (i < data->parsed_args.number_of_coders)
	{
		data->dongles[i].id = i;
		data->dongles[i].is_available = 1;
		if (pthread_mutex_init(&data->dongles[i].dongle_mutex, NULL) != 0
			|| pthread_cond_init(&data->dongles[i].dongle_cond, NULL) != 0)
			return(print_error("Failed to init a dongle mutex or condition"));
		i++;
	}
	return (1);
}
static int init_data_coders(t_data *data)
{
	size_t		i;

	i = 0;
	data->coders = malloc(sizeof(t_coder) * data->parsed_args.number_of_coders);
	if (!(data->coders))
		return(print_error("Malloc failed for simulation data (coders)"));
	while (i < data->parsed_args.number_of_coders)
	{
		data->coders[i].id = i + 1;
		data->coders[i].data = data;
		data->coders[i].last_compilation_time = 0;
		data->coders[i].achieved_compilations_nb = 0;
		data->coders[i].left_dongle = &data->dongles[i];
		data->coders[i].right_dongle = &data->dongles[(i + 1) % data->parsed_args.number_of_coders];
		i++;
	}
	return (1);
}

