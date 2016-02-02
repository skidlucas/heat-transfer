#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>

int SIZE_GRID = 1;

void checkOptions(int argc, char * argv[]){
	int c;
	while ((c = getopt(argc , argv, "s:")) != -1){
		switch (c) {
	    case 's':
	      printf("option S");
	      if(strlen(optarg) == 1 && isdigit(optarg[0])){
	      	SIZE_GRID = atoi(optarg);
	      }
	      break;
	    case 'b':
	      printf("option b");
	      break;
	    case 'c':
	      printf("option c");
	      break;
	    case '?':
	      printf("Case ?");
	      break;
	    default:
	      //printf("c);
	      break;
	    }
	}
}

int processTab(int * tab){
	int i = 0;
	while(i < 7){
		int valeur = tab[3];

	}
}

int main(int argc, char * argv[]){
	//checkOptions(argc, argv);
	int tableau[7];
	int nbIter = 4;
	tableau[3] = 256;

	return 0;
} 