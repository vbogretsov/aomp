#include "bakerylock.h"

#include <barrier.h>
#include <stdbool.h>
#include <stdlib.h>

static int getmax(int* array, int len) {
    if (len < 1) {
        return -1;
    }

    int max = array[0];
    for (int i = 1; i < len; i++) {
        if (max < array[i]) {
            max = array[i];
        }
    }

    return max;
}

static inline bool less(int* labels, int i, int j) {
    int a = labels[i];
    int b = labels[j];
    return a == b ? i < j : a < b;
}

struct bakerylock {
    int   n;
    bool* flag;
    int*  label;
};

bakerylock_t* bakerylock_new(int n) {
    bakerylock_t* self = (bakerylock_t*)malloc(sizeof(bakerylock_t));
    if (!self) {
        return NULL;
    }

    self->n = n;

    self->flag = (bool*)calloc(n, sizeof(bool));
    if (!self->flag) {
        free(self);
        return NULL;
    }

    self->label = (int*)calloc(n, sizeof(int));
    if (!self->label) {
        free(self->flag);
        free(self);
        return NULL;
    }

    return self;
}

void bakerylock_free(bakerylock_t* self) {
    free((void*)self->label);
    free((void*)self->flag);
    free((void*)self);
}

void bakerylock_lock(bakerylock_t* self, int tid) {
    self->flag[tid] = true;
    BARRIER;
    self->label[tid] = getmax(self->label, self->n) + 1;
    BARRIER;

    for (int k = 0; k < self->n; k++) {
        if (k != tid) {
            while (self->flag[k] && less(self->label, k, tid)) {
                // wait
            }
        }
    }
}

void bakerylock_unlock(bakerylock_t* self, int tid) {
    self->flag[tid] = false;
}