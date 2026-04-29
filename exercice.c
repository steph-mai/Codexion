#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct s_shared
{
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    int pont_occupe;  
} t_shared;

typedef struct s_car_context
{
    t_shared *data;
    int id;
} t_car_context;

void* car(void* arg)
{
    t_car_context *car_context= (t_car_context *)arg;
    
    printf("Voiture %d : Arrive devant le pont.\n", car_context->id);

    pthread_mutex_lock(&car_context->data->mutex);
    while(car_context->data->pont_occupe != 0)
    {
        pthread_cond_wait(&car_context->data->cond, &car_context->data->mutex);
    }
    car_context->data->pont_occupe = 1;
    pthread_mutex_unlock(&car_context->data->mutex);
    
    printf("Voiture %d : Je traverse le pont...\n", car_context->id);
    sleep(1); 
    pthread_mutex_lock(&car_context->data->mutex);
    car_context->data->pont_occupe = 0;
    pthread_cond_broadcast(&car_context->data->cond);
    pthread_mutex_unlock(&car_context->data->mutex);
    
    printf("Voiture %d : J'ai quitté le pont.\n", car_context->id);
    free(car_context);
    return NULL;
}

int main() {
    pthread_t fils[5];
    t_shared data;
    int i;
    i = 0;
    
    data.pont_occupe = 0;
    pthread_mutex_init(&data.mutex, NULL);
    pthread_cond_init(&data.cond, NULL);

    while(i<5)
    {
        t_car_context *ctx = malloc(sizeof(t_car_context));
        if(ctx == NULL)
        {
            return(-1);
        }
        ctx->data = &data;
        ctx->id = i + 1;
        if (pthread_create(&fils[i], NULL, car, ctx) != 0)
            return(-1);
        i += 1;
    }
    i = 0;
    while(i < 5)
    {
        pthread_join(fils[i], NULL);
        i++;
    }
    pthread_mutex_destroy(&data.mutex);
    pthread_cond_destroy(&data.cond);

    printf("Fin de la journée, le pont est fermé.\n");
    return 0;
}
