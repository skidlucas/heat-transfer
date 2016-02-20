/*
*
* Auteurs : Lucas Martinez / Lucas Soumille
*
* Simulation de la progression de la chaleur sur la surface
*
*/

#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include "heatTransfer.h"

//Positionne les cases centrales comme les cases chauffantes en fonction de la taille de la matrice
// void positionneCaseChauffante(caseDansMat * mat, int taille, int N, double temp_chaude){
// 	int minInd = (1 << (N - 1)) - (1 << (N - 4));
// 	int maxInd = (1 << (N - 1)) + (1 << (N - 4));
// 	for(int i = minInd ; i < maxInd ; ++i){
// 		for(int j = minInd ; j < maxInd ; ++j){
// 			caseDansMat * caseMat = mat + i * taille + j;
// 			caseMat->valN = temp_chaude;
// 			caseMat->valNPlus1 = temp_chaude;
// 			caseMat->estChauffante = 1;
// 		}
// 	}
// }


//propage la chaleur a toutes les cases autour de la case d'indice passe en parametre
// eurTmpmule une diffusion horizontale de chaleur
void simulationHori(caseDansMat * mat, int taille){
	caseDansMat * caseMat;
	caseDansMat * caseMatGauche;
	caseDansMat * caseMatDroite; 
	for(int i = 0 ; i < taille  ; ++i){
		for(int j = 0 ; j < taille ; ++j){
			caseMat = mat + i * taille + j;
			caseMatGauche = mat + i * taille + (j - 1);
			caseMatDroite = mat + i * taille + (j + 1); 
			if(j == 0)
				caseMat->valeurTmp = 2 * (caseMat->valeur / 3) + caseMatDroite->valeur / 6;
			else if (j == taille - 1) 
				caseMat->valeurTmp = 2 * (caseMat->valeur / 3) + caseMatGauche->valeur / 6;
			else 
				caseMat->valeurTmp = 2 * (caseMat->valeur / 3) + caseMatDroite->valeur / 6 + caseMatGauche->valeur / 6;
			
		}
	}
}

//simule une diffusion verticale de chaleur
void simulationVerti(caseDansMat * mat, int taille){
	caseDansMat * caseMat;
	caseDansMat * caseMatHaut;
	caseDansMat * caseMatBas;
	for(int i = 0 ; i < taille  ; ++i){
		for(int j = 0 ; j < taille ; ++j){
			caseMat = mat + i * taille + j;
			//si la case est chauffante on fait pas la mise a jour de sa valeur
			if(caseMat->estChauffante == 1) continue;
			caseMatHaut = mat + (i - 1) * taille + j;
			caseMatBas = mat + (i + 1) * taille + j; 
			if(i == 0)
				caseMat->valeur = 2 * (caseMat->valeurTmp / 3) + caseMatBas->valeurTmp  / 6;
			else if (i == taille - 1) 
				caseMat->valeur = 2 * (caseMat->valeurTmp / 3) + caseMatHaut->valeurTmp / 6;
			else 
				caseMat->valeur = 2 * (caseMat->valeurTmp / 3) + caseMatBas->valeurTmp / 6 + caseMatHaut->valeurTmp / 6;
			
		}
	}
}

//Simule une itération de propagation de chaleur
void simulationIteration(int taille, int N, caseDansMat * mat){
	simulationHori(mat, taille);
	simulationVerti(mat, taille);
}
