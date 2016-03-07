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

pthread_barrier_t barriere;
caseDansMat * matGeneral;
int tailleTotale;

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
	simulationHori(infos);
	pthread_barrier_wait(&barriere);
	simulationVerti(infos);
	pthread_exit(NULL);
}

/**
 * Permet de simuler une iteration de propagation de chaleur
 *
 * @author   Lucas Soumille, Lucas Martinez
 */
void simulationIteration(int taille, int nbThread, caseDansMat * mat, int etape, int premiere, int derniere){
	int cpt = 0;
	pthread_t * allThread = malloc(nbThread  * sizeof(pthread_t));
	caseAndIndex * infos = malloc(nbThread * sizeof(caseAndIndex));
	int pas = sqrt(taille * taille / nbThread);
	if(pas < 0) // si le nombre de thread est trop grand 
		pas = 1;
	tailleTotale = taille + 2;
	matGeneral = mat;
	if(premiere)
		pthread_barrier_init(&barriere, NULL, nbThread);
	for(int i = 1 ; i <= taille ; i += pas){
		for(int j = 1 ; j <= taille ; j += pas){
			infos[cpt].indXDeb = i;
			infos[cpt].indYDeb = j;
			infos[cpt].indXFin = i + pas;
			infos[cpt].indYFin = j + pas;
			pthread_create(&(allThread[cpt]), NULL, simulationPourChaqueThread,(void *) &infos[cpt]);
			cpt++;
		}
	}
	for(int k = 0; k < nbThread; ++k)
		pthread_join(allThread[k], NULL);
	if(derniere)
		pthread_barrier_destroy(&barriere);
	free(infos);
	free(allThread);
}
