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

/**
 * Destruction des barrieres (du mutex et de la variables condition)
 *
 * @author   Lucas Soumille
 */
void barrier_destroy(barrier_t *b)
{
	int erreur;
	erreur = pthread_cond_destroy(&b->cv);
	if(erreur)
	{
		printf("erreur dans la destruction de la condition (erreur %d)\n", erreur);
		exit(1);
	}
	erreur = pthread_mutex_destroy(&b->m);
	if(erreur)
	{
		printf("erreur dans la destruction du mutex (erreur %d)\n", erreur);
		exit(1);
	}
}

/**
 * Initialisation des barrieres en particulier du mutex, de la variable condition
 * et des compteurs
 *
 * @author   Lucas Soumille
 */
void barrier_init(barrier_t * b, unsigned limite)
{
	int erreur;
	erreur = pthread_mutex_init(&b->m, NULL);
	if(erreur)
	{
		printf("erreur dans l'initialisation du mutex (erreur %d)\n", erreur);
		exit(1);
	}
	erreur = pthread_cond_init(&b->cv, NULL);
	if(erreur)
	{
		printf("erreur dans l'initialisation de la condition (erreur %d)\n", erreur);
		exit(1);
	}
	b->limite = limite;
	b->count = 0;
}

/**
 * Initialisation des barrieres en particulier des deux semaphores
 * et des compteurs
 *
 * @author   Lucas Soumille
 */
void barrier_sema_init(barrier_sema_t * b, unsigned limite)
{
	int erreur;
	erreur = sem_init(&b->m, 0, 1);
	if(erreur)
	{
		printf("erreur dans l'initialisation de la semaphore m(erreur %d)\n", erreur);
		exit(1);
	}
	erreur = sem_init(&b->attente, 0, 0);
	if(erreur)
	{
		printf("erreur dans l'initialisation de la semaphore attente(erreur %d)\n", erreur);
		exit(1);
	}
	b->limite = limite;
	b->count = 0;
}

/**
 * Destruction des barrieres (des semaphores)
 *
 * @author   Lucas Soumille
 */
void barrier_sema_destroy(barrier_sema_t * b)
{
	int erreur;
	erreur = sem_destroy(&b->m);
	if(erreur)
	{
		printf("erreur dans la destruction de la semaphore (erreur %d)\n", erreur);
		exit(1);
	}
	erreur = sem_destroy(&b->attente);
	if(erreur)
	{
		printf("erreur dans la destruction de la semaphore (erreur %d)\n", erreur);
		exit(1);
	}
}


/**
 * 
 * Implementation de la fonction barriere wait en utilisant les semaphores
 *
 * @author   Lucas Soumille
 */
int barriere_sema_wait(barrier_sema_t * b)
{
	int erreur;
	int returnValeur;
	//entree en section critique
	erreur = sem_wait(&b->m);
	if(erreur) 
	{
		printf("Erreur dans le wait de la semaphore mutex (erreur %d)\n", erreur);
		exit(1);
	}
	//un thread supplementaire est arrive
	b->count++;
	if (b->count >= b->limite)
	{
		//on reveille les autres threads
		for(int i = 1 ; i < b->limite ; ++i)
		{
			erreur = sem_post(&b->attente);
			if(erreur) 
			{	
				printf("Erreur dans le post de la semaphore attente (erreur %d)\n", erreur);
				exit(1);
			}
		}

		//reinitialisation du compteur
		b->count = 0;
		//on deverouille le mutex
		erreur = sem_post(&b->m);
		if(erreur) 
		{	
			printf("Erreur dans le post de la semaphore mutex (erreur %d)\n", erreur);
			exit(1);
		}
		returnValeur = PTHREAD_BARRIER_SERIAL_THREAD;
	}
	else
	{	
		//deverouille le mutex et attente
		erreur = sem_post(&b->m);
		if(erreur) 
		{	
			printf("Erreur dans le post de la semaphore mutex (erreur %d)\n", erreur);
			exit(1);
		}
		erreur = sem_wait(&b->attente);
		if(erreur) 
		{	
			printf("Erreur dans le unlock du mutex (erreur %d)\n", erreur);
			exit(1);
		}
		returnValeur = 0;
	}
	return returnValeur;
}

/**
 * 
 * Implementation de la fonction barriere wait en utilisant les variables conditions (moniteur)
 *
 * @author   Lucas Soumille
 */
