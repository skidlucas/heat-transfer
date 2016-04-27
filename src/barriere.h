/**
 *
 * Auteur : Lucas Martinez
 *
 * Simulation de la progression de la chaleur sur la surface
 *
 */
#include <pthread.h>
#include <semaphore.h>

typedef struct {
	unsigned limite;
	unsigned cpt;
	pthread_mutex_t mutex;
	pthread_cond_t condVar;
} maBarriere;

typedef struct {
	unsigned limite;
	unsigned cpt;
	sem_t m;
	sem_t b;
} maBarriereSem;

/**
 * Fonction qui initialise le mutex et la variable condition de la barriere donnee en parametre,
 * cette fonction met egalement la limite (le nombre max de threads) de la barriere et initialise le compteur a 0
 */
void barrier_init(maBarriere* barriere, unsigned limite);

/**
 * Fonction qui simule le wait d'une barriere posix
 */
int barrier_wait(maBarriere* barriere);

/**
 * Fonction qui detruit le mutex et la variable condition de la barriere passee en parametre
 */
void barrier_destroy(maBarriere* barriere);

/**
 * Fonction qui initialise les semaphores de la barriere donnee en parametre,
 * cette fonction met egalement la limite (le nombre max de threads) de la barriere et initialise le compteur a 0
 */
void barrier_sem_init(maBarriereSem* barriere, unsigned limite);

/**
 * Fonction qui simule le wait d'une barriere posix avec les semaphores
 */
int barrier_sem_wait(maBarriereSem* barriere);

/**
 * Fonction qui detruit les semaphores de la barriere passee en parametre
 */
void barrier_sem_destroy(maBarriereSem* barriere);
