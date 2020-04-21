#include <pthread.h>
#include <stdatomic.h>
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
    atomic_long counter;
    integer_t max_value;
} param_t;

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

integer_t increment(atomic_long* counter)
{
    return atomic_fetch_add(counter, 1);
}

void find_primes(void* arg)
{
    clock_t start = clock();
    param_t* param = (param_t*)arg;
    size_t primes = 0;
    integer_t number;
    while ((number = increment(&(param->counter))) < param->max_value)
    {
        if (isprime(number))
        {
            ++primes;
        }
    }
    clock_t finish = clock();
    double elapsed = 1000.0 * (finish - start) / CLOCKS_PER_SEC;
    printf("%ld primes found in %f2 ms\n", primes, elapsed);
}

int main(int argc, char const* argv[])
{
    pthread_t threads[NTHREADS];
    param_t param;
    param.counter = 0;
    param.max_value = MAX_INTEGER;
    clock_t start = clock();

    for (int i = 0; i < NTHREADS; ++i)
    {
        pthread_create(&(threads[i]), NULL, &find_primes, &param);
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
