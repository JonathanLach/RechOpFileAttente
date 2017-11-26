#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

int genererNbArrivees(void);
int genererDureeService(void);

int main(void)
{
	srand((unsigned int)time(NULL));
	for (int i = 0; i < 10; i++) {
		int nbClients = genererNbArrivees();
	}
	for (int i = 0; i < 10; i++) {
		int dureeService = genererDureeService();
	}
	system("pause");
	return EXIT_SUCCESS;
}

int genererNbArrivees(void) {
	int dureeService;
	double nombre = (double)rand() / RAND_MAX;
	printf("Test: Nombre g�n�r�: %f \n", nombre);
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
	printf("Test: Nombre de clients g�n�r�: %d\n", dureeService);
	return dureeService;
}


int genererDureeService(void) {
	int dureeService;
	int nombre = rand() % 59 + 1;
	printf("Test: Nombre g�n�r�: %d \n", nombre);
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
	printf("Test: Dur�e de service g�n�r�e: %d\n", dureeService);
	return dureeService;
}

