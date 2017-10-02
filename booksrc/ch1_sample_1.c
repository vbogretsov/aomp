#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

const int NTHREADS = 10;
const int MIN_INTEGER = 0;
const int MAX_INTEGER = 100000;
const int RANGE_LENGTH = (MAX_INTEGER - MIN_INTEGER) / NTHREADS;

typedef long int integer_t;

typedef struct
{
    integer_t a;
    integer_t b;
} range_t;

bool isprime(integer_t n)
{
    bool result = true;
    for (integer_t i = 2; i < n / 2; ++i)
    {
        if (n % i == 0)
        {
            result = false;
            break;
        }
    }
    return result;
}

void find_primes(void* param)
{
    clock_t start = clock();
    size_t primes = 0;
    range_t* range = (range_t*)param;
    for (int i = range->a; i < range->b; ++i)
    {
        if (isprime(i))
        {
            ++primes;
        }
    }
    clock_t finish = clock();
    double elapsed = 1000.0 * (finish - start) / CLOCKS_PER_SEC;
    printf("range [%ld, %ld[, %ld primes found in %f2 ms\n", range->a, range->b,
           primes, elapsed);
}

int main(int argc, char const* argv[])
{
    pthread_t threads[NTHREADS];
    range_t ranges[NTHREADS];
    clock_t start = clock();

    for (int i = 0; i < NTHREADS; ++i)
    {
        ranges[i].a = MIN_INTEGER + i * RANGE_LENGTH;
        ranges[i].b = MIN_INTEGER + (i + 1) * RANGE_LENGTH;
        pthread_create(&(threads[i]), NULL, &find_primes, &(ranges[i]));
    }

    for (int i = 0; i < NTHREADS; ++i)
    {
        pthread_join(threads[i], NULL);
    }

    clock_t finish = clock();
    double elapsed = 1000.0 * (finish - start) / CLOCKS_PER_SEC;
    printf("completed in %f2 mc\n", elapsed);

    return 0;
}
