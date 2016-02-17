/*
*
* Auteurs : Lucas Martinez / Lucas Soumille
*
* Permet de gerer la matrice correspondant à la plaque
*
*/
#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include <math.h>
#include <stdlib.h>
#include "matrice.h"


//Initialise les cases dans le matrice
void initMatrice(caseDansMat * mat, int taille, int N, double temp_froid, double temp_chaud){
	int minInd = (1 << (N - 1)) - (1 << (N - 4));
	int maxInd = (1 << (N - 1)) + (1 << (N - 4));
	for(int i = 0 ; i < taille ; ++i){
		for(int j = 0 ; j < taille ; ++j){
			caseDansMat * caseMat = mat + i * taille + j;
			if(i >= minInd && i < maxInd && j >= minInd && j < maxInd){
				caseMat->valeur = temp_chaud;
				caseMat->valeurTmp = temp_chaud;
				caseMat->estChauffante = 1;	
			} else {		
				caseMat->valeur = temp_froid;
				caseMat->valeurTmp = temp_froid;
				caseMat->estChauffante = 0;	
			}
			
		}
	}
}

//creation d'une matrice carree de la taille passee en parametre 
caseDansMat * creationMatrice(int taille, double temp_froid){
	caseDansMat * mat = malloc( taille * taille * sizeof(caseDansMat));
	return mat;
}

//affiche la matrice sur la sortie standard
void afficheMatriceStandard(caseDansMat * mat, int taille){
	/*for(int i = 0 ; i < taille ; ++i){
		for(int j = 0 ; j < taille ; ++j){
			caseDansMat * tmp = mat + i * taille + j;
			printf("|%.2f|", tmp->valN);
		}
		printf("\n");
	}*/
	int d = 0;
	int tailleMax = taille * taille;
	while(d < tailleMax){	
		printf("|%.2f|", round(mat[d].valeur*100)/100);
		if(d++ != 0 && d % taille == 0)
			printf("\n");
	}
}

//ecrit la matrice dans le fichier passe en parametre
void afficheMatriceFile(caseDansMat ** mat, int taille, FILE * fic){
	for(int i = 0 ; i < taille  ; ++i){
		for(int j = 0 ; j < taille ; ++j){
			fprintf(fic, "|%d|", (int)mat[i][j].valeur );//round(mat[i][j].valN*100)/100);
		}
		fprintf(fic,"\n");
	}
}

//Affiche le quart superieur de la matrice
void afficheQuartMatrice(caseDansMat * mat, int taille){
	int pas = taille / 16; //on affiche une valeur tous les 2^s indices, soit tous les taille/16 indices
	
	int tailleMax = taille * taille;
	int i = 0, j = 0, cpt = 1;
	for(int d = 0; d < 64; ++cpt){ 
		for(; i < taille / 2; j += pas){
			printf("|%d:%.2f|", j, round(mat[j].valeur*100)/100);
			i += pas;
			++d;
		}
		printf("\n");
		i = 0; // on remet à 0 pour l'itération suivante
		j = cpt * taille * pas; //parcours d'indice, permet d'aller à la ligne suivante
	}
}

//Affecte la valeur N+1 à la variable N
//Reinitialise la valeur N+1
void miseAJourMatrice(caseDansMat ** mat, int taille, double coefSurCase, double temp_froid){
	for(int i = 0 ; i < taille  ; ++i){
		for(int j = 0 ; j < taille ; ++j){
			if(! mat[i][j].estChauffante){
				mat[i][j].valeur = mat[i][j].valeur * coefSurCase + mat[i][j].valeurTmp;
				mat[i][j].valeurTmp = temp_froid;
			} else {
				mat[i][j].valeurTmp = mat[i][j].valeur;
			}
		}
	}
}

//Supprime la matrice en paramètre
void suppressionMatrice(caseDansMat * mat, int taille){
	/*for(int i = 0 ; i < taille ; ++i)
		free(mat[i]);*/
	free(mat);
}