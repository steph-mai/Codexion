/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads_dongles.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stephanie <stephanie@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/29 17:17:23 by stmaire           #+#    #+#             */
/*   Updated: 2026/04/30 09:36:39 by stephanie        ###   ########.fr       */
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

typedef struct s_coder
{
    int coder_id;
    size_t time_to_compil;
    t_dongles *dongles;
} t_coder;

void *compil(void *arg)
{
    t_coder *coder = (t_coder *)arg;
    printf("%d needs a dongle\n", coder->coder_id);
    pthread_mutex_lock(&coder->dongles->dongles_mtx);
    while(coder->dongles->dongles_qty <2)
        pthread_cond_wait(&coder->dongles->dongles_cond, &coder->dongles->dongles_mtx);
    printf("%d has taken a dongle\n", coder->coder_id);
    printf("%d has taken a dongle\n", coder->coder_id);
    coder->dongles->dongles_qty -=2;
    pthread_mutex_unlock(&coder->dongles->dongles_mtx);
    
    printf("%d is compiling\n", coder->coder_id);
    usleep(400000);

    pthread_mutex_lock(&coder->dongles->dongles_mtx);
    coder->dongles->dongles_qty +=2;
    pthread_cond_broadcast(&coder->dongles->dongles_cond);
    pthread_mutex_unlock(&coder->dongles->dongles_mtx);
    
    printf("%d is refactoring\n", coder->coder_id);
    
    free(coder);
    return NULL;
}

int main(void)
{
    t_dongles dongles;
    pthread_t thread[5];
    int i;
    
    dongles.dongles_qty = 5;
    pthread_mutex_init(&dongles.dongles_mtx, NULL);
    pthread_cond_init(&dongles.dongles_cond, NULL);
    i = 0;

    while(i < 5)
    {
        t_coder *coder=malloc(sizeof(t_coder));
        if (!coder)
            return(-1);
        coder->coder_id = i + 1;
        coder->dongles = &dongles;
        if (pthread_create(&thread[i], NULL, compil, coder) != 0)
            return(-1);
        i++;              
    }
    i = 0;
    while(i <5)
    {
        pthread_join(thread[i], NULL);
        i++;
    }
    pthread_cond_destroy(&dongles.dongles_cond);
    pthread_mutex_destroy(&dongles.dongles_mtx);
    
}


