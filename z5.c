#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define N 5

pthread_mutex_t m;
pthread_cond_t red[N];

int vilica[N]; //inicijalizacija vilice
char filozof[N]; //inicijalizacija filozofa

void ispiši_stanje(int n) //ipsis stanja u kojem se filozof nalazi
{
    printf("Stanje filozofa: ");
    for (int i = 0; i < N; i++)
        printf("%c ", filozof[i]);
    printf("\n");
}

void misliti()
{
    sleep(3);
}

void cekaj_u_redu_uvjeta(pthread_cond_t *c)
{
    pthread_cond_wait(c, &m);
}

void oslobodi_dretvu_iz_reda(pthread_cond_t *c)
{
    pthread_cond_signal(c);
}

void uđi_u_kritični_odsječak()
{
    pthread_mutex_lock(&m);
}

void izađi_iz_kritičnog_odsječka()
{
    pthread_mutex_unlock(&m);
}

/*funkcija jesti predstavlja stanje zadano prema uputama zadatka*/
void jesti(int n) 
{
    uđi_u_kritični_odsječak();

    filozof[n] = 'o';

    while (vilica[n] == 0 || vilica[(n + 1) % N] == 0)
        cekaj_u_redu_uvjeta(&red[n]);

    vilica[n] = vilica[(n + 1) % N] = 0;

    filozof[n] = 'X';

    ispiši_stanje(n);

    izađi_iz_kritičnog_odsječka();

    sleep(2);
}

/* izlazak iz jela */
void zavrsi_jelo(int n)
{
    uđi_u_kritični_odsječak();

    filozof[n] = 'O';

    vilica[n] = vilica[(n + 1) % N] = 1;

    oslobodi_dretvu_iz_reda(&red[(n - 1 + N) % N]);
    oslobodi_dretvu_iz_reda(&red[(n + 1) % N]);

    ispiši_stanje(n);

    izađi_iz_kritičnog_odsječka();
}

/* filozof i */
void* filozof_funkcija(void *arg)
{
    int n = *(int*)arg;

    while (1)
    {
        misliti();
        jesti(n);
        zavrsi_jelo(n);
    }

    return NULL;
}

/*glavni program*/
int main()
{
    pthread_t t[N];
    int id[N];

    pthread_mutex_init(&m, NULL);

    for (int i = 0; i < N; i++)
    {
        vilica[i] = 1;
        filozof[i] = 'O';
        pthread_cond_init(&red[i], NULL);
    }

    for (int i = 0; i < N; i++)
    {
        id[i] = i;
        pthread_create(&t[i], NULL, filozof_funkcija, &id[i]);
    }

    for (int i = 0; i < N; i++)
        pthread_join(t[i], NULL);

    return 0;
}