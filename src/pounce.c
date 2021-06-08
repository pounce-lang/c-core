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

pq_instance_ptr parse(const char *pt)
{
    int i = 0;
    int len = strlen(pt);
    char word[MAX_W_LEN] = "";
    int word_i = 0;
    bool we_are_good = true;
    // quoted string
    char in_string = 0;
    char str[MAX_S_LEN] = "";
    int str_i = 0;
    pq_instance_ptr pq = pq_init();
    while (we_are_good && pq && i < len)
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
                return false;
            }
            if (pt[i] == in_string)
            {
                in_string = 0;
            }
            str[str_i++] = pt[i++];
            if (!in_string)
            {
                str[str_i] = 0; // null terminate the string
                pq_enqueue(pq, 's', xstrdup(str));
                printf("add quoted string to program queue (pp) %s\n", str);
                str[0] = 0;
                str_i = 0;
            }
        }
        else if (pt[i] == ' ')
        {
            if (word_i > 0)
            {
                word[word_i] = 0;
                pq_enqueue(pq, 's', xstrdup(word));
                printf("add word to program queue (pp) %s\n", word);
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
    return pq;
};

int main()
{
    int option = 0, value = 0;
    char input_program[100] = "    2 4 + goofy 'cool say'  `jj`  \"543\"   ";
    char key[10];
    char str_value[80];
    dictionary *d = dictionary_new(10);
    pq_instance_ptr pq = NULL;
    pq = parse(input_program);
    if (pq) 
    {
        printf("parsed\n");
        pq_display(pq);
    }
    else
    {
        printf("failed to parse [%s]", input_program);
        return 1;
    }
    return 0;
}
