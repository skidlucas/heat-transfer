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
int TAILLE_GRILLE[10] = {16, 64, 256};
int N[10] = {4, 6, 8};
int nbTaille = 3;
int NB_ITER = 10000;
int ETAPE[6] = {0, 1, 2, 3};
int nbEtape = 4;
int NB_THREADS[6] = {4, 64};
int nbThread = 2;
clock_t start_cpu, end_cpu;
time_t start_user, end_user;
double TEMP_FROID = 0.0;
double TEMP_CHAUD = 36.0;
int NB_EXECUTION = 10;

struct rusage usage;

/**
 * Permet de recuperer les parametres passees en argument lors de l'execution du programme
 *
 * @author Lucas Soumille, Lucas Martinez
 */
void checkOptions(int argc, char * argv[])
{
	int c;
	while ((c = getopt(argc , argv, "s:mMai:e:t:")) != -1)
	{
		switch (c) {
	    case 's':
	      flags += OPT_S;
	      int cpt = 0; //utilisation d'une variable locale
	      int size = strlen(optarg);
	      for(int i = 0 ; i < size ; ++i, ++cpt)
	      {
	      	if((optarg[i] - '0') < 0 || (optarg[i] - '0') > 9)
	      		continue;
	      	N[i] = (optarg[i] - '0') + 4;
	      	TAILLE_GRILLE[i] = 1 << N[i]; //equivalent a utiliser pow 
	      }
	      nbTaille = cpt;
	      break;
	    case 'm':
	      flags += OPT_M;
	      NB_EXECUTION = 6;
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
	      cpt = 0;
	      size = strlen(optarg);
	      for(int i = 0 ; i < size ; ++i, cpt++)
	      {
	      	if((optarg[i] - '0') < 0 || (optarg[i] - '0') > 5)
	      		continue;
	      	ETAPE[i] = optarg[i] - '0'; //conversion en int  
	 		++nbEtape;
	      }
	      nbEtape = cpt;
	      break;
	    case 't':
	      flags += OPT_T;
	      cpt = 0;
	      size = strlen(optarg);
	      for(int i = 0 ; i < size ; ++i, ++cpt)
	      {
	      	if((optarg[i] - '0') < 0 || (optarg[i] - '0') > 5)
	      		continue;
	      	int nb = 1 << (optarg[i] - '0');
	      	NB_THREADS[i] = nb * nb; 
	      }
	      nbThread = cpt;
	      break;
	    default:
	      flags+= OPT_M;
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
void execute(double * tab_cpu, double * tab_user, int taille, int n, int nbEtape, int nbThread)
{
	caseDansMat * mat = creationMatrice(taille, TEMP_FROID);
	for(int i = 0 ; i < NB_EXECUTION ; ++i)
	{
		start_cpu = clock();
		start_user = time(NULL);
		initMatrice(mat, taille, n, TEMP_FROID, TEMP_CHAUD);
		 //afficher la matrice avec les valeurs initiales
		if(i == 0 && (flags & OPT_A))
		{
			printf("Valeurs initiales de la plaque:\n");
			afficheQuartMatrice(mat, taille, n);
		} 	 
		simulation(taille, NB_ITER, nbThread, mat, nbEtape);
		//calcul de l'empreinte mémoire pour une exécution
		end_cpu = clock();
		end_user = time(NULL);
		tab_cpu[i] = (double) (end_cpu - start_cpu) / CLOCKS_PER_SEC;
		tab_user[i] = (double) (end_user - start_user);
	}
	if(flags & OPT_A)
	{
		printf("Valeurs finales de la plaque:\n");
		afficheQuartMatrice(mat, taille, n);
	}
	if(getrusage(RUSAGE_SELF, &usage) != -1)
		printf("empreinte mémoire max d'une execution %ld kb\n", usage.ru_maxrss);
	suppressionMatrice(mat);	
}

/**
 * Permet de supprimer les valeurs extremes de mesure
 *
 * @author   Lucas Soumille, Lucas Martinez
 */
void supprimeMins(double * tab)
{
	double valMin = tab[0];
	int indMin = 0;
	double valMax = tab[0];
	int indMax = 0;
	for(int i = 1 ; i < NB_EXECUTION ; ++i)
	{
		if(tab[i] < valMin) 
		{
			valMin = tab[i];
			indMin = i;
		}
		if(tab[i] > valMax)
		{
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
double calculMoyenne(double * tab)
{
	supprimeMins(tab);
	double total = 0;
	if(NB_EXECUTION == 1 || NB_EXECUTION == 2)
		return tab[0];
	for(int i = 0 ; i < NB_EXECUTION ; ++i)
		total += tab[i];
	return total / (NB_EXECUTION - 2);
}


int main(int argc, char * argv[])
{
	checkOptions(argc, argv);
	double tempsCpuExecute[NB_EXECUTION], tempsUserExecute[NB_EXECUTION];
	for(int i = 0 ; i < nbEtape ; ++i)
	{
		for(int j = 0 ; j < nbTaille ; ++j)
		{
			for(int k = 0 ; k < nbThread ; ++k)
			{
				if(ETAPE[i] == 0)
				{
					printf("\nExecution de l'étape 0 sur une plaque de taille %d * %d\n", TAILLE_GRILLE[j], TAILLE_GRILLE[j]);
					execute(tempsCpuExecute, tempsUserExecute, TAILLE_GRILLE[j], N[j], 0, 1);
					if(flags & OPT_M)
						printf("Temps total de consommation CPU d'une execution (en sec): %f\n", calculMoyenne(tempsCpuExecute));
					if(flags & OPT_BIGM)
						printf("Temps total utilisateur d'une execution (en sec): %f\n", calculMoyenne(tempsUserExecute));	
					break;
				} 
				else 
				{
					printf("\nExecution de l'étape %d sur une plaque de taille %d * %d avec %d thread(s)\n", 
					ETAPE[i], TAILLE_GRILLE[j], TAILLE_GRILLE[j], NB_THREADS[k]);
					execute(tempsCpuExecute, tempsUserExecute, TAILLE_GRILLE[j], N[j], ETAPE[i], NB_THREADS[k]);
					if(flags & OPT_M)
						printf("Temps total de consommation CPU d'une execution (en sec): %f\n", calculMoyenne(tempsCpuExecute));
					if(flags & OPT_BIGM)
						printf("Temps total utilisateur d'une execution (en sec): %f\n", calculMoyenne(tempsUserExecute));	
				}
			}
		}
	}
	return 0;
} 
