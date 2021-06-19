#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "queue.c"
#include "stack.c"

pq_node_ptr make_string_node(char * s){
    pq_node_ptr n = pq_init_node();
    n->type = 's';
    n->data->w.s = xstrcp(s);
    return n;
}

int main()
{
    pq_node_ptr temp_value;
    bool all_passing = true;
    ps_instance_ptr ps = ps_init();
    ps_push_s(ps, "test0");
    ps_push_s(ps, "test");
    temp_value = ps_pop(ps);
    if (type_s(temp_value) && strcmp(temp_value->data->w.s, "test"))
    {
        all_passing = false;
        printf("Assertion #1 found an unexpected value was ps_pop'ed\n");
        return -1;
    }
    pq_free_node(temp_value);
    temp_value = ps_pop(ps);
    pq_free_node(temp_value);
    if (!is_ps_empty(ps))
    {
        all_passing = false;
        printf("Assertion #2 found an unexpected non-empty queue\n");
        return -1;
    }
    ps_push_s(ps, "test1");
    ps_push_s(ps, "test2");
    ps_push_s(ps, "test3");
    ps_push_s(ps, "test4");

    ps_clear(ps);

    if (!is_ps_empty(ps))
    {
        all_passing = false;
        printf("Assertion #3 found an unexpected non-empty queue\n");
        return -1;
    }
    ps_push_s(ps, "test-a");
    ps_push_s(ps, "test-b");
    ps_push_s(ps, "test-c");
    temp_value = ps_pop(ps);
    if (type_s(temp_value) && strcmp(temp_value->data->w.s, "test-c"))
    {
        all_passing = false;
        printf("Assertion #4.1 found [%d] an unexpected value was ps_pop'ed\n", temp_value);
        return -1;
    }
    pq_free_node(temp_value);
    temp_value = ps_pop(ps);
    if (type_s(temp_value) && strcmp(temp_value->data->w.s, "test-b"))
    {
        all_passing = false;
        printf("Assertion #4.2 found [%d] an unexpected value was ps_pop'ed\n", temp_value);
        return -1;
    }
    pq_free_node(temp_value);
    ps_push_s(ps, "test 5");
    ps_push_s(ps, "test 4");
    temp_value = ps_pop(ps);
    if (type_s(temp_value) && strcmp(temp_value->data->w.s, "test 4"))
    {
        all_passing = false;
        printf("Assertion #6 found [%d] an unexpected value was ps_pop'ed\n", temp_value);
        return -1;
    }
    pq_free_node(temp_value);
    temp_value = ps_pop(ps);
    if (type_s(temp_value) && strcmp(temp_value->data->w.s, "test 5"))
    {
        all_passing = false;
        printf("Assertion #7 found [%d] an unexpected value was ps_pop'ed\n", temp_value);
        return -1;
    }
    pq_free_node(temp_value);
    printf("start stress test\n.");
    for (int t = 0; t < 50; t++)
    {
        printf(".");
        for (int i = 0; i < 5000; i++)
        {
            ps_push_s(ps, "test-z");
        }
        for (int i = 0; i < 5000; i++)
        {
            temp_value = ps_pop(ps);
            pq_free_node(temp_value);
        }
    }
    if (all_passing)
    {
        printf("\nall tests pass\n");
        return 0;
    }
    return -1;
}
