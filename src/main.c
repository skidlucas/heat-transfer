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
#include <sys/resource.h>


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
int TAILLE_GRILLE = 16;
int NB_ITER = 10000;
int ETAPE = 0;
int NB_THREADS = 1;
clock_t start, end;
double TEMP_FROID = 0.0;
double TEMP_CHAUD = 10000000.0;
int NB_EXECUTION = 1;

//verifie que s est un chiffre
int isNumber(char * s){
	int i = 0;
    while (s[i]){
        if (!isdigit(s[i]))
            return 0;
        ++i;
    }
    return 1;
}

//recuperation des arguments
void checkOptions(int argc, char * argv[]){
	int c;
	while ((c = getopt(argc , argv, "s:mMai:e:t:")) != -1){
		switch (c) {
	    case 's':
	      flags += OPT_S;
	      printf("option s\n");  //tmp
	      if(strlen(optarg) == 1 && isdigit(optarg[0])){
	      	N = atoi(optarg) + 4;
	      	TAILLE_GRILLE = 1 << N;
	      	//N = atoi(optarg);
	      	//TAILLE_GRILLE = TAILLE_GRILLE << (atoi(optarg) + 4);
	      	printf("TAILLE_GRILLE = %d\n", TAILLE_GRILLE);
	      } else {
	      	printf("Argument error : a number (0 <= x <= 9) was expected.\n");
	      }
	      break;
	    case 'm':
	      printf("option m\n");
	      flags += OPT_M;
	      NB_EXECUTION = 10;
	      //start = clock();
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


//simulation d'un scénario / retourne un tableau des temps d'execution
void execute(double * tab){
	caseDansMat * mat = creationMatrice(TAILLE_GRILLE, TEMP_FROID);
	for(int i = 0 ; i < NB_EXECUTION ; ++i){
		start = clock();
		initMatrice(mat, TAILLE_GRILLE, TEMP_FROID);
		positionneCaseChauffante(mat, TAILLE_GRILLE, N, TEMP_CHAUD);
		for(int j = 0 ; j < NB_ITER ; ++j){
			simulationIteration(TAILLE_GRILLE, N, mat);
		}
		end = clock();
		tab[i] = (double) (end - start) / CLOCKS_PER_SEC;
	}
	if(flags & OPT_A)
		afficheQuartMatrice(mat, TAILLE_GRILLE);//afficheMatriceStandard(mat, TAILLE_GRILLE);
	suppressionMatrice(mat, TAILLE_GRILLE);	
}

//supprime les deux plus petites valeurs
void supprimeMins(double * tab){
	double valMin = tab[0];
	int indMin = 0;
	double valMax = tab[0];
	int indMax = 0;
	for(int i = 1 ; i < NB_EXECUTION ; ++i) {
		if(tab[i] < valMin) {
			valMin = tab[i];
			indMin = i;
		}
		if(tab[i] > valMax){
			valMax = tab[i];
			indMax = i;
		}
	}
	tab[indMax] = tab[indMin] = 0.0;
}

//calcule la moyenne des executions
double calculMoyenne(double * tab){
	supprimeMins(tab);
	double total = 0;
	if(NB_EXECUTION == 1 || NB_EXECUTION == 2)
		return tab[0];
	for(int i = 0 ; i < NB_EXECUTION ; ++i)
		total += tab[i];
	return total / (NB_EXECUTION - 2);
}

int main(int argc, char * argv[]){
	checkOptions(argc, argv);
	double tempsExecute[NB_EXECUTION];
	execute(tempsExecute);
	struct rusage ru;
		getrusage(RUSAGE_SELF, &ru);
    	long maxrss = ru.ru_maxrss;
 		printf("RUSAGE :ru_maxrss => %ld [kilobytes], %ld [struct Cell], %ld [nb Cell]\n", maxrss, maxrss / sizeof(caseDansMat) * 1024, (2 + (1 << (TAILLE_GRILLE + 4))) * (2 + (1 << (TAILLE_GRILLE + 4))));
	if(flags & OPT_M){
		printf("Temps total de consommation CPU: %f\n", calculMoyenne(tempsExecute));

	}
	return 0;
} 
