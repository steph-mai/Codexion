#include "codexion.h"

/* Règle de priorité déterministe (Tie-breaker) */
static int is_higher_priority(t_node a, t_node b)
{
    if (a.priority < b.priority)
        return (1);
    if (a.priority > b.priority)
        return (0);
    /* En cas d'égalité de temps, l'ID le plus petit est prioritaire */
    return (a.coder_id < b.coder_id);
}

void heap_push(t_queue *q, int id, long long priority)
{
    int index;
    int parent;
    t_node tmp;

    index = q->size;
    q->heap[index].coder_id = id;
    q->heap[index].priority = priority;
    q->size++;

    /* Percolate Up (Remontée) */
    while (index > 0)
    {
        parent = (index - 1) / 2;
        if (is_higher_priority(q->heap[index], q->heap[parent]))
        {
            tmp = q->heap[index];
            q->heap[index] = q->heap[parent];
            q->heap[parent] = tmp;
            index = parent;
        }
        else
            break;
    }
}

void heap_pop(t_queue *q)
{
    int index = 0;
    int smallest;
    int left, right;
    t_node tmp;

    if (q->size <= 0)
        return;

    q->heap[0] = q->heap[q->size - 1];
    q->size--;

    /* Percolate Down (Descente) */
    while (1)
    {
        left = 2 * index + 1;
        right = 2 * index + 2;
        smallest = index;

        if (left < q->size && is_higher_priority(q->heap[left], q->heap[smallest]))
            smallest = left;
        if (right < q->size && is_higher_priority(q->heap[right], q->heap[smallest]))
            smallest = right;

        if (smallest != index)
        {
            tmp = q->heap[index];
            q->heap[index] = q->heap[smallest];
            q->heap[smallest] = tmp;
            index = smallest;
        }
        else
            break;
    }
}

int heap_peek(t_queue *q)
{
    if (q->size <= 0)
        return (-1);
    return (q->heap[0].coder_id);
}
int secure_wait_for_dongle(t_coder *coder, t_dongle *dongle)
{
    struct timespec ts;
    long long       target_ms;
    long long       my_prio;

    /* 1. Définition de la priorité selon le scheduler */
    if (coder->data->parsed_args.scheduler == SCHEDULER_EDF)
        my_prio = coder->last_compilation_time + coder->data->parsed_args.time_to_burnout;
    else
        my_prio = get_time_ms();

    pthread_mutex_lock(&dongle->dongle_mutex);

    /* 2. Inscription dans la file d'attente */
    heap_push(&dongle->wait_queue, (int)coder->id, my_prio);

    while (1)
    {
        if (!get_simulation_status(coder->data))
        {
            /* Optionnel : tu peux créer une fonction pour retirer l'id du heap ici */
            pthread_mutex_unlock(&dongle->dongle_mutex);
            return (0);
        }

        /* Arbitrage : Suis-je en haut de la file ? */
        int is_my_turn = (heap_peek(&dongle->wait_queue) == (int)coder->id);
        /* Physique : Le dongle est-il libre et froid ? */
        int is_ready = (dongle->is_available && get_timestamp(coder->data) >= dongle->available_at);

        if (is_my_turn && is_ready)
            break;

        if (is_my_turn && !is_ready)
        {
            /* C'est mon tour mais le dongle est encore chaud */
            target_ms = coder->data->start_time + dongle->available_at;
            ts.tv_sec = target_ms / 1000;
            ts.tv_nsec = (target_ms % 1000) * 1000000;
            pthread_cond_timedwait(&dongle->dongle_cond, &dongle->dongle_mutex, &ts);
        }
        else
        {
            /* Soit occupé, soit pas mon tour : attente simple */
            pthread_cond_wait(&dongle->dongle_cond, &dongle->dongle_mutex);
        }
    }

    /* 3. Prise de la ressource et retrait du tas */
    heap_pop(&dongle->wait_queue);
    dongle->is_available = 0;

    return (1);
}
