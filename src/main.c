/**
 *
 * Auteurs : Lucas Martinez, Lucas Soumille
 *
 * Programme principal qui cree une plaque et lance la simulation
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
 


/* Flags pour savoir quelles options sont activees */
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
clock_t start_cpu, end_cpu;
time_t start_user, end_user;
double TEMP_FROID = 0.0;
double TEMP_CHAUD = 36.0;
int NB_EXECUTION = 1;

/**
 * Permet de recuperer les parametres passees en argument lors de l'execution du programme
 *
 * @author Lucas Soumille, Lucas Martinez
 */
void checkOptions(int argc, char * argv[]){
	int c;
	while ((c = getopt(argc , argv, "s:mMai:e:t:")) != -1){
		switch (c) {
	    case 's':
	      flags += OPT_S;
	      if(strlen(optarg) == 1 && isdigit(optarg[0])){
	      	N = atoi(optarg) + 4;
	      	TAILLE_GRILLE = 1 << N;
	      } else {
	      	printf("-s => Erreur d'argument : un nombre compris entre 0 et 9 est attendu.\n");
	      }
	      break;
	    case 'm':
	      flags += OPT_M;
	      NB_EXECUTION = 10;
	      break;
	    case 'M':
	      flags += OPT_BIGM;
	      NB_EXECUTION = 10;	      
	      break;
	    case 'a':
	      flags += OPT_A;
	      break;
	    case 'i':
	      flags += OPT_I;
	      if(atoi(optarg))
	      	NB_ITER = atoi(optarg);
	      else 
	      	printf("-i => Erreur d'argument : un nombre supérieur à 0 est attendu.\n");
	      break;
	    case 'e':
	      flags += OPT_E;
	      if(strlen(optarg) == 1 && isdigit(optarg[0])){
	      	int tmp = atoi(optarg);
	      	if (tmp >= 0 && tmp <= 5)
	      		ETAPE = tmp;
	      	else
	      		printf("-e => Erreur d'argument : le nombre doit etre compris entre 0 et 5.\n");
	      		
	      } else
	      	printf("-e => Erreur d'argument : un nombre compris entre 0 et 5 est attendu.\n");
	      break;
	    case 't':
	      flags += OPT_T;
	      if(strlen(optarg) == 1 && isdigit(optarg[0])){
	      	int tmp = atoi(optarg);
	      	if (tmp >= 0 && tmp <= 5){
	      		NB_THREADS = NB_THREADS << tmp;
	      		NB_THREADS *= NB_THREADS;
	      	} else 
	      		printf("-t => Erreur d'argument : le nombre doit etre compris entre 0 et 5.\n");	
	      } else
	      		printf("-t => Erreur d'argument : un nombre compris entre 0 et 5 est attendu.\n");
	      break;
	    case '?':
	      break;
	    default:
	      break;
	    }
	}
}


/**
 * Permet d'executer la simulation du transfert de chaleur en fonction des parametres
 * passes precedemment.
 *
 * @author   Lucas Soumille, Lucas Martinez
 */
void execute(double * tab_cpu, double * tab_user){
	caseDansMat * mat = creationMatrice(TAILLE_GRILLE, TEMP_FROID);
	for(int i = 0 ; i < NB_EXECUTION ; ++i){
		start_cpu = clock();
		start_user = time(NULL);
		initMatrice(mat, TAILLE_GRILLE, N, TEMP_FROID, TEMP_CHAUD);
		if(i == 0 && (flags & OPT_A)){ //afficher la matrice avec les valeurs initiales une seule fois
			printf("Valeurs initiales de la plaque:\n");
			afficheMatriceStandard(mat, TAILLE_GRILLE);
		} 	 
		for(int j = 0 ; j < NB_ITER ; ++j){
		//	simulationIteration(TAILLE_GRILLE, NB_THREADS, mat);
		}
		end_cpu = clock();
		end_user = time(NULL);
		tab_cpu[i] = (double) (end_cpu - start_cpu) / CLOCKS_PER_SEC;
		tab_user[i] = (double) (end_user - start_user);
	}
	if(flags & OPT_A){
		printf("Valeurs finales de la plaque:\n");
		afficheQuartMatrice(mat, TAILLE_GRILLE);
	}
	suppressionMatrice(mat);	
}

/**
 * Permet de supprimer les valeurs extremes de mesure
 *
 * @author   Lucas Soumille, Lucas Martinez
 */
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

/**
 * Permet de calculer la moyenne des mesures des executions
 *
 * @author   Lucas Soumille, Lucas Martinez
 */
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
	double tempsCpuExecute[NB_EXECUTION], tempsUserExecute[NB_EXECUTION];
	execute(tempsCpuExecute, tempsUserExecute);
	if(flags & OPT_M)
		printf("Temps total de consommation CPU d'une itération (en sec): %f\n", calculMoyenne(tempsCpuExecute));
	if(flags & OPT_BIGM)
		printf("Temps total utilisateur d'une itération (en sec): %f\n", calculMoyenne(tempsUserExecute));
	return 0;
} 
