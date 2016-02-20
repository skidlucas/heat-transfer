/*
*
* Auteurs : Lucas Martinez / Lucas Soumille
*
* Permet de gerer la matrice correspondant à la plaque
*
*/

#include <stdio.h>

/* struct qui represente une case de la plaque */
typedef struct {
	double valeur;
	double valeurTmp;
	int estChauffante;
} caseDansMat;

//creation d'une matrice carree de la taille passee en parametre 
caseDansMat * creationMatrice(int taille,  double temp_froid);

//Initialise les cases dans le matrice
void initMatrice(caseDansMat * mat, int taille, int N, double temp_froid, double temp_chaud);

//affiche la matrice sur la sortie standard
void afficheMatriceStandard(caseDansMat * mat, int taille);

//ecrit la matrice dans le fichier passe en parametre
void afficheMatriceFile(caseDansMat ** mat, int taille, FILE * fic);

//Affiche le quart superieur de la matrice
void afficheQuartMatrice(caseDansMat * mat, int taille);

//Supprime la matrice en parametre
void suppressionMatrice(caseDansMat * mat);
