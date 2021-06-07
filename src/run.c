#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "queue.c"
#include "dict.c"

void pq_display_front()
{

    if (is_pq_empty())
    {

        printf("\nThe queue is empty!\n");
        return;
    }

    printf("\n[%d]\n", front->data);
}

void pq_display_rear()
{

    if (is_pq_empty())
    {

        printf("\nThe queue is empty!\n");
        return;
    }

    printf("\n[%d]\n", rear->data);
}

void pq_display()
{

    if (is_pq_empty())
    {
        printf("\nThe queue is empty!\n");
        return;
    }

    pq_node_ptr temp = front;

    printf("\n[front -> ");

    while (temp != NULL)
    {
        printf("[%d]", temp->data);
        temp = temp->previous;
    }

    printf(" <- rear]\n");
}

void main()
{

    int option = 0, value = 0;
    char key[10];
    char c = 0;
    int i = 0;
    char str_value[80];
    dictionary * d = dictionary_new(10);
    do
    {

        printf("\nQUEUE Menu\n"
               "\n1 - Is Empty?"
               "\n2 - Enqueue"
               "\n3 - Requeue"
               "\n4 - Dequeue"
               "\n5 - Show FRONT"
               "\n6 - Show REAR"
               "\n7 - Display elements"
               "\n8 - Clear queue"
               "\n9 - add Dictionary item"
               "\n\n0 - EXIT\n\n");

        printf("option: ");
        scanf("%d", &option);

        switch (option)
        {

        case 0:
            if (front != NULL)
                pq_clear();
            break;

        case 1:
            printf(is_pq_empty() ? "\nTrue\n" : "\nFalse\n");
            break;

        case 2:
            printf("\nInsert a integer value to enqueue: ");
            scanf("%d", &value);
            printf(enqueue(value) ? "\nEnqueued :)\n" : "\nSomething went wrong :(\n");
            break;

        case 3:
            printf("\nInsert a integer value to requeue: ");
            scanf("%d", &value);
            printf(requeue(value) ? "\nRequeued :)\n" : "\nSomething went wrong :(\n");
            break;

        case 4:
            value = dequeue();
            printf("\nDequeued : [%d]\n", value);
            break;

        case 5:
            pq_display_front();
            break;

        case 6:
            pq_display_rear();
            break;

        case 7:
            pq_display();
            break;

        case 8:
            printf(pq_clear() ? "\nCleared :)\n" : "\nSomething went wrong :(\n");
            break;

        case 9:
            printf("\nAdd a string key and value to dictionary:\nkey: ");
            i = 0;
            c = 0;
            while (c != 32 && i < 10) {
                scanf("%c", &c);
                key[i] = c;
                i++;    
            }
            key[i] = 0;
            if (strlen(key) >= (10 - 1))
            {
                printf(" warning max key size %d\n", (10 - 1));
                scanf("%s", &str_value);
            }
            printf("value: ");
            scanf("%s", &str_value);
            printf( dictionary_set(d, key, str_value ) ? "\nNot Set :)\n" : "\nDictionary set :)\n");
            dictionary_dump(d);
            break;


        default:
            printf("\nINVALID OPTION!!!\n");
            break;
        }

    } while (option != 0);

}
