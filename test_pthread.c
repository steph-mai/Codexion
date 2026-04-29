/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_pthread.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stmaire <stmaire@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/28 13:58:36 by stmaire           #+#    #+#             */
/*   Updated: 2026/04/29 11:36:33 by stmaire          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <pthread.h>

typedef struct s_data
{
    char *id;
    int n;
} t_data;

static void *task_a(void *p_data)
{
    int i;
    t_data *p;
    char *s;
    
    p = (t_data *)p_data;
    s = "steph";
    i = 0;
    
    while (i < p->n)
    {
        printf("%s, %s, %d\n", s, p->id,  i);
        i++;
    }
    return NULL;
}

// static void *task_b(void *p_data)
// {
//     int i;
//     char *s;
    
//     s = p_data;
//     i = 0;
    
//     while(i < 4)
//     {
//         printf("%s, %d\n", s, i);
//         i++;        
//     }
//     (void)p_data;
//     return NULL;
// }

int main(void)
{
    pthread_t ta;
    pthread_t tb;
    
    t_data data_a = {"A", 5};
    t_data data_b = {"B", 4};

    printf("main init\n");

    pthread_create(&ta, NULL, task_a, &data_a);
    pthread_create(&tb, NULL, task_a, &data_b);

    pthread_join(ta, NULL);
    pthread_join(tb, NULL);
    
    printf("main end\n");
    return (0);
}
