/**
 *
 * Auteurs : Lucas Martinez, Lucas Soumille
 *
 * Simulation de la progression de la chaleur sur la surface
 *
 */
#include "matrice.h"

/* Informations passees à chaque thread */
 typedef struct {
 	int indXDeb;
 	int indYDeb;
 	int indXFin;
 	int indYFin;
 	pthread_barrier_t * barriereMil;
 	pthread_barrier_t * barriereFin;
 	int nbIter;
 	caseDansMat * matGeneral;
	int tailleTotale;
 } caseAndIndex;


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
 * Permet de simuler une iteration de propagation de chaleur
 */
void simulation(int taille, int nbIter, int nbThread, caseDansMat * mat, int etape);

/**
 * Retourne le pointeur sur la fonction a executer pour l'etape passee en parametre
 * de la simulation
 */
void * fonctionEtape(int etape);