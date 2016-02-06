#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>

int getopt (int argc, char * const argv[],
                  const char *optstring);
 
extern char *optarg;
extern int optind, opterr, optopt;
 
#include <getopt.h>

int SIZE_GRID = 1;
int NB_ITER = 10000;
int STEP = 0;
int NB_THREADS = 4;
clock_t start, end;

int isNumber(char * s){
	int i = 0;
    while (s[i]){
        if (!isdigit(s[i]))
            return 0;
        ++i;
    }
    return 1;
}

typedef struct {
	double valN;
	double valNPlus1;
	int isHeating;
} caseInMat;

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
	      start = clock();
	      /*quand le scénario est fini :
			end = clock();
			double total = (double) (end - start) / CLOCKS_PER_SEC;
			printf("Temps total de consommation CPU: %f\n", total);
			*/
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

void placeRedCase(caseInMat ** mat, int n, int val){
	/*int minInd = pow(2.0, n - 1) - pow(2.0, n - 4);
	int maxInd = pow(2.0, n - 1) + pow(2.0, n - 4);
	printf("min ind = %d\n", minInd);
	printf("maxInd ind = %d\n", maxInd);*/
	int minInd = (1 << n-1) - (1 << n-4);// pow(2.0, n - 4);
	int maxInd = (1 << n-1) + (1 << n-4);;
	printf("min ind = %d\n", minInd);
	printf("maxInd ind = %d\n", maxInd);
	for(int i = minInd ; i < maxInd ; ++i){
		for(int j = minInd ; j < maxInd ; ++j){
			mat[i][j].valN = val;
			mat[i][j].valNPlus1 = val;
			mat[i][j].isHeating = 1;
		}
	}
}

/*
int ** createMatrix(int height, int width ){
	int ** mat2 = (int **)malloc(height * sizeof(int));
	for(int i = 0 ; i < height ; ++i)
		mat2[i] = (int *)malloc(width  * sizeof(int));
	for(int i = 0 ; i < height ; ++i){
		for(int j = 0 ; j < width  ; ++j){
			mat2[i][j] = 0;
		}
	}
	return mat2;
}*/

caseInMat ** createMatrix(int height, int width ){
	printf("before malloc");
	caseInMat ** mat2 = (caseInMat **)malloc(height * sizeof(caseInMat));
	for(int i = 0 ; i < height ; ++i)
		mat2[i] = (caseInMat *)malloc(width  * sizeof(caseInMat));
	printf("after malloc\n");
	for(int i = 0 ; i < height ; ++i){
		for(int j = 0 ; j < width  ; ++j){
			//caseInMat tmp = {0,0,0};
			mat2[i][j].valN = 0;
			mat2[i][j].valNPlus1 = 0;
			mat2[i][j].isHeating = 0;
		}
	}
	return mat2;
}

/*
void printMatrix(int ** mat, int width  , int height){
	for(int i = 0 ; i < width  ; ++i){
		for(int j = 0 ; j < height ; ++j){
			printf("|%d|", mat[i][j]);
		}
		printf("\n");
	}
}*/


void printMatrix(caseInMat ** mat, int width  , int height, FILE * fic){
	for(int i = 0 ; i < width  ; ++i){
		for(int j = 0 ; j < height ; ++j){
			fprintf(fic, "|%f|", round(mat[i][j].valN*100)/100);
		}
		fprintf(fic,"\n");
	}
}


void updateMat(caseInMat ** mat, int height, int width ){
	for(int i = 0 ; i < width  ; ++i){
		for(int j = 0 ; j < height ; ++j){
			if(! mat[i][j].isHeating){
				mat[i][j].valN = mat[i][j].valN * 16/36 + mat[i][j].valNPlus1;
				mat[i][j].valNPlus1 = 0.0;
			} else 
				mat[i][j].valNPlus1 = mat[i][j].valN;
		}
	}
}

/* test pour pas sortir de la matrice a faire */
void processMatInLine(caseInMat ** mat, int height, int width ){
	for(int i = 0 ; i < width  ; ++i){
		for(int j = 0 ; j < height ; ++j){
			if(mat[i][j].valN == 0) continue;
		//	printf("N = %f\n", mat[i][j].valN);
			double valDeb = mat[i][j].valN;
			mat[i][j - 1].valNPlus1 += valDeb * 4/36;
			mat[i][j + 1].valNPlus1 += valDeb * 4/36;
		//	printf("J + 1 %f\n", mat[i][j + 1].valNPlus1);
		//	mat[i][j].valNPlus1 += valDeb * 16/36; 
		//	printf("N + 1 %f\n", mat[i][j].valNPlus1);
		}
	}
}

/* test pour pas sortir de la matrice a faire */
void processMatInCol(caseInMat ** mat, int height, int width){
	for(int i = 0 ; i < width  ; ++i){
		for(int j = 0 ; j < height ; ++j){
			if(mat[i][j].valN == 0) continue;
			double valDeb = mat[i][j].valN;
			mat[i - 1][j - 1].valNPlus1 += valDeb * 1/36;
			mat[i + 1][j - 1].valNPlus1 += valDeb * 1/36;
			mat[i - 1][j].valNPlus1 += valDeb * 4/36;
			mat[i + 1][j].valNPlus1 += valDeb * 4/36;
			mat[i - 1][j + 1].valNPlus1 += valDeb * 1/36;
			mat[i + 1][j + 1].valNPlus1 += valDeb * 1/36;    
		}
	}
}

int main(int argc, char * argv[]){
	FILE* fichier = fopen("test.txt", "w+");
	printf("startmain\n");
	//checkOptions(argc, argv);
	/*int tableau[7];
	int nbIter = 4;
	tableau[3] = 256;*/
	/*int mat[3][3];
	for(int i = 0 ; i < 3 ; ++i){
		for(int j = 0 ; j < 3 ; ++j){
			mat[j][i] = 60;
			//printMatrix(&mat, 3, 3);
		}
	}
	printMatrix(&mat, 3, 3);*/

	/*int ** mat = createMatrix(3,3);
	mat[0][1] = 36;
	//placeRedCase(mat2, 4, 10);
	processMatInLine(mat, 3, 3);
	printMatrix(mat, 3, 3);*/
	caseInMat ** mat = createMatrix(16,16);
	printf("after create\n");
	placeRedCase(mat, 4, 36);
	/*for(int i = 0 ; i < 3 ; ++i){
		processMatInLine(mat, 16, 16);	
	}*/
	processMatInCol(mat, 16, 16);
	updateMat(mat, 16, 16 );
	processMatInCol(mat, 16, 16);
	updateMat(mat, 16, 16 );
	printMatrix(mat, 16,16, fichier);
	return 0;
} 