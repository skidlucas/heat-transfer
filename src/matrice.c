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

int tailleLigneBords;

/**
 * Permet de creer une matrice carree de la taille passee en parametre
 *
 * @author   Lucas Soumille, Lucas Martinez
 */
caseDansMat * creationMatrice(int taille, double temp_froid){
	size_t vraieTailleMat = ((taille + 2) * (taille + 2) * sizeof(caseDansMat)); //a cause des bords
	caseDansMat * mat = malloc(vraieTailleMat);
	
	if (mat){
		return mat;
	} else {
		perror("Erreur d'allocation mémoire, arret du programme.");
		exit(1);
	}
}


/**
 * Permet d'initialiser les cases dans la matrice
 *
 * @author   Lucas Soumille, Lucas Martinez
 */
void initMatrice(caseDansMat * mat, int taille, int N, double temp_froid, double temp_chaud){
	int minInd = (1 << (N - 1)) - (1 << (N - 4)) + 1;
	int maxInd = (1 << (N - 1)) + (1 << (N - 4)) + 1;
	caseDansMat * caseMat;
	tailleLigneBords = taille + 2; //bord

	for(int i = 0 ; i <= taille + 1; ++i){ //+ 1 car bord
		for(int j = 0 ; j <= taille + 1; ++j){
			caseMat = mat + i * tailleLigneBords + j;
			if(i >= minInd && i < maxInd && j >= minInd && j < maxInd){ //milieu chauffant
				caseMat->valeur = temp_chaud;
				caseMat->valeurTmp = temp_chaud;
				caseMat->estConstante = 1;	
			} else if (i == 0 || i == taille + 1 || j == 0 || j == taille + 1){ //bord froid
				caseMat->valeur = temp_froid;
				caseMat->valeurTmp = temp_froid;
				caseMat->estConstante = 1;
			} else {		
				caseMat->valeur = temp_froid;
				caseMat->valeurTmp = temp_froid;
				caseMat->estConstante = 0;	
			}
		}
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
	tailleLigneBords = taille + 2;

	for(int i = 1 ; i <= indMil ; i += pas){
		for(int j = 1 ; j <= indMil ; j += pas){
			caseMat = mat + i * tailleLigneBords + j;
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