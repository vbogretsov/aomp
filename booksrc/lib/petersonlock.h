#ifndef PETERSONLOCK_H
#define PETERSONLOCK_H

typedef struct petersonlock petersonlock_t;

/**
 * Creates new patersonlock.
 */
petersonlock_t* petersonlock_new();

/**
 * Deallocates petersonlock.
 * @param self pointer to be deallocated.
 */
void petersonlock_free(petersonlock_t* self);

/**
 * Enter critical section.
 * @param self lock object.
 * @param tid thread ID, 0 or 1 only.
 */
void peterson_lock(petersonlock_t* self, int tid);

/**
 * Exit critical section.
 * @param self lock object.
 * @param tid thread ID, 0 or 1 only.
 */
void peterson_unlock(petersonlock_t* self, int tid);

#endif // ! PETERSONLOCK_H