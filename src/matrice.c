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

//creation d'une matrice carree de la taille passee en parametre 
caseDansMat ** creationMatrice(int taille, double temp_froid){
	caseDansMat ** mat = (caseDansMat **)malloc(taille * sizeof(caseDansMat));
	for(int i = 0 ; i < taille ; ++i)
		mat[i] = (caseDansMat *)malloc(taille  * sizeof(caseDansMat));
	for(int i = 0 ; i < taille ; ++i){
		for(int j = 0 ; j < taille  ; ++j){
			mat[i][j].valN = temp_froid;
			mat[i][j].valNPlus1 = temp_froid;
			mat[i][j].estChauffante = 0;
		}
	}
	return mat;
}

//affiche la matrice sur la sortie standard
void afficheMatriceStandard(caseDansMat ** mat, int taille){
	for(int i = 0 ; i < taille ; ++i){
		for(int j = 0 ; j < taille ; ++j){
			printf("|%f|", round(mat[i][j].valN*100)/100);
		}
		printf("\n");
	}
}

//ecrit la matrice dans le fichier passe en parametre
void afficheMatriceFile(caseDansMat ** mat, int taille, FILE * fic){
	for(int i = 0 ; i < taille  ; ++i){
		for(int j = 0 ; j < taille ; ++j){
			fprintf(fic, "|%d|", (int)mat[i][j].valN );//round(mat[i][j].valN*100)/100);
		}
		fprintf(fic,"\n");
	}
}

//Affiche le quart superieur de la matrice
void afficheQuartMatrice(caseDansMat ** mat, int taille){
	for(int i = 0 ; i < taille / 2 ; ++i){
		for(int j = 0 ; j < taille / 2 ; ++j){
			printf("|%.2f|", round(mat[i][j].valN*100)/100);
		}
		printf("\n");
	}
}

//Affecte la valeur N+1 à la variable N
//Reinitialise la valeur N+1
void miseAJourMatrice(caseDansMat ** mat, int taille, double coefSurCase, double temp_froid){
	for(int i = 0 ; i < taille  ; ++i){
		for(int j = 0 ; j < taille ; ++j){
			if(! mat[i][j].estChauffante){
				mat[i][j].valN = mat[i][j].valN * coefSurCase + mat[i][j].valNPlus1;
				mat[i][j].valNPlus1 = temp_froid;
			} else {
				mat[i][j].valNPlus1 = mat[i][j].valN;
			}
		}
	}
}

//Supprime la matrice en paramètre
void suppressionMatrice(caseDansMat ** mat, int taille){
	for(int i = 0 ; i < taille ; ++i)
		free(mat[i]);
	free(mat);
}