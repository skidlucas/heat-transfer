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
caseDansMat * creationMatrice(int taille, double temp_froid)
{
	caseDansMat * mat = malloc( (taille + 2) * (taille + 2) * sizeof(caseDansMat));
	if(mat == NULL)
	{
		printf("Erreur dans l'allocation mémoire de la matrice \n");
		exit(1);
	}
	return mat;
}


/**
 * Permet d'initialiser les cases dans la matrice
 *
 * @author   Lucas Soumille
 */
void initMatrice(caseDansMat * mat, int taille, int N, double temp_froid, double temp_chaud)
{
	int minInd = (1 << (N - 1)) - (1 << (N - 4)) + 1;
	int maxInd = (1 << (N - 1)) + (1 << (N - 4)) + 1;
	caseDansMat * caseMat;
	int tailleLigne = taille + 2;
	for(int i = 0 ; i <= taille + 1 ; ++i)
	{
		for(int j = 0 ; j <= taille + 1 ; ++j)
		{
			caseMat = mat + i * tailleLigne + j;
			if(i >= minInd && i < maxInd && j >= minInd && j < maxInd)
			{
				caseMat->valeur = temp_chaud;
				caseMat->valeurTmp = temp_chaud;
				caseMat->estConstante = 1;	
			} 
			else if( i == 0 || j == 0 || i == taille + 1 || j == taille + 1)
			{
				caseMat->valeur = temp_froid;
				caseMat->valeurTmp = temp_froid;
				caseMat->estConstante = 1;
			} 
			else 
			{		
				caseMat->valeur = temp_froid;
				caseMat->valeurTmp = temp_froid;
				caseMat->estConstante = 0;	
			}
		}
	}
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
	for(int i = 0 ; i <= taille + 1 ; ++i)
	{
		for(int j = 0 ; j <= taille + 1 ; ++j)
		{
			caseMat = mat + i * tailleLigne + j;
			printf("%d|", (int) caseMat->valeur);
		}
		printf("\n");
	}
	printf("\n");
}

/**
 * Permet d'afficher les valeurs aux indices i % 2^s = 0 du quart superieur gauche de la matrice
 *
 * @author   Lucas Soumille, Lucas Martinez
 */
void afficheQuartMatrice(caseDansMat * mat, int taille, int n)
{
	int pas = taille / 16; //on affiche une valeur tous les 2^s indices, soit tous les taille/16 indices
	int indMil = taille / 2;
	caseDansMat * caseMat;
	for(int i = 1 ; i <= indMil  ; i += pas)
	{
		for(int j = 1 ; j <= indMil ; j += pas)
		{
			caseMat = mat + i * (taille + 2) + j;
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
void suppressionMatrice(caseDansMat * mat)
{
	free(mat);
}