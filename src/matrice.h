/**
 *
 * Auteurs : Lucas Martinez, Lucas Soumille
 *
 * Permet de gerer la matrice correspondant à la plaque
 *
 */

#include <stdio.h>

/* struct qui represente une case de la plaque */
typedef struct {
	double valeur;
	double valeurTmp;
	int estConstante;
} caseDansMat;

/**
 * Permet de creer une matrice carree de la taille passee en parametre
 */
caseDansMat * creationMatrice(int taille,  double temp_froid);

/**
 * Permet d'initialiser les cases dans la matrice
 */
void initMatrice(caseDansMat * mat, int taille, int N, double temp_froid, double temp_chaud);

/**
 * Permet d'afficher les valeurs aux indices i % 2^s = 0 du quart superieur gauche de la matrice
 */
void afficheQuartMatrice(caseDansMat * mat, int taille);
	
/**
 * Permet de supprimer la matrice donnee en parametre
 */
void suppressionMatrice(caseDansMat * mat);
