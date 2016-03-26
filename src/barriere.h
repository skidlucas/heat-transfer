/**
 *
 * Auteur : Lucas Martinez
 *
 * Simulation de la progression de la chaleur sur la surface
 *
 */
#include <pthread.h>

typedef struct {
	unsigned limite;
	unsigned cpt;
	pthread_mutex_t mutex;
	pthread_cond_t condVar;
} maBarriere;

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

