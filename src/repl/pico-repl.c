#include <stdio.h>
// #include <stdlib.h>
#include "pico/stdlib.h"
#include <string.h>
#include "../pounce.c"
#include "pico-history.c"

const uint LED_PIN = 25;

void clear_input(char *s)
{
    for (short i; i < INPUT_SIZE; i++)
    {
        s[i] = 0;
    }
}

void exit_pounce(ps_instance_ptr stack, dictionary *wd)
{
    ps_clear(stack);
    free(stack);
    dictionary_del(wd);
    gpio_put(LED_PIN, 1);
    sleep_ms(1000);
    gpio_put(LED_PIN, 0);
    printf("exiting Pounce -\\_(*|*)_/-");
}

int main()
{
    stdio_init_all();
    const char blank_line[INPUT_SIZE] = {[0 ...(INPUT_SIZE - 2)] ' ', 0};
    short histor_i = 0;

    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    gpio_put(LED_PIN, 1);
    sleep_ms(5000);
    gpio_put(LED_PIN, 0);
    sleep_ms(1000);

    dictionary *wd = init_core_word_dictionary();

    char input_program[INPUT_SIZE] = {0x00};
    char input_char = getchar();
    short input_i;

    ps_instance_ptr stack = ps_init();
    gpio_put(LED_PIN, 1);
    sleep_ms(250);
    gpio_put(LED_PIN, 0);
    sleep_ms(250);
    printf("Pounce REPL (enter Pounce words or type 'exit' or <Ctrl-c> to leave)\n");
    while (true)
    {
        printf("> ");
        gpio_put(LED_PIN, 1);
        sleep_ms(250);
        gpio_put(LED_PIN, 0);
        sleep_ms(250);

        input_i = 0;
        input_char = ' ';

        while (input_char != '\n' && input_char != '\r' && input_i < (INPUT_SIZE - 1))
        {
            input_char = getchar();
            if (input_char == 27) // escape sequence
            {
                input_char = getchar();
                if (input_char == '[')
                {
                    input_char = getchar();
                    if (input_char == 'A')
                    {
                        // up arrow
                        if (histor_i == 0) // up from the last line, so stash it
                        {
                            stash_history(input_program);
                        }
                        if (histor_i < sizeOfHist())
                        {
                            strcpy(input_program, peek_history(++histor_i));
                            input_i = strlen(input_program);
                            printf("\r%s\r> %s", blank_line, input_program);
                        }
                    }
                    else if (input_char == 'B')
                    {
                        // down arrow
                        histor_i -= 1;
                        if (histor_i < 0)
                        {
                            histor_i = 0;
                        }

                        strcpy(input_program, peek_history(histor_i));
                        input_i = strlen(input_program);
                        printf("\r%s\r> %s", blank_line, input_program);
                    }
                    else if (input_char == 'C')
                    {
                        // right arrow
                    }
                    else if (input_char == 'D')
                    {
                        // left arrow
                    }
                    else
                    {
                        printf("got a ctrl key '%c'\n", input_char);
                    }
                }
            }
            else
            {
                input_program[input_i++] = input_char;
                printf("%c", input_char);
            }
        }
        input_program[input_i - 1] = 0;
        printf("\n");

        if (strcmp(input_program, "exit") == 0)
        {
            exit_pounce(stack, wd);
            return 0;
        }
        if (strlen(input_program) > 0)
        {
            push_history(input_program);
            histor_i = 0;
        }
        parser_result_ptr pr = parse(0, input_program);

        if (pr && pr->pq)
        {
            gpio_put(LED_PIN, 1);
            sleep_ms(250);
            gpio_put(LED_PIN, 0);

            stack = purr(stack, pr->pq, wd);
            ps_display(stack);

            free(pr->pq);
            free(pr);
        }
        else
        {
            printf("failed to parse!");
            return 1;
        }
        clear_input(input_program);
    }
    exit_pounce(stack, wd);
}
