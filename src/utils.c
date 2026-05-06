/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stmaire <stmaire@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/04 09:28:16 by stmaire           #+#    #+#             */
/*   Updated: 2026/05/06 13:39:26 by stmaire          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	is_simulation_over(t_data *data)
{
	int	status;

	pthread_mutex_lock(&data->simulation_over_mutex);
	if (data->is_burn_out || data->is_simulation_a_success)
		status = 1;
	status = 0;
	pthread_mutex_unlock (&data->simulation_over_mutex);
	return (status);
}

long long	get_time_ms(void)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL) == -1)
		return (-1);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

void	print_status(t_coder *coder, char *msg)
{
	long long	timestamp_in_ms;

	pthread_mutex_lock(&coder->data->print_mutex);
	if (!get_simulation_status(coder->data))
	{
		pthread_mutex_unlock(&coder->data->print_mutex);
		return;
	}
	timestamp_in_ms = get_time_ms() - coder->data->start_time;
	printf("%lld %zu %s\n", timestamp_in_ms, coder->id, msg);
	pthread_mutex_unlock(&coder->data->print_mutex);
}

int print_error(char *message)
{
	fprintf(stderr, "Codexion Error: %s", message);
	return (0);
}

void smart_sleep(long long time_to_wait, t_data *data)
{
	long long start;

	start = get_time_ms();
	while ((get_time_ms() - start) < time_to_wait)
	{
		// Si la simulation s'arrête (burnout d'un autre), on sort du sommeil
		if (!get_simulation_status(data))
			break;
		// On dort par tranches de 500 microsecondes
		usleep(500);
	}
}
