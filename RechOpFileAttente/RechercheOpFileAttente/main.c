#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include<limits.h>
#include <stdbool.h>

#define A 69069
#define C 0
#define GERME 292
#define NBSTATIONSMIN 5
#define NBSTATIONSMAX 30
#define TAILLEFILEEXPRESS 4
#define TEMPSEJECTION 4
#define REPETITIONDS1 20
#define REPETITIONDS2 30
#define REPETITIONDS3 20
#define REPETITIONDS4 10
#define REPETITIONDS5 18
#define REPETITIONDS6 24
typedef struct client
{
	bool estPrioritaire;
	int dureeServiceInitiale;
	int dureeServiceRestante;
}Client;
typedef struct station
{
	double coutTotalService;
	Client clientServi;
}Station;

int genererNbArrivees(unsigned int *generationPrecedente);
int genererDureeService(unsigned int *generationPrecedente);
double generationAleatoireZeroAUn(unsigned int *generationPrecedente);
Client generationClient(unsigned int *generationPrecedente);
void repartirClient(Client clientGenere, Station *tabStations, Client *tabFileNormale, Client *tabFileExpress, int *nbClientsFileExpress, int *nbClientsFileNormale,int nbStation, int *coutPassageClientExpressEnFileNormale);
void gestionClientExpress(Client clientGenere, Station *tabStations, Client *tabFileNormale, Client *tabFileExpress, int *nbClientsFileExpress, int *nbClientsFileNormale, int nbStations, int *coutPassageClientExpressEnFileNormale);
void rechercheEtPlacementStationNormale(Client clientGenere, Station *tabStations, Client *tabFileNormale, int *nbClientsFileNormale, int nbStations, int *coutPassageClientExpressEnFileNormale);
void placementClientExpressEnFileNormale(Client clientGenere, Client *tabFileNormale, int *nbClientsFileNormale, int *coutPassageClientExpressEnFileNormale);
void decalageFileNormale(Client *tabFileNormale, int iFileNormale, int *nbClientsFileNormale);
void montrerFile(Client *file, int tailleFile);
void montrerTabStations(Station *tabStations, int nbStations);
void initTableaux(Station *tabStations, int nbStations, int *nbClientsFileExpress, int *nbClientsFileNormale);
void traitementStations(int nbStations, Station tabStations[], double  *coutTotalPresenceClient);
void traitementClients(int nbStations, Station tabStations[], double *coutTotalPresenceClient, Client tabFileExpress[], Client tabFileNormale[], int * nbClientsFileExpress, int * nbClientsFileNormale);
bool estLibre(Station tabStations[], int iStation);
void traitementFileExpress(int nbStations, int * nbClientsFileExpress, Client tabFileExpress[], Station tabStations[], double  *coutTotalPresenceClient);
void traitementFileNormale(int nbStations, Station tabStations[], int * nbClientsFileNormale, Client tabFileNormale[], double *coutTotalPresenceClient);
void suppressionClientFile(Station tabStations[], int iStation, Client tabFiles[], int * tailleFille);



