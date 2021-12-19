/* controller for a cooperative multitasking pounce core 
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../pounce.c"


char *read_new_str(FILE *f) {
    size_t bufsize = 8;
    char *buf = (char *) malloc(bufsize);
    size_t pos = 0;

    while (1) {
        int c = fgetc(f);

        //read until EOF or 0 is read
        if (c < 0 || c == '\0') {
            buf[pos] = '\0';
            return buf;
        }

        buf[pos++] = (char) c;

        //enlarge buf to hold whole string
        if (pos == bufsize) {
            bufsize *= 2;
            buf = (char *) realloc((void *) buf, bufsize);
        }
    }
}

int main(int argc, char **argv)
{
    bool display_dictionary = false;
    bool display_queue = false;
    if (argc == 2 && (strcmp(argv[1], "-u") == 0 || strcmp(argv[1], "--usage") == 0))
    {
        printf("Usage: %s [options] [pounceSourceFile] [stackOutPutFile]\n", argv[0]);
        return 1;
    }
    if (argc == 2 && (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0))
    {
        printf("Help:\n -u --usage prints command line arguments to %s\n", argv[0]);
        printf(" -h --help prints this message\n");
        printf(" -i --input <inputfile>\n");        
        printf(" -v --version prints the current repl version\n");
        printf(" -dd --display-dictionary prints the core word dictionary\n");
        printf(" -dq --display-program-queue prints the program queue before each run\n");
        return 1;
    }
    if (argc == 2 && (strcmp(argv[1], "-v") == 0 || strcmp(argv[1], "--version") == 0))
    {
        printf("Pounce c-core version: 0.02\n");
        return 1;
    }

    dictionary *wd = init_core_word_dictionary();
    if ((argc >= 2 && strcmp(argv[1], "-dd") == 0) || (argc >= 3 && strcmp(argv[2], "-dd") == 0) || (argc >= 4 && strcmp(argv[3], "-dd") == 0))
    {
        display_dictionary = true;
    }
    if ((argc >= 2 && strcmp(argv[1], "-dq") == 0) || (argc >= 3 && strcmp(argv[2], "-dq") == 0) || (argc >= 4 && strcmp(argv[3], "-dq") == 0))
    {
        display_queue = true;
    }
    // possibly run pounce via file io
    if (argc >= 2)
    {
        const char *input_fn = argv[argc-1];
        // const char *output_fn = argv[2];

        FILE *program_source = fopen(input_fn, "r");
        if (!program_source)
        {
            fprintf(stderr, "Error: %s is unable to open file %s\n", argv[0], argv[1]);
            return 1;
        }
        char *pp = read_new_str(program_source);
        // printf("pp:\n%s\n", pp);
        fclose(program_source);
        stack_instance_ptr stack = stack_init();
        parser_result_ptr pr = parse(0, pp);
        free(pp);
        if (pr && pr->pq)
        {
            if (display_queue) {
                printf("Pounce queue (AST):\n");
                pdq_display(pr->pq, " ");
            }
            pdq_instance_ptr pq2 = process_compose(pr->pq, wd);
            if (display_dictionary) {
                printf("Dictionary (word defs):\n");
                dictionary_dump(wd);
            }
            if (pq2)
            {
                stack = purr(stack, pq2, wd);
                stack_display(stack);
                free(pq2);
            }
            free(pr->pq);
            free(pr);
        }
        stack_clear(stack);
        free(stack);
        dictionary_del(wd);
        return 0;
    }
}
