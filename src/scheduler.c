/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scheduler.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stmaire <stmaire@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/07 10:03:26 by stmaire           #+#    #+#             */
/*   Updated: 2026/05/07 16:37:08 by stmaire          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	is_priority(t_node a, t_node b)
{
	if (a.priority_marker < b.priority_marker)
		return (1);
	if (b.priority_marker < a.priority_marker)
		return (0);
	if (a.arrival_time < b.arrival_time)
		return (1);
	if (b.arrival_time < a.arrival_time)
		return (0);
	return (a.coder_id < b.coder_id);
}

void push_in_wait_queue(t_queue *wait_queue, t_node new_node)
{
	size_t	child_index;
	size_t	parent_index;

	if (!wait_queue)
		return;
	child_index = wait_queue->size;
	wait_queue->node[child_index] = new_node;
	wait_queue->size += 1;
	while (child_index > 0)
	{
		parent_index = (child_index - 1) / 2;
		if (is_priority(wait_queue->node[child_index], wait_queue->node[parent_index]))
		{
			swap_nodes(&wait_queue->node[child_index], &wait_queue->node[parent_index]);
			child_index = parent_index;
		}
		else
			break;
	}
}

void wait_queue_pop(t_queue *wait_queue)
{
	size_t	i;
	size_t	smallest_index;

	i = 0;
	if (!wait_queue || wait_queue->size <= 0)
		return ;
	wait_queue->node[0] = wait_queue->node[wait_queue->size -1];
	wait_queue->size -= 1;
	while (1)
	{
		smallest_index = get_smallest_child(wait_queue, i);
		if (smallest_index != i)
		{
			swap_nodes(&wait_queue->node[i], &wait_queue->node[smallest_index]);
			i = smallest_index;
		}
		else
			break;
	}
}

void remove_from_wait_queue(t_queue *wait_queue, size_t coder_id)
{
    size_t i = 0;
    if (!wait_queue || wait_queue->size == 0)
        return;
    while (i < wait_queue->size && wait_queue->node[i].coder_id != coder_id)
        i++;
    if (i < wait_queue->size)
    {
        wait_queue->node[i] = wait_queue->node[wait_queue->size - 1];
        wait_queue->size -= 1;
        // Sécurité : si on a remplacé au milieu, on stabilise l'index i
		//pour que le premier de la heap soit le bon !
        if (i < wait_queue->size)
        {
            size_t smallest = get_smallest_child(wait_queue, i);
            if (smallest != i)
                swap_nodes(&wait_queue->node[i], &wait_queue->node[smallest]);
        }
    }
}

size_t get_first_in_wait_queue(t_queue *wait_queue)
{
	if (!wait_queue || wait_queue->size <= 0)
		return(SIZE_MAX);
	return (wait_queue->node[0].coder_id);
}

