#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "queue.c"
#include "dict.c"
#include "pounce.h"


void pq_display(pq_instance_ptr pq)
{
    if (is_pq_empty(pq))
    {
        printf("\nThe queue is empty!\n");
        return;
    }

    pq_node_ptr n = pq->front;

    printf("\n");
    pq_attendance(n);
    printf("\n");
}

void pq_display_front(pq_instance_ptr pq)
{

    if (is_pq_empty(pq))
    {

        printf("\nThe queue is empty!\n");
        return;
    }

    pq_display_word(pq->front);
}

void pq_display_rear(pq_instance_ptr pq)
{

    if (is_pq_empty(pq))
    {

        printf("\nThe queue is empty!\n");
        return;
    }

    pq_display_word(pq->rear);
};

#define MAX_W_LEN 40
#define MAX_S_LEN 4096
typedef struct parser_result
{
    pq_instance_ptr pq;
    int i;
} * parser_result_ptr;

parser_result_ptr parse(int i, const char *pt)
{
    int len = strlen(pt);
    // a regular word (perhaps in the dictionary)
    char word[MAX_W_LEN] = "";
    int word_i = 0;
    // a quoted string
    char in_string = 0;
    char str[MAX_S_LEN] = "";
    int str_i = 0;

    parser_result_ptr result = (parser_result_ptr)malloc(sizeof(struct parser_result));

    if (result == NULL)
    {
        printf("could not malloc parser_result\n");
        return NULL;
    }

    result->pq = pq_init();
    if (!result->pq)
    {
        printf("could not malloc pq\n");
        return NULL;
    }
    while (result->pq && i < len)
    {
        // start parsing a quoted string
        if (!in_string && (pt[i] == '\'' || pt[i] == '"' || pt[i] == '`'))
        {
            in_string = pt[i];
            str_i = 0;
            str[str_i++] = pt[i++];
        }
        else if (in_string)
        {
            // printf("parser- in a string\n");
            if (str_i >= MAX_S_LEN - 1)
            {
                printf("word too long\n");
                result->i = -1;
                return result;
            }
            if (pt[i] == in_string)
            {
                in_string = 0;
            }
            str[str_i++] = pt[i++];
            if (!in_string)
            {
                str[str_i] = 0; // null terminate the string
                pq_enqueue_s(result->pq, str);
                str[0] = 0;
                str_i = 0;
            }
        }
        else if (pt[i] == '[')
        {
            // printf("parser- start a list '['\n");
            if (word_i > 0)
            {
                word[word_i] = 0;
                pq_enqueue_s(result->pq, word);
                word[0] = 0;
                word_i = 0;
            }
            i++;
            parser_result_ptr pr = parse(i, pt);
            i = pr->i;
            pq_enqueue_l(result->pq, pr->pq->front);
            pq_delete(pr->pq);
            free(pr);
        }
        else if (pt[i] == ']')
        {
            // printf("parser- end a list ']'\n");
            if (word_i > 0)
            {
                word[word_i] = 0;
                pq_enqueue_s(result->pq, word);
                word[0] = 0;
                word_i = 0;
            }
            i++;
            result->i = i;
            return result;
        }
        else if (pt[i] == ' ')
        {
            // printf("parser- space ends a word ' '\n");
            if (word_i > 0)
            {
                word[word_i] = 0;
                pq_enqueue_s(result->pq, word);
                // printf("add word to program queue (pp) %s\n", word);
                word[0] = 0;
                word_i = 0;
            }
            i++;
        }
        else
        {
            if (word_i >= MAX_W_LEN - 1)
            {
                printf("word too long\n");
                return false;
            }
            word[word_i++] = pt[i++];
        }
    }
    // last word
    if (word_i > 0)
    {
        word[word_i] = 0;
        pq_enqueue_s(result->pq, word);
        // printf("add word to program queue (pp) %s\n", word);
        word[0] = 0;
        word_i = 0;
    }

    result->i = i;
    return result;
};

int main()
{  
    char input_program[100] = "a[b c] 00 [1[2]3]x y[]z"; // "a[b c] 00 [1[2]3]x y[z"; //" 2 4 + goofy 'cool say'  `jj`  \"543\" ";
    parser_result_ptr pr = parse(0, input_program);
    if (pr)
    {
        printf("parsed\n");
        pq_display(pr->pq);
    }
    else
    {
        printf("failed to parse [%s]", input_program);
        return 1;
    }
    return 0;
}
