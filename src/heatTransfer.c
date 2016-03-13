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
#include <limits.h>

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
			caseMat = matAvecInfos->matGeneral + i * matAvecInfos->tailleTotale + j;
			caseMatGauche = matAvecInfos->matGeneral + i * matAvecInfos->tailleTotale + (j - 1);
			caseMatDroite = matAvecInfos->matGeneral + i * matAvecInfos->tailleTotale + (j + 1); 
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
			caseMat = matAvecInfos->matGeneral + i * matAvecInfos->tailleTotale + j;
			//si la case est chauffante on fait pas la mise a jour de sa valeur
			if(caseMat->estConstante == 1) 
				continue;
			caseMatHaut = matAvecInfos->matGeneral + (i - 1) * matAvecInfos->tailleTotale + j;
			caseMatBas = matAvecInfos->matGeneral + (i + 1) * matAvecInfos->tailleTotale + j;
			caseMat->valeur = (4.0 * caseMat->valeurTmp + caseMatBas->valeurTmp + caseMatHaut->valeurTmp) / 6;
			
		}
	}
}

/**
 * Fonction de simulation appelee par chaque thread pour l'etape 1
 *
 * @author   Lucas Soumille 
 */
void * simulationEtape1(void * infos){
	caseAndIndex * infosNbIter = (caseAndIndex *)infos;
	int erreur;
	for(int i = 0 ; i < (infosNbIter->nbIter) ; ++i){
		simulationHori(infos);
		erreur = pthread_barrier_wait(infosNbIter->barriereMil);
		if(erreur && erreur != PTHREAD_BARRIER_SERIAL_THREAD){
			printf("Erreur dans le wait de la barriere milieu (erreur %d)\n", erreur);
			exit(1);
		}
		simulationVerti(infos);
		erreur = pthread_barrier_wait(infosNbIter->barriereFin);
		if(erreur && erreur != PTHREAD_BARRIER_SERIAL_THREAD){
			printf("Erreur dans le wait de la barriere fin (erreur %d)\n", erreur);
			exit(1);
		}
	}
	pthread_exit(NULL);
}

/**
 * Fonction de simulation appelee par chaque thread pour l'etape 0
 *
 * @author   Lucas Soumille 
 */
void * simulationEtape0(void * infos){
	caseAndIndex * infosNbIter = (caseAndIndex *)infos;
	for(int i = 0 ; i < (infosNbIter->nbIter) ; ++i){
		simulationHori(infos);
		simulationVerti(infos);	
	}
}

/**
 * Permet de simuler une iteration de propagation de chaleur
 *
 * @author   Lucas Soumille, Lucas Martinez
 */
void simulation(int taille, int nbIter, int nbThread, caseDansMat * mat, int etape){
	int erreur;
	caseAndIndex * infos = malloc(nbThread * sizeof(caseAndIndex));
	if(infos == NULL){
		printf("Erreur dans l'allocation mémoire dans la simulation");
		exit(1);
	}
	int cpt = 0;
	int pas = sqrt(taille * taille / nbThread);
	// si le nombre de thread est trop grand 
	if(pas < 1){
		pas = 1;
		nbThread = taille * taille;
	}
	if(etape == 0){
		infos[0].indXDeb = 1;
		infos[0].indYDeb = 1;
		infos[0].indXFin = taille + 1;
		infos[0].indYFin = taille + 1;
		infos[0].nbIter = nbIter;
		infos[0].matGeneral = mat;
		infos[0].tailleTotale = taille + 2;
		simulationEtape0((void*)&infos[0]);
		free(infos);
		return;
	}
	void * fonctionAExecuter = fonctionEtape(etape);
	pthread_t * allThread = malloc(nbThread  * sizeof(pthread_t));
	pthread_barrier_t * barriereMil = malloc(sizeof(pthread_barrier_t));
	pthread_barrier_t * barriereFin = malloc(sizeof(pthread_barrier_t));
	if(allThread == NULL || barriereMil == NULL || barriereFin == NULL){
		printf("Erreur dans l'allocation mémoire dans la simulation");
		exit(1);
	}
	//initialisation des barrieres
	erreur = pthread_barrier_init(barriereMil, NULL, nbThread);
	if(erreur) {
		printf("Erreur dans l'initialisation de la barriere du milieu (erreur %d)\n", erreur);
		exit(1);
	}
	erreur = pthread_barrier_init(barriereFin, NULL, nbThread);
	if(erreur) {
		printf("Erreur dans l'initialisation de la barriere de fin (erreur %d)\n", erreur);
		exit(1);
	}
	//Lancement des threads
	for(int i = 1 ; i <= taille ; i += pas){
		for(int j = 1 ; j <= taille ; j += pas){
			infos[cpt].indXDeb = i;
			infos[cpt].indYDeb = j;
			infos[cpt].indXFin = i + pas;
			infos[cpt].indYFin = j + pas;
			infos[cpt].nbIter = nbIter;
			infos[cpt].barriereMil = barriereMil;
			infos[cpt].barriereFin = barriereFin;
			infos[cpt].matGeneral = mat;
			infos[cpt].tailleTotale = taille + 2;
			erreur = pthread_create(&(allThread[cpt]), NULL, fonctionAExecuter,(void *) &infos[cpt]);
			if(erreur){
				printf("Erreur dans la creation du thread (erreur %d)\n", erreur);
				exit(1);
			}
			cpt++;
		}
	}
	//on attend la fin des threads
	for(int i = 0 ; i < cpt ; ++i){
		erreur = pthread_join(allThread[i], NULL);
		if(erreur){
			printf("Erreur dans le join d'un thread (erreur %d)\n", erreur);
			exit(1);
		}
	}
	//on rend la mémoire
	erreur = pthread_barrier_destroy(barriereMil);
	if(erreur) 
		printf("Erreur dans la destruction de la barriere du milieu (erreur %d)\n", erreur);
	erreur = pthread_barrier_destroy(barriereFin);
	if(erreur) 
		printf("Erreur dans la destruction de la barriere de fin (erreur %d)\n", erreur);		
	free(barriereFin);
	free(barriereMil);
	free(infos);
	free(allThread);
}

/**
 * Retourne le pointeur sur la fonction a executer pour l'etape passee en parametre
 * de la simulation
 *
 * @Author Lucas Soumille
 */
void * fonctionEtape(int etape){
	return &simulationEtape1;
}