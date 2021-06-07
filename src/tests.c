#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "queue.c"
#include "dict.c"

void main()
{
    const char *temp_value;
    bool all_passing = true;
    pq_enqueue('s', "test0");
    pq_enqueue('s', "test");
    temp_value = pq_dequeue();
    if (strcmp(temp_value, "test0"))
    {
        all_passing = false;
        printf("Assertion #1 found [%d] an unexpected value was pq_dequeued\n", temp_value);
    }
    temp_value = pq_dequeue();
    if (!is_pq_empty())
    {
        all_passing = false;
        printf("Assertion #2 found an unexpected non-empty queue\n");
    }
    pq_enqueue('s', "test");
    pq_enqueue('s', "test");
    pq_enqueue('s', "test");
    pq_enqueue('s', "test");
    pq_clear();
    if (!is_pq_empty())
    {
        all_passing = false;
        printf("Assertion #3 found an unexpected non-empty queue\n");
    }
    pq_enqueue('s', "test-a");
    pq_enqueue('s', "test-b");
    pq_enqueue('s', "test-c");
    temp_value = pq_dequeue();
    if (strcmp(temp_value, "test-a"))
    {
        all_passing = false;
        printf("Assertion #4.1 found [%d] an unexpected value was pq_dequeued\n", temp_value);
    }
    temp_value = pq_dequeue();
    if (strcmp(temp_value, "test-b"))
    {
        all_passing = false;
        printf("Assertion #4.2 found [%d] an unexpected value was pq_dequeued\n", temp_value);
    }
    // test requeue
    pq_requeue('s', "test-z");
    temp_value = pq_dequeue();
    if (strcmp(temp_value, "test-z"))
    {
        all_passing = false;
        printf("Assertion #5 found [%d] an unexpected value was pq_dequeued\n", temp_value);
    }
    pq_requeue('s', "test 5");
    pq_requeue('s', "test 4");
    temp_value = pq_dequeue();
    if (strcmp(temp_value, "test 4"))
    {
        all_passing = false;
        printf("Assertion #6 found [%d] an unexpected value was pq_dequeued\n", temp_value);
    }
    temp_value = pq_dequeue();
    if (strcmp(temp_value, "test 5"))
    {
        all_passing = false;
        printf("Assertion #7 found [%d] an unexpected value was pq_dequeued\n", temp_value);
    }
    for (int t = 0; t < 5000; t++)
    {
        for (int i = 0; i < 5000; i++)
        {
            pq_requeue('s', "test-z");
        }
        for (int i = 0; i < 5000; i++)
        {
            temp_value = pq_dequeue();
        }
    }

    dictionary *d = dictionary_new(10);
    dictionary_set(d, "q", "abc");
    dictionary_set(d, "w", "thi");
    dictionary_set(d, "e", "sis");
    dictionary_set(d, "r", "the");
    dictionary_set(d, "t", "first");
    dictionary_set(d, "y", "dup");
    dictionary_set(d, "u", "ini");
    dictionary_set(d, "i", "drop");
    dictionary_set(d, "q2", "abc");
    dictionary_set(d, "w2", "thi");
    dictionary_set(d, "e2", "sis");
    dictionary_set(d, "r2", "the");
    dictionary_set(d, "t2", "first");
    dictionary_set(d, "y2", "dup");
    dictionary_set(d, "u2", "ini");
    dictionary_set(d, "i2", "drop");
    dictionary_set(d, "i2", "nope");
    dictionary_dump(d);

    if (all_passing)
    {
        printf("all tests pass\n");
    }
}
