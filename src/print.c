/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stmaire <stmaire@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/04 09:28:16 by stmaire           #+#    #+#             */
/*   Updated: 2026/05/12 09:48:35 by stmaire          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	print_status(t_coder *coder, char *msg)
{
	pthread_mutex_lock(&coder->data->print_mutex);
	if (!get_simulation_status(coder->data))
	{
		pthread_mutex_unlock(&coder->data->print_mutex);
		return ;
	}
	printf("%lld %zu %s\n", get_timestamp(coder->data), coder->id, msg);
	pthread_mutex_unlock(&coder->data->print_mutex);
}

int	print_error(char *message)
{
	fprintf(stderr, "Codexion Error: %s\n", message);
	return (0);
}

int	print_use(void)
{
	fprintf(stderr, "Error: Invalid number of arguments.\n");
	fprintf(stderr, "Required arguments are:\n <number_of_coders> "
		"<time_to_burnout> <time_to_compile> <time_to_debug> "
		"<time_to_refactor> <number_of_compiles_required> "
		"<dongle_cooldown> <edf/fifo>");
	return (0);
}
