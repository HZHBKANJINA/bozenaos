#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

int *stolovi; //inicijalizacija pokazivača na stolove
int broj_stolova; //inicijalizacija broja stolova

pthread_mutex_t mutex; //inicijalizacija mutexa

/* provjera ima li slobodnih stolova */
int ima_slobodnih()
{
    for (int i = 0; i < broj_stolova; i++)
    {
        if (stolovi[i] == 0)
        {
            return 1;
        }
    }

    return 0;
}

/* ispis stanja stolova */
void ispisi_stolove()
{
    for (int i = 0; i < broj_stolova; i++)
    {
        if (stolovi[i] == 0)
        {
            printf("-");
        }
        else
        {
            printf("%d", stolovi[i]);
        }
    }

    printf("\n");
}

/*funkcija za rezervaciju stolova putem dretvi uz prethodnu provjeru jeli stol slobodan ili ne */
void* rezervacija(void *arg)
{
    int id = *(int*)arg;

    while (1)
    {
        sleep(1);

        /* prvo provjeri ima li slobodnih stolova */
        if (!ima_slobodnih())
        {
            break;
        }

        /* slučajan odabir stola */
        int stol = rand() % broj_stolova; //uzima slučajan stol

        pthread_mutex_lock(&mutex); //zaštićuje kritični odsječak ako je stol rezerviran

        /* ponovna provjera unutar kritičnog odsječka */
        if (stolovi[stol] == 0)
        {
            stolovi[stol] = id;

            printf("Dretva %d: rezerviram stol %d, stanje:\n", id, stol + 1);
        }
        else
        {
            printf("Dretva %d: neuspjela rezervacija stola %d, stanje:\n", id, stol + 1);
        }

        ispisi_stolove();

        pthread_mutex_unlock(&mutex); //otključava adresu mutexa
    }

    return NULL;
}

/*glavni program */
int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("Upotreba: %s broj_dretvi broj_stolova\n", argv[0]);
        return 1;
    }

    int broj_dretvi = atoi(argv[1]);
    broj_stolova = atoi(argv[2]);

    pthread_t dretve[broj_dretvi];
    int id[broj_dretvi];

    stolovi = malloc(broj_stolova * sizeof(int));

    for (int i = 0; i < broj_stolova; i++)
    {
        stolovi[i] = 0;
    }

    pthread_mutex_init(&mutex, NULL);

    srand(time(NULL));

    /* stvaranje dretvi */
    for (int i = 0; i < broj_dretvi; i++)
    {
        id[i] = i + 1;

        pthread_create(&dretve[i], NULL, rezervacija, &id[i]);
    }

    /* čekanje dretvi */
    for (int i = 0; i < broj_dretvi; i++)
    {
        pthread_join(dretve[i], NULL);
    }

    pthread_mutex_destroy(&mutex);

    free(stolovi);

    return 0;
}