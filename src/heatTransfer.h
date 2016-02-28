/**
 *
 * Auteurs : Lucas Martinez, Lucas Soumille
 *
 * Simulation de la progression de la chaleur sur la surface
 *
 */
#include "matrice.h"

 typedef struct {
 	caseDansMat * mat;
 	int indXDeb;
 	int indYDeb;
 	int indXFin;
 	int indYFin;
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
void simulationIteration(int taille, int nbThread, caseDansMat * mat);