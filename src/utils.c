/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stmaire <stmaire@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/04 09:28:16 by stmaire           #+#    #+#             */
/*   Updated: 2026/05/05 10:22:01 by stmaire          ###   ########.fr       */
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
	long long	current_time;

	current_time = get_time_ms() - coder->data->start_time;
	pthread_mutex_lock(&coder->data->print_mutex);
	if (!is_simulation_over(coder->data) || strcmp(msg, "burned out") == 0)
		printf("%lld %zu %s\n", current_time, coder->id, msg);
	pthread_mutex_unlock(&coder->data->print_mutex);
	return ;
}
int print_error(char *message)
{
	fprintf(stderr, "Codexion Error: %s\n", message);
	return (0);
}
