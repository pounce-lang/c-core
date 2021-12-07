#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "queue.c"
#include "stack.c"

pounce_node_ptr make_string_node(char * s){
    pounce_node_ptr n = pounce_init_node();
    n->type = STRING_T;
    n->data->w.s = xstrcp(s);
    return n;
}

int main()
{
    pounce_node_ptr temp_value;
    bool all_passing = true;
    stack_instance_ptr ps = stack_init();
    stack_push_s(ps, "test0");
    stack_push_s(ps, "test");
    temp_value = stack_pop(ps);
    if (type_s(temp_value) && strcmp(temp_value->data->w.s, "test"))
    {
        all_passing = false;
        printf("Assertion #1 found an unexpected value was stack_pop'ed\n");
        return -1;
    }
    pounce_free_node(temp_value);
    temp_value = stack_pop(ps);
    pounce_free_node(temp_value);
    if (!is_stack_empty(ps))
    {
        all_passing = false;
        printf("Assertion #2 found an unexpected non-empty queue\n");
        return -1;
    }
    stack_push_s(ps, "test1");
    stack_push_s(ps, "test2");
    stack_push_s(ps, "test3");
    stack_push_s(ps, "test4");

    stack_clear(ps);

    if (!is_stack_empty(ps))
    {
        all_passing = false;
        printf("Assertion #3 found an unexpected non-empty queue\n");
        return -1;
    }
    stack_push_s(ps, "test-a");
    stack_push_s(ps, "test-b");
    stack_push_s(ps, "test-c");
    temp_value = stack_pop(ps);
    if (type_s(temp_value) && strcmp(temp_value->data->w.s, "test-c"))
    {
        all_passing = false;
        printf("Assertion #4.1 found [%d] an unexpected value was stack_pop'ed\n", temp_value);
        return -1;
    }
    pounce_free_node(temp_value);
    temp_value = stack_pop(ps);
    if (type_s(temp_value) && strcmp(temp_value->data->w.s, "test-b"))
    {
        all_passing = false;
        printf("Assertion #4.2 found [%d] an unexpected value was stack_pop'ed\n", temp_value);
        return -1;
    }
    pounce_free_node(temp_value);
    stack_push_s(ps, "test 5");
    stack_push_s(ps, "test 4");
    temp_value = stack_pop(ps);
    if (type_s(temp_value) && strcmp(temp_value->data->w.s, "test 4"))
    {
        all_passing = false;
        printf("Assertion #6 found [%d] an unexpected value was stack_pop'ed\n", temp_value);
        return -1;
    }
    pounce_free_node(temp_value);
    temp_value = stack_pop(ps);
    if (type_s(temp_value) && strcmp(temp_value->data->w.s, "test 5"))
    {
        all_passing = false;
        printf("Assertion #7 found [%d] an unexpected value was stack_pop'ed\n", temp_value);
        return -1;
    }
    pounce_free_node(temp_value);
    printf("start stress test\n.");
    for (int t = 0; t < 50; t++)
    {
        printf(".");
        for (int i = 0; i < 5000; i++)
        {
            stack_push_s(ps, "test-z");
        }
        for (int i = 0; i < 5000; i++)
        {
            temp_value = stack_pop(ps);
            pounce_free_node(temp_value);
        }
    }
    if (all_passing)
    {
        printf("\nall tests pass\n");
        return 0;
    }
    return -1;
}
