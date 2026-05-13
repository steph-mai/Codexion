/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stephanie <stephanie@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/07 16:32:37 by stmaire           #+#    #+#             */
/*   Updated: 2026/05/13 15:22:15 by stephanie        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	swap_nodes(t_node *a, t_node *b)
{
	t_node	temp_node;

	if (!a || !b)
		return ;
	temp_node = *a;
	*a = *b;
	*b = temp_node;
}

size_t	get_smallest_child(t_queue *wait_queue, size_t index)
{
	size_t	left_child_index;
	size_t	right_child_index;
	size_t	smallest_index;

	if (!wait_queue)
		return (index);
	smallest_index = index;
	left_child_index = (index) * 2 + 1;
	right_child_index = (index) * 2 + 2;
	if (left_child_index < wait_queue->size
		&& is_priority(wait_queue->node[left_child_index],
			wait_queue->node[smallest_index]))
		smallest_index = left_child_index;
	if (right_child_index < wait_queue->size
		&& is_priority(wait_queue->node[right_child_index],
			wait_queue->node[smallest_index]))
		smallest_index = right_child_index;
	return (smallest_index);
}
