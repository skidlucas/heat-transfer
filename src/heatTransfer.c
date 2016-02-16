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
void positionneCaseChauffante(caseDansMat * mat, int taille, int N, double temp_chaude){
	int minInd = (1 << (N - 1)) - (1 << (N - 4));
	int maxInd = (1 << (N - 1)) + (1 << (N - 4));
	for(int i = minInd ; i < maxInd ; ++i){
		for(int j = minInd ; j < maxInd ; ++j){
			caseDansMat * caseMat = mat + i * taille + j;
			caseMat->valN = temp_chaude;
			caseMat->valNPlus1 = temp_chaude;
			caseMat->estChauffante = 1;
		}
	}
}


//propage la chaleur a toutes les cases autour de la case d'indice passe en parametre
void simulationChaqueCase(caseDansMat ** mat, int * taille, int * i, int * j, double * coefHori, double * coefDiag, double * temp_froid){
	if(*i < 0 || *i >= *taille || 
	   *j < 0 || *j >= *taille ||
	   mat[*i][*j].valN == *temp_froid)
		return;
	//On calcule la chaleur transmise aux voisins horizontaux et verticaux
	double chaleurHoriVert = mat[*i][*j].valN * *(coefHori);
	//affectation aux horizontaux verification pour pas sortir de la matrice
	if(*(j) - 1 >= 0)
		mat[*i][*(j) - 1].valNPlus1 += chaleurHoriVert;
	if(*(j) + 1 < *taille)
		mat[*(i)][*(j) + 1].valNPlus1 += chaleurHoriVert;
	//affectation aux verticaux
	if(*(i) - 1 >= 0)
		mat[*(i) - 1][*j].valNPlus1 += chaleurHoriVert;
	if(*(i) + 1 < *taille)		
		mat[*(i) + 1][*j].valNPlus1 += chaleurHoriVert;
	//On calcule la chaleur transmise aux voisins diagonaux
	double chaleurDiag = mat[*i][*j].valN * *(coefDiag);
	if(*(j) - 1 >= 0 && *(i) - 1 >= 0)
		mat[*(i) - 1][*(j) - 1].valNPlus1 += chaleurDiag;
	if(*(j) - 1 >= 0 && *(i) + 1 < *taille)
		mat[*(i) + 1][*(j) - 1].valNPlus1 += chaleurDiag;
	if(*(j) + 1 < *taille && *(i) - 1 > 0)
		mat[*(i) - 1][*(j) + 1].valNPlus1 += chaleurDiag;
	if(*(j) + 1 < *taille && *(i) + 1 < *taille)
		mat[*(i) + 1][*(j) + 1].valNPlus1 += chaleurDiag;  
}


void simulateHori(caseDansMat * mat, int taille){
	caseDansMat * caseMat;
	caseDansMat * caseMatGauche;
	caseDansMat * caseMatDroite; 
	for(int i = 0 ; i < taille  ; ++i){
		for(int j = 0 ; j < taille ; ++j){
			caseMat = mat + i * taille + j;
			caseMatGauche = mat + i * taille + (j - 1);
			caseMatDroite = mat + i * taille + (j + 1); 
			if(j == 0)
				caseMat->valNPlus1 = 2 * (caseMat->valN / 3) + caseMatDroite->valN / 6;
			else if (j == taille - 1) 
				caseMat->valNPlus1 = 2 * (caseMat->valN / 3) + caseMatGauche->valN / 6;
			else 
				caseMat->valNPlus1 = 2 * (caseMat->valN / 3) + caseMatDroite->valN / 6 + caseMatGauche->valN / 6;
			
		}
	}
}

void simulateVerti(caseDansMat * mat, int taille){
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
				caseMat->valN = 2 * (caseMat->valNPlus1 / 3) + caseMatBas->valNPlus1  / 6;
			else if (i == taille - 1) 
				caseMat->valN = 2 * (caseMat->valNPlus1 / 3) + caseMatHaut->valNPlus1 / 6;
			else 
				caseMat->valN = 2 * (caseMat->valNPlus1 / 3) + caseMatBas->valNPlus1 / 6 + caseMatHaut->valNPlus1 / 6;
			
		}
	}
// 	/*if(i == 0){
// 		mat[i][j].valN = (2 * (mat[i][j].valNPlus1 /3)) + (mat[i + 1][j].valNPlus1 / 6);
// 	} else if (i == taille - 1) {
// 		mat[i][j].valN = (2 * (mat[i][j].valNPlus1 /3)) + (mat[i - 1][j].valNPlus1 / 6);
// 	} else {
// 		mat[i][j].valN = (2 * (mat[i][j].valNPlus1 /3)) + (mat[i - 1][j].valNPlus1 / 6) + (mat[i + 1][j].valNPlus1 / 6);
// 	}*/
}

//Simule une itération de propagation de chaleur
void simulationIteration(int taille, int N, caseDansMat * mat){
	/*for(int i = 0 ; i < taille  ; ++i){
		for(int j = 0 ; j < taille ; ++j){
			simulateHori(mat, taille, i, j);		
		}
	}*/
		simulateHori(mat, taille);
		simulateVerti(mat, taille);
	/*for(int i = 0 ; i < taille  ; ++i){
		for(int j = 0 ; j < taille ; ++j){
			simulateVerti(mat, taille, i, j);					
		}
	}*/
	
	//miseAJourMatrice(mat, *taille, *coefCase, *temp_froid);
	//positionneCaseChauffante(mat, taille, N, 36.0);
}