int main(void)
{
	double coutTotalMin = 10000000000000000000;
	int meilleurNbStations;
	unsigned int generationPrecedente = GERME;
	for (int nbStations = NBSTATIONSMIN; nbStations < NBSTATIONSMAX; nbStations++) {
		Station tabStations[NBSTATIONSMAX];
		Client tabFileExpress[TAILLEFILEEXPRESS];
		Client tabFileNormale[10000];
		double coutTotal = 0;
		int nbClientsFileExpress = 0;
		int nbClientsFileNormale = 0;
		int coutPassageClientExpressEnFileNormale = 0;
		double coutTotalPresenceClient = 0;
		Client clientGenere;
		initTableaux(tabStations, nbStations, &nbClientsFileExpress, &nbClientsFileNormale);
		for (int minute = 0; minute < 960; minute++) {
			int nbClients = genererNbArrivees(&generationPrecedente);
			for (int iClient = 0; iClient < nbClients; iClient++)
			{
				clientGenere = generationClient(&generationPrecedente);
				repartirClient(clientGenere, tabStations, tabFileNormale, tabFileExpress, &nbClientsFileExpress, &nbClientsFileNormale, nbStations, &coutPassageClientExpressEnFileNormale);
			}
			traitementClients(nbStations, tabStations, &coutTotalPresenceClient, tabFileExpress, tabFileNormale, &nbClientsFileExpress, &nbClientsFileNormale);
			if (nbStations == NBSTATIONSMIN || nbStations == 6) {
				if (minute <= 30) {
					printf("Nombre de stations: %d\n", nbStations);
					printf("Minute : %d\n", minute);
					printf("Nombre d'arrivees: %d\n\n", nbClients);
					montrerTabStations(tabStations, nbStations);
					printf("Etat de la file express:\n");
					montrerFile(tabFileExpress, nbClientsFileExpress);
					printf("Etat de la file normale:\n");
					montrerFile(tabFileNormale, nbClientsFileNormale);
				}
			}
			printf("-------------------- \n");
			printf("-------------------- \n\n\n");
		}


		for (int i = 0; i < nbStations; i++) {
			coutTotal += tabStations[i].coutTotalService;
		}
		coutTotal += (double)coutPassageClientExpressEnFileNormale;
		coutTotal += coutTotalPresenceClient;
		printf("Cout total de la station: %f\n", coutTotal);
		printf("-------------------- \n");
		printf("-------------------- \n\n\n");
		if (coutTotal < coutTotalMin) {
			coutTotalMin = coutTotal;
			meilleurNbStations = nbStations;
		}
	}
	printf("Nombre de stations optimal: %d\n", meilleurNbStations);
	system("pause");
}
void initTableaux(Station *tabStations, int nbStations, int *nbClientsFileExpress, int *nbClientsFileNormale)
{
	int iStation = 0;
	while (iStation < nbStations)
	{
		tabStations[iStation].coutTotalService = 0;
		tabStations[iStation].clientServi.dureeServiceInitiale = 0;
		tabStations[iStation].clientServi.dureeServiceRestante = 0;
		tabStations[iStation].clientServi.estPrioritaire = false;
		iStation++;
	}
	*nbClientsFileExpress = 0;
	*nbClientsFileNormale = 0;
}

void montrerFile(Client *file, int tailleFile)
{
	int iFile = 0;
	while (iFile < tailleFile)
	{
		printf("client %d : duree de service : %d, restant: %d, prioritaire : %d\n, type: %s\n", iFile, file[iFile].dureeServiceInitiale, file[iFile].dureeServiceRestante, (file[iFile].estPrioritaire) ? 1 : 0, (file[iFile].dureeServiceInitiale > 3)?"normal":"express");
		iFile++;
	}
}
void montrerTabStations(Station *tabStations, int nbStations)
{
	int iStation = 0;
	printf("Etat du tableau des stations:\n");
	while (iStation < nbStations)
	{
		printf("Station %d : duree de service: %d, restant %d, prioritaire: %d\n, type: %s\n", iStation, tabStations[iStation].clientServi.dureeServiceInitiale, tabStations[iStation].clientServi.dureeServiceRestante, (tabStations[iStation].clientServi.estPrioritaire)?1:0, (tabStations[iStation].clientServi.dureeServiceInitiale > 3)?"normal":"express");
		iStation++;
	}
}
Client generationClient(unsigned int *generationPrecedente)
{
	Client nouveauClient;
	int dureeServiceGeneree = genererDureeService(generationPrecedente);
	nouveauClient.dureeServiceInitiale = dureeServiceGeneree;
	nouveauClient.dureeServiceRestante = dureeServiceGeneree;
	nouveauClient.estPrioritaire = false;
	return nouveauClient;
}

void repartirClient(Client clientGenere, Station *tabStations,Client *tabFileNormale, Client *tabFileExpress, int *nbClientsFileExpress, int *nbClientsFileNormale,int nbStations, int *coutPassageClientExpressEnFileNormale)
{
	if (clientGenere.dureeServiceInitiale <= 3)
	{
		gestionClientExpress(clientGenere,tabStations,tabFileNormale,tabFileExpress,nbClientsFileExpress,nbClientsFileNormale, nbStations, coutPassageClientExpressEnFileNormale);
	}
	else
	{
		rechercheEtPlacementStationNormale(clientGenere,tabStations,tabFileNormale,nbClientsFileNormale, nbStations, coutPassageClientExpressEnFileNormale);
	}
}

