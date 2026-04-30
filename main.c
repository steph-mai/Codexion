/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stephanie <stephanie@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/30 12:16:46 by stephanie         #+#    #+#             */
/*   Updated: 2026/04/30 15:30:22 by stephanie        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int main(int argc, char **argv)
{
	t_data data;
	size_t i;

	i = 0;
	memset(&data, 0, sizeof(t_data));

	if(!fill_args_structure(&data.parsed_args,argc, argv))
	{
		print("Error: parsing failed\n");
		return(1);
	}
	print("parsing OK");

	if(!init_data(&data))
		return(free_everything_and_return(&data));

	print("Initialization data OK\n");

	if (!start_simulation(&data))
	{
		set_simulation_stop(&data);
        cleanup_simulation(&data);
        return (free_everything_and_return(&data));
	}

	print("Simulation is starting.\n");


	clean_up_simulation(&data);

	free_everything_and_return(&data);
	}

