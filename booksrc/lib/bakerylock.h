#ifndef BAKERYLOCK_H
#define BAKERYLOCK_H

typedef struct bakerylock bakerylock_t;

/**
 * Creates new patersonlock.
 * @param n number of threads to be syncrhonized.
 */
bakerylock_t* bakerylock_new(int n);

/**
 * Deallocates petersonlock.
 * @param self pointer to be deallocated.
 */
void bakerylock_free(bakerylock_t* self);

/**
 * Enter critical section.
 * @param self lock object.
 * @param tid thread ID, 0 or 1 only.
 */
void bakerylock_lock(bakerylock_t* self, int tid);

/**
 * Exit critical section.
 * @param self lock object.
 * @param tid thread ID, 0 or 1 only.
 */
void bakerylock_unlock(bakerylock_t* self, int tid);

#endif // ! BAKERYLOCK_H