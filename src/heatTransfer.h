/*
*
* Auteurs : Lucas Martinez / Lucas Soumille
*
* Simulation de la progression de la chaleur sur la surface
*
*/
#include "matrice.h"

//Positionne les cases centrales comme les cases chauffantes en fonction de la taille de la matrice
void positionneCaseChauffante(caseDansMat ** mat, int N, double temp_chaude);

//Simule une itération de propagation de chaleur
void simulationIteration(caseDansMat ** mat, int taille, int N);