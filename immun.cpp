#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define LAENGE 50
#define ANTIKOERPER 40
#define FREMDE_ANTIGENE 100 
#define EIGENE_ANTIGENE 10
#define WAEHLE_BESTE 20
#define DURCHLAEUFE 250
			
void print_gene(const char* string, int* array, int size, int length)
{
	printf("%s", string);
	for(int i = 0; i < size; i++)
	{
		for(int j = 0; j < length; j++)
			printf("%i", array[j+i*length]);
		if(i != size-1)
			printf(", ");
		else printf("\n");
	}
}

void mutiere(int* array, int length)
{
	for(int i = 0; i < 2; i++)
	{
	int punktmutation = rand()%length;
	int wert = array[punktmutation];
	if(wert == 0)
		array[punktmutation] = 1;
	else array[punktmutation] = 0;
	}
}

int fitness_funktion(int a, int b)
{
	if((a == 0) && (b == 0))
		return 0;
	else 
	if((a == 1) && (b == 0))
		return 1;
	else
	if((a == 0) && (b == 1))
		return 1;
	else
	if((a == 1) && (b == 1))
		return -1;
}

void erstelle_zufaelligen_string(int* array, int length)
{
	for(int i = 0; i < length; i++)
		array[i] = rand()%2;
}

int main()
{
	srand(time(NULL));
	int fremde_antigene[FREMDE_ANTIGENE][LAENGE];
	int eigene_antigene[EIGENE_ANTIGENE][LAENGE];
	int antikoerper[ANTIKOERPER][LAENGE];
	int fitness[ANTIKOERPER];
	int max_fitness[WAEHLE_BESTE];
	int waehle_beste[WAEHLE_BESTE];
	int ersetze[WAEHLE_BESTE];
	int ersetze_zaehler;

	for(int i = 0; i < ANTIKOERPER; i++)
		erstelle_zufaelligen_string(antikoerper[i], LAENGE);
	for(int i = 0; i < FREMDE_ANTIGENE; i++)
		erstelle_zufaelligen_string(fremde_antigene[i], LAENGE);
	for(int i = 0; i < EIGENE_ANTIGENE; i++)
		erstelle_zufaelligen_string(eigene_antigene[i], LAENGE);
	
	print_gene("Koerpereigene Antigene: ", &eigene_antigene[0][0], EIGENE_ANTIGENE, LAENGE);
	print_gene("Koerperfremde Antigene: ", &fremde_antigene[0][0], FREMDE_ANTIGENE, LAENGE);
	
	for(int d = 0; d < DURCHLAEUFE*2; d++)
	{
//		print_gene("Antikoerper: ", &antikoerper[0][0], ANTIKOERPER, LAENGE);
		for(int i = 0; i < ANTIKOERPER; i++)
		{
//			printf("Fitness Antikoerper %i:", i);
			fitness[i] = 0;
			for(int j = 0; j < FREMDE_ANTIGENE; j++)
			{
				int summe = 0;
				for(int k = 0; k < LAENGE; k++)
					summe += fitness_funktion(antikoerper[i][k], fremde_antigene[j][k]);
//				printf("%2i", summe);
//				if(j != FREMDE_ANTIGENE - 1)
//					printf(" + ");
//				else printf(" = ");
				fitness[i] += summe;
			}
//			printf("%3i\n", fitness[i]);
			bool erkennt_selbst = false;
			for(int j = 0; j < EIGENE_ANTIGENE; j++)
			{
				int n = 0;
				for(int k = 0; k < LAENGE; k++)
					n += (fitness_funktion(antikoerper[i][k], eigene_antigene[j][k]) == -1) ? 1 : 0;
				if(n == LAENGE)
				{
					erkennt_selbst = true;
					break;
				}
			}
			if(erkennt_selbst)
			{
				fitness[i] = -9999;
//				printf("Antikoerper %i erkennt koerpereigene Antigene => wird aussortiert\n");
			}
		}

		for(int i = 0; i < WAEHLE_BESTE; i++)
		{
			max_fitness[i] = fitness[i];
			waehle_beste[i] = i;
			ersetze[i] = i+WAEHLE_BESTE;
		}
		for(int i = WAEHLE_BESTE; i < ANTIKOERPER; i++)
		{
			bool schon_gewaehlt = false;
			for(int j = 0; j < WAEHLE_BESTE; j++)
				if(waehle_beste[j] == i)
				{
					schon_gewaehlt = true;
					break;
				}
			if(schon_gewaehlt)
				continue;
			int min_fitness = max_fitness[0];
			int min = 0;
			for(int j = 1; j < WAEHLE_BESTE; j++)
				if(min_fitness > max_fitness[j])
				{
					min_fitness = max_fitness[j];
					min = j;
				}
			
			if(min_fitness < fitness[i])
			{
				max_fitness[min] = fitness[i];
				waehle_beste[min] = i;
			}
		}
		ersetze_zaehler = 0;
		for(int i = 0; i < ANTIKOERPER; i++)
		{
			bool gewaehlt = false;
			for(int j = 0; j < WAEHLE_BESTE; j++)
				if(waehle_beste[j] == i)
				{
					gewaehlt = true;
					break;
				}
			if(!gewaehlt)
			{
				ersetze[ersetze_zaehler] = i;
				ersetze_zaehler++;
			}
		}

		int max = fitness[waehle_beste[0]];
		int best = waehle_beste[0];
		for(int j = 1; j < WAEHLE_BESTE; j++)
			if(fitness[waehle_beste[j]] > max)
			{
				max = fitness[waehle_beste[j]];
				best = waehle_beste[j];
			}
		
		if(d < DURCHLAEUFE)
//		printf("Waehle ");
		for(int j = 0; j < WAEHLE_BESTE; j++)
		{
//			printf("%i", waehle_beste[j]);
//			if(j != WAEHLE_BESTE - 1)
//				printf(", ");
//			else printf(" und mutiere zufaellig an einer Stelle:\n");
			for(int k = 0; k < LAENGE; k++)
				antikoerper[ersetze[j]][k] == antikoerper[waehle_beste[j]][k];
			mutiere(antikoerper[ersetze[j]], LAENGE);
		}
		else
		{
			for(int i = 0; i < ANTIKOERPER; i++)
				if((i != best) || (d == DURCHLAEUFE))
					erstelle_zufaelligen_string(antikoerper[i], LAENGE);
		}
		
		int total = 0;
		for(int i = 0; i < WAEHLE_BESTE; i++)
			total += fitness[waehle_beste[i]];
		if(d == 0)
			printf("Immunsystem mit Immunalgorithmus:\n");
		else if(d == DURCHLAEUFE)
			printf("Immunsystem mit zufaelliger Suche:\n");
		printf("=====> Fitness des Immunsystems: %i [%i]\n", total, max);
	}

	
	return EXIT_SUCCESS;
}

