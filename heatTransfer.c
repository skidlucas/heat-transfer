#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

int getopt (int argc, char * const argv[],
                  const char *optstring);
 
extern char *optarg;
extern int optind, opterr, optopt;
 
#include <getopt.h>

int SIZE_GRID = 1;
int NB_ITER = 10000;
int STEP = 0;
int NB_THREADS = 4;

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
	      printf("option s\n");  //tmp
	      if(strlen(optarg) == 1 && isdigit(optarg[0])){
	      	SIZE_GRID = SIZE_GRID << (atoi(optarg) + 4);
	      	printf("size_grid = %d\n", SIZE_GRID); //tmp
	      } else {
	      	printf("Argument error : a number (0 <= x <= 9) was expected.\n");
	      }
	      break;
	    case 'm':
	      printf("option m\n");
	      break;
	    case 'M':
	      printf("option M\n");
	      break;
	    case 'a':
	      printf("option a\n");
	      break;
	    case 'i':
	      printf("option i\n");
	      if(atoi(optarg)){
	      	NB_ITER = atoi(optarg);
	      	printf("nb_iter = %d\n", NB_ITER); //tmp
	      } else {
	      	printf("Argument error : a number (> 0) was expected.\n");
	      }
	      break;
	    case 'e':
	      printf("option e\n");
	      // comme s mais en vérifiant 0 <= optarg <= 5
	      // chercher la meilleure manière de faire

	      //STEP = atoi(optarg);
	      break;
	    case 't':
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

/*a test*/
void placeRedCase(int ** mat, int n, int val){
	int minInd = pow(2.0, n - 1) - pow(2.0, n - 4);
	int maxInd = pow(2.0, n - 1) + pow(2.0, n - 4);
	printf("min ind = %d", minInd);
	printf("maxInd ind = %d", maxInd);
	for(int i = minInd ; i < maxInd ; ++i){
		for(int j = minInd ; j < maxInd ; ++j){
			mat[i][j] = val;
		}
	}
}

int ** createMatrice(int hauteur, int largeur){
	int ** mat2 = (int **)malloc(hauteur * sizeof(int));
	for(int i = 0 ; i < hauteur ; ++i)
		mat2[i] = (int *)malloc(largeur * sizeof(int));
	for(int i = 0 ; i < hauteur ; ++i){
		for(int j = 0 ; j < largeur ; ++j){
			mat2[i][j] = 0;
		}
	}
	return mat2;
}

void printMatrice(int ** mat, int largeur , int hauteur){
	for(int i = 0 ; i < largeur ; ++i){
		for(int j = 0 ; j < hauteur ; ++j){
			printf("|%d|", mat[i][j]);
		}
		printf("\n");
	}
}


void copyMat(int ** toMat, int ** fromMat, int hauteur, int largeur){
	for(int i = 0 ; i < largeur ; ++i){
		for(int j = 0 ; j < hauteur ; ++j){
			toMat[i][j] = fromMat[i][j];
		}
	}
}

void processMatInLine(int ** mat, int hauteur, int largeur){
	int ** newMat = createMatrice(hauteur, largeur);
	for(int i = 0 ; i < largeur ; ++i){
		for(int j = 0 ; j < hauteur ; ++j){
			if(mat[i][j] == 0) continue;
			int valDeb = mat[i][j];
			newMat[i][j - 1] += valDeb * 4/36;
			newMat[i][j + 1] += valDeb * 4/36;
			newMat[i][j] = valDeb * 16/36; 
		}
	}
	copyMat(mat, newMat, hauteur, largeur);
}

int main(int argc, char * argv[]){
	//checkOptions(argc, argv);
	/*int tableau[7];
	int nbIter = 4;
	tableau[3] = 256;*/
	/*int mat[3][3];
	for(int i = 0 ; i < 3 ; ++i){
		for(int j = 0 ; j < 3 ; ++j){
			mat[j][i] = 60;
			//printMatrice(&mat, 3, 3);
		}
	}
	printMatrice(&mat, 3, 3);*/

	int ** mat = createMatrice(3,3);
	mat[0][1] = 36;
	//placeRedCase(mat2, 4, 10);
	processMatInLine(mat, 3, 3);
	printMatrice(mat, 3, 3);
	return 0;
} 