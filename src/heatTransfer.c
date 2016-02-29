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

/**
 * Permet de simuler une diffusion de la chaleur de maniere horizontale autour de la
 * case passee en parametre
 *
 * @author   Lucas Soumille, Lucas Martinez
 */
/*void * simulationHori(caseDansMat * mat, int taille){
	caseDansMat * caseMat;
	caseDansMat * caseMatGauche;
	caseDansMat * caseMatDroite; 
	for(int i = 0 ; i < taille  ; ++i){
		for(int j = 0 ; j < taille ; ++j){
			caseMat = mat + i * taille + j;
			caseMatGauche = mat + i * taille + (j - 1);
			caseMatDroite = mat + i * taille + (j + 1); 
			if(j == 0)
				caseMat->valeurTmp = (4.0 * caseMat->valeur + caseMatDroite->valeur) / 6;
			else if (j == taille - 1) 
				caseMat->valeurTmp = (4.0 * caseMat->valeur + caseMatGauche->valeur) / 6;
			else 
				caseMat->valeurTmp = (4.0 * caseMat->valeur + caseMatDroite->valeur + caseMatGauche->valeur) / 6;
			
		}
	}
	pthread_barriere_wait(&barriere);
}*/

void * simulationHori(void * infos){
	printf("debut simulation horizontale\n");
	caseAndIndex * matAvecInfos = (caseAndIndex *)infos;
	caseDansMat * caseMat;
	caseDansMat * caseMatGauche;
	caseDansMat * caseMatDroite;
	int taille = matAvecInfos->indXFin - matAvecInfos->indXDeb + 2;
	printf("X fin : %d\n", matAvecInfos->indXFin);
	printf("Y fin : %d\n", matAvecInfos->indYFin);
	for(int i = matAvecInfos->indXDeb - 1 ; i < matAvecInfos->indXFin - 1; ++i){
		for(int j = matAvecInfos->indYDeb - 1 ; j < matAvecInfos->indYFin - 1 ; ++j){
			caseMat = matAvecInfos->mat + i * taille + j;
			printf("Id = %d\n", caseMat->estConstante);
			caseMatGauche = matAvecInfos->mat + i * taille + (j - 1);
			caseMatDroite = matAvecInfos->mat + i * taille + (j + 1); 
			/*if(j == 0)
				caseMat->valeurTmp = (4.0 * caseMat->valeur + caseMatDroite->valeur) / 6;
			else if (j == taille - 1) 
				caseMat->valeurTmp = (4.0 * caseMat->valeur + caseMatGauche->valeur) / 6;
			else {*/
			caseMat->valeurTmp = (4.0 * caseMat->valeur + caseMatDroite->valeur + caseMatGauche->valeur) / 6;
			//}
		}
	}
	printf("fin simulation horizontale\n");
}

/**
 * Permet de simuler une diffusion de la chaleur de maniere verticale autour de la
 * case passee en parametre
 *
 * @author   Lucas Soumille, Lucas Martinez
 */
/*void * simulationVerti(caseDansMat * mat, int taille){
	caseDansMat * caseMat;
	caseDansMat * caseMatHaut;
	caseDansMat * caseMatBas;
	for(int i = 0 ; i < taille  ; ++i){
		for(int j = 0 ; j < taille ; ++j){
			caseMat = mat + i * taille + j;
			//si la case est chauffante on fait pas la mise a jour de sa valeur
			if(caseMat->estChauffante == 1) continue;
			caseMatHaut = mat + (i - 1) * taille + j;
			caseMatBas = mat + (i + 1) * taille + j; 
			if(i == 0)
				caseMat->valeur = (4.0 * caseMat->valeurTmp + caseMatBas->valeurTmp)  / 6;
			else if (i == taille - 1) 
				caseMat->valeur = (4.0 * caseMat->valeurTmp + caseMatHaut->valeurTmp) / 6;
			else 
				caseMat->valeur = (4.0 * caseMat->valeurTmp + caseMatBas->valeurTmp + caseMatHaut->valeurTmp) / 6;
			
		}
	}
}*/

