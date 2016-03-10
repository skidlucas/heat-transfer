/**
 *
 * Auteurs : Lucas Martinez, Lucas Soumille
 *
 * Simulation de la progression de la chaleur sur la surface
 *
 */

#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include "heatTransfer.h"
#include <pthread.h>

caseDansMat * matGeneral;
int tailleTotale;
int compteur = 0;

/**
 * Permet de simuler une diffusion de la chaleur de maniere horizontale autour de la
 * case passee en parametre
 *
 * @author   Lucas Soumille, Lucas Martinez
 */
void * simulationHori(void * infos){
	caseAndIndex * matAvecInfos = (caseAndIndex *) infos;
	caseDansMat * caseMat;
	caseDansMat * caseMatGauche;
	caseDansMat * caseMatDroite;
	for(int i = matAvecInfos->indXDeb ; i < matAvecInfos->indXFin ; ++i){
		for(int j = matAvecInfos->indYDeb ; j < matAvecInfos->indYFin  ; ++j){
			caseMat = matGeneral + i * tailleTotale + j;
			caseMatGauche = matGeneral + i * tailleTotale + (j - 1);
			caseMatDroite = matGeneral + i * tailleTotale + (j + 1); 
			caseMat->valeurTmp = (4.0 * caseMat->valeur + caseMatDroite->valeur + caseMatGauche->valeur) / 6;
		}
	}	
}

/**
 * Permet de simuler une diffusion de la chaleur de maniere verticale autour de la
 * case passee en parametre
 *
 * @author   Lucas Soumille, Lucas Martinez
 */
void * simulationVerti(void * infos){
	caseAndIndex * matAvecInfos = (caseAndIndex *)infos;
	caseDansMat * caseMat;
	caseDansMat * caseMatHaut;
	caseDansMat * caseMatBas;
	for(int i = matAvecInfos->indXDeb ; i < matAvecInfos->indXFin ; ++i){
		for(int j = matAvecInfos->indYDeb ; j < matAvecInfos->indYFin  ; ++j){
			caseMat = matGeneral + i * tailleTotale + j;
			//si la case est chauffante on fait pas la mise a jour de sa valeur
			if(caseMat->estConstante == 1) 
				continue;
			caseMatHaut = matGeneral + (i - 1) * tailleTotale + j;
			caseMatBas = matGeneral + (i + 1) * tailleTotale + j;
			caseMat->valeur = (4.0 * caseMat->valeurTmp + caseMatBas->valeurTmp + caseMatHaut->valeurTmp) / 6;
			
		}
	}
}

/**
 * Fonction de simulation appelee par chaque thread
 *
 * @author   Lucas Soumille 
 */
void * simulationPourChaqueThread(void * infos){
	caseAndIndex * infosNbIter = (caseAndIndex *)infos;
	//printf("debut simulation thread\n");
	printf("nb Iter : %d\n", infosNbIter->nbIter);
	for(int i = 0 ; i < (infosNbIter->nbIter) ; ++i){
		simulationHori(infos);
		printf("compteur %d\n", ++compteur);
		pthread_barrier_wait(infosNbIter->barriereMil);
		printf("passage barriereMil\n");
		simulationVerti(infos);
	//	printf("bloquer verticale %d\n", i);
		//pthread_barrier_wait(infosNbIter->barriereFin);	
		pthread_barrier_wait(infosNbIter->barriereMil);
	}
	//printf("Adresse barriere execution : %d\n",infosNbIter->barriereExecution);
	//printf("Passage compteur : %d\n", ++compteur);
	pthread_barrier_wait(infosNbIter->barriereExecution);
	pthread_exit(NULL);
}

/**
 * Permet de simuler une iteration de propagation de chaleur
 *
 * @author   Lucas Soumille, Lucas Martinez
 */
void simulation(int taille, int nbIter, int nbThread, caseDansMat * mat, int etape){
	printf("debut simulation nbThread %d\n", nbThread);
	//compteur = 0;
	pthread_t * allThread = malloc(nbThread  * sizeof(pthread_t));
	caseAndIndex * infos = malloc(nbThread * sizeof(caseAndIndex));
	pthread_barrier_t * barriereMil = malloc(sizeof(pthread_barrier_t));
	pthread_barrier_t * barriereFin = malloc(sizeof(pthread_barrier_t));
	pthread_barrier_t * barriereExecution = malloc(sizeof(pthread_barrier_t));
	int cpt = 0;
	int rc;
	int pas = sqrt(taille * taille / nbThread);
	if(pas < 1){
		pas = 1;
		nbThread = taille * taille;
	} // si le nombre de thread est trop grand 
	pthread_barrier_init(barriereMil, NULL, nbThread);
	pthread_barrier_init(barriereFin, NULL, nbThread);
	pthread_barrier_init(barriereExecution, NULL, nbThread + 1);
	tailleTotale = taille + 2;
	matGeneral = mat;
	for(int i = 1 ; i <= taille ; i += pas){
		for(int j = 1 ; j <= taille ; j += pas){
			infos[cpt].indXDeb = i;
			infos[cpt].indYDeb = j;
			infos[cpt].indXFin = i + pas;
			infos[cpt].indYFin = j + pas;
			infos[cpt].nbIter = nbIter;
			infos[cpt].barriereMil = barriereMil;
			infos[cpt].barriereFin = barriereFin;
			infos[cpt].barriereExecution = barriereExecution;
			//printf("adresse mil %d\n", infos[cpt].barriereMil);
			rc = pthread_create(&(allThread[cpt]), NULL, simulationPourChaqueThread,(void *) &infos[cpt]);
			if(rc){
				printf("%d\n", rc);
			}
			printf("cpt : %d | taille  :%d | pas : %d | nbThread :%d\n", cpt++, taille, pas, nbThread);
		}
	}
	waitFunction(barriereExecution);
	pthread_barrier_destroy(barriereMil);
	pthread_barrier_destroy(barriereFin);
	pthread_barrier_destroy(barriereExecution);
	free(barriereFin);
	free(barriereMil);
	free(barriereExecution); //to do destroy + free
	free(infos);
	free(allThread);
}

void waitFunction(pthread_barrier_t * barrier){
	pthread_barrier_wait(barrier);
}