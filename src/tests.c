#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "queue.c"
#include "dict.c"

pounce_node_ptr make_string_node(char * s){
    pounce_node_ptr n = pounce_init_node();
    n->type = STRING_T;
    n->data->w.s = xstrcp(s);
    return n;
}

void main()
{
    pounce_node_ptr temp_value;
    bool all_passing = true;
    pounce_instance_ptr pq = pounce_init();
    pounce_enpounce_s(pq, STRING_T, "test0");
    pounce_enpounce_s(pq, STRING_T, "test");
    temp_value = pounce_dequeue(pq);
    if (type_s(temp_value) && strcmp(temp_value->data->w.s, "test0"))
    {
        all_passing = false;
        printf("Assertion #1 found [%d] an unexpected value was pounce_dequeued\n", temp_value);
    }
    pounce_free_node(temp_value);
    temp_value = pounce_dequeue(pq);
    pounce_free_node(temp_value);
    if (!is_pounce_empty(pq))
    {
        all_passing = false;
        printf("Assertion #2 found an unexpected non-empty queue\n");
    }
    pounce_enpounce_s(pq, STRING_T, "test");
    pounce_enpounce_s(pq, STRING_T, "test");
    pounce_enpounce_s(pq, STRING_T, "test");
    pounce_enpounce_s(pq, STRING_T, "test");
    pounce_clear(pq);
    if (!is_pounce_empty(pq))
    {
        all_passing = false;
        printf("Assertion #3 found an unexpected non-empty queue\n");
    }
    pounce_enpounce_s(pq, STRING_T, "test-a");
    pounce_enpounce_s(pq, STRING_T, "test-b");
    pounce_enpounce_s(pq, STRING_T, "test-c");
    temp_value = pounce_dequeue(pq);
    if (type_s(temp_value) && strcmp(temp_value->data->w.s, "test-a"))
    {
        all_passing = false;
        printf("Assertion #4.1 found [%d] an unexpected value was pounce_dequeued\n", temp_value);
    }
    pounce_free_node(temp_value);
    temp_value = pounce_dequeue(pq);
    if (type_s(temp_value) && strcmp(temp_value->data->w.s, "test-b"))
    {
        all_passing = false;
        printf("Assertion #4.2 found [%d] an unexpected value was pounce_dequeued\n", temp_value);
    }
    pounce_free_node(temp_value);
    // test requeue
    pounce_repounce_s(pq, "test-z");
    temp_value = pounce_dequeue(pq);
    if (type_s(temp_value) && strcmp(temp_value->data->w.s, "test-z"))
    {
        all_passing = false;
        printf("Assertion #5 found [%d] an unexpected value was pounce_dequeued\n", temp_value);
    }
    pounce_free_node(temp_value);
    pounce_repounce_s(pq, "test 5");
    pounce_repounce_s(pq, "test 4");
    temp_value = pounce_dequeue(pq);
    if (type_s(temp_value) && strcmp(temp_value->data->w.s, "test 4"))
    {
        all_passing = false;
        printf("Assertion #6 found [%d] an unexpected value was pounce_dequeued\n", temp_value);
    }
    pounce_free_node(temp_value);
    temp_value = pounce_dequeue(pq);
    if (type_s(temp_value) && strcmp(temp_value->data->w.s, "test 5"))
    {
        all_passing = false;
        printf("Assertion #7 found [%d] an unexpected value was pounce_dequeued\n", temp_value);
    }
    pounce_free_node(temp_value);
    for (int t = 0; t < 5000; t++)
    {
        for (int i = 0; i < 5000; i++)
        {
            pounce_repounce_s(pq, "test-z");
        }
        for (int i = 0; i < 5000; i++)
        {
            temp_value = pounce_dequeue(pq);
            pounce_free_node(temp_value);
        }
    }

    dictionary *d = dictionary_new(10);
    dictionary_set(d, "qs", make_string_node("abc"));
    dictionary_set(d, "w", make_string_node("thi"));
    dictionary_set(d, "e", make_string_node("sis"));
    dictionary_set(d, "r", make_string_node("the"));
    dictionary_set(d, "t", make_string_node("first"));
    dictionary_set(d, "y", make_string_node("dup"));
    dictionary_set(d, "u", make_string_node("ini"));
    dictionary_set(d, "i", make_string_node("drop"));
    dictionary_set(d, "q2", make_string_node("abc"));
    dictionary_set(d, "w2", make_string_node("thi"));
    dictionary_set(d, "e2", make_string_node("sis"));
    dictionary_set(d, "r2", make_string_node("the"));
    dictionary_set(d, "t2", make_string_node("first"));
    dictionary_set(d, "y2", make_string_node("dup"));
    dictionary_set(d, "u2", make_string_node("ini"));
    dictionary_set(d, "i2", make_string_node("drop"));
    dictionary_set(d, "i2", make_string_node("nope"));
    dictionary_dump(d);

    if (all_passing)
    {
        printf("all tests pass\n");
    }
}
