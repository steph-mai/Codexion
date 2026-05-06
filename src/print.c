/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stmaire <stmaire@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/04 09:28:16 by stmaire           #+#    #+#             */
/*   Updated: 2026/05/06 14:10:44 by stmaire          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"



void	print_status(t_coder *coder, char *msg)
{
	pthread_mutex_lock(&coder->data->print_mutex);
	if (!get_simulation_status(coder->data))
	{
		pthread_mutex_unlock(&coder->data->print_mutex);
		return;
	}
	// timestamp_in_ms = get_time_ms() - coder->data->start_time;
	printf("%lld %zu %s\n", get_timestamp(coder->data), coder->id, msg);
	pthread_mutex_unlock(&coder->data->print_mutex);
}

int print_error(char *message)
{
	fprintf(stderr, "Codexion Error: %s", message);
	return (0);
}
