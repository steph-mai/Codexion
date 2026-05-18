/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_dongles_manipulation.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stmaire <stmaire@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/06 11:45:09 by stmaire           #+#    #+#             */
/*   Updated: 2026/05/18 14:40:49 by stmaire          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	can_take_dongles(t_coder *coder, t_node my_node)
{
	size_t	i;
	t_node	*q;

	i = 0;
	q = coder->data->wait_queue.node;
	while (i < coder->data->wait_queue.size)
	{
		if (q[i].coder_id != coder->id && is_priority(q[i], my_node))
		{
			if (q[i].left_dongle == coder->left_dongle
				|| q[i].right_dongle == coder->left_dongle
				|| q[i].left_dongle == coder->right_dongle
				|| q[i].right_dongle == coder->right_dongle)
				return (0);
		}
		i++;
	}
	return (1);
}

static int	confirm_dongles(t_coder *coder)
{
	coder->left_dongle->is_unused = 0;
	coder->right_dongle->is_unused = 0;
	print_status(coder, "has taken a dongle");
	print_status(coder, "has taken a dongle");
	pthread_mutex_unlock(&coder->data->queue_mutex);
	return (1);
}

static void	wait_before_take(t_coder *coder, t_node node)
{
	struct timespec	ts;
	long long		t;
	long long		now;

	while (get_simulation_status(coder->data))
	{
		t = coder->right_dongle->available_at;
		if (coder->left_dongle->available_at > t)
			t = coder->left_dongle->available_at;
		now = get_time_ms();
		if (coder->left_dongle->is_unused && coder->right_dongle->is_unused
			&& now >= t && can_take_dongles(coder, node))
			break ;
		if (coder->left_dongle->is_unused && coder->right_dongle->is_unused
			&& now < t)
		{
			ts.tv_sec = t / 1000;
			ts.tv_nsec = (t % 1000) * 1000000;
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
	node.left_dongle = coder->left_dongle;
	node.right_dongle = coder->right_dongle;
	pthread_mutex_lock(&coder->data->simulation_mutex);
	node.priority_marker = node.arrival_time;
	if (coder->data->parsed_args.scheduler == SCHEDULER_EDF)
		node.priority_marker = coder->last_compilation_time
			+ coder->data->parsed_args.time_to_burnout;
	pthread_mutex_unlock(&coder->data->simulation_mutex);
	pthread_mutex_lock(&coder->data->queue_mutex);
	push_in_wait_queue(&coder->data->wait_queue, node);
	wait_before_take(coder, node);
	remove_from_wait_queue(&coder->data->wait_queue, coder->id);
	if (get_simulation_status(coder->data))
		return (confirm_dongles(coder));
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
