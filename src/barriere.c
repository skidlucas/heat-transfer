/**
 *
 * Auteurs : Lucas Martinez
 *
 * Simulation de la progression de la chaleur sur la surface
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include "barriere.h"


/**
 * Fonction qui initialise le mutex et la variable condition de la barriere donnee en parametre,
 * cette fonction met egalement la limite (le nombre max de threads) de la barriere et initialise le compteur a 0
 *
 * @author   Lucas Martinez
 */
void barrier_init(maBarriere* barriere, unsigned limite){
    int ret = 0;

	ret = pthread_mutex_init(&barriere->mutex, NULL);
    if (ret){
        perror("Erreur pthread_mutex_init");
        exit(1);
    }
	
    ret = pthread_cond_init(&barriere->condVar, NULL);
    if (ret){
        perror("Erreur pthread_cond_init");
        exit(1);
    }

	barriere->limite = limite;
	barriere->cpt = 0;
}

/**
 * Fonction qui simule le wait d'une barriere posix
 *
 * @author   Lucas Martinez
 */
int barrier_wait(maBarriere* barriere){
    int ret = 0;
	ret = pthread_mutex_lock(&barriere->mutex);
    if (ret){
        perror("Erreur pthread_mutex_lock");
        exit(1);
    }

    barriere->cpt++;
    if (barriere->cpt == barriere->limite) { //si on a atteint le nombre max de threads
        barriere->cpt = 0;
        ret = pthread_cond_broadcast(&barriere->condVar); //on broadcast
        if (ret){
            perror("Erreur pthread_cond_broadcast");
            exit(1);
        }
        ret = pthread_mutex_unlock(&barriere->mutex);
        if (ret){
            perror("Erreur pthread_mutex_unlock");
            exit(1);
        }

        return PTHREAD_BARRIER_SERIAL_THREAD;
    } else {
        while(barriere->cpt != 0){ //tant que le compteur est different de 0 (cad tq le dernier thread n'est pas arrive)
            pthread_cond_wait(&barriere->condVar, &barriere->mutex); //on wait
            if (ret){
                perror("Erreur pthread_cond_wait");
                exit(1);
            }
        }
        pthread_mutex_unlock(&barriere->mutex);
        if (ret){
            perror("Erreur pthread_mutex_unlock");
            exit(1);
        }

        return 0;
    }
}

/**
 * Fonction qui detruit le mutex et la variable condition de la barriere passee en parametre
 *
 * @author   Lucas Martinez
 */
void barrier_destroy(maBarriere* barriere){
    int ret = 0;

	ret = pthread_cond_destroy(&barriere->condVar);
    if (ret){
        perror("Erreur pthread_cond_destroy");
        exit(1);
    }

	ret = pthread_mutex_destroy(&barriere->mutex);
    if (ret){
        perror("Erreur pthread_mutex_destroy");
        exit(1);
    }
}