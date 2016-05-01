/**
 *
 * Auteurs : Lucas Martinez, Lucas Soumille
 *
 * Simulation de la progression de la chaleur sur la surface
 *
 */
#include "matrice.h"
#include <semaphore.h> 

/* representation d'une barriere utilisant les semaphores */
typedef struct 
{
	unsigned limite;
	unsigned count;
	sem_t m;
	sem_t attente;
} barrier_sema_t;

/* representation d'une barriere utilisant les variables conditions */
typedef struct 
{
	unsigned limite;
	unsigned count;
	pthread_mutex_t m;
	pthread_cond_t cv;
} barrier_t;

/* Informations passees à chaque thread */
typedef struct {
 	int indXDeb;
 	int indYDeb;
 	int indXFin;
 	int indYFin;
 	pthread_barrier_t * barriereMil;
 	pthread_barrier_t * barriereFin;
 	barrier_t * maBarriereMil;
 	barrier_t * maBarriereFin;
 	barrier_sema_t * maBarriereSemaMil;
 	barrier_sema_t * maBarriereSemaFin;
 	int nbIter;
 	caseDansMat * matGeneral;
	int tailleTotale;
 } caseAndIndex;

/**
 * Initialisation des barrieres en particulier du mutex, de la variable condition
 * et des compteurs
 */
void barrier_init(barrier_t * b, unsigned limite);

/**
 * Initialisation des barrieres en particulier des deux semaphores
 * et des compteurs
 */
void barrier_sema_init(barrier_sema_t * b, unsigned limite);

/**
 * Destruction des barrieres (du mutex et de la variables condition)
 */
void barrier_destroy(barrier_t *b);

/**
 * Destruction des barrieres (des semaphores)
 */
void barrier_sema_destroy(barrier_sema_t * b);

/**
 * Implementation de la fonction barriere wait en utilisant les variables conditions (moniteur)
 */
int barrier_wait(barrier_t *b);

/** 
 * Implementation de la fonction barriere wait en utilisant les semaphores
 */
int barriere_sema_wait(barrier_sema_t * b);

/**
 * Permet de simuler une diffusion de la chaleur de maniere horizontale autour de la
 * case passee en parametre
 */
void simulationHori(void * infos);

/**
 * Permet de simuler une diffusion de la chaleur de maniere verticale autour de la
 * case passee en parametre
 */
void simulationVerti(void * infos);

/**
 * Fonction de simulation appelee par chaque thread pour l'etape 0
 */
void simulationEtape0(void * infos);

/**
 * Fonction de simulation appelee par chaque thread pour l'etape 1
 */
void simulationEtape1(void * infos);

/**
 * Fonction de simulation appelee par chaque thread pour l'etape 2
 */
void simulationEtape2(void * infos);

/**
 * Permet de simuler une iteration de propagation de chaleur
 */
void simulation(int taille, int nbIter, int nbThread, caseDansMat * mat, int etape);

/**
 * Retourne le pointeur sur la fonction a executer pour l'etape passee en parametre
 * de la simulation
 */
void * fonctionEtape(int etape);