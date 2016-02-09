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
void positionneCaseChauffante(caseDansMat ** mat, int N, double temp_chaude){
	int minInd = (1 << (N - 1)) - (1 << (N - 4));
	int maxInd = (1 << (N - 1)) + (1 << (N - 4));
	for(int i = minInd ; i < maxInd ; ++i){
		for(int j = minInd ; j < maxInd ; ++j){
			mat[i][j].valN = temp_chaude;
			mat[i][j].valNPlus1 = temp_chaude;
			mat[i][j].estChauffante = 1;
		}
	}
}


//propage la chaleur a toutes les cases autour de la case d'indice passe en parametre
void simulationChaqueCase(caseDansMat ** mat, int taille, int i, int j, double coefHori, double coefDiag, double temp_froid){
	if(i < 0 || i >= taille || 
	   j < 0 || j >= taille ||
	   mat[i][j].valN == temp_froid)
		return;
	//On calcule la chaleur transmise aux voisins horizontaux et verticaux
	double chaleurHoriVert = mat[i][j].valN * coefHori; // remplacer constante
	//affectation aux horizontaux verification pour pas sortir de la matrice
	if(j - 1 >= 0)
		mat[i][j - 1].valNPlus1 += chaleurHoriVert;
	if(j + 1 < taille)
		mat[i][j + 1].valNPlus1 += chaleurHoriVert;
	//affectation aux verticaux
	if(i - 1 >= 0)
		mat[i - 1][j].valNPlus1 += chaleurHoriVert;
	if(i + 1 < taille)		
		mat[i + 1][j].valNPlus1 += chaleurHoriVert;
	//On calcule la chaleur transmise aux voisins diagonaux
	double chaleurDiag = mat[i][j].valN * coefDiag;
	if(j - 1 >= 0 && i - 1 >= 0)
		mat[i - 1][j - 1].valNPlus1 += chaleurDiag;
	if(j - 1 >= 0 && i + 1 < taille)
		mat[i + 1][j - 1].valNPlus1 += chaleurDiag;
	if(j + 1 < taille && i - 1 > 0)
		mat[i - 1][j + 1].valNPlus1 += chaleurDiag;
	if(j + 1 < taille && i + 1 < taille)
		mat[i + 1][j + 1].valNPlus1 += chaleurDiag;  
}

//Simule une itération de propagation de chaleur
void simulationIteration(caseDansMat ** mat, int taille, double coefCase, double coefHori, double coefDiag, double temp_froid){
	printf("debut maj = %f\n", coefCase);
	for(int i = 0 ; i < taille  ; ++i){
		for(int j = 0 ; j < taille ; ++j){
			simulationChaqueCase(mat, taille, i, j, coefHori, coefDiag, temp_froid);
		}
	}
	miseAJourMatrice(mat, taille, coefCase, temp_froid);
}