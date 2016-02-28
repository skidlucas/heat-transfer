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
	caseDansMat * mat = malloc( (taille + 2) * (taille + 2) * sizeof(caseDansMat));
	return mat;
}


/**
 * Permet d'initialiser les cases dans la matrice
 *
 * @author   Lucas Soumille
 */
void initMatrice(caseDansMat * mat, int taille, int N, double temp_froid, double temp_chaud){
	int minInd = (1 << (N - 1)) - (1 << (N - 4)) + 1;
	int maxInd = (1 << (N - 1)) + (1 << (N - 4)) + 1;
	caseDansMat * caseMat;
	int tailleLigne = taille + 2;
	for(int i = 0 ; i <= taille + 1 ; ++i){
		for(int j = 0 ; j <= taille + 1 ; ++j){
			printf("j : %d\n",j );
			caseMat = mat + i * tailleLigne + j;
			if(i >= minInd && i < maxInd && j >= minInd && j < maxInd){
				caseMat->valeur = temp_chaud;
				caseMat->valeurTmp = temp_chaud;
				caseMat->estChauffante = 1;	
			} else if( i == 0 || j == 0 || i == taille + 1 || j == taille + 1){
					printf("taille %d\n", i);
				caseMat->valeur = -1;
				caseMat->valeurTmp = -1;
				caseMat->estChauffante = 1;
			} else {		
				caseMat->valeur = temp_froid;
				caseMat->valeurTmp = temp_froid;
				caseMat->estChauffante = 0;	
			}
		}
	}
	/*
	for(int i = 0 ; i <= taille + 1; ++i){
		for(int j = 0 ; j <= taille + 1 ; j += taille + 1){
			caseMat = mat + i * taille + j;
			caseMat->valeur = -1;
			caseMat->valeurTmp = -1;
			caseMat->estChauffante = 1;
		}
	}*/
}


/**
 * Permet d'afficher la matrice sur la sortie standard
 *
 * @author   Lucas Soumille
 */
 // a refaire
void afficheMatriceStandard(caseDansMat * mat, int taille){
	caseDansMat * caseMat;
	int tailleLigne = taille + 2;
	for(int i = 0 ; i <= taille + 1 ; ++i){
		for(int j = 0 ; j <= taille + 1 ; ++j){
			caseMat = mat + i * tailleLigne + j;
			printf("|%.2f|", round(caseMat->valeur*100)/100);
		}
		printf("\n");
	}
	printf("\n");
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
 * Sépare la matrice en fonction du nombre de parties voulues
 * Retourne un tableau contenant la première case des sous matrices
 *
 * @author Lucas Soumille
 */
int * separeMatrice(caseDansMat * mat, int taille, int nbThread){
	printf("taille %d\n", taille);
	int nbSeparationParCote = nbThread / 2;
	/*if(taille < nbSeparationParCote)
		return mat;*/
	//caseDansMat * tabSousMatrice[nbThread];
	int * adrDebSousMat = (int *)malloc(nbThread * sizeof(int));
	int pas = sqrt(taille * taille / nbThread);
	printf("%d\n", pas);
	int cpt = 0;
	for(int i = 0 ; i < taille ; i += pas){
		for(int j = 0 ; j < taille ; j += pas, cpt++){
			adrDebSousMat[cpt] = &mat + i * taille + j;
			printf("cpt %d adresse case = %d\n", cpt, adrDebSousMat[cpt]);
		}
	}
	return adrDebSousMat;
}

/**
 * Permet de supprimer la matrice donnee en parametre
 *
 * @author   Lucas Soumille
 */
void suppressionMatrice(caseDansMat * mat){
	free(mat);
}