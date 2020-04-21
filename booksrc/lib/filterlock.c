#include "filterlock.h"

#include <barrier.h>
#include <stdlib.h>

struct filterlock {
    int  n;
    int* level;
    int* victim;
};

filterlock_t* filterlock_new(int n) {
    filterlock_t* self = (filterlock_t*)malloc(sizeof(filterlock_t));
    if (!self) {
        return self;
    }

    self->n = n;

    self->level = (int*)calloc(n, sizeof(int));
    if (!self->level) {
        free(self);
        return NULL;
    }

    self->victim = (int*)calloc(n, sizeof(int));
    if (!self->victim) {
        free(self->level);
        free(self);
        return NULL;
    }

    return self;
}

void filterlock_free(filterlock_t* self) {
    free((void*)self->victim);
    free((void*)self->level);
    free(self);
}

void filterlock_lock(filterlock_t* self, int tid) {
    for (int i = 1; i < self->n; i++) {
        self->level[tid] = i;
        BARRIER;
        self->victim[i] = tid;

        for (int k = 0; k < self->n; k++) {
            if (k != tid) {
                while (self->level[k] >= i && self->victim[i] == tid) {
                    // wait
                }
            }
        }
    }
}

void filterlock_unlock(filterlock_t* self, int tid) {
    self->level[tid] = 0;
}