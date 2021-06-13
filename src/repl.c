#include <stdio.h>
#include "pounce.c"

int main(int argc, char **argv)
{
    if (argc == 2 && (strcmp(argv[1], "-u") == 0 || strcmp(argv[1], "--usage") == 0))
    {
        printf("Usage: %s [options] [pounceSourceFile] [stackOutPutFile]\n", argv[0]);
        return 1;
    }
    if (argc == 2 && (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0))
    {
        printf("Help:\n -u --usage prints command line arguments to %s\n", argv[0]);
        printf(" -h --help prints this message\n");
        printf(" -v --version prints the current repl version\n");
        printf(" -dd --display-dictionary prints the core word dictionary\n");
        printf(" -dq --display-program-queue prints the program queue before each run\n");
        return 1;
    }
    if (argc == 2 && (strcmp(argv[1], "-v") == 0 || strcmp(argv[1], "--version") == 0))
    {
        printf("Pounce c-core version: 0.01\n");
        return 1;
    }
    if (argc >= 3)
    {
        const char *input_fn = argv[1];
        const char *output_fn = argv[2];

        FILE *program_source = fopen(input_fn, "r");
        if (!program_source)
        {
            fprintf(stderr, "Error: %s is unable to open file %s\n", argv[0], argv[1]);
            return 1;
        }

        FILE *result_stack_out = fopen(input_fn, "w");
        if (!result_stack_out)
        {
            fprintf(stderr, "Error: %s is unable to open file %s\n", argv[0], argv[2]);
            return 1;
        }
    }
    // otherwise terminal i/o

    char input_program[1024];
    dictionary *wd = init_core_word_dictionary();
    if (argc == 2 && (strcmp(argv[1], "-dd") == 0 || strcmp(argv[1], "--display-word-dictionary") == 0))
    {
        printf("Dictionary (word defs):\n");
        dictionary_dump(wd);
    }
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
            if (argc == 2 && (strcmp(argv[1], "-dq") == 0 || strcmp(argv[1], "--display-program-queue") == 0))
            {
                printf("Program queue: ");
                pq_display(pr->pq);
            }
            ps_instance_ptr result_stack = purr(ps_init(), pr->pq, wd);
            // printf("Stack (result):\n");
            ps_display(result_stack);
        }
        else
        {
            printf("failed to parse [%s]", input_program);
            return 1;
        }
    }
}
