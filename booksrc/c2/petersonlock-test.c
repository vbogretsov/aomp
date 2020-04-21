#include <petersonlock.h>
#include <pthread.h>
#include <stdio.h>

#define NTHREADS 2
#define NITERS 100000

static petersonlock_t* lock;

static int N = 0;

static void* inc(void* ctx) {
    int tid = (int)ctx;

    for (size_t i = 0; i < NITERS; i++) {
        peterson_lock(lock, tid);
        ++N;
        peterson_unlock(lock, tid);
    }

    return NULL;
}

int main() {
    lock = petersonlock_new();

    pthread_t threads[NTHREADS];
    for (size_t i = 0; i < NTHREADS; i++) {
        pthread_create(&threads[i], NULL, &inc, (void*)i);
    }

    for (size_t i = 0; i < NTHREADS; i++) {
        pthread_join(threads[i], NULL);
        pthread_cancel(threads[i]);
    }

    petersonlock_free(lock);

    printf("%d\n", N);

    return N == NTHREADS * NITERS ? 0 : 1;
}