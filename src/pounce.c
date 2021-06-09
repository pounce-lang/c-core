#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "queue.c"
#include "dict.c"

void pq_display_front(pq_instance_ptr pq)
{

    if (is_pq_empty(pq))
    {

        printf("\nThe queue is empty!\n");
        return;
    }

    printf("\n[%d]\n", pq->front->data);
}

void pq_display_rear(pq_instance_ptr pq)
{

    if (is_pq_empty(pq))
    {

        printf("\nThe queue is empty!\n");
        return;
    }

    printf("\n[%d]\n", pq->rear->data);
}

void pq_display(pq_instance_ptr pq)
{
    if (is_pq_empty(pq))
    {
        printf("\nThe queue is empty!\n");
        return;
    }

    pq_node_ptr temp = pq->front;

    printf("\n[front -> ");

    while (temp != NULL)
    {
        printf("[%s]", temp->data);
        temp = temp->previous;
    }

    printf(" <- rear]\n");
}

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
                word_ptr w = word_init();

                w->w.s = xstrdup(str);
                pq_enqueue(result->pq, 's', w);
                printf("add quoted string to program queue (pp) %s\n", str);
                str[0] = 0;
                str_i = 0;
            }
        }
        else if (pt[i] == '[')
        {
            if (word_i > 0)
            {
                word[word_i] = 0;
                word_ptr w = word_init();
                w->w.s = xstrdup(word);
                pq_enqueue(result->pq, 's', w);
                printf("add word to program queue (pp) %s\n", word);
                word[0] = 0;
                word_i = 0;
            }
            i++;
            word_ptr w = word_init();
            w->w.list = parse(i, pt)->pq->front;
            pq_enqueue(result->pq, 'l', w);
        }
        else if (pt[i] == ']')
        {
            if (word_i > 0)
            {
                word[word_i] = 0;
                word_ptr w = word_init();
                w->w.s =  xstrdup(word);
                pq_enqueue(result->pq, 's', w);
                printf("add word to program queue (pp) %s\n", word);
                word[0] = 0;
                word_i = 0;
            }
            i++;
            result->i = i;
            return result;
        }
        else if (pt[i] == ' ')
        {
            if (word_i > 0)
            {
                word[word_i] = 0;
                word_ptr w = word_init();
                w->w.s =  xstrdup(word);
                pq_enqueue(result->pq, 's', w);                printf("add word to program queue (pp) %s\n", word);
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
    return result;
};

int main()
{
    int option = 0, value = 0;
    char input_program[100] = " 2 4 + goofy 'cool say'  `jj`  \"543\" ";
    char key[10];
    char str_value[80];
    dictionary *d = dictionary_new(10);
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
