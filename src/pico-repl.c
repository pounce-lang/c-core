#include <stdio.h>
// #include <stdlib.h>
#include "pico/stdlib.h"
#include <string.h>
#include "pounce.c"

int main()
{
    stdio_init_all();

    dictionary *wd = init_core_word_dictionary();

    char input_program[1024];
    ps_instance_ptr stack = ps_init();
    printf("Pounce REPL (enter Pounce words or type 'exit' or <Ctrl-c> to leave)\n");
    while (true)
    {
        printf("> ");
        fgets(input_program, sizeof(input_program), stdin);
        input_program[strlen(input_program) - 1] = 0;
        if (strcmp(input_program, "exit") == 0)
        {
            return 0;
        }
        parser_result_ptr pr = parse(0, input_program);

        if (pr)
        {
            stack = purr(stack, pr->pq, wd);
            // printf("Stack (result):\n");
            ps_display(stack);

            free(pr->pq);
            free(pr);
        }
        else
        {
            printf("failed to parse!");
            return 1;
        }
    }
    ps_clear(stack);
    free(stack);
    dictionary_del(wd);
}
