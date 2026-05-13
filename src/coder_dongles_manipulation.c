/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_dongles_manipulation.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stephanie <stephanie@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/06 11:45:09 by stmaire           #+#    #+#             */
/*   Updated: 2026/05/13 16:12:54 by stephanie        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static long long	get_full_availability_time(t_coder *coder)
{
	if (coder->left_dongle->available_at > coder->right_dongle->available_at)
		return (coder->left_dongle->available_at);
	return (coder->right_dongle->available_at);
}

static void	wait_before_take(t_coder *coder)
{
	struct timespec	ts;
	long long		target;

	while (get_simulation_status(coder->data))
	{
		target = get_full_availability_time(coder);
		if (get_first_in_wait_queue(&coder->data->wait_queue) == coder->id
			&& coder->left_dongle->is_unused && coder->right_dongle->is_unused
			&& get_time_ms() >= target)
			break ;
		if (coder->left_dongle->is_unused && coder->right_dongle->is_unused)
		{
			ts.tv_sec = target / 1000;
			ts.tv_nsec = (target % 1000) * 1000000;
			pthread_cond_timedwait(&coder->data->queue_cond,
				&coder->data->queue_mutex, &ts);
		}
		else
			pthread_cond_wait(&coder->data->queue_cond,
				&coder->data->queue_mutex);
	}
}

int	identify_and_take_dongles(t_coder *coder)
{
	t_node	node;

	node.coder_id = coder->id;
	node.arrival_time = get_time_ms();
	if (coder->data->parsed_args.scheduler == SCHEDULER_EDF)
		node.priority_marker = coder->last_compilation_time
			+ coder->data->parsed_args.time_to_burnout;
	else
		node.priority_marker = node.arrival_time;
	pthread_mutex_lock(&coder->data->queue_mutex);
	push_in_wait_queue(&coder->data->wait_queue, node);
	wait_before_take(coder);
	if (get_simulation_status(coder->data))
	{
		wait_queue_pop(&coder->data->wait_queue);
		coder->left_dongle->is_unused = 0;
		coder->right_dongle->is_unused = 0;
		print_status(coder, "has taken a dongle");
		print_status(coder, "has taken a dongle");
		pthread_mutex_unlock(&coder->data->queue_mutex);
		return (1);
	}
	remove_from_wait_queue(&coder->data->wait_queue, coder->id);
	pthread_mutex_unlock(&coder->data->queue_mutex);
	return (0);
}

void	put_dongles_away(t_coder *coder)
{
	long long	now;

	pthread_mutex_lock(&coder->data->queue_mutex);
	now = get_time_ms();
	coder->left_dongle->is_unused = 1;
	coder->left_dongle->available_at
		= now + coder->data->parsed_args.dongle_cooldown;
	coder->right_dongle->is_unused = 1;
	coder->right_dongle->available_at
		= now + coder->data->parsed_args.dongle_cooldown;
	pthread_cond_broadcast(&coder->data->queue_cond);
	pthread_mutex_unlock(&coder->data->queue_mutex);
}
