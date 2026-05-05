/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stmaire <stmaire@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/30 12:16:46 by stephanie         #+#    #+#             */
/*   Updated: 2026/05/05 15:25:23 by stmaire          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int main(int argc, char **argv)
{
	t_data data;

	memset(&data, 0, sizeof(t_data));
	if(!fill_args_structure(&data.parsed_args,argc, argv))
		return(1); //message d erreur à ajouter ?
	printf("parsing OK\n");

	if(!init_data(&data))
		return(free_everything_and_return(&data));

	printf("Initialization data OK\n");

	if (!start_simulation(&data))
	{
		set_simulation_stop(&data);
        cleanup_simulation(&data);
        return (free_everything_and_return(&data));
	}

	printf("Simulation is starting.\n");


	cleanup_simulation(&data);

	free_everything_and_return(&data);
	}

