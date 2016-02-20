/**
 *
 * Auteurs : Lucas Martinez, Lucas Soumille
 *
 * Permet de gerer la matrice correspondant a la plaque
 *
 */
#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include <math.h>
#include <stdlib.h>
#include "matrice.h"


/**
 * Permet de creer une matrice carree de la taille passee en parametre
 *
 * @author   Lucas Soumille
 */
caseDansMat * creationMatrice(int taille, double temp_froid){
	caseDansMat * mat = malloc( taille * taille * sizeof(caseDansMat));
	return mat;
}


/**
 * Permet d'initialiser les cases dans la matrice
 *
 * @author   Lucas Soumille
 */
void initMatrice(caseDansMat * mat, int taille, int N, double temp_froid, double temp_chaud){
	int minInd = (1 << (N - 1)) - (1 << (N - 4));
	int maxInd = (1 << (N - 1)) + (1 << (N - 4));
	caseDansMat * caseMat;
	for(int i = 0 ; i < taille ; ++i){
		for(int j = 0 ; j < taille ; ++j){
			caseMat = mat + i * taille + j;
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


/**
 * Permet d'afficher la matrice sur la sortie standard
 *
 * @author   Lucas Soumille
 */
void afficheMatriceStandard(caseDansMat * mat, int taille){
	int d = 0;
	int tailleMax = taille * taille;
	while(d < tailleMax){	
		printf("|%.2f|", round(mat[d].valeur*100)/100);
		if(d++ != 0 && d % taille == 0)
			printf("\n");
	}
}

/**
 * Permet d'ecrire la matrice dans le fichier passe en parametre
 *
 * @author   Lucas Soumille
 */
void afficheMatriceFile(caseDansMat * mat, int taille, FILE * fic){
	int d = 0;
	int tailleMax = taille * taille;
	while(d < tailleMax){	
		fprintf(fic, "|%.2f|", round(mat[d].valeur*100)/100);
		if(d++ != 0 && d % taille == 0)
			fprintf(fic, "\n");
	}
}

/**
 * Permet d'afficher les valeurs aux indices i % 2^s = 0 du quart superieur gauche de la matrice
 *
 * @author   Lucas Soumille, Lucas Martinez
 */
void afficheQuartMatrice(caseDansMat * mat, int taille){
	int pas = taille / 16; //on affiche une valeur tous les 2^s indices, soit tous les taille/16 indices
	
	int indMil = taille / 2;
	caseDansMat * caseMat;
	for(int i = 0 ; i < indMil ; i += pas){
		for(int j = 0 ; j < indMil ; j += pas){
			caseMat = mat + i * taille + j;
			printf("|%.2f|", round(caseMat->valeur*100)/100);
		}
		printf("\n");
	}
	printf("\n");
}

/**
 * Permet de supprimer la matrice donnee en parametre
 *
 * @author   Lucas Soumille
 */
void suppressionMatrice(caseDansMat * mat){
	free(mat);
}