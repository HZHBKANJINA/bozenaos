#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>

#define N 5   // broj mjesta

/* inicijalizacija semafora*/
sem_t slobodna_mjesta;
sem_t puna;
sem_t mutex;

int ukrcani = 0;

void* posjetitelj(void* arg) //funkcija brojanja posjetitelja
{
    int id = *(int*)arg;

    while (1)
    {
        sleep(rand() % 3);

        sem_wait(&slobodna_mjesta);   // čeka slobodno mjesto
        sem_wait(&mutex);

        ukrcani++;
        printf("Posjetitelj %d sjeo (%d/%d)\n", id, ukrcani, N);

        if (ukrcani == N)
        {
            sem_post(&puna); // vrtuljak se može pokrenuti
        }

        sem_post(&mutex);
    }

    return NULL;
}

void* vrtuljak(void* arg) //funkcija pokretanja vrtuljka uz provjere
{
    while (1)
    {
        sem_wait(&puna); // čeka da se napuni

        printf("\n Vrtuljak kreće!\n\n");
        sleep(2);

        printf("\n Vrtuljak staje!\n\n");

        sem_wait(&mutex);

        ukrcani = 0;

        for (int i = 0; i < N; i++)
        {
            sem_post(&slobodna_mjesta); // svi izlaze
        }

        sem_post(&mutex);
    }

    return NULL;
}

int main() // glavni program
{
    pthread_t t_vrtuljak, t_posjetitelji[10];
    int id[10];

    srand(time(NULL));

    sem_init(&slobodna_mjesta, 0, N);
    sem_init(&puna, 0, 0);
    sem_init(&mutex, 0, 1);

    pthread_create(&t_vrtuljak, NULL, vrtuljak, NULL);

    for (int i = 0; i < 10; i++)
    {
        id[i] = i + 1;
        pthread_create(&t_posjetitelji[i], NULL, posjetitelj, &id[i]);
    }

    for (int i = 0; i < 10; i++)
    {
        pthread_join(t_posjetitelji[i], NULL);
    }

    pthread_join(t_vrtuljak, NULL);

    return 0;
}