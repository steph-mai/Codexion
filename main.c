/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stephanie <stephanie@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/06 10:52:44 by stmaire           #+#    #+#             */
/*   Updated: 2026/05/13 17:07:21 by stephanie        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	main(int argc, char **argv)
{
	t_data	data;

	memset(&data, 0, sizeof(t_data));
	if (!fill_args_structure(&data.parsed_args, argc, argv))
		return (1);
	if (!init_data(&data))
		return (free_everything_and_return(&data, 1));
	if (!start_simulation(&data))
	{
		set_simulation_stop(&data);
		cleanup_simulation(&data);
		return (free_everything_and_return(&data, 1));
	}
	cleanup_simulation(&data);
	return (free_everything_and_return(&data, 0));
}
