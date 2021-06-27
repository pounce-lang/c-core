#include <stdio.h>
// #include <stdlib.h>
#include "pico/stdlib.h"
#include <string.h>
#include "pounce.c"

#define INPUT_SIZE 81

int main()
{
    stdio_init_all();

    const uint LED_PIN = 25;
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    gpio_put(LED_PIN, 1);
    sleep_ms(5000);
    gpio_put(LED_PIN, 0);
    sleep_ms(1000);

    dictionary *wd = init_core_word_dictionary();

    char input_program[INPUT_SIZE];
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

        // fgets(input_program, INPUT_SIZE, stdin);
        // input_program[strlen(input_program) - 1] = 0;
        input_i = 0;
        input_char = ' ';
        while (input_char != '\n' && input_char != '\r' && input_i < (INPUT_SIZE - 1))
        {
            input_char = getchar();
            input_program[input_i++] = input_char;
            printf("%c", input_char);
        }
        input_program[input_i - 1] = 0;
        printf("\n");

        if (strcmp(input_program, "exit") == 0)
        {
            return 0;
        }
        parser_result_ptr pr = parse(0, input_program);

        if (pr)
        {
            gpio_put(LED_PIN, 1);
            sleep_ms(250);
            gpio_put(LED_PIN, 0);

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
    gpio_put(LED_PIN, 1);
    sleep_ms(1000);
    gpio_put(LED_PIN, 0);
    printf("exiting Pounce -\\_(*|*)_/-");
}
