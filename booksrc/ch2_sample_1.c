#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

static const size_t NUM_ITERATIONS = 100;
static const size_t NUM_THREADS = 4;

typedef struct
{
    pthread_mutex_t lock;
    size_t value;
} counter_t;

counter_t* counter_init(size_t initial_value)
{
    counter_t* counter = (counter_t*)malloc(sizeof(counter_t));
    counter->value = initial_value;
    pthread_mutex_init(&counter->lock, NULL);
    return counter;
}

void counter_free(counter_t* counter)
{
    pthread_mutex_destroy(&counter->lock);
    free(counter);
}

void counter_increment(counter_t* counter)
{
    pthread_mutex_lock(&counter->lock);
    ++counter->value;
    pthread_mutex_unlock(&counter->lock);
}

void counter_decrement(counter_t* counter)
{
    pthread_mutex_lock(&counter->lock);
    --counter->value;
    pthread_mutex_unlock(&counter->lock);
}

void* test_counter(void* param)
{
    counter_t* counter = (counter_t*)param;
    for (size_t i = 0; i < NUM_ITERATIONS; ++i)
    {
        counter_increment(counter);
    }
    return NULL;
}

int main(int argc, char const* argv[])
{
    counter_t* counter = counter_init(0);
    pthread_t threads[NUM_THREADS];

    for (size_t i = 0; i < NUM_THREADS; ++i)
    {
        pthread_create(&threads[i], NULL, &test_counter, counter);
    }

    for (size_t i = 0; i < NUM_THREADS; ++i)
    {
        pthread_join(threads[i], NULL);
    }

    printf("counter: %ld\n", counter->value);

    counter_free(counter);
    return 0;
}