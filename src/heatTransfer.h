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

/**
 * Fonction qui lance la simulation de l'etape 0 (sans threads)
 */
void simulationE0(wrappedMatrice* wrappedMat);

/**
 * Fonction qui lance la simulation de l'etape 1 (avec barrieres)
 */
void simulationE1(wrappedMatrice* wrappedMat);

/**
 * Fonction qui renvoie la fonction a lancer dans le thread en fonction de l'etape en parametre
 */
void* simulation(int etape);

/**
 * Fonction qui initialise les barrieres posix en fonction du nombre de threads
 */
void initBarrieres(int nbThread, pthread_barrier_t* barriereHori, pthread_barrier_t* barriereVerti);

/**
 * Fonction qui initialise la matrice enveloppee et lance la simulation de l'etape 0
 */
void lancerThread(int taille, int nbIter, caseDansMat* mat, wrappedMatrice* wrappedMat);

/**
 * Fonction qui initialise les matrices enveloppees et lance les threads qui effectueront la simulation
 */
void lancerThreads(int taille, int etape, int nbIter, caseDansMat* mat, pthread_t* threads, 
				   wrappedMatrice* wrappedMat, pthread_barrier_t* barriereHori, pthread_barrier_t* barriereVerti);

/**
 * Fonction qui detruit les barrieres et libere la memoire
 */
void rendreBarrieres(pthread_barrier_t* barriereHori, pthread_barrier_t* barriereVerti);

/**
 * Permet de simuler une iteration de propagation de chaleur
 */
void initSimulation(int taille, int etape, int nbIter, int nbThread, caseDansMat * mat);