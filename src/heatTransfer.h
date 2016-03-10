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
 	pthread_barrier_t * barriereExecution;
 	int nbIter;
 } caseAndIndex;


/**
 * Permet de simuler une diffusion de la chaleur de maniere horizontale autour de la
 * case passee en parametre
 */
//void simulationHori(caseDansMat * mat, int taille);
 void * simulationHori(void * infos);

/**
 * Permet de simuler une diffusion de la chaleur de maniere verticale autour de la
 * case passee en parametre
 */
//void simulationVerti(caseDansMat * mat, int taille);
 void * simulationVerti(void * infos);


/**
 * Permet de simuler une iteration de propagation de chaleur
 */
void simulation(int taille, int nbIter, int nbThread, caseDansMat * mat, int etape);