/* controller for a cooperative multitasking pounce core 
*/
#define MICROPROCESSOR
// set in CMakeLists.txt #define POUNCE_PICO_RP2040
#include <stdio.h>
// #include <stdlib.h>
#include "pico/stdlib.h"
#include <string.h>
#include "../pounce.c"



int main()
{
    stdio_init_all();
    sleep_ms(100);
    dictionary *wd = init_core_word_dictionary();
    char *pp = "initLED -1 IN gpioInit [flicker] [-1 gpioGet ! [on on off] [[off] 5000 times] if-else flicker] compose flicker";
    // printf("pp:\n%s\n", pp);
    stack_instance_ptr stack = stack_init();
    parser_result_ptr pr = parse(0, pp);
//        free(pp);
    if (pr && pr->pq)
    {
        // if (display_queue) {
        //     printf("Pounce queue (AST):\n");
        //     pdq_display(pr->pq, " ");
        // }
        pdq_instance_ptr pq2 = process_compose(pr->pq, wd);
        // if (display_dictionary) {
        //     printf("Dictionary (word defs):\n");
        //     dictionary_dump(wd);
        // }
        if (pq2)
        {
            stack = purr(stack, pq2, wd);
            stack_display(stack);
            free(pq2);
        }
        free(pr->pq);
        free(pr);
    }
}
