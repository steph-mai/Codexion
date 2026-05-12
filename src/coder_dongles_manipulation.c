/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_dongles_manipulation.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stmaire <stmaire@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/06 11:45:09 by stmaire           #+#    #+#             */
/*   Updated: 2026/05/12 16:52:02 by stmaire          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	wait_until_ready(t_coder *coder, t_dongle *dongle)
{
	struct timespec	ts;
	long long		target;
	int				is_priority;

	while (get_simulation_status(coder->data))
	{
		is_priority
			= (get_first_in_wait_queue(&dongle->wait_queue) == coder->id);
		dongle->is_available = (dongle->is_unused
				&& get_timestamp(coder->data) >= dongle->available_at);
		if (is_priority && dongle->is_available)
			return ;
		if (is_priority && dongle->is_unused && !dongle->is_available)
		{
			target = coder->data->start_time + dongle->available_at;
			ts.tv_sec = target / 1000;
			ts.tv_nsec = (target % 1000) * 1000000;
			pthread_cond_timedwait
				(&dongle->dongle_cond, &dongle->dongle_mutex, &ts);
		}
		else
			pthread_cond_wait(&dongle->dongle_cond, &dongle->dongle_mutex);
	}
}

static int	request_single_dongle(t_coder *coder, t_dongle *dongle)
{
	t_node	node;

	node.coder_id = coder->id;
	node.arrival_time = get_time_ms();
	node.priority_marker = node.arrival_time;
	if (coder->data->parsed_args.scheduler == SCHEDULER_EDF)
		node.priority_marker = coder->last_compilation_time
			+ coder->data->parsed_args.time_to_burnout;
	pthread_mutex_lock(&dongle->dongle_mutex);
	push_in_wait_queue(&dongle->wait_queue, node);
	wait_until_ready(coder, dongle);
	if (!get_simulation_status(coder->data))
	{
		remove_from_wait_queue(&dongle->wait_queue, coder->id);
		pthread_mutex_unlock(&dongle->dongle_mutex);
		return (0);
	}
	wait_queue_pop(&dongle->wait_queue);
	dongle->is_available = 0;
	dongle->is_unused = 0;
	return (1);
}

static int	try_acquire_both_dongles(t_coder *coder,
	t_dongle *first, t_dongle *second)
{
	if (!request_single_dongle(coder, first))
		return (0);
	if (first == second)
	{
		remove_from_wait_queue(&first->wait_queue, coder->id);
		first->is_unused = 1;
		pthread_cond_broadcast(&first->dongle_cond);
		pthread_mutex_unlock(&first->dongle_mutex);
		active_sleep(10, coder->data);
		return (0);
	}
	if (!request_single_dongle(coder, second))
	{
		first->is_unused = 1;
		pthread_cond_broadcast(&first->dongle_cond);
		pthread_mutex_unlock(&first->dongle_mutex);
		return (0);
	}
	print_status(coder, "has taken a dongle");
	print_status(coder, "has taken a dongle");
	return (1);
}

int	identify_and_take_dongles(t_coder *coder)
{
	t_dongle	*first;
	t_dongle	*second;

	if (coder->left_dongle->id < coder->right_dongle->id)
	{
		first = coder->left_dongle;
		second = coder->right_dongle;
	}
	else
	{
		first = coder->right_dongle;
		second = coder->left_dongle;
	}
	if (!try_acquire_both_dongles(coder, first, second))
		return (0);
	return (1);
}

void	put_dongles_away(t_coder *coder)
{
	long long	current_time;

	current_time = get_timestamp(coder->data);
	coder->left_dongle->available_at
		= current_time + coder->data->parsed_args.dongle_cooldown;
	coder->left_dongle->is_unused = 1;
	pthread_cond_broadcast(&coder->left_dongle->dongle_cond);
	pthread_mutex_unlock(&coder->left_dongle->dongle_mutex);
	coder->right_dongle->available_at
		= current_time + coder->data->parsed_args.dongle_cooldown;
	coder->right_dongle->is_unused = 1;
	pthread_cond_broadcast(&coder->right_dongle->dongle_cond);
	pthread_mutex_unlock(&coder->right_dongle->dongle_mutex);
}
