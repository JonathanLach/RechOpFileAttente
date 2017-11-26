#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

int genererNbArrivees(void);

int main(void)
{
	srand((unsigned int)time(NULL));
	for (int i = 0; i < 10; i++) {
		int nbClients = genererNbArrivees();
	}
	system("pause");
	return EXIT_SUCCESS;
}

int genererNbArrivees(void) {
	int nbClients;
	double nombre = (double)rand() / RAND_MAX;
	printf("Test: Nombre généré: %f \n", nombre);
	nbClients = 8;
	if (nombre < 0.111) {
		nbClients = 0;
	}
	else if (nombre < 0.355) {
		nbClients = 1;
	}
	else if (nombre < 0.623) {
		nbClients = 2;
	}
	else if (nombre < 0.819) {
		nbClients = 3;
	}
	else if (nombre < 0.927) {
		nbClients = 4;
	}
	else if (nombre < 0.975) {
		nbClients = 5;
	}
	else if (nombre < 0.992) {
		nbClients = 6;
	}
	else if (nombre < 0.998) {
		nbClients = 7;
	}
	printf("Test: Nombre de clients généré: %d\n", nbClients);
	return nbClients;
}

