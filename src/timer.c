/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   timer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stmaire <stmaire@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/06 13:45:03 by stmaire           #+#    #+#             */
/*   Updated: 2026/05/06 14:11:08 by stmaire          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "codexion.h"

long long	get_time_ms(void)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL) == -1)
		return (-1);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

long long	get_timestamp(t_data *data)
{

	long long current_time;
	long long timestamp;

	current_time = get_time_ms();
	timestamp = get_time_ms() - data->start_time;
	return (timestamp);
}

void smart_sleep(long long time_to_wait, t_data *data)
{
	long long start;

	start = get_time_ms();
	while ((get_time_ms() - start) < time_to_wait)
	{
		if (!get_simulation_status(data))
			break;
		usleep(500);
	}
}
