#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "queue.c"
#include "dict.c"
#include "stack.c"
#include "pounce.h"

pq_node_ptr pf_play(ps_instance_ptr s, pq_instance_ptr p);

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
pq_node_ptr dup_node(pq_node_ptr e);

// returns 'i' for int, 'f' for float and 's' for string (NaN)
char strToNumber(long *ival, double *fval, char *s)
{
    char *extra;
    double float_value = strtod(s, &extra);
    if (*extra)
    {
        return 's';
    }
    long int_value = atol(s);
    if (float_value == (double)int_value && !strchr(s, '.'))
    {
        *ival = int_value;
        return 'i';
    }
    *fval = float_value;
    return 'f';
}

int save_word(int word_i, char *word, parser_result_ptr result)
{
    if (word_i > 0)
    {
        word[word_i] = 0;
        long intVal;
        double floatVal;
        char kind = strToNumber(&intVal, &floatVal, word);
        if (kind == 'i')
        {
            pq_enqueue_i(result->pq, intVal);
        }
        else if (kind == 'f')
        {
            pq_enqueue_d(result->pq, floatVal);
        }
        else if (kind == 's')
        {
            pq_enqueue_s(result->pq, word);
        }
        else
        {
            printf("save_word got bad value from strToNumber");
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
            free(pr->pq);
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
                pq_free_node(w);
                if (w_def->type == 's')
                {
                    ps_push_s(s, w_def->data->w.s);
                }
                else if (w_def->type == 'i')
                {
                    ps_push_i(s, w_def->data->w.i);
                }
                else if (w_def->type == 'd')
                {
                    ps_push_d(s, w_def->data->w.d);
                }
                else if (w_def->type == 'l')
                {
                    ps_push_l(s, dup_node(w_def)->data->w.list);
                    pq_requeue_s(p, "play");
                }
                else if (w_def->type == 'f')
                {
                    pq_node_ptr res = w_def->data->w.fun(s, p);
                    if (!res)
                    {
                        // OK NULL means nothing to add to the stack
                    }
                    else if (res->type == 's')
                    {
                        ps_push_node(s, res);
                    }
                    else if (res->type == 'i')
                    {
                        ps_push_node(s, res);
                    }
                    else if (res->type == 'd')
                    {
                        ps_push_node(s, res);
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
                ps_push_node(s, w);
            }
        }
        else 
        {
            ps_push_node(s, w);
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
    free(pr->pq);
    free(pr);
    return n;
}
pq_node_ptr make_fun_node(pq_node_ptr (*fun)(ps_instance_ptr, pq_instance_ptr))
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
pq_node_ptr make_double_node(double d)
{
    pq_node_ptr n = pq_init_node();
    n->type = 'd';
    n->data->w.d = d;
    return n;
}

pq_node_ptr pf_strAppend(ps_instance_ptr s, pq_instance_ptr p)
{
    pq_node_ptr a = ps_pop(s);
    if (a && a->type == 's')
    {
        pq_node_ptr b = ps_pop(s);
        if (b && b->type == 's')
        {
            char *result;
            size_t len;

            len = strlen(b->data->w.s) + strlen(a->data->w.s) + 1;
            result = (char *)malloc(len);
            if (result)
            {
                strcpy(result, b->data->w.s);
            }
            strcat(result, a->data->w.s);
            pq_free_node(a);
            pq_free_node(b);
            pq_node_ptr n = make_string_node(result);
            free(result);
            return n;
        }
        else
        {
            fprintf(stderr, "strAppend encountered a non string at top(-1) of stack\n");
        }
        pq_free_node(b);
    }
    else
    {
        fprintf(stderr, "strAppend encountered a non string at top of stack\n");
    }
    pq_free_node(a);
    return NULL;
};

pq_node_ptr pf_intAdd(ps_instance_ptr s, pq_instance_ptr p)
{
    // ps_display(s);
    pq_node_ptr a = ps_pop(s);
    if (a && (a->type == 'i' || a->type == 'd'))
    {
        pq_node_ptr b = ps_pop(s);
        if (b && (b->type == 'i' || b->type == 'd'))
        {
            if (a->type == 'i' && b->type == 'i')
            {
                long sum = b->data->w.i + a->data->w.i;
                pq_free_node(a);
                pq_free_node(b);
                return make_integer_node(sum);
            }
            else if (a->type == 'd' && b->type == 'd')
            {
                double sum = b->data->w.i + a->data->w.i;
                pq_free_node(a);
                pq_free_node(b);
                return make_double_node(sum);
            }
            else
            {
                printf("intAdd both args to be integer or both to be floating point, but got a mix\n");
            }
        }
        else
        {
            printf("intAdd expected top(-1) to be integer, but got type %c instead\n", a->type);
        }
        pq_free_node(b);
    }
    else
    {
        printf("intAdd expected top to be integer, but got type %c instead\n", a->type);
    }
    pq_free_node(a);
    return NULL;
};
pq_node_ptr pf_intMult(ps_instance_ptr s, pq_instance_ptr p)
{
    // ps_display(s);
    pq_node_ptr a = ps_pop(s);
    if (a && (a->type == 'i' || a->type == 'd'))
    {
        pq_node_ptr b = ps_pop(s);
        if (b && (b->type == 'i' || b->type == 'd'))
        {
            if (a->type == 'i' && b->type == 'i')
            {
                long prod = b->data->w.i * a->data->w.i;
                pq_free_node(a);
                pq_free_node(b);
                return make_integer_node(prod);
            }
            else if (a->type == 'd' && b->type == 'd')
            {
                double prod = b->data->w.i * a->data->w.i;
                pq_free_node(a);
                pq_free_node(b);
                return make_double_node(prod);
            }
            else
            {
                printf("intMult both args to be integer or both to be floating point, but got a mix\n");
            }
        }
        else
        {
            printf("intMult expected top(-1) to be integer, but got type %c instead\n", a->type);
        }
        pq_free_node(b);
    }
    else
    {
        printf("intMult expected top to be integer, but got type %c instead\n", a->type);
    }
    pq_free_node(a);
    return NULL;
};

pq_node_ptr pf_swap(ps_instance_ptr s, pq_instance_ptr p)
{
    pq_node_ptr new_second = s->top;
    if (new_second)
    {
        pq_node_ptr new_top = s->top->previous;
        if (new_top)
        {
            new_second->previous = new_top->previous;
            new_top->previous = new_second;
            s->top = new_top;
        }
        else
        {
            printf("swap expected top(-1) to be a stack element\n");
        }
    }
    else
    {
        printf("swap expected top to be a stack element\n");
    }
    return NULL;
};

pq_node_ptr pf_drop(ps_instance_ptr s, pq_instance_ptr p)
{
    pq_node_ptr ele = ps_pop(s);
    if (!ele)
    {
        printf("'drop' expected an element at the top of the stack, but it was empty\n");
    }
    else
    {
        pq_free_node(ele);
    }
    return NULL;
};
// size of list
pq_node_ptr pf_size(ps_instance_ptr s, pq_instance_ptr p)
{
    pq_node_ptr e = s->top;
    if (!e || e->type != 'l')
    {
        printf("'size' expected a list at the top of the stack\n");
        return NULL;
    }
    long c = 0;
    pq_node_ptr l = e->data->w.list;
    while (l)
    {
        c++;
        l = l->previous;
    }
    return make_integer_node(c);
};

// duplicate a node, recursive for lists
pq_node_ptr dup_node(pq_node_ptr e)
{
    if (!e)
        return NULL;
    if (e->type == 's')
        return make_string_node(e->data->w.s);
    if (e->type == 'i')
        return make_integer_node(e->data->w.i);
    if (e->type == 'd')
        return make_double_node(e->data->w.d);
    if (e->type == 'l')
    {
        pq_node_ptr l = e->data->w.list;
        pq_node_ptr new_l = pq_init_node();
        new_l->type = 'l';
        new_l->data->w.list = NULL;
        pq_node_ptr cur, head = NULL;
        if (l)
        {
            cur = head = dup_node(l);
            l = l->previous;
            while (l)
            {
                cur->previous = dup_node(l);
                l = l->previous;
                cur = cur->previous;
            }
            cur->previous = NULL;
        }
        new_l->data->w.list = head;
        return new_l;
    }
}
pq_node_ptr pf_dup(ps_instance_ptr s, pq_instance_ptr p)
{
    pq_node_ptr e = s->top;
    if (!e)
    {
        printf("'dup' expected an element at the top of the stack\n");
        return NULL;
    }
    return dup_node(e);
};
pq_node_ptr pf_cons(ps_instance_ptr s, pq_instance_ptr p)
{
    pq_node_ptr list = s->top;
    if (list && list->type == 'l')
    {
        pq_node_ptr ele = s->top->previous;
        if (ele)
        {
            list->previous = ele->previous;
            ele->previous = list->data->w.list;
            list->data->w.list = ele;
        }
        else
        {
            printf("'cons' expected top(-1) to be a stack element\n");
        }
    }
    else
    {
        printf("'cons' expected the top the stack to be a list\n");
    }
    return NULL;
};
pq_node_ptr pf_uncons(ps_instance_ptr s, pq_instance_ptr p)
{
    pq_node_ptr e = s->top;
    if (!e || e->type != 'l')
    {
        printf("'uncons' expected a list at the top of the stack\n");
        return NULL;
    }
    pq_node_ptr l = e->data->w.list;
    pq_node_ptr first;
    if (l)
    {
        first = l;
        e->data->w.list = l->previous;
        first->previous = e->previous;
        e->previous = first;
    }
    else
    {
        printf("'uncons' expected a list with at least one element\n");
    }
    return NULL;
};

pq_node_ptr reverse_copy(pq_node_ptr l)
{
    pq_node_ptr rev;
    pq_node_ptr link = NULL;
    while (l)
    {
        rev = dup_node(l);
        l = l->previous;
        rev->previous = link;
        link = rev;
    }
    return rev;
}

pq_node_ptr pf_play(ps_instance_ptr s, pq_instance_ptr p)
{
    pq_node_ptr phrase = ps_pop(s);
    if (!phrase || phrase->type != 'l')
    {
        printf("'play' expected a list at the top of the stack\n");
        return NULL;
    }
    pq_node_ptr l = phrase->data->w.list;
    pq_node_ptr rev = reverse_copy(l);
    while (rev)
    {
        l = rev->previous;
        pq_requeue(p, rev);
        rev = l;
    }
    pq_free_node(phrase);
    return NULL;
};

pq_node_ptr pf_dip(ps_instance_ptr s, pq_instance_ptr p)
{
    pq_node_ptr e = ps_pop(s);
    pq_node_ptr t = ps_pop(s);
    if (!e || e->type != 'l')
    {
        printf("'dip' expected a list at the top of the stack\n");
        return NULL;
    }
    pq_requeue(p, t);
    pq_node_ptr l = e->data->w.list;
    pq_node_ptr rev = reverse_copy(l);
    while (rev)
    {
        l = rev->previous;
        pq_requeue(p, rev);
        rev = l;
    }
    pq_free_node(e);
    return NULL;
};

dictionary *init_core_word_dictionary()
{
    dictionary *wd = dictionary_new(10);
    dictionary_set(wd, "twice", make_list_node("dup +"));
    dictionary_set(wd, "strAppend", make_fun_node(pf_strAppend));
    dictionary_set(wd, "+", make_fun_node(pf_intAdd));
    dictionary_set(wd, "*", make_fun_node(pf_intMult));
    dictionary_set(wd, "swap", make_fun_node(pf_swap));
    dictionary_set(wd, "drop", make_fun_node(pf_drop));
    dictionary_set(wd, "size", make_fun_node(pf_size));
    dictionary_set(wd, "dup", make_fun_node(pf_dup));
    dictionary_set(wd, "cons", make_fun_node(pf_cons));
    dictionary_set(wd, "uncons", make_fun_node(pf_uncons));
    dictionary_set(wd, "play", make_fun_node(pf_play));
    dictionary_set(wd, "dip", make_fun_node(pf_dip));
    return wd;
}

// test (early) "5 [[1 []] 2 3] uncons cons cons"; //"5 [32 9 s [3 4] 23] dup [] dup"; // "aaa bbb end noop 3 4 + anId IDK drop [one] w ow strAppend swap size"; // "a[b c] 00 [1[2]3]x y[]z"; // "a[b c] 00 [1[2]3]x y[z"; //" 2 4 + goofy 'cool say'  `jj`  \"543\" ";
