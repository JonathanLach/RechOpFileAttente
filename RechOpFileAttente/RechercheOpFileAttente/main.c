#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include<limits.h>

#define A 69069
#define C 0
#define M INT_MAX

int genererNbArrivees(void);
int genererDureeService(void);
double generationAleatoireZeroAUn(int *generationPrecedente);
int generationAleatoire(int *generationPrecedente);
int main(void)
{
	int generationPrecedente = 292;
	double generation;
	for (int i = 0; i < 100; i++)
	{
		generation = generationAleatoireZeroAUn(&generationPrecedente);
		printf("Nombre genere: %f\n", generation);
	}
	system("pause");
	return EXIT_SUCCESS;
}

int generationAleatoire(int *generationPrecedente)
{	 
	*generationPrecedente = (A*(*generationPrecedente) + C) % M;
	return *generationPrecedente;
}

double generationAleatoireZeroAUn(int *generationPrecedente)
{
	int rng = generationAleatoire(generationPrecedente);
	double resultat = (double)rng / M;
	if (resultat < 0)
	{
		resultat = resultat *(-1);
	}
	return resultat;
}

int genererNbArrivees(void) {
	int dureeService;
	double nombre = (double)rand() / RAND_MAX;
	printf("Test: Nombre genere: %f \n", nombre);
	dureeService = 8;
	if (nombre < 0.111) {
		dureeService = 0;
	}
	else if (nombre < 0.355) {
		dureeService = 1;
	}
	else if (nombre < 0.623) {
		dureeService = 2;
	}
	else if (nombre < 0.819) {
		dureeService = 3;
	}
	else if (nombre < 0.927) {
		dureeService = 4;
	}
	else if (nombre < 0.975) {
		dureeService = 5;
	}
	else if (nombre < 0.992) {
		dureeService = 6;
	}
	else if (nombre < 0.998) {
		dureeService = 7;
	}
	printf("Test: Nombre de clients genere: %d\n", dureeService);
	return dureeService;
}


int genererDureeService(void) {
	int dureeService;
	int nombre = rand() % 59 + 1;
	printf("Test: Nombre genere: %d \n", nombre);
	dureeService = 6;
	if (nombre < 2) {
		dureeService = 0;
	}
	else if (nombre < 5) {
		dureeService = 1;
	}
	else if (nombre < 7) {
		dureeService = 2;
	}
	else if (nombre < 17) {
		dureeService = 3;
	}
	else if (nombre < 35) {
		dureeService = 4;
	}
	printf("Test: Duree de service generee: %d\n", dureeService);
	return dureeService;
}

