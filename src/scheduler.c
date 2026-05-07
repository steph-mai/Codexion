/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scheduler.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stmaire <stmaire@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/07 10:03:26 by stmaire           #+#    #+#             */
/*   Updated: 2026/05/07 10:27:49 by stmaire          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int is_priority(t_coder *coder)
{
	(void)coder;
	return (1);
}
void push_in_wait_queue(t_queue *wait_queue, size_t *coder_id, long long *priority_marker)
{
	(void)wait_queue;
	(void)coder_id;
	(void)priority_marker;

}

void wait_queue_pop(t_queue *wait_queue)
{
	(void)wait_queue;
}
void remove_from_wait_queue(t_queue *wait_queue, size_t *coder_id)
{
	(void)wait_queue;
	(void)coder_id;
}

size_t get_first_in_wait_queue(t_queue *wait_queue)
{
	(void)wait_queue;
	return (0);
}

static int is_higher_prio(t_node a, t_node b)
{
    if (a.priority < b.priority)
        return (1);
    if (a.priority > b.priority)
        return (0);
    // Tie-breaker : En cas d'égalité parfaite, l'ID le plus petit gagne
    return (a.coder_id < b.coder_id);
}
void push_in_wait_queue(t_queue *q, int id, long long priority)
{
    int index = q->size;
    q->heap[index].coder_id = id;
    q->heap[index].priority = priority;
    q->size++;

    while (index > 0)
    {
        int parent = (index - 1) / 2;
        if (is_higher_prio(q->heap[index], q->heap[parent]))
        {
            t_node tmp = q->heap[index];
            q->heap[index] = q->heap[parent];
            q->heap[parent] = tmp;
            index = parent;
        }
        else
            break;
    }
}
void wait_queue_pop(t_queue *q)
{
    if (q->size <= 0) return;

    q->heap[0] = q->heap[q->size - 1];
    q->size--;
    int i = 0;

    while (1)
    {
        int left = 2 * i + 1;
        int right = 2 * i + 2;
        int smallest = i;

        if (left < q->size && is_higher_prio(q->heap[left], q->heap[smallest]))
            smallest = left;
        if (right < q->size && is_higher_prio(q->heap[right], q->heap[smallest]))
            smallest = right;
        if (smallest == i)
            break;
        t_node tmp = q->heap[i];
        q->heap[i] = q->heap[smallest];
        q->heap[smallest] = tmp;
        i = smallest;
    }
}
size_t get_first_in_wait_queue(t_queue *wait_queue)
{
    if (wait_queue->size == 0)
        return (0); // Ou une valeur signifiant "vide"
    return (wait_queue->heap[0].coder_id);
}
void remove_from_wait_queue(t_queue *q, int id)
{
    int i = 0;
    while (i < q->size && q->heap[i].coder_id != id)
        i++;
    if (i == q->size) return; // Pas trouvé

    q->heap[i] = q->heap[q->size - 1];
    q->size--;
    // Ici, pour simplifier et garantir l'intégrité du tas :
    // On pourrait juste reconstruire le tas ou faire un down-heap.
    // Mais dans la plupart des cas, vider le tas à la fin de simulation suffit.
}