void * simulationVerti(void * infos){
	printf("debut simulation verticale\n");
	caseAndIndex * matAvecInfos = (caseAndIndex *)infos;
	caseDansMat * caseMat;
	caseDansMat * caseMatHaut;
	caseDansMat * caseMatBas;
	int taille = matAvecInfos->indXFin - matAvecInfos->indXDeb + 2;
	for(int i = matAvecInfos->indXDeb - 1 ; i < matAvecInfos->indXFin - 1; ++i){
		for(int j = matAvecInfos->indYDeb - 1 ; j < matAvecInfos->indYFin - 1 ; ++j){
			caseMat = matAvecInfos->mat + i * taille + j;
			//si la case est chauffante on fait pas la mise a jour de sa valeur
			if(caseMat->estConstante == 1) 
				continue;
			caseMatHaut = matAvecInfos->mat + (i - 1) * taille + j;
			caseMatBas = matAvecInfos->mat + (i + 1) * taille + j;
			/*if(i == 0)
				caseMat->valeur = (4.0 * caseMat->valeurTmp + caseMatBas->valeurTmp)  / 6;
			else if (i == taille - 1) 
				caseMat->valeur = (4.0 * caseMat->valeurTmp + caseMatHaut->valeurTmp) / 6;*/
			//else 
			caseMat->valeur = (4.0 * caseMat->valeurTmp + caseMatBas->valeurTmp + caseMatHaut->valeurTmp) / 6;
			
		}
	}
	//pthread_barrier_wait(&barriere);
	printf("fin simulation vertical\n");
}

void * simulation(void * infos){
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
void simulationIteration(int taille, int nbThread, caseDansMat * mat){
	/*simulationHori(mat, taille);
	simulationVerti(mat, taille);*/
	printf("debut simulation\n");
	int cpt = 0;
	pthread_t * allThread = malloc(nbThread  * sizeof(pthread_t));
	caseAndIndex * infos = malloc(nbThread * sizeof(caseAndIndex));
	int pas = sqrt(taille * taille / nbThread);// car on veut pas déborder
	int tailleTotale = taille + 2;
	pthread_barrier_init(&barriere, NULL, nbThread);
	/*for(int i = 0 ; i < tailleTotale ; i += pas + 1){
		for(int j = 0 ; j < tailleTotale ; j += pas + 1){
			//printf("i : %d / j : %d", i , j);
			if(i == 0 || j == 0 || i == tailleTotale - 1 || j == tailleTotale - 1)continue;
			infos[cpt].mat = mat + i * taille + j;
			printf("%f\n", infos[cpt].mat->valeur);
			infos[cpt].indXDeb = i + 1;
			infos[cpt].indYDeb = j + 1;
			infos[cpt].indXFin = i + pas + 1;
			infos[cpt].indYFin = j + pas + 1;
			//pthread_create(&allThread[cpt], NULL, simulationHori, (void *)&infos[cpt]);
			//pthread_create(&allThread[cpt], NULL, simulationVerti, (void *)&infos[cpt]);
			printDatas(infos, nbThread);
			pthread_create(&(allThread[cpt]), NULL, simulation, (void *)&infos[cpt]);
			++cpt;
		}
	}*/
	for(int i = 1 ; i <= taille ; i += pas){
		for(int j = 1 ; j <= taille ; j += pas){
			printf("i : %d / j : %d\n", i, j);
			infos[cpt].mat = mat + i * tailleTotale + j;
			infos[cpt].indXDeb = i;
			infos[cpt].indYDeb = j;
			infos[cpt].indXFin = i + pas;
			infos[cpt].indYFin = j + pas;
			//pthread_create(&allThread[cpt], NULL, simulationHori, (void *)&infos[cpt]);
			//pthread_create(&allThread[cpt], NULL, simulationVerti, (void *)&infos[cpt]);
			//printDatas(infos, nbThread);
			pthread_create(&(allThread[cpt]), NULL, simulation, (void *)&infos[cpt]);
			++cpt;
		}
	}
	for(cpt = 0 ; cpt < nbThread; ++cpt)
	{
		pthread_join(allThread[cpt], NULL);
	}
	pthread_barrier_destroy(&barriere);
	free(infos);
	free(allThread);
	printf("fin simulation\n");
}

void printDatas(caseAndIndex * cas, int nbThread){
	for(int i = 0 ; i < nbThread ; ++i){
		printf("indX : %d\n", cas[i].indXDeb);
		printf("indY : %d\n", cas[i].indYDeb);
		printf("oldX : %d\n", cas[i].indXFin);
		printf("oldY : %d\n", cas[i].indYFin);
		printf("idCase : %d\n", cas[i].mat->estConstante);
	}
}
