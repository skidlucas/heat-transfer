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
#include <sys/resource.h>
#include "heatTransfer.h"


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

clock_t start_cpu, end_cpu;
time_t start_user, end_user;
double TEMP_FROID = 0.0;
double TEMP_CHAUD = 36.0;
int NB_EXECUTION = 1;
struct rusage usage;

//par defaut :
int N[10] = {4, 6, 8};
int TAILLE_GRILLE[10] = {16, 64, 256};
int NB_ITER = 10000;
int ETAPE[6] = {0, 1, 2, 3};
int NB_THREADS[6] = {4, 64};
int nbExecTaille = 3;
int nbExecEtape = 4;
int nbExecThread = 2;


/**
 * Permet de recuperer les parametres passees en argument lors de l'execution du programme
 *
 * @author Lucas Soumille, Lucas Martinez
 */
void checkOptions(int argc, char * argv[]){
	int c;

	while ((c = getopt(argc , argv, "s:mMai:e:t:")) != -1){
		switch (c) {
			int size_opt;
	    case 's':
	      flags += OPT_S;
	      nbExecTaille = 0;
	      size_opt = strlen(optarg);

	      for (int i = 0; i < size_opt; ++i){
	      	if (!isdigit(optarg[i])) {
	      		printf("-s => Erreur d'argument : un nombre compris entre 0 et 9 est attendu. \n");
	      		exit(1);
	      	}

	      	N[i] = optarg[i] - '0' + 4; //transforme optarg[i] en int
	      	TAILLE_GRILLE[i] = 1 << N[i];
	      	++nbExecTaille;
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
	      nbExecEtape = 0;
	      size_opt = strlen(optarg);
	      for (int i = 0; i < size_opt; ++i){
	      	if (!isdigit(optarg[i])){
	      		printf("-e => Erreur d'argument : un nombre compris entre 0 et 5 est attendu.\n");
	      		exit(1);
	      	}
	      	int tmp = optarg[i] - '0';
	      	if (tmp >= 0 && tmp <= 5){
	      		ETAPE[i] = tmp;
	      		++nbExecEtape;
	      	} else {
	      		printf("-e => Erreur d'argument : le nombre doit etre compris entre 0 et 5.\n");
	      		exit(1);
	      	}
	      }
	      break;
	    case 't':
	      flags += OPT_T;
	      nbExecThread = 0;
	      size_opt = strlen(optarg);
	      for (int i = 0; i < size_opt; ++i){
	      	if (!isdigit(optarg[i])){
	      		printf("-t => Erreur d'argument : un nombre compris entre 0 et 5 est attendu.\n");
	      		exit(1);
	      	}
	      	int tmp = optarg[i] - '0';
	      	if (tmp >= 0 && tmp <= 5){
	      		NB_THREADS[i] = 1 << tmp;
	      		NB_THREADS[i] *= NB_THREADS[i];
	      		++nbExecThread;
	      	} else {
	      		printf("-t => Erreur d'argument : le nombre doit etre compris entre 0 et 5.\n");
	      		exit(1);
	      	}
	      }
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
void execute(double * tab_cpu, double * tab_user, int etape, int taille, int n, int nbThread){
	caseDansMat * mat = creationMatrice(taille, TEMP_FROID);
	for(int i = 0 ; i < NB_EXECUTION ; ++i){
		start_cpu = clock();
		start_user = time(NULL);
		initMatrice(mat, taille, n, TEMP_FROID, TEMP_CHAUD);
		if(i == 0 && (flags & OPT_A)){ //afficher la matrice avec les valeurs initiales une seule fois
			printf("Valeurs initiales de la plaque:\n");
			afficheQuartMatrice(mat, taille);
		}

		initSimulation(taille, etape, NB_ITER, nbThread, mat);

		end_cpu = clock();
		end_user = time(NULL);
		tab_cpu[i] = (double) (end_cpu - start_cpu) / CLOCKS_PER_SEC;
		tab_user[i] = (double) (end_user - start_user);
	}
	if(flags & OPT_A){
		printf("Valeurs finales de la plaque:\n");
		afficheQuartMatrice(mat, taille);
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

/**
 * Permet d'afficher les metriques
 *
 * @author Lucas Martinez
 */
void afficherInfos(double* tempsCpuExecute, double* tempsUserExecute){
	if(flags & OPT_M)
		printf("Temps total de consommation CPU d'une itération (en sec): %f\n", calculMoyenne(tempsCpuExecute));
	if(flags & OPT_BIGM)
		printf("Temps total utilisateur d'une itération (en sec): %f\n", calculMoyenne(tempsUserExecute));
	if(getrusage(RUSAGE_SELF, &usage) != -1)
		printf("Empreinte mémoire: %ld ko\n", usage.ru_maxrss);
}


int main(int argc, char * argv[]){
	checkOptions(argc, argv);
	double tempsCpuExecute[NB_EXECUTION], tempsUserExecute[NB_EXECUTION];

	for(int i = 0; i < nbExecEtape; ++i){
		for(int j = 0; j < nbExecTaille; ++j){
			for(int k = 0; k < nbExecThread; ++k){
				if (ETAPE[i] == 0){
					printf("\t\t--- ETAPE 0 ---\n");
					printf("*** Plaque de %d*%d, avec 1 thread ***\n", TAILLE_GRILLE[j], TAILLE_GRILLE[j]);
					execute(tempsCpuExecute, tempsUserExecute, 0, TAILLE_GRILLE[j], N[j], 1);
					afficherInfos(tempsCpuExecute, tempsUserExecute);
					break;
				} else {
					printf("\t\t--- ETAPE %d ---\n", ETAPE[i]);
					printf("*** Plaque de %d*%d, avec %d threads ***\n", TAILLE_GRILLE[j], TAILLE_GRILLE[j], NB_THREADS[k]);
					execute(tempsCpuExecute, tempsUserExecute, ETAPE[i], TAILLE_GRILLE[j], N[j], NB_THREADS[k]);
					afficherInfos(tempsCpuExecute, tempsUserExecute);
				}
			}
		}
	}
	return 0;
}
