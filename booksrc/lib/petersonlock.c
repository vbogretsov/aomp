#include "petersonlock.h"

#include <barrier.h>
#include <stdbool.h>
#include <stdlib.h>

#define __NTHREADS 2

struct petersonlock {
    bool flag[__NTHREADS];
    int  victim;
};

petersonlock_t* petersonlock_new() {
    petersonlock_t* self = (petersonlock_t*)malloc(sizeof(petersonlock_t));
    if (!self) {
        return NULL;
    }

    self->victim = 0;
    for (size_t i = 0; i < __NTHREADS; i++) {
        self->flag[i] = false;
    }

    return self;
}

void petersonlock_free(petersonlock_t* self) {
    free((void*)self);
}

void peterson_lock(petersonlock_t* self, int tid) {
    int other = 1 - tid;

    self->flag[tid] = true;
    BARRIER;
    self->victim = tid;

    while (self->flag[other] && self->victim == tid) {
        // wait;
    }
}

void peterson_unlock(petersonlock_t* self, int tid) {
    self->flag[tid] = false;
}