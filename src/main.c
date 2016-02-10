/*
*
* Auteurs : Lucas Martinez / Lucas Soumille
*
* main qui crée une plaque et lance la simulation
*
*/

#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include "heatTransfer.h"

/* Pour la compilation en -std=c11 */
int getopt (int argc, char * const argv[],
                  const char *optstring);
 
extern char *optarg;
extern int optind, opterr, optopt;
 
#include <getopt.h>

/* flags pour savoir les options activées */
enum Flags {
	OPT_S       = 0x01,
	OPT_M       = 0x02,
	OPT_BIGM    = 0x04,
	OPT_A       = 0x08,
	OPT_I       = 0x10,
	OPT_E 	  	= 0x20,
	OPT_T		= 0x30
};

int flags;
int N = 4;
int TAILLE_GRILLE = 1;
int NB_ITER = 10000;
int ETAPE = 0;
int NB_THREADS = 1;
clock_t start, end;
double TEMP_FROID = 0.0;
double TEMP_CHAUD = 10000.0;

int isNumber(char * s){
	int i = 0;
    while (s[i]){
        if (!isdigit(s[i]))
            return 0;
        ++i;
    }
    return 1;
}

void checkOptions(int argc, char * argv[]){
	int c;
	while ((c = getopt(argc , argv, "s:mMai:e:t:")) != -1){
		switch (c) {
	    case 's':
	      flags += OPT_S;
	      printf("option s\n");  //tmp
	      if(strlen(optarg) == 1 && isdigit(optarg[0])){
	      	//N = atoi(optarg); c'est quoi ce N ?
	      	TAILLE_GRILLE = TAILLE_GRILLE << (atoi(optarg) + 4);
	      	printf("TAILLE_GRILLE = %d\n", TAILLE_GRILLE); //tmp
	      } else {
	      	printf("Argument error : a number (0 <= x <= 9) was expected.\n");
	      }
	      break;
	    case 'm':
	      printf("option m\n");
	      flags += OPT_M;
	      start = clock();
	      break;
	    case 'M':
	      flags += OPT_BIGM;
	      printf("option M\n");
	      break;
	    case 'a':
	      flags += OPT_A;
	      printf("option a\n");
	      break;
	    case 'i':
	      flags += OPT_I;
	      printf("option i\n");
	      if(atoi(optarg)){
	      	NB_ITER = atoi(optarg);
	      	printf("nb_iter = %d\n", NB_ITER); //tmp
	      } else {
	      	printf("Erreur d'argument : un nombre supérieur à 0 est attendu.\n");
	      }
	      break;
	    case 'e':
	      flags += OPT_E;
	      printf("option e\n");

	      if(strlen(optarg) == 1 && isdigit(optarg[0])){
	      	int tmp = atoi(optarg);
	      	if (tmp >= 0 && tmp <= 5){
	      		ETAPE = tmp;
	      		printf("ETAPE = %d\n", ETAPE); //tmp
	      	} else {
	      		printf("Erreur d'argument : le nombre doit etre compris entre 0 et 5.\n");
	      	}	
	      } else {
	      	printf("Erreur d'argument : un nombre compris entre 0 et 5 est attendu.\n");
	      }
	     
	      break;
	    case 't':
	      flags += OPT_T;
	      printf("option t\n");
	      
	      if(strlen(optarg) == 1 && isdigit(optarg[0])){
	      	int tmp = atoi(optarg);
	      	if (tmp >= 0 && tmp <= 5){
	      		NB_THREADS = NB_THREADS << tmp;
	      		NB_THREADS *= NB_THREADS;
	      		printf("NB_THREADS = %d\n", NB_THREADS); //tmp
	      	} else {
	      		printf("Erreur d'argument : le nombre doit etre compris entre 0 et 5.\n");
	      	}	
	      } else {
	      	printf("Erreur d'argument : un nombre compris entre 0 et 5 est attendu.\n");
	      }
	      break;
	    case '?':
	      printf("Case ?\n");
	      break;
	    default:
	      //printf("c);
	      break;
	    }
	}
}

int main(int argc, char * argv[]){
	checkOptions(argc, argv);
	printf("startmain\n");
	FILE* fichier = fopen("test.txt", "w+");
	TAILLE_GRILLE = 16;
	NB_ITER = 10;
	caseDansMat ** mat = creationMatrice(TAILLE_GRILLE, TEMP_FROID);
	printf("after create\n");
	positionneCaseChauffante(mat, N, TEMP_CHAUD);
	double coefCase = 16.0/36.0;
	double coefHori = 4.0/36.0;
	double coefDiag = 1.0/36.0;
	for(int i = 0 ; i < NB_ITER ; ++i)
		simulationIteration(mat, TAILLE_GRILLE, coefCase, coefHori, coefDiag, TEMP_FROID);
	afficheMatriceFile(mat, TAILLE_GRILLE, fichier);
	suppressionMatrice(mat, TAILLE_GRILLE);

	if(flags & OPT_M){
		end = clock();
		double total = (double) (end - start) / CLOCKS_PER_SEC;
		printf("Temps total de consommation CPU: %f\n", total);

	}
	return 0;
} 