#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct s_salle {
    pthread_mutex_t mutex_salle;
    pthread_cond_t  place_free;
    int             nb_personnes;
} t_salle;

typedef struct s_sportif {
    t_salle *salle;
    int      id;
} t_sportif;

void* entrainement(void* arg) {
    t_sportif *member = (t_sportif *)arg;

    printf("Sportif %d : Arrive à la salle.\n", member->id);

    pthread_mutex_lock(&member->salle->mutex_salle);
    while(member->salle->nb_personnes >= 3)
        pthread_cond_wait(&member->salle->place_free, &member->salle->mutex_salle);
    member->salle->nb_personnes += 1;
    pthread_mutex_unlock(&member->salle->mutex_salle);
   
    printf("Sportif %d : Entre dans la salle. (Occupation : %d/3)\n", 
            member->id, member->salle->nb_personnes);
    
    sleep(2);

    pthread_mutex_lock(&member->salle->mutex_salle);
    member->salle->nb_personnes -= 1;
    pthread_cond_broadcast(&member->salle->place_free);
    pthread_mutex_unlock(&member->salle->mutex_salle);

    printf("Sportif %d : Sort de la salle.\n", member->id);
        
    free(member);
    return NULL;
}

int main() {
    pthread_t thread[8];
    t_salle salle;
    int i;
    
    i = 0;
    salle.nb_personnes = 0;
    pthread_mutex_init(&salle.mutex_salle, NULL);
    pthread_cond_init(&salle.place_free, NULL);

    while(i < 8)
    {
        t_sportif *sportif = malloc(sizeof(t_sportif));
        if(!sportif)
            return(-1);
        sportif->id = (i + 1);
        sportif->salle = &salle;
        if(pthread_create(&thread[i], NULL, entrainement, sportif) != 0)
            return(-1);
        i++;
    }

    i = 0;
    while(i < 8)
    {
        pthread_join(thread[i], NULL);
        i += 1;
    }
    pthread_mutex_destroy(&salle.mutex_salle);
    pthread_cond_destroy(&salle.place_free);
}