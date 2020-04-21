#ifndef BARRIER_H
#define BARRIER_H

/**
 * General memory barrier: prevents both instruction reordering by compiler and
 * load/store operations reordering by CPU.
 */
#define BARRIER asm volatile("mfence" ::: "memory")

#endif // ! BARRIER_H