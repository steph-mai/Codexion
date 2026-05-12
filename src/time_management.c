/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time_management.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stmaire <stmaire@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/06 13:45:03 by stmaire           #+#    #+#             */
/*   Updated: 2026/05/12 17:49:36 by stmaire          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

long long	get_time_ms(void)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL) == -1)
		return (-1);
	return ((long long)tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

long long	get_timestamp(t_data *data)
{
	long long	timestamp;

	timestamp = get_time_ms() - data->start_time;
	return (timestamp);
}

void	active_sleep(long long time_to_wait, t_data *data)
{
	long long	start;

	start = get_time_ms();
	while ((get_time_ms() - start) < time_to_wait)
	{
		if (!get_simulation_status(data))
			break ;
		usleep(500);
	}
}
