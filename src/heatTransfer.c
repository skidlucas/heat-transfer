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
#include "barriere.h"


int vraieTaille, nbCaseParThread;
/**
 * Permet de simuler une diffusion de la chaleur de maniere horizontale autour de la
 * case passee en parametre
 *
 * @author   Lucas Soumille, Lucas Martinez
 */
void simulationHori(wrappedMatrice* wrappedMat){
	caseDansMat * caseMat;
	caseDansMat * caseMatGauche;
	caseDansMat * caseMatDroite;

	for(int i = wrappedMat->x ; i < (wrappedMat->x + wrappedMat->tailleLigne) ; ++i){
		for(int j = wrappedMat->y ; j < (wrappedMat->y + wrappedMat->tailleLigne) ; ++j){
			caseMat = wrappedMat->matrice + i * vraieTaille + j;
			caseMatGauche = wrappedMat->matrice + i * vraieTaille + (j - 1);
			caseMatDroite = wrappedMat->matrice + i * vraieTaille + (j + 1);
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
void simulationVerti(wrappedMatrice* wrappedMat){
	caseDansMat * caseMat;
	caseDansMat * caseMatHaut;
	caseDansMat * caseMatBas;

	for(int i = wrappedMat->x ; i < (wrappedMat->x + wrappedMat->tailleLigne) ; ++i){
		for(int j = wrappedMat->y ; j < (wrappedMat->y + wrappedMat->tailleLigne) ; ++j){
			caseMat = wrappedMat->matrice + i * vraieTaille + j;
			//si la case est chauffante on fait pas la mise a jour de sa valeur
			if(caseMat->estConstante == 1) continue;

			caseMatHaut = wrappedMat->matrice + (i - 1) * vraieTaille + j;
			caseMatBas = wrappedMat->matrice + (i + 1) * vraieTaille + j;
			caseMat->valeur = (4.0 * caseMat->valeurTmp + caseMatHaut->valeurTmp + caseMatBas->valeurTmp) / 6;

		}
	}
}

/**
 * Fonction qui lance la simulation de l'etape 0 (sans threads)
 *
 * @author Lucas Martinez
 */
void simulationE0(wrappedMatrice* wrappedMat){
	for(int i = 0 ; i < wrappedMat->nbIter ; ++i){
		simulationHori(wrappedMat);
		simulationVerti(wrappedMat);
	}
}

/**
 * Fonction qui lance la simulation de l'etape 1 (avec barrieres)
 *
 * @author Lucas Martinez
 */
void simulationE1(wrappedMatrice* wrappedMat){
	int ret = 0;
	for (int i = 0; i < wrappedMat->nbIter; ++i){
		simulationHori(wrappedMat);
		ret = pthread_barrier_wait(wrappedMat->barriereHori);
		if (ret && ret != PTHREAD_BARRIER_SERIAL_THREAD){
			perror("Erreur pthread_barrier_wait : barriereHori\n");
			exit(1);
		}

		simulationVerti(wrappedMat);
		ret = pthread_barrier_wait(wrappedMat->barriereVerti);
		if (ret && ret != PTHREAD_BARRIER_SERIAL_THREAD){
			perror("Erreur pthread_barrier_wait : barriereVerti\n");
			exit(1);
		}
	}

	pthread_exit(NULL);
}

/**
 * Fonction qui lance la simulation de l'etape 2 (avec variables conditions)
 *
 * @author Lucas Martinez
 */
void simulationE2(wrappedMatrice* wrappedMat){
	int ret = 0;
	for (int i = 0; i < wrappedMat->nbIter; ++i){
		simulationHori(wrappedMat);
		ret = barrier_wait(wrappedMat->barriereHori);
		if (ret && ret != PTHREAD_BARRIER_SERIAL_THREAD){
			perror("Erreur barrier_wait : barriereHori\n");
			exit(1);
		}

		simulationVerti(wrappedMat);
		ret = barrier_wait(wrappedMat->barriereVerti);
		if (ret && ret != PTHREAD_BARRIER_SERIAL_THREAD){
			perror("Erreur barrier_wait : barriereVerti\n");
			exit(1);
		}
	}

	pthread_exit(NULL);
}

/**
 * Fonction qui lance la simulation de l'etape 3 (avec semaphores)
 *
 * @author Lucas Martinez
 */
void simulationE3(wrappedMatrice* wrappedMat){
	int ret = 0;
	for (int i = 0; i < wrappedMat->nbIter; ++i){
		simulationHori(wrappedMat);
		ret = barrier_sem_wait(wrappedMat->barriereHori);
		if (ret && ret != PTHREAD_BARRIER_SERIAL_THREAD){
			perror("Erreur barrier_sem_wait : barriereHori\n");
			exit(1);
		}

		simulationVerti(wrappedMat);
		ret = barrier_sem_wait(wrappedMat->barriereVerti);
		if (ret && ret != PTHREAD_BARRIER_SERIAL_THREAD){
			perror("Erreur barrier_sem_wait : barriereVerti\n");
			exit(1);
		}
	}

	pthread_exit(NULL);
}

/**
 * Fonction qui renvoie la fonction a lancer dans le thread en fonction de l'etape en parametre
 * (N'est pas encore utile car traitement different si etape = 0)
 *
 * @author Lucas Martinez
 */
void* simulation(int etape){
	switch (etape){
		case 0:
			return &simulationE0;
		case 1:
			return &simulationE1;
		case 2:
			return &simulationE2;
		case 3:
			return &simulationE3;
		default:
			break;
	}
	return 0;
}

/**
 * Fonction qui initialise les barrieres posix en fonction du nombre de threads
 *
 * @author Lucas Martinez
 */
void initBarrieres(int nbThread, pthread_barrier_t* barriereHori, pthread_barrier_t* barriereVerti){
	int ret = 0;

	ret = pthread_barrier_init(barriereHori, NULL, nbThread);
	if(ret) {
		perror("Erreur pthread_barrier_init : barriereHori\n");
		exit(1);
	}
	ret = pthread_barrier_init(barriereVerti, NULL, nbThread);
	if(ret) {
		perror("Erreur pthread_barrier_wait : barriereVerti\n");
		exit(1);
	}
}

/**
 * Fonction qui initialise la matrice enveloppee et lance la simulation de l'etape 0
 *
 * @author Lucas Martinez
 */
void lancerThread(int taille, int nbIter, caseDansMat* mat, wrappedMatrice* wrappedMat){
	wrappedMat->x = 1;
	wrappedMat->y = 1;
	wrappedMat->tailleLigne = nbCaseParThread;
	wrappedMat->nbIter = nbIter;
	wrappedMat->matrice = mat;
	simulationE0(wrappedMat);
}

/**
 * Fonction qui initialise les matrices enveloppees et lance les threads qui effectueront la simulation
 *
 * @author Lucas Martinez
 */
void lancerThreads(int taille, int etape, int nbIter, caseDansMat* mat, pthread_t* threads,
				   wrappedMatrice* wrappedMat, void* barriereHori, void* barriereVerti){
	int n = 0;
	int ret = 0; //nb threads crees, valeur de retour
	void* fctEtape = simulation(etape); //suivant etape, renvoie la fonction a executer

	for(int i = 1 ; i <= taille ; i += nbCaseParThread){
		for(int j = 1 ; j <= taille ; j += nbCaseParThread){
			wrappedMat[n].x = i;
			wrappedMat[n].y = j;
			wrappedMat[n].tailleLigne = nbCaseParThread;
			wrappedMat[n].nbIter = nbIter;
			wrappedMat[n].matrice = mat;
			wrappedMat[n].barriereHori = barriereHori;
			wrappedMat[n].barriereVerti = barriereVerti;

			ret = pthread_create(&threads[n], NULL, fctEtape, (void *) &wrappedMat[n]);
			if(ret){
				free(&wrappedMat[n]);
				perror("Erreur pthread_create\n");
				exit(1);
			}
			++n;
		}
	}

	for(int i = 0 ; i < n ; ++i){

		ret = pthread_join(threads[i], NULL);
		if(ret){
			perror("Erreur pthread_join");
			exit(1);
		}
	}
}

/**
 * Fonction qui detruit les barrieres et libere la memoire
 *
 * @author Lucas Martinez
 */
void rendreBarrieres(pthread_barrier_t* barriereHori, pthread_barrier_t* barriereVerti){
	int ret = 0;
	ret = pthread_barrier_destroy(barriereHori);
	if (ret){
		perror("Erreur pthread_barrier_destroy : barriereHori\n");
	}

	ret = pthread_barrier_destroy(barriereVerti);
	if (ret){
		perror("Erreur pthread_barrier_destroy : barriereHori\n");
	}

	free(barriereHori);
	free(barriereVerti);
}


/**
 * Permet de simuler une iteration de propagation de chaleur
 *
 * @author   Lucas Martinez
 */
void initSimulation(int taille, int etape, int nbIter, int nbThread, caseDansMat * mat){
	vraieTaille = taille + 2; //bords

	nbCaseParThread = sqrt(taille * taille / nbThread); //nbCaseParThread par ligne en fait
	if(nbCaseParThread < 1){
		nbCaseParThread = 1;
		nbThread = taille * taille;
	}

	pthread_t* threads = malloc(nbThread * sizeof(pthread_t));
	if (!threads){
		perror("Erreur d'allocation mémoire, arret du programme.");
		exit(1);
	}

	wrappedMatrice* wrappedMat = malloc(nbThread * sizeof(wrappedMatrice));
	if (!wrappedMat){
		perror("Erreur d'allocation mémoire, arret du programme.");
		exit(1);
	}

	switch (etape){
		case 0:
			lancerThread(taille, nbIter, mat, wrappedMat); //un seul thread, comportement different
			break;
		case 1: ;
			pthread_barrier_t* barriereHori = malloc(sizeof(pthread_barrier_t));
			if (!barriereHori){
				perror("Erreur d'allocation mémoire, arret du programme.");
				exit(1);
			}

			pthread_barrier_t* barriereVerti = malloc(sizeof(pthread_barrier_t));
			if (!barriereVerti){
				perror("Erreur d'allocation mémoire, arret du programme.");
				exit(1);
			}

			initBarrieres(nbThread, barriereHori, barriereVerti);
			lancerThreads(taille, etape, nbIter, mat, threads, wrappedMat, barriereHori, barriereVerti);
			rendreBarrieres(barriereHori, barriereVerti);
			break;
		case 2: ;
			maBarriere* maBarriereHori = malloc(sizeof(maBarriere));
			if (!maBarriereHori){
				perror("Erreur d'allocation mémoire, arret du programme.");
				exit(1);
			}

			maBarriere* maBarriereVerti = malloc(sizeof(maBarriere));
			if (!maBarriereVerti){
				perror("Erreur d'allocation mémoire, arret du programme.");
				exit(1);
			}

			barrier_init(maBarriereHori, nbThread);
			barrier_init(maBarriereVerti, nbThread);

			lancerThreads(taille, etape, nbIter, mat, threads, wrappedMat, maBarriereHori, maBarriereVerti);

			barrier_destroy(maBarriereHori);
			barrier_destroy(maBarriereVerti);
			free(maBarriereHori);
			free(maBarriereVerti);

			break;
		case 3: ;
			maBarriereSem* maBarriereSemHori = malloc(sizeof(maBarriereSem));
			if (!maBarriereSemHori){
				perror("Erreur d'allocation mémoire, arret du programme.");
				exit(1);
			}

			maBarriereSem* maBarriereSemVerti = malloc(sizeof(maBarriereSem));
			if (!maBarriereSemVerti){
				perror("Erreur d'allocation mémoire, arret du programme.");
				exit(1);
			}

			barrier_sem_init(maBarriereSemHori, nbThread);
			barrier_sem_init(maBarriereSemVerti, nbThread);

			lancerThreads(taille, etape, nbIter, mat, threads, wrappedMat, maBarriereSemHori, maBarriereSemVerti);

			barrier_sem_destroy(maBarriereSemHori);
			barrier_sem_destroy(maBarriereSemVerti);
			free(maBarriereSemHori);
			free(maBarriereSemVerti);

			break;
	}

	free(wrappedMat);
	free(threads);

}