void gestionClientExpress(Client clientGenere, Station *tabStations, Client *tabFileNormale, Client *tabFileExpress, int *nbClientsFileExpress, int *nbClientsFileNormale,int nbStations, int *coutPassageClientExpressEnFileNormale)
{
	int i = 0;
	while (i < 2 && !estLibre(tabStations, i))
	{
		i++;
	}
	if (i == 2)
	{
		if (*nbClientsFileExpress < TAILLEFILEEXPRESS)
		{
			tabFileExpress[*nbClientsFileExpress] = clientGenere;
			*nbClientsFileExpress = *nbClientsFileExpress + 1;
		}
		else
		{
			rechercheEtPlacementStationNormale(clientGenere, tabStations, tabFileNormale, nbClientsFileNormale,nbStations, coutPassageClientExpressEnFileNormale);
		}
	}
	else
	{
		tabStations[i].clientServi = clientGenere;
	}
}
void rechercheEtPlacementStationNormale(Client clientGenere, Station *tabStations, Client *tabFileNormale, int *nbClientsFileNormale, int nbStations, int *coutPassageClientExpressEnFileNormale)
{
	int iStation = 2;
	while (iStation < nbStations && !estLibre(tabStations, iStation))
	{
		iStation++;
	}
	if (iStation == nbStations)
	{
		if (clientGenere.dureeServiceInitiale <= 3)
		{
			placementClientExpressEnFileNormale(clientGenere, tabFileNormale, nbClientsFileNormale, coutPassageClientExpressEnFileNormale);
		}
		else
		{
			tabFileNormale[*nbClientsFileNormale] = clientGenere;
			*nbClientsFileNormale = *nbClientsFileNormale + 1;
		}
	}
	else
	{
		tabStations[iStation].clientServi = clientGenere;
	}
}
void placementClientExpressEnFileNormale(Client clientGenere, Client *tabFileNormale, int *nbClientsFileNormale, int *coutPassageClientExpressEnFileNormale)
{
	int iFileNormale = 0;
	while (iFileNormale < *nbClientsFileNormale&&(tabFileNormale[iFileNormale].dureeServiceInitiale <= 3 || tabFileNormale[iFileNormale].estPrioritaire))
	{
		iFileNormale++;
	}
	if (iFileNormale != *nbClientsFileNormale)
	{
		decalageFileNormale(tabFileNormale, iFileNormale, nbClientsFileNormale);
	}
	*coutPassageClientExpressEnFileNormale += 5;
	tabFileNormale[iFileNormale] = clientGenere;
	*nbClientsFileNormale = *nbClientsFileNormale + 1;
}
void decalageFileNormale(Client *tabFileNormale, int iFileNormale, int *nbClientsFileNormale)
{
	int limInf = iFileNormale;
	int limSup = *nbClientsFileNormale;
	while (limSup > limInf)
	{
		tabFileNormale[limSup] = tabFileNormale[limSup - 1];
		limSup--;
	}
}

double generationAleatoireZeroAUn(unsigned int *generationPrecedente)
{
	*generationPrecedente = A * (*generationPrecedente) + C;
	return *generationPrecedente / (UINT_MAX+1.);
}

int genererNbArrivees(unsigned int *generationPrecedente) {
	int nbArrivees;
	double nombre = generationAleatoireZeroAUn(generationPrecedente);
	//printf("Test: Nombre genere: %f \n", nombre);
	nbArrivees = 7;
	if (nombre < 0.111) {
		nbArrivees = 0;
	}
	else if (nombre < 0.355) {
		nbArrivees = 1;
	}
	else if (nombre < 0.623) {
		nbArrivees = 2;
	}
	else if (nombre < 0.819) {
		nbArrivees = 3;
	}
	else if (nombre < 0.927) {
		nbArrivees = 4;
	}
	else if (nombre < 0.975) {
		nbArrivees = 5;
	}
	else if (nombre < 0.992) {
		nbArrivees = 6;
	}
	
	//printf("Test: Nombre de clients genere: %d\n", dureeService);
	return nbArrivees;
}


int genererDureeService(unsigned int *generationPrecedente) {
	int dureeService;
	double nombre = generationAleatoireZeroAUn(generationPrecedente);
	
	int sommeRepetitions = REPETITIONDS1+REPETITIONDS2+REPETITIONDS3+REPETITIONDS4+REPETITIONDS5+REPETITIONDS6;
	double limite1 = (double)REPETITIONDS1 / sommeRepetitions;
	double limite2 = (double)REPETITIONDS2 / sommeRepetitions + limite1;
	double limite3 = (double)REPETITIONDS3 / sommeRepetitions + limite2;
	double limite4 = (double)REPETITIONDS4 / sommeRepetitions + limite3;
	double limite5 = (double)REPETITIONDS5 / sommeRepetitions + limite4;
	//printf("Test: Nombre genere: %d \n", nombre);
	dureeService = 6;
	if (nombre < limite1) {
		dureeService = 1;
	}
	else if (nombre < limite2) {
		dureeService = 2;
	}
	else if (nombre < limite3) {
		dureeService = 3;
	}
	else if (nombre < limite4) {
		dureeService = 4;
	}
	else if (nombre < limite5) {
		dureeService = 5;
	}
	//printf("Test: Duree de service generee: %d\n", dureeService);
	return dureeService;
}

