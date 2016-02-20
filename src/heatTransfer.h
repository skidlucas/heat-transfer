/**
 *
 * Auteurs : Lucas Martinez, Lucas Soumille
 *
 * Simulation de la progression de la chaleur sur la surface
 *
 */
#include "matrice.h"


/**
 * Permet de simuler une diffusion de la chaleur de maniere horizontale autour de la
 * case passee en parametre
 */
void simulationHori(caseDansMat * mat, int taille);

/**
 * Permet de simuler une diffusion de la chaleur de maniere verticale autour de la
 * case passee en parametre
 */
void simulationVerti(caseDansMat * mat, int taille);


/**
 * Permet de simuler une iteration de propagation de chaleur
 */
void simulationIteration( int taille, int N, caseDansMat * mat);