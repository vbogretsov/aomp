#include <unistd.h>
#include <pthread.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>

#define N 5

typedef enum
{
    THINKING,
    EATING,
} status_t;

static const int MAX_TIMEOUT = 10;
static atomic_bool RUNNING = ATOMIC_VAR_INIT(true);
static pthread_mutex_t CHOPSTICS[N];
static pthread_mutex_t GUARD;
static status_t STATUSES[N];
static int STAT[N];

static void showstat()
{
    printf("%s", "\n-------------------\n");
    for (int i = 0; i < N; i++)
    {
        printf("philosopher %d eaten %d times\n", i, STAT[i]);
    }
    printf("%s", "\n");
}

static void handle_sigint(int sig)
{
    atomic_store(&RUNNING, false);
    // showstat();
}

static void *philosopher2(void *ctx)
{
    int number = (int)ctx;
    while (atomic_load(&RUNNING))
    {
        printf("philosopher %d thinking\n", number);
        usleep(rand() % MAX_TIMEOUT);

        if (!pthread_mutex_trylock(&GUARD))
        {
            continue;
        }

        int l = (N + number - 1) % N;
        int r = (N + number + 1) % N;

        if (STATUSES[l] != EATING && STATUSES[r] != EATING)
        {
            STATUSES[number] = EATING;
        }

        pthread_mutex_unlock(&GUARD);

        if (STATUSES[number] != EATING)
        {
            continue;
        }

        pthread_mutex_lock(&CHOPSTICS[l < r ? l : r]);
        pthread_mutex_lock(&CHOPSTICS[l < r ? r : l]);

        ++STAT[number];
        printf("philosopher %d eating\n", number);
        usleep(rand() % MAX_TIMEOUT);

        pthread_mutex_unlock(&CHOPSTICS[r]);
        pthread_mutex_unlock(&CHOPSTICS[l]);

        STATUSES[number] = THINKING;
    }
    // try to become eating
    // take forks
    // eat
    // put forks
}

static void *philosopher1(void *ctx)
{
    int number = (int)ctx;

    while (atomic_load(&RUNNING))
    {
        printf("philosopher %d thinking\n", number);
        usleep(rand() % MAX_TIMEOUT);

        int l = number;
        int r = (number + 1) % N;

        pthread_mutex_lock(&CHOPSTICS[l < r ? l : r]);
        pthread_mutex_lock(&CHOPSTICS[l < r ? r : l]);

        ++STAT[number];
        printf("philosopher %d eating\n", number);
        usleep(rand() % MAX_TIMEOUT);

        pthread_mutex_unlock(&CHOPSTICS[r]);
        pthread_mutex_unlock(&CHOPSTICS[l]);
    }

    printf("philosopher %d exiting\n", number);
    return NULL;
}

int main()
{
    srand(time(NULL));
    signal(SIGINT, handle_sigint);

    pthread_mutex_init(&GUARD, NULL);

    pthread_t philosophers[N];
    for (int i = 0; i < N; i++)
    {
        STAT[i] = 0;
        STATUSES[i] = THINKING;
        pthread_mutex_init(&CHOPSTICS[i], NULL);
        pthread_create(&philosophers[(int)i], NULL, philosopher2, (void *)i);
    }

    for (int i = 0; i < N; i++)
    {
        pthread_join(philosophers[i], NULL);
        pthread_cancel(philosophers[i]);
        pthread_mutex_destroy(&CHOPSTICS[i]);
    }

    showstat();

    pthread_mutex_destroy(&GUARD);

    return 0;
}