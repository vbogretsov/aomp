#ifndef FILTERLOCK_H
#define FILTERLOCK_H

typedef struct filterlock filterlock_t;

/**
 * Creates new patersonlock.
 * @param n number of threads to be syncrhonized.
 */
filterlock_t* filterlock_new(int n);

/**
 * Deallocates petersonlock.
 * @param self pointer to be deallocated.
 */
void filterlock_free(filterlock_t* self);

/**
 * Enter critical section.
 * @param self lock object.
 * @param tid thread ID, 0 or 1 only.
 */
void filterlock_lock(filterlock_t* self, int tid);

/**
 * Exit critical section.
 * @param self lock object.
 * @param tid thread ID, 0 or 1 only.
 */
void filterlock_unlock(filterlock_t* self, int tid);

#endif // ! FILTERLOCK_H