#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <math.h>

int broj; //definiranje broja

char obradaDat[] = "obrada.txt"; //datoteka obrada.txt
char statusDat[] = "status.txt";//datoteka status.txt- obje datoteke su stvorene ručno

/* petlja za ispis trenutnog broja*/
void sigusr1_handler(int sig)
{
    printf("Trenutni broj: %d\n", broj);
}

/* SIGTERM za spremanje stanja i izlaza */
void sigterm_handler(int sig)
{
    FILE *f = fopen(statusDat, "w"); 
    if (f != NULL)
    {
        fprintf(f, "%d", broj);
        fclose(f);
    }
    exit(0);
}

/* SIGINT izlaz po defaultu na 0, da nije 0, ispisao bi korijene sve od brojeva u obrada.txt, koji su kvadrati svih brojeva u 
beskonačnoj petlji */
void sigint_handler(int sig)
{
    exit(0);
}

/* glavni program */
int main()
{
    FILE *f; // inicijaliziranje datoteka 
    int status;
    int zadnji;
    int x;

    /* signali i njihove naredbe */
    signal(SIGUSR1, sigusr1_handler);
    signal(SIGTERM, sigterm_handler);
    signal(SIGINT, sigint_handler);

    /* čitanje datoteke status.txt */
    f = fopen(statusDat, "r");
    if (f == NULL) return 1;

    fscanf(f, "%d", &status);
    fclose(f);

    /* ako je status 0 oporavak iz obrada.txt */
    if (status == 0)
    {
        f = fopen(obradaDat, "r");
        if (f == NULL) return 1;

        while (fscanf(f, "%d", &zadnji) != EOF);

        fclose(f);

        broj = (int)sqrt(zadnji);
    }
    else
    {
        broj = status;
    }

    /* upisuje 0 u status.txt */
    f = fopen(statusDat, "w");
    if (f != NULL) //provjera stanja datoteke
    {
        fprintf(f, "0");
        fclose(f);
    }

    /* beskonačna petlja koja vrti brojeve u obrada.txt, brojevi idu od 0 do beskonačno, sve dok se program ne prekine*/
    while (1)
    {
        broj++;

        x = broj * broj;

        f = fopen(obradaDat, "a");
        if (f != NULL)
        {
            fprintf(f, "%d\n", x);
            fclose(f);
        }

        sleep(5); // korak između prikaza brojeva u obrada.txt
    }

    return 0;
}