int barrier_wait(barrier_t *b)
{
	int erreur;
	int returnValeur;
	//entree en section critique
	erreur = pthread_mutex_lock(&b->m);
	if(erreur) 
	{
		printf("Erreur dans le lock du mutex (erreur %d)\n", erreur);
		exit(1);
	}
	//un thread supplementaire est arrive
	b->count++;
	//si le thread est le dernier attendu
	if (b->count >= b->limite)
	{
		//reinitialisation du compteur
		b->count = 0;
		//on reveille les autres threads
		erreur = pthread_cond_broadcast(&b->cv);
		if(erreur) {
			printf("Erreur dans le broadcast (erreur %d)\n", erreur);
			exit(1);
		}
		//on deverouille le mutex
		erreur = pthread_mutex_unlock(&b->m);
		if(erreur) 
		{	
			printf("Erreur dans le unlock du mutex (erreur %d)\n", erreur);
			exit(1);
		}
		returnValeur = PTHREAD_BARRIER_SERIAL_THREAD;
	}
	else
	{	
		//les threads se mettent en attente et deverouille le mutex
		while (b->count != 0) 
			pthread_cond_wait(&b->cv, &b->m);
		erreur = pthread_mutex_unlock(&b->m);
		if(erreur) 
		{	
			printf("Erreur dans le unlock du mutex (erreur %d)\n", erreur);
			exit(1);
		}
		returnValeur = 0;
	}
	return returnValeur;
}

/**
 * Permet de simuler une diffusion de la chaleur de maniere horizontale autour de la
 * case passee en parametre
 *
 * @author   Lucas Soumille, Lucas Martinez
 */
