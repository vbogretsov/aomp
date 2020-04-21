#include <filterlock.h>
#include <pthread.h>
#include <stdio.h>

#define NTHREADS 8
#define NITERS 10000

static filterlock_t* lock;

static int N = 0;

static void* inc(void* ctx) {
    int tid = (int)ctx;

    for (size_t i = 0; i < NITERS; i++) {
        filterlock_lock(lock, tid);
        ++N;
        filterlock_unlock(lock, tid);
    }

    return NULL;
}

int main() {
    lock = filterlock_new(NTHREADS);

    pthread_t threads[NTHREADS];
    for (size_t i = 0; i < NTHREADS; i++) {
        pthread_create(&threads[i], NULL, &inc, (void*)i);
    }

    for (size_t i = 0; i < NTHREADS; i++) {
        pthread_join(threads[i], NULL);
        pthread_cancel(threads[i]);
    }

    filterlock_free(lock);

    printf("%d\n", N);

    return N == NTHREADS * NITERS ? 0 : 1;
}
