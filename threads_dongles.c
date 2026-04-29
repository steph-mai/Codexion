/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads_dongles.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stmaire <stmaire@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/29 17:17:23 by stmaire           #+#    #+#             */
/*   Updated: 2026/04/29 17:42:48 by stmaire          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct s_dongles
{
    int             dongles_qty;
    pthread_mutex_t dongles_mtx;
    pthread_cond_t  dongles_cond;  
} t_dongles;

typedef s_struct coder
{
    int coder_id;
    t_dongles *dongles;
} t_coder

void *compil(void *arg)
{
    t_coder *coder = (t_coder *)arg;
    printf("coder %d needs a dongle", &coder->coder_id);
    pthread_mutex_lock(&coder->dongles->dongles_mutex);
    while(&coder->dongles->dong)
    pthread_cond_wait
    
}