void simulationHori(void * infos)
{
	caseAndIndex * matAvecInfos = (caseAndIndex *) infos;
	caseDansMat * caseMat;
	caseDansMat * caseMatGauche;
	caseDansMat * caseMatDroite;
	for(int i = matAvecInfos->indXDeb ; i < matAvecInfos->indXFin ; ++i)
	{
		for(int j = matAvecInfos->indYDeb ; j < matAvecInfos->indYFin  ; ++j)
		{
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
void simulationVerti(void * infos)
{
	caseAndIndex * matAvecInfos = (caseAndIndex *)infos;
	caseDansMat * caseMat;
	caseDansMat * caseMatHaut;
	caseDansMat * caseMatBas;
	for(int i = matAvecInfos->indXDeb ; i < matAvecInfos->indXFin ; ++i)
	{
		for(int j = matAvecInfos->indYDeb ; j < matAvecInfos->indYFin  ; ++j)
		{
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
 * Fonction de simulation appelee par chaque thread pour l'etape 3
 *
 * @author   Lucas Soumille 
 */
void simulationEtape3(void * infos) 
{
	caseAndIndex * infosNbIter = (caseAndIndex *)infos;
	int erreur;
	for(int i = 0 ; i < (infosNbIter->nbIter) ; ++i){
		simulationHori(infos);
		erreur = barriere_sema_wait(infosNbIter->maBarriereSemaMil);
		if(erreur && erreur != PTHREAD_BARRIER_SERIAL_THREAD)
		{
			printf("Erreur dans le wait de la barriere milieu (erreur %d)\n", erreur);
			exit(1);
		}
		simulationVerti(infos);
		erreur = barriere_sema_wait(infosNbIter->maBarriereSemaFin);
		if(erreur && erreur != PTHREAD_BARRIER_SERIAL_THREAD)
		{
			printf("Erreur dans le wait de la barriere fin (erreur %d)\n", erreur);
			exit(1);
		}
		
	}
	pthread_exit(NULL);
}

/**
 * Fonction de simulation appelee par chaque thread pour l'etape 2
 *
 * @author   Lucas Soumille 
 */
void simulationEtape2(void * infos) 
{
	caseAndIndex * infosNbIter = (caseAndIndex *)infos;
	int erreur;
	for(int i = 0 ; i < (infosNbIter->nbIter) ; ++i){
		simulationHori(infos);
		erreur = barrier_wait(infosNbIter->maBarriereMil);
		if(erreur && erreur != PTHREAD_BARRIER_SERIAL_THREAD)
		{
			printf("Erreur dans le wait de la barriere milieu (erreur %d)\n", erreur);
			exit(1);
		}
		simulationVerti(infos);
		erreur = barrier_wait(infosNbIter->maBarriereFin);
		if(erreur && erreur != PTHREAD_BARRIER_SERIAL_THREAD)
		{
			printf("Erreur dans le wait de la barriere fin (erreur %d)\n", erreur);
			exit(1);
		}
		
	}
	pthread_exit(NULL);
}

/**
 * Fonction de simulation appelee par chaque thread pour l'etape 1
 *
 * @author   Lucas Soumille 
 */
void simulationEtape1(void * infos)
{
	caseAndIndex * infosNbIter = (caseAndIndex *)infos;	
	int erreur;
	for(int i = 0 ; i < (infosNbIter->nbIter) ; ++i)
	{
		simulationHori(infos);
		erreur = pthread_barrier_wait(infosNbIter->barriereMil);
		if(erreur && erreur != PTHREAD_BARRIER_SERIAL_THREAD)
		{
			printf("Erreur dans le wait de la barriere milieu (erreur %d)\n", erreur);
			exit(1);
		}
		simulationVerti(infos);
		erreur = pthread_barrier_wait(infosNbIter->barriereFin);
		if(erreur && erreur != PTHREAD_BARRIER_SERIAL_THREAD)
		{
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
void simulationEtape0(void * infos)
{
	caseAndIndex * infosNbIter = (caseAndIndex *)infos;
	for(int i = 0 ; i < (infosNbIter->nbIter) ; ++i)
	{
		simulationHori(infos);
		simulationVerti(infos);	
	}
}

//retourne le pas
void * initialisation(int etape, int taille, int nbThread, int nbIter, caseDansMat * mat, int pas)
{
	int erreur;
	int cpt = 0;
	caseAndIndex * infos = malloc(nbThread * sizeof(caseAndIndex));
	if(infos == NULL)
	{
		printf("Erreur dans l'allocation mémoire des informations pour les threads\n");
		exit(1);
	}
	if(etape == 0)
	{
		infos[0].indXDeb = 1;
		infos[0].indYDeb = 1;
		infos[0].indXFin = taille + 1;
		infos[0].indYFin = taille + 1;
		infos[0].nbIter = nbIter;
		infos[0].matGeneral = mat;
		infos[0].tailleTotale = taille + 2;
		return infos;
	} 
	else if (etape == 1)
	{
		pthread_barrier_t * barriereMil = malloc(sizeof(pthread_barrier_t));
		if(barriereMil == NULL)
		{	
			printf("Erreur dans l'allocation mémoire de la barriere Posix du milieu\n");
			exit(1);
		}	
		pthread_barrier_t * barriereFin = malloc(sizeof(pthread_barrier_t));
		if(barriereFin == NULL)
		{
			printf("Erreur dans l'allocation mémoire de la barriere Posix de la fin\n");
			exit(1);
		}
	
		//initialisation des barrieres
		erreur = pthread_barrier_init(barriereMil, NULL, nbThread);
		if(erreur) 
		{
			printf("Erreur dans l'initialisation de la barriere du milieu (erreur %d)\n", erreur);
			exit(1);
		}
		erreur = pthread_barrier_init(barriereFin, NULL, nbThread);
		if(erreur) 
		{
			printf("Erreur dans l'initialisation de la barriere de fin (erreur %d)\n", erreur);
			exit(1);
		}
		for(int i = 1 ; i <= taille ; i += pas)
		{
			for(int j = 1 ; j <= taille ; j += pas)
			{
				infos[cpt].indXDeb = i;
				infos[cpt].indYDeb = j;
				infos[cpt].indXFin = i + pas;
				infos[cpt].indYFin = j + pas;
				infos[cpt].nbIter = nbIter;
				infos[cpt].barriereMil = barriereMil;
				infos[cpt].barriereFin = barriereFin;
				infos[cpt].matGeneral = mat;
				infos[cpt].tailleTotale = taille + 2;
				++cpt;
			}
		}
		return infos;
	} 
	else if (etape == 2)
	{
		//initialisation de nos barrieres avec variables conditions
		barrier_t * barriereMil = malloc(sizeof(barrier_t));
		if(barriereMil == NULL)
		{
			printf("Erreur dans l'allocation mémoire de la barriere milieu avec condition\n");
			exit(1);
		}
		barrier_t * barriereFin = malloc(sizeof(barrier_t));
		if(barriereFin == NULL)
		{
			printf("Erreur dans l'allocation mémoire de la barriere fin avec condition\n");
			exit(1);
		}
		barrier_init(barriereMil, nbThread);
		barrier_init(barriereFin, nbThread);
		for(int i = 1 ; i <= taille ; i += pas)
		{
			for(int j = 1 ; j <= taille ; j += pas)
			{
				infos[cpt].indXDeb = i;
				infos[cpt].indYDeb = j;
				infos[cpt].indXFin = i + pas;
				infos[cpt].indYFin = j + pas;
				infos[cpt].nbIter = nbIter;
				infos[cpt].maBarriereMil = barriereMil;
				infos[cpt].maBarriereFin = barriereFin;
				infos[cpt].matGeneral = mat;
				infos[cpt].tailleTotale = taille + 2;
				++cpt;
			}
		}
	}
	else if (etape == 3)
	{
		//initialisation de nos barrieres avec semaphores
		barrier_sema_t * barriereMil = malloc(sizeof(barrier_sema_t));
		if(barriereMil == NULL)
		{
			printf("Erreur dans l'allocation mémoire de la barriere milieu avec condition\n");
			exit(1);
		}
		barrier_sema_t * barriereFin = malloc(sizeof(barrier_sema_t));
		if(barriereFin == NULL)
		{
			printf("Erreur dans l'allocation mémoire de la barriere fin avec condition\n");
			exit(1);
		}
		barrier_sema_init(barriereMil, nbThread);
		barrier_sema_init(barriereFin, nbThread);
		for(int i = 1 ; i <= taille ; i += pas)
		{
			for(int j = 1 ; j <= taille ; j += pas)
			{
				infos[cpt].indXDeb = i;
				infos[cpt].indYDeb = j;
				infos[cpt].indXFin = i + pas;
				infos[cpt].indYFin = j + pas;
				infos[cpt].nbIter = nbIter;
				infos[cpt].maBarriereSemaMil = barriereMil;
				infos[cpt].maBarriereSemaFin = barriereFin;
				infos[cpt].matGeneral = mat;
				infos[cpt].tailleTotale = taille + 2;
				++cpt;
			}
		}
	}
	return infos;
}


/**
 * 
 * Detruit les variables allouees
 *
 * @author   Lucas Soumille
 */
void destruction(int etape, pthread_t * allThread, void * tableauInfos, int cpt)
{
	if(etape != 0)
	{
		int erreur;
		caseAndIndex * infos = (caseAndIndex *)tableauInfos;
		if(etape == 1)
		{
			//destruction des barrieres posix
			erreur = pthread_barrier_destroy(infos[0].barriereMil);
			if(erreur) 
				printf("Erreur dans la destruction de la barriere du milieu (erreur %d)\n", erreur);
			erreur = pthread_barrier_destroy(infos[0].barriereFin);
			if(erreur) 
				printf("Erreur dans la destruction de la barriere de fin (erreur %d)\n", erreur);	
			free(infos[0].barriereMil);
			free(infos[0].barriereFin);
		}
		else if(etape == 2)
		{
			//destruction de nos barriere avec variable condition
			barrier_destroy(infos[0].maBarriereMil);
			barrier_destroy(infos[0].maBarriereFin);
			free(infos[0].maBarriereSemaMil);
			free(infos[0].maBarriereSemaFin);
		}
		else if(etape == 3)
		{
			//destruction de nos barriere avec variable condition
			barrier_sema_destroy(infos[0].maBarriereSemaMil);
			barrier_sema_destroy(infos[0].maBarriereSemaFin);
			free(infos[0].maBarriereSemaMil);
			free(infos[0].maBarriereSemaFin);
		}
	}
}

/**
 * Permet de simuler une iteration de propagation de chaleur
 *
 * @author   Lucas Soumille, Lucas Martinez
 */
void simulation(int taille, int nbIter, int nbThread, caseDansMat * mat, int etape)
{
	int erreur;
	//calcul du pas pour determiner la zone de parcours de chaque thread
	int pas = sqrt(taille * taille / nbThread);
	// si le nombre de thread est trop grand 
	if(pas < 1)
	{
		pas = 1;
		nbThread = taille * taille;
	}
	//initialisation des infos necessaires aux threads et a la synchronisation
	caseAndIndex * infos = initialisation(etape, taille, nbThread, nbIter, mat, pas);
	//fonction a executer pour l'etape choisie
	void * fonctionAExecuter = fonctionEtape(etape);
	//allocation de l'espace memoire des threads
	pthread_t * allThread = malloc(nbThread  * sizeof(pthread_t));
	if(allThread == NULL)
	{
		printf("Erreur dans l'allocation mémoire des threads\n");
		exit(1);
	}
	int cpt = 0;
	if(etape == 0)
		simulationEtape0(&infos[0]);
	else
	{
		//Lancement des threads
		for(int i = 1 ; i <= taille ; i += pas)
		{
			for(int j = 1 ; j <= taille ; j += pas)
			{
				erreur = pthread_create(&(allThread[cpt]), NULL, fonctionAExecuter, &infos[cpt]);
				if(erreur){
					printf("Erreur dans la creation du thread (erreur %d)\n", erreur);
					exit(1);
				}
				cpt++;
			}
		}
	}
	//on attend la fin des threads
	for(int i = 0 ; i < cpt ; ++i)
	{
		erreur = pthread_join(allThread[i], NULL);
		if(erreur)
		{
			printf("Erreur dans le join d'un thread (erreur %d)\n", erreur);
			exit(1);
		}
	}
	//on libere les ressources utilisees pour la synchro
	destruction(etape, allThread, infos, cpt);	
	//liberation des infos necessaires aux threads
	free(infos);
	//liberation du tableau de threads
	free(allThread);
}

/**
 * Retourne le pointeur sur la fonction a executer pour l'etape passee en parametre
 * de la simulation 
 *
 * @Author Lucas Soumille
 */
void * fonctionEtape(int etape)
{
	if(etape == 0)
		return &simulationEtape0;
	else if (etape == 1)
		return &simulationEtape1;
	else if (etape == 2)
		return &simulationEtape2;
	else if (etape == 3)
		return &simulationEtape3;
}