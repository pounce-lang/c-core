#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "queue.c"
#include "dict.c"
#include "stack.c"
#include "pounce.h"

void ps_display(ps_instance_ptr ps)
{
    if (is_ps_empty(ps))
    {
        printf("[] This stack is empty!\n");
        return;
    }

    pq_node_ptr n = ps->top;

    pq_attendance(n, "\n ");
    printf("\n");
}

void pq_display(pq_instance_ptr pq)
{
    if (is_pq_empty(pq))
    {
        printf("The queue is empty!\n");
        return;
    }

    pq_node_ptr n = pq->front;

    pq_attendance(n, "");
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

bool strToInt(long *val, char *s)
{
    // printf("*** try convet %s", s);
    long value = atol(s);
    // printf(" got %d\n", value);
    if (value == 0)
    {
        if (strcmp(s, "0") == 0 || strcmp(s, ".0") == 0 || strcmp(s, "0.0") == 0)
        {
            *val = 0;
            return true;
        }
        else
        {
            return false;
        }
    }
    *val = value;
    return true;
}

int save_word(int word_i, char *word, parser_result_ptr result)
{
    if (word_i > 0)
    {
        word[word_i] = 0;
        long intVal;
        if (strToInt(&intVal, word))
        {
            pq_enqueue_i(result->pq, intVal);
        }
        else
        {
            pq_enqueue_s(result->pq, word);
        }
        word[0] = 0;
        word_i = 0;
    }
    return word_i;
}

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
            word_i = save_word(word_i, word, result);
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
            word_i = save_word(word_i, word, result);
            i++;
            result->i = i;
            return result;
        }
        else if (pt[i] == ' ')
        {
            // printf("parser- space ends a word ' '\n");
            word_i = save_word(word_i, word, result);
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
    word_i = save_word(word_i, word, result);

    result->i = i;
    return result;
};

ps_instance_ptr purr(ps_instance_ptr s, pq_instance_ptr p, dictionary *d)
{
    while (!is_pq_empty(p))
    {
        pq_node_ptr w = pq_dequeue(p);
        // pq_display_word(w);
        if (w && w->type == 's')
        {
            pq_node_ptr w_def = dictionary_get(d, w->data->w.s, NULL);
            if (w_def)
            {
                if (w_def->type == 's')
                {
                    ps_push_s(s, w_def->data->w.s);
                }
                else if (w_def->type == 'i')
                {
                    ps_push_i(s, w_def->data->w.i);
                }
                else if (w_def->type == 'l')
                {
                    ps_push_l(s, w_def->data->w.list);
                }
                else if (w_def->type == 'f')
                {
                    printf("run fun\n");
                    pq_node_ptr res = w_def->data->w.fun(s);
                    if (!res)
                    {
                        // OK NULL means nothing to add to the stack
                    }
                    else if (res->type == 's')
                    {
                        ps_push_s(s, res->data->w.s);
                    }
                    else if (res->type == 'i')
                    {
                        ps_push_i(s, res->data->w.i);
                    }
                    else if (res->type == 'l')
                    {
                        ps_push_l(s, res->data->w.list);
                    }
                    else
                    {
                        printf("word dictionary def of type %c is not handled\n", res->type);
                    }
                }
                else
                {
                    printf("word dictionary def is not handled\n");
                }
            }
            else
            {
                if (w->type == 's')
                {
                    ps_push_s(s, w->data->w.s);
                }
                else if (w->type == 'i')
                {
                    ps_push_i(s, w->data->w.i);
                }
                else if (w->type == 'l')
                {
                    ps_push_l(s, w->data->w.list);
                }
                else
                {
                    printf("word from pq is not handled\n");
                }
            }
        }
        else if (w && w->type == 'i')
        {
            ps_push_i(s, w->data->w.i);
        }
    }
    return s;
};

pq_node_ptr make_string_node(char *s)
{
    pq_node_ptr n = pq_init_node();
    n->type = 's';
    n->data->w.s = xstrcp(s);
    return n;
}
pq_node_ptr make_list_node(char *s)
{
    pq_node_ptr n = pq_init_node();
    n->type = 'l';
    parser_result_ptr pr = parse(0, s);
    n->data->w.list = pr->pq->front;
    return n;
}
pq_node_ptr make_fun_node(pq_node_ptr (*fun)(ps_instance_ptr))
{
    pq_node_ptr n = pq_init_node();
    n->type = 'f';
    n->data->w.fun = fun;
    return n;
}
pq_node_ptr make_integer_node(long i)
{
    pq_node_ptr n = pq_init_node();
    n->type = 'i';
    n->data->w.i = i;
    return n;
}

pq_node_ptr pf_noop(ps_instance_ptr s)
{
    return NULL;
};

pq_node_ptr pf_anId(ps_instance_ptr s)
{
    return make_string_node("_anId_");
};
pq_node_ptr pf_strAppend(ps_instance_ptr s)
{
    pq_node_ptr a = ps_pop(s);
    if (a && a->type == 's')
    {
        pq_node_ptr b = ps_pop(s);
        if (b && b->type == 's')
        {
            strncat(b->data->w.s, a->data->w.s, strlen(a->data->w.s));
            return make_string_node(b->data->w.s);
        }
    }
    return NULL;
};

pq_node_ptr pf_intAdd(ps_instance_ptr s)
{
    ps_display(s);
    pq_node_ptr a = ps_pop(s);
    if (a && a->type == 'i')
    {
        pq_node_ptr b = ps_pop(s);
        if (b && b->type == 'i')
        {
            return make_integer_node(b->data->w.i + a->data->w.i);
        }
        else
        {
            printf("intAdd expected top-1 to be integer, but got type %c instead\n", a->type);
        }
    }
    else
    {
        printf("intAdd expected top to be integer, but got type %c instead\n", a->type);
    }
    return NULL;
};

int main()
{
    char input_program[100] = "aaa bbb end noop 3 4 + anId w ow strAppend"; // "a[b c] 00 [1[2]3]x y[]z"; // "a[b c] 00 [1[2]3]x y[z"; //" 2 4 + goofy 'cool say'  `jj`  \"543\" ";
    parser_result_ptr pr = parse(0, input_program);
    if (pr)
    {
        printf("Parsed (pounce program):\n");
        pq_display(pr->pq);
        dictionary *wd = dictionary_new(10);
        dictionary_set(wd, "aaa", make_string_node("AAA"));
        dictionary_set(wd, "bbb", make_list_node("abc def"));
        dictionary_set(wd, "noop", make_fun_node(pf_noop));
        dictionary_set(wd, "anId", make_fun_node(pf_anId));
        dictionary_set(wd, "strAppend", make_fun_node(pf_strAppend));
        dictionary_set(wd, "+", make_fun_node(pf_intAdd));
        printf("Dictionary (word defs):\n");
        dictionary_dump(wd);
        ps_instance_ptr result_stack = purr(ps_init(), pr->pq, wd);
        printf("Stack (result):\n");
        ps_display(result_stack);
    }
    else
    {
        printf("failed to parse [%s]", input_program);
        return 1;
    }
    return 0;
}
