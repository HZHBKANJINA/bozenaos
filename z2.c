#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define N 2 // definiranje dvodimenzionalnog prostora, npr. za matrice, u ovom slučaju za niti

int PRAVO = 0; //postavljanje prava na 0
int ZASTAVICA[N] = {0, 0}; // postavljanje zastavica

void udji_u_kriticni_odsjecak(int i, int j) // ulazak u odsječak, mora biti i,j jer je define N 2
{
    ZASTAVICA[i] = 1;

    while (ZASTAVICA[j] != 0) // while beskonačna petlja ako je j od zastavice različit od 0
    {
        if (PRAVO == j)
        {
            ZASTAVICA[i] = 0;

            while (PRAVO == j)
            {
                // čekaj
            }

            ZASTAVICA[i] = 1; // kraj if petlje
        }
    }
}

void izadji_iz_kriticnog_odsjecka(int i, int j) // izlazak iz odsječka
{
    PRAVO = j;
    ZASTAVICA[i] = 0;
}

void* proc(void* arg) // pokazivač na procesor s pozvanim funkcijama iznad
{
    int i = *(int*)arg;
    int j = 1 - i;

    for (int k = 1; k <= 5; k++)
    {
        udji_u_kriticni_odsjecak(i, j);

        for (int m = 1; m <= 5; m++)
        {
            printf("(%d, %d, %d)\n", i, k, m);
            fflush(stdout);
            usleep(100000);
        }

        izadji_iz_kriticnog_odsjecka(i, j);
    }

    return NULL;
}

int main() //glavni program
{
    pthread_t t0, t1;// inicijaliziranje niti
    int id0 = 0, id1 = 1; // postavljanje procesora 0 na 0 i 1 na 1

    pthread_create(&t0, NULL, proc, &id0); //kreiranje niti za procesor id0
    pthread_create(&t1, NULL, proc, &id1);// za id1

    pthread_join(t0, NULL); //pridruživanje niti
    pthread_join(t1, NULL);//pridruživanje niti

    return 0; // prekid programa
}