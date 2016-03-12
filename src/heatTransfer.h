/**
 *
 * Auteurs : Lucas Martinez, Lucas Soumille
 *
 * Simulation de la progression de la chaleur sur la surface
 *
 */
#include <pthread.h>
#include "matrice.h"

typedef struct {
	int x;
	int y;
	int tailleLigne;
	int nbIter;
	caseDansMat* matrice;
	pthread_barrier_t* barriereHori;
	pthread_barrier_t* barriereVerti;
} wrappedMatrice;

/**
 * Permet de simuler une diffusion de la chaleur de maniere horizontale autour de la
 * case passee en parametre
 */
void simulationHori(wrappedMatrice* wrappedMat);

/**
 * Permet de simuler une diffusion de la chaleur de maniere verticale autour de la
 * case passee en parametre
 */
void simulationVerti(wrappedMatrice* wrappedMat);

void simulationE0(wrappedMatrice* wrappedMat);

void simulationE1(wrappedMatrice* wrappedMat);

void* simulation(int etape);

void initBarrieres(int taille, int nbThread, pthread_barrier_t* barriereHori, pthread_barrier_t* barriereVerti);

void lancerThreads(int taille, int etape, int nbIter, caseDansMat* mat, pthread_t* threads, 
				   wrappedMatrice* wrappedMat, pthread_barrier_t* barriereHori, pthread_barrier_t* barriereVerti);

void rendreBarrieres(pthread_barrier_t* barriereHori, pthread_barrier_t* barriereVerti);

void initSimulation(int taille, int etape, int nbIter, int nbThread, caseDansMat * mat);
/**
 * Permet de simuler une iteration de propagation de chaleur
 */
void simulationIteration(int taille, int nbIter, int nbThread, caseDansMat * mat);