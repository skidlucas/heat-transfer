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
int SIZE_GRID = 16;
int NB_ITER = 10000;
int STEP = 0;
int NB_THREADS = 4;
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
	      	N = atoi(optarg);
	      	SIZE_GRID = SIZE_GRID << (atoi(optarg) + 4);
	      	printf("size_grid = %d\n", SIZE_GRID); //tmp
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
	      	printf("Argument error : a number (> 0) was expected.\n");
	      }
	      break;
	    case 'e':
	      flags += OPT_E;
	      printf("option e\n");
	      // comme s mais en vérifiant 0 <= optarg <= 5
	      // chercher la meilleure manière de faire

	      //STEP = atoi(optarg);
	      break;
	    case 't':
	      flags += OPT_T;
	      printf("option t\n");
	      // comme s mais en vérifiant 0 <= optarg <= 5
	      // chercher la meilleure manière de faire

	      //NB_THREADS = NB_THREADS << atoi(optarg); (sauf si optarg = 0...)
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
	SIZE_GRID = 16;
	NB_ITER = 10;
	caseDansMat ** mat = creationMatrice(SIZE_GRID, TEMP_FROID);
	printf("after create\n");
	positionneCaseChauffante(mat, N, TEMP_CHAUD);
	double coefCase = 16.0/36.0;
	double coefHori = 4.0/36.0;
	double coefDiag = 1.0/36.0;
	for(int i = 0 ; i < NB_ITER ; ++i)
		simulationIteration(mat, SIZE_GRID, coefCase, coefHori, coefDiag, TEMP_FROID);
	afficheMatriceFile(mat, SIZE_GRID, fichier);
	suppressionMatrice(mat, SIZE_GRID);

	if(flags & OPT_M){
		end = clock();
		double total = (double) (end - start) / CLOCKS_PER_SEC;
		printf("Temps total de consommation CPU: %f\n", total);

	}
	return 0;
} 