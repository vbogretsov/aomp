#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <pthread.h>

static const size_t NUM_TIMES = 5000000;

typedef struct
{
    bool flags[2];
    int current;
} lock_t;

typedef struct
{
    lock_t* lck;
    int tid;
    size_t* cnt;
} tls_t;

void lock(lock_t* lck, int tid)
{
    int other = 1 - tid;
    lck->flags[tid] = true;
    lck->current = tid;

    while (lck->current == tid && lck->flags[other])
    {
        // wait
    }
}

void unlock(lock_t* lck, int tid)
{
    lck->flags[tid] = false;
}

void* start(void* context)
{
    tls_t* tls = (tls_t*)context;

    for (int i = 0; i < NUM_TIMES; ++i)
    {
        lock(tls->lck, tls->tid);
        ++(*tls->cnt);
        unlock(tls->lck, tls->tid);
    }

    return NULL;
}

int main(int argc, char const* argv[])
{
    size_t cnt = 0;

    lock_t lck;
    lck.flags[0] = false;
    lck.flags[1] = false;
    lck.current = 0;

    tls_t tlss[2];
    tlss[0].tid = 0;
    tlss[0].cnt = &cnt;
    tlss[0].lck = &lck;
    tlss[1].tid = 1;
    tlss[1].cnt = &cnt;
    tlss[1].lck = &lck;

    pthread_t threads[2];
    pthread_create(&threads[0], NULL, &start, &tlss[0]);
    pthread_create(&threads[1], NULL, &start, &tlss[1]);

    pthread_join(threads[0], NULL);
    pthread_join(threads[1], NULL);

    printf("counter: %ld\n", cnt);

    return 0;
}