void traitementClients(int nbStations, Station tabStations[], double *coutTotalPresenceClient, Client tabFileExpress[], Client tabFileNormale[], int * nbClientsFileExpress, int * nbClientsFileNormale) {
	traitementStations(nbStations, tabStations, coutTotalPresenceClient);
	traitementFileExpress(nbStations, nbClientsFileExpress, tabFileExpress, tabStations, coutTotalPresenceClient);
	traitementFileNormale(nbStations, tabStations, nbClientsFileNormale, tabFileNormale, coutTotalPresenceClient);
}

void traitementStations(int nbStations, Station tabStations[], double  *coutTotalPresenceClient) {
	int iStation = 0;
	while (iStation < nbStations) {
		if (estLibre(tabStations, iStation)) {
			tabStations[iStation].coutTotalService += 18. / 60;
		}
		else {
			if (tabStations[iStation].clientServi.estPrioritaire) {
				*coutTotalPresenceClient += 42.5 / 60;
			}
			else {
				if (tabStations[iStation].clientServi.dureeServiceInitiale <= 3) {
					*coutTotalPresenceClient += 37.5 / 60;
				}
				else {
					*coutTotalPresenceClient += 25.5 / 60;
				}
			}
			tabStations[iStation].clientServi.dureeServiceRestante--;
		}
		iStation++;
	}
}

void traitementFileNormale(int nbStations, Station tabStations[], int * nbClientsFileNormale, Client tabFileNormale[], double *coutTotalPresenceClient) {
	int iFileNormale = 0;
	int iStation;
	bool possibiliteStationLibre = true;
	while (iFileNormale < *nbClientsFileNormale) {
		if (possibiliteStationLibre) {
			iStation = 0;
			while (iStation < nbStations && !estLibre(tabStations, iStation)) {
				iStation++;
			}
			if (iStation == nbStations) {
				possibiliteStationLibre = false;
			}
		}

		if (tabFileNormale[iFileNormale].dureeServiceInitiale <= 3) {
			*coutTotalPresenceClient += 37.5 / 60;
			if (!possibiliteStationLibre && iFileNormale == 0) {
				int iStation = 0;
				while (iStation < nbStations && tabStations[iStation].clientServi.dureeServiceRestante < 5 && tabStations[iStation].clientServi.estPrioritaire) {
					iStation++;
				}

				if (iStation != nbStations) {
					Client clientMemorise = tabFileNormale[iFileNormale];
					tabStations[iStation].clientServi.estPrioritaire = true;
					tabFileNormale[iFileNormale] = tabStations[iStation].clientServi;
					tabStations[iStation].clientServi = clientMemorise;
				}
			}
			else {
				if (tabFileNormale[iFileNormale].estPrioritaire) {
					*coutTotalPresenceClient += 42.5 / 60;
				}
				else {
					*coutTotalPresenceClient += 25.5 / 60;
				}
			}
		}
		if (possibiliteStationLibre) {
			suppressionClientFile(tabStations, iStation, tabFileNormale, nbClientsFileNormale);
		}
		iFileNormale++;
	}
}


void traitementFileExpress(int nbStations, int * nbClientsFileExpress, Client tabFileExpress[], Station tabStations[], double  *coutTotalPresenceClient) {
	int iFileExpress = 0;
	bool possibiliteStationLibre = true;
	while (iFileExpress < *nbClientsFileExpress && possibiliteStationLibre) {
		*coutTotalPresenceClient += 37.5 / 60;
		int iStation = 0;
		while (iStation < nbStations && !estLibre(tabStations, iStation)) {
			iStation++;
		}

		if (iStation == nbStations) {
			possibiliteStationLibre = false;
		}
		else {
			tabStations[iStation].clientServi = tabFileExpress[iFileExpress];
			suppressionClientFile(tabStations, iStation, tabFileExpress, nbClientsFileExpress);
		}
		iFileExpress++;
	}
}



void suppressionClientFile(Station tabStations[], int iStation, Client tabFiles[], int * tailleFille) {
	tabStations[iStation].clientServi = tabFiles[0];

	for (int i = 0; i < *tailleFille - 1; i++) {
		tabFiles[i] = tabFiles[i + 1];
	}
	*tailleFille = *tailleFille - 1;
}

bool estLibre(Station tabStations[], int iStation) {
	return (tabStations[iStation].clientServi.dureeServiceRestante == 0);
}



