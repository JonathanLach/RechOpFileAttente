#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include<limits.h>

#define A 69069
#define C 0
#define GERME 292
#define FALSE 0
#define TRUE 1
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
	int estPrioritaire;
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
int main(void)
{
	int nbStations = NBSTATIONSMIN;
	double coutTotalMin = 100000000;
	Station tabStations[NBSTATIONSMAX];
	Client tabFileExpress[TAILLEFILEEXPRESS];
	Client tabFileNormale[10000];
	unsigned int generationPrecedente = GERME;
	int nbClientsFileExpress = 0;
	int nbClientsFileNormale = 0;
	int coutPassageClientExpressEnFileNormale = 0;
	Client clientGenere;
	int iGeneration = 0;
	initTableaux(tabStations, nbStations,&nbClientsFileExpress,&nbClientsFileNormale);
	while (iGeneration < 50)
	{
		clientGenere = generationClient(&generationPrecedente);
		repartirClient(clientGenere, tabStations, tabFileNormale, tabFileExpress, &nbClientsFileExpress, &nbClientsFileNormale, nbStations, &coutPassageClientExpressEnFileNormale);
		iGeneration++;
	}
	printf("Etat de la file express:\n");
	montrerFile(tabFileExpress, nbClientsFileExpress);
	printf("Etat de la file normale:\n");
	montrerFile(tabFileNormale, nbClientsFileNormale);
	montrerTabStations(tabStations, nbStations);
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
		tabStations[iStation].clientServi.estPrioritaire = FALSE;
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
		printf("client %d : duree de service : %d prioritaire : %d\n", iFile, file[iFile].dureeServiceInitiale, file[iFile].estPrioritaire);
		iFile++;
	}
}
void montrerTabStations(Station *tabStations, int nbStations)
{
	int iStation = 0;
	printf("Etat du tableau des stations:\n");
	while (iStation < nbStations)
	{
		printf("Station %d : duree de service: %d prioritaire: %d\n", iStation, tabStations[iStation].clientServi.dureeServiceInitiale, tabStations[iStation].clientServi.estPrioritaire);
		iStation++;
	}
}
Client generationClient(unsigned int *generationPrecedente)
{
	Client nouveauClient;
	int dureeServiceGeneree = genererDureeService(generationPrecedente);
	nouveauClient.dureeServiceInitiale = dureeServiceGeneree;
	nouveauClient.dureeServiceRestante = dureeServiceGeneree;
	nouveauClient.estPrioritaire = FALSE;
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
	while (i < 2 && tabStations[i].clientServi.dureeServiceRestante!=0)
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
	while (iStation < nbStations && tabStations[iStation].clientServi.dureeServiceRestante != 0)
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

