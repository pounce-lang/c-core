#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "queue.c"
#include "dict.c"
#include "stack.c"
#include "pounce.h"
#ifdef MICROPROCESSOR
#include "hal/gpio.h"
#endif
#if SF_PRO_MICRO_RP2040
#include "hal/led/WS2812.h"
#endif

pdq_node_ptr pf_play(stack_instance_ptr s, pdq_instance_ptr p);

void stack_display(stack_instance_ptr ps)
{
    if (is_stack_empty(ps))
    {
        printf("[] The stack is empty!\n");
        return;
    }

    pdq_node_ptr n = ps->top;

    pdq_attendance(n, "\n ");
    printf("\n");
}

void pdq_display(pdq_instance_ptr pq, char *sep)
{
    if (is_pdq_empty(pq))
    {
        printf("The queue is empty!\n");
        return;
    }

    pdq_node_ptr n = pq->front;

    if (!sep)
    {
        pdq_attendance(n, "");
    }
    else
    {
        pdq_attendance(n, sep);
    }
    printf("\n");
}

void pdq_display_front(pdq_instance_ptr pq)
{

    if (is_pdq_empty(pq))
    {

        printf("\nThe queue is empty!\n");
        return;
    }

    pdq_display_word(pq->front);
}

void pdq_display_rear(pdq_instance_ptr pq)
{

    if (is_pdq_empty(pq))
    {

        printf("\nThe queue is empty!\n");
        return;
    }

    pdq_display_word(pq->rear);
};

#define MAX_W_LEN 40
#define MAX_S_LEN 4096
typedef struct parser_result
{
    pdq_instance_ptr pq;
    int i;
} * parser_result_ptr;

pdq_node_ptr copy_node(pdq_node_ptr e);

// returns INT_T for int, REAL_T for float and STRING_T for string (NaN)
char strToNumber(long *ival, double *fval, char *s)
{
    char *extra;
    double float_value = strtod(s, &extra);
    if (*extra)
    {
        if (strcmp(extra, "true") == 0)
        {
            *ival = 1;
            return BOOL_T;
        }
        if (strcmp(extra, "false") == 0)
        {
            *ival = 0;
            return BOOL_T;
        }
        return STRING_T;
    }
    long int_value = atol(s);
    if (float_value == (double)int_value && !strchr(s, '.'))
    {
        *ival = int_value;
        return INT_T;
    }
    *fval = float_value;
    return REAL_T;
}

int save_word(int word_i, char *word, parser_result_ptr result)
{
    if (word_i > 0)
    {
        word[word_i] = 0;
        long intVal;
        double floatVal;
        char kind = strToNumber(&intVal, &floatVal, word);
        if (kind == INT_T)
        {
            pdq_enqueue_i(result->pq, intVal);
        }
        else if (kind == BOOL_T)
        {
            pdq_enqueue_b(result->pq, (bool)intVal);
        }
        else if (kind == REAL_T)
        {
            pdq_enqueue_d(result->pq, floatVal);
        }
        else if (kind == STRING_T)
        {
            pdq_enqueue_s(result->pq, STRING_T, word);
        }
        else
        {
            printf("save_word got bad value from strToNumber\n");
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
    char in_comment = '\0';

    parser_result_ptr result = (parser_result_ptr)malloc(sizeof(struct parser_result));

    if (result == NULL)
    {
        printf("could not malloc parser_result\n");
        return NULL;
    }

    result->pq = pdq_init();
    if (!result->pq)
    {
        printf("could not malloc pq\n");
        return NULL;
    }
    while (result->pq && i < len)
    {
        if (!in_string && pt[i] == '#')
        {
            in_comment = pt[i];
            i++;
        }
        else if (in_comment)
        {
            // printf("parser- in a comment %c\n", pt[i]);
            if (pt[i] == '\n' || pt[i] == '\r')
            {
                in_comment = '\0';
            }
            i++;
        }
        // start parsing a quoted string
        else if (!in_comment && !in_string && (pt[i] == '\'' || pt[i] == '"' || pt[i] == '`'))
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
                str[str_i - 1] = 0; // null terminate the string r_trim

                pdq_enqueue_s(result->pq, str[0], &str[1]); //and l_trim
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
            pdq_enqueue_l(result->pq, pr->pq->front);
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
        else if (pt[i] == ' ' || pt[i] == '\r' || pt[i] == '\n')
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

pdq_instance_ptr process_compose(pdq_instance_ptr p, dictionary *d)
{
    pdq_instance_ptr result_pq = pdq_init();
    int i = 0;
    while (!is_pdq_empty(p))
    {
        i++;
        pdq_node_ptr w = pdq_dequeue(p);
        if (w->type == STRING_T && strcmp(w->word.s, "compose") == 0)
        {
            pdq_node_ptr phrase = pdq_popout(result_pq);
            pdq_node_ptr new_word = pdq_popout(result_pq);

            // check on these two ingredients
            if (phrase && phrase->type == LIST_T && new_word && new_word->type == LIST_T)
            {
                // process the composition
                dictionary_set(d, new_word->word.list->word.s, phrase);
            }
            else
            {
                // type parsing error
                printf("Stack error in compose.\n");
            }
        }
        else
        {
            pdq_enqueue(result_pq, w);
        }
    }
    return result_pq;
};

pdq_node_ptr reverse_copy(pdq_node_ptr l)
{
    pdq_node_ptr rev;
    pdq_node_ptr link = NULL;
    while (l)
    {
        rev = copy_node(l);
        l = l->previous;
        rev->previous = link;
        link = rev;
    }
    return rev;
}

pdq_node_ptr list_getLast(pdq_node_ptr l)
{
    pdq_node_ptr link = l->previous;
    while (l && link)
    {
        l = l->previous;
        link = l->previous;
    }
    return l;
}

bool node_cmp(pdq_node_ptr a, pdq_node_ptr b);

bool list_cmp(pdq_node_ptr a, pdq_node_ptr b)
{
    if (!a && !b) return true;
    pdq_node_ptr a_t = a;
    pdq_node_ptr b_t = b;
    while (a_t && b_t)
    {
        if (!node_cmp(a_t, b_t)) 
        {
            return false;
        }
        a_t = a_t->previous;
        b_t = b_t->previous;
    }
    if (!a_t && !b_t) 
        return true;
    return false;
}

bool node_cmp(pdq_node_ptr a, pdq_node_ptr b) {
    if ((a->type == INT_T && b->type == INT_T) || (a->type == REAL_T && b->type == REAL_T))
    {
        if (a->type == INT_T && b->type == INT_T)
        {
            return (b->word.i == a->word.i);
        }
        else if (a->type == REAL_T && b->type == REAL_T)
        {
            return (b->word.d == a->word.d);
        }
        else
        {
            printf("'=' both args to be integer or both to be floating point, but got a mix\n");
            return false;
        }
    } else if (a->type == LIST_T && b->type == LIST_T) {
        return list_cmp(a->word.list, b->word.list);
    } else if (a->type == STRING_T && b->type == STRING_T) {
        return (strcmp(a->word.s, b->word.s) == 0);
    }
    printf("'=' unhandled types %c and %c\n", a->type, b->type);
    // pdq_display_word(a);
    // pdq_display_word(b);
    return false;

}

void playback_list(pdq_node_ptr phrase, pdq_instance_ptr p, bool free_up)
{
    pdq_node_ptr l = phrase->word.list;
    pdq_node_ptr rev = reverse_copy(l);
    while (rev)
    {
        l = rev->previous;
        pdq_requeue(p, rev);
        rev = l;
    }
    if (free_up)
    {
        pdq_free_list(phrase);
    }
    return;
};

stack_instance_ptr purr(stack_instance_ptr s, pdq_instance_ptr p, dictionary *d)
{
    while (!is_pdq_empty(p))
    {
        pdq_node_ptr w = pdq_dequeue(p);
        if (w && type_s(w))
        {
            pdq_node_ptr w_def = dictionary_get(d, w->word.s, NULL);

            if (w_def)
            {
                pdq_free_node(w);
                if (type_s(w_def))
                {
                    stack_push_s(s, w_def->word.s);
                }
                else if (w_def->type == BOOL_T)
                {
                    stack_push_b(s, w_def->word.b);
                }
                else if (w_def->type == INT_T)
                {
                    stack_push_i(s, w_def->word.i);
                }
                else if (w_def->type == REAL_T)
                {
                    stack_push_d(s, w_def->word.d);
                }
                else if (w_def->type == LIST_T)
                {
                    playback_list(w_def, p, false);
                }
                else if (w_def->type == IFUNC_T)
                {
                    pdq_node_ptr res = w_def->word.fun(s, p, d);
                    if (!res)
                    {
                        // OK NULL means nothing to add to the stack
                    }
                    else if (res->type == LIST_T)
                    {
                        stack_push_l(s, res->word.list);
                    }
                    else
                    {
                        stack_push_node(s, res);
                    }
                }
                else
                {
                    printf("word dictionary definition %c is not handled\n", w_def->type);
                    pdq_display_word(w_def);
                    printf("\nthis is not good and you must ask yourself \"Good god man, what have you done?\"\n");
                }
            }
            else
            {
                stack_push_node(s, w);
            }
        }
        else
        {
            stack_push_node(s, w);
        }
    }
    return s;
};

pdq_node_ptr make_string_node(char t, char *s)
{
    pdq_node_ptr n = pdq_init_node();
    n->type = t;
    n->word.s = xstrcp(s);
    return n;
}

pdq_node_ptr make_list_node(pdq_node_ptr l)
{
    pdq_node_ptr n = pdq_init_node();
    n->type = LIST_T;

    pdq_node_ptr cur, prev, head = NULL, old = NULL;
    if (l)
    {
        head = copy_node(l);
        prev = l->previous;
        cur = head;
        old = NULL;
        while (prev)
        {
            old = cur;
            cur = copy_node(prev);
            old->previous = cur;
            prev = prev->previous;
        }
        cur->previous = NULL;
    }

    n->word.list = head;
    return n;
}

pdq_node_ptr parse_list_node(char *s)
{
    pdq_node_ptr n = pdq_init_node();
    n->type = LIST_T;
    parser_result_ptr pr = parse(0, s);
    n->word.list = pr->pq->front;
    free(pr->pq);
    free(pr);
    return n;
}

pdq_node_ptr make_fun_node_introspect(pdq_node_ptr (*fun)(stack_instance_ptr, pdq_instance_ptr, dictionary_ptr))
{
    pdq_node_ptr n = pdq_init_node();
    n->type = IFUNC_T;
    n->word.fun = fun;
    return n;
}
pdq_node_ptr make_fun_node(pdq_node_ptr (*fun)(stack_instance_ptr, pdq_instance_ptr))
{
    pdq_node_ptr n = pdq_init_node();
    n->type = IFUNC_T;
    n->word.fun = *fun;
    return n;
}
pdq_node_ptr make_boolean_node(bool b)
{
    pdq_node_ptr n = pdq_init_node();
    n->type = BOOL_T;
    n->word.b = b;
    return n;
}
pdq_node_ptr make_integer_node(long i)
{
    pdq_node_ptr n = pdq_init_node();
    n->type = INT_T;
    n->word.i = i;
    return n;
}
pdq_node_ptr make_double_node(double d)
{
    pdq_node_ptr n = pdq_init_node();
    n->type = REAL_T;
    n->word.d = d;
    return n;
}
char min_char(char num1, char num2)
{
    return (num1 > num2) ? num2 : num1;
}

pdq_node_ptr pf_strAppend(stack_instance_ptr s, pdq_instance_ptr p)
{
    pdq_node_ptr a = stack_pop(s);
    if (a && type_s(a))
    {
        pdq_node_ptr b = stack_pop(s);
        if (b && type_s(b))
        {
            char *result;
            size_t len;

            len = strlen(b->word.s) + strlen(a->word.s) + 1;
            result = (char *)malloc(len);
            if (result)
            {
                strcpy(result, b->word.s);
            }
            strcat(result, a->word.s);
            pdq_node_ptr n = make_string_node(min_char(a->type, b->type), result);
            pdq_free_node(a);
            pdq_free_node(b);
            free(result);
            return n;
        }
        else
        {
            fprintf(stderr, "strAppend encountered a non string at top(-1) of stack\n");
        }
        if (b)
            pdq_free_node(b);
    }
    else
    {
        fprintf(stderr, "strAppend encountered a non string at top of stack\n");
    }
    if (a)
        pdq_free_node(a);
    return NULL;
};

pdq_node_ptr pf_boolOr(stack_instance_ptr s, pdq_instance_ptr p)
{
    pdq_node_ptr a = stack_pop(s);
    if (a && a->type == BOOL_T)
    {
        pdq_node_ptr b = stack_pop(s);
        if (b && b->type == BOOL_T)
        {
            long result = b->word.b || a->word.b;
            pdq_free_node(a);
            pdq_free_node(b);
            return make_boolean_node(result);
        }
        else
        {
            printf("'||' expected top(-1) to be boolean, but got type %c instead\n", b->type);
        }
        pdq_free_node(b);
    }
    else
    {
        printf("'||' expected top to be a Boolean, but got type %c instead\n", a->type);
    }
    pdq_free_node(a);
    return NULL;
};
pdq_node_ptr pf_boolAnd(stack_instance_ptr s, pdq_instance_ptr p)
{
    pdq_node_ptr a = stack_pop(s);
    if (a && a->type == BOOL_T)
    {
        pdq_node_ptr b = stack_pop(s);
        if (b && b->type == BOOL_T)
        {
            long result = b->word.b && a->word.b;
            pdq_free_node(a);
            pdq_free_node(b);
            return make_boolean_node(result);
        }
        else
        {
            printf("'&&' expected top(-1) to be Boolean, but got type %c instead\n", b->type);
        }
        pdq_free_node(b);
    }
    else
    {
        printf("'&&' expected top to be a Boolean, but got type %c instead\n", a->type);
    }
    pdq_free_node(a);
    return NULL;
};
pdq_node_ptr pf_boolNot(stack_instance_ptr s, pdq_instance_ptr p)
{
    pdq_node_ptr a = stack_pop(s);
    if (a && a->type == BOOL_T)
    {
        long result = !a->word.b;
        pdq_free_node(a);
        return make_boolean_node(result);
    }
    else
    {
        printf("'!' expected the item at the top of the stack to be boolean, but got type %c instead\n", a->type);
    }
    pdq_free_node(a);
    return NULL;
};
pdq_node_ptr pf_numAdd(stack_instance_ptr s, pdq_instance_ptr p)
{
    pdq_node_ptr a = stack_pop(s);
    if (a && (a->type == INT_T || a->type == REAL_T))
    {
        pdq_node_ptr b = stack_pop(s);
        if (b && (b->type == INT_T || b->type == REAL_T))
        {
            if (a->type == INT_T && b->type == INT_T)
            {
                long sum = b->word.i + a->word.i;
                pdq_free_node(a);
                pdq_free_node(b);
                return make_integer_node(sum);
            }
            else if (a->type == REAL_T && b->type == REAL_T)
            {
                double sum = b->word.d + a->word.d;
                pdq_free_node(a);
                pdq_free_node(b);
                return make_double_node(sum);
            }
            else
            {
                printf("add both args to be integer or both to be floating point, but got a mix\n");
            }
        }
        else
        {
            printf("add expected top(-1) to be integer, but got type %c instead\n", b->type);
        }
        pdq_free_node(b);
    }
    else
    {
        printf("add expected top to be integer, but got type %c instead\n", a->type);
    }
    pdq_free_node(a);
    return NULL;
};
pdq_node_ptr pf_numSubtract(stack_instance_ptr s, pdq_instance_ptr p)
{
    pdq_node_ptr a = stack_pop(s);
    if (a && (a->type == INT_T || a->type == REAL_T))
    {
        pdq_node_ptr b = stack_pop(s);
        if (b && (b->type == INT_T || b->type == REAL_T))
        {
            if (a->type == INT_T && b->type == INT_T)
            {
                long sum = b->word.i - a->word.i;
                pdq_free_node(a);
                pdq_free_node(b);
                return make_integer_node(sum);
            }
            else if (a->type == REAL_T && b->type == REAL_T)
            {
                double sum = b->word.d - a->word.d;
                pdq_free_node(a);
                pdq_free_node(b);
                return make_double_node(sum);
            }
            else
            {
                printf("subtract both args to be integer or both to be floating point, but got a mix\n");
            }
        }
        else
        {
            printf("subtract expected top(-1) to be integer, but got type %c instead\n", b->type);
        }
        pdq_free_node(b);
    }
    else
    {
        printf("subtract expected top to be integer, but got type %c instead\n", a->type);
    }
    pdq_free_node(a);
    return NULL;
};
pdq_node_ptr pf_numMult(stack_instance_ptr s, pdq_instance_ptr p)
{
    pdq_node_ptr a = stack_pop(s);
    if (a == NULL)
        return NULL;
    if (a->type == INT_T || a->type == REAL_T)
    {
        pdq_node_ptr b = stack_pop(s);
        if (b == NULL)
        {
            stack_push_node(s, a);
            return NULL;
        }
        if (b->type == INT_T || b->type == REAL_T)
        {
            if (a->type == INT_T && b->type == INT_T)
            {
                long prod = b->word.i * a->word.i;
                pdq_free_node(a);
                pdq_free_node(b);
                return make_integer_node(prod);
            }
            else if (a->type == REAL_T && b->type == REAL_T)
            {
                double prod = b->word.d * a->word.d;
                pdq_free_node(a);
                pdq_free_node(b);
                return make_double_node(prod);
            }
            else
            {
                printf("numMult both args to be integer or both to be floating point, but got a mix\n");
                stack_push_node(s, b);
                stack_push_node(s, a);
                return NULL;
            }
        }
        else
        {
            printf("numMult expected top(-1) to be a number, but got type %c instead\n", b->type);
            stack_push_node(s, b);
            stack_push_node(s, a);
            return NULL;
        }
    }
    else
    {
        printf("numMult expected top to be a number, but got type %c instead\n", a->type);
        stack_push_node(s, a);
        return NULL;
    }
};
pdq_node_ptr pf_numDivide(stack_instance_ptr s, pdq_instance_ptr p)
{
    pdq_node_ptr a = stack_pop(s);
    if (a && (a->type == INT_T || a->type == REAL_T))
    {
        pdq_node_ptr b = stack_pop(s);
        if (b && (b->type == INT_T || b->type == REAL_T))
        {
            if (a->type == INT_T && b->type == INT_T)
            {
                long prod = b->word.i / a->word.i;
                pdq_free_node(a);
                pdq_free_node(b);
                return make_integer_node(prod);
            }
            else if (a->type == REAL_T && b->type == REAL_T)
            {
                double prod = b->word.d / a->word.d;
                pdq_free_node(a);
                pdq_free_node(b);
                return make_double_node(prod);
            }
            else
            {
                printf("numDivide both args to be integer or both to be floating point, but got a mix\n");
            }
            pdq_free_node(b);
        }
        else
        {
            printf("numDivide expected top(-1) to be integer, but got type %c instead\n", b->type);
        }
        pdq_free_node(a);
    }
    else
    {
        printf("numDivide expected top to be integer, but got type %c instead\n", a->type);
    }
    return NULL;
};

pdq_node_ptr pf_swap(stack_instance_ptr s, pdq_instance_ptr p)
{
    pdq_node_ptr new_second = s->top;
    if (new_second)
    {
        pdq_node_ptr new_top = s->top->previous;
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

pdq_node_ptr pf_drop(stack_instance_ptr s, pdq_instance_ptr p)
{
    pdq_node_ptr ele = stack_pop(s);
    if (!ele)
    {
        printf("'drop' expected an element at the top of the stack, but it was empty\n");
    }
    else
    {
        pdq_free_node(ele);
    }
    return NULL;
};
// size of list
pdq_node_ptr pf_size(stack_instance_ptr s, pdq_instance_ptr p)
{
    pdq_node_ptr e = s->top;
    if (!e || e->type != LIST_T)
    {
        printf("'size' expected a list at the top of the stack\n");
        return NULL;
    }
    long c = 0;
    pdq_node_ptr l = e->word.list;
    while (l)
    {
        c++;
        l = l->previous;
    }
    return make_integer_node(c);
};

// duplicate a node, recursive for lists
pdq_node_ptr copy_node(pdq_node_ptr e)
{
    if (!e)
        return NULL;
    if (type_s(e))
        return make_string_node(e->type, e->word.s);
    if (e->type == INT_T)
        return make_integer_node(e->word.i);
    if (e->type == BOOL_T)
        return make_boolean_node(e->word.b);
    if (e->type == REAL_T)
        return make_double_node(e->word.d);
    if (e->type == LIST_T)
        return make_list_node(e->word.list);
    printf("Error in copy_node, type not handled.");
    return NULL;
}

// pounce (internal word) function
pdq_node_ptr pf_dup(stack_instance_ptr s, pdq_instance_ptr p)
{
    pdq_node_ptr e = s->top;
    if (!e)
    {
        printf("'dup' expected an element at the top of the stack\n");
        return NULL;
    }
    stack_push_node(s, copy_node(e));
    return NULL;
};
pdq_node_ptr pf_cons(stack_instance_ptr s, pdq_instance_ptr p)
{
    pdq_node_ptr list = s->top;
    if (list && list->type == LIST_T)
    {
        pdq_node_ptr ele = s->top->previous;
        if (ele)
        {
            list->previous = ele->previous;
            ele->previous = list->word.list;
            list->word.list = ele;
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
pdq_node_ptr pf_concatArr(stack_instance_ptr s, pdq_instance_ptr p)
{
    pdq_node_ptr list1 = stack_pop(s);
    if (list1 && list1->type == LIST_T)
    {
        pdq_node_ptr list0 = s->top;
        if (list0 && list0->type == LIST_T)
        {
            pdq_node_ptr lastIn0 = list_getLast(list0->word.list);
            lastIn0->previous = list1->word.list;
            free(list1);
        }
        else
        {
            printf("'concat' expected top(-1) to be a list.\n");
        }
    }
    else
    {
        printf("'concat' expected the top the stack to be a list\n");
    }
    return NULL;
};
pdq_node_ptr pf_uncons(stack_instance_ptr s, pdq_instance_ptr p)
{
    pdq_node_ptr e = s->top;
    if (!e || e->type != LIST_T)
    {
        printf("'uncons' expected a list at the top of the stack\n");
        return NULL;
    }
    pdq_node_ptr l = e->word.list;
    pdq_node_ptr first;
    if (l)
    {
        first = l;
        e->word.list = l->previous;
        first->previous = e->previous;
        e->previous = first;
    }
    else
    {
        printf("'uncons' expected a list with at least one element\n");
    }
    return NULL;
};

pdq_node_ptr pf_reverse(stack_instance_ptr s, pdq_instance_ptr p)
{
    pdq_node_ptr e = stack_pop(s);
    if (!e || e->type != LIST_T)
    {
        printf("'reverse' expected a list at the top of the stack\n");
        return NULL;
    }
    pdq_node_ptr newrl = reverse_copy(e->word.list);
    pdq_free_list(e);
    stack_push_l(s, newrl);
    // pdq_node_ptr l = e->word.list;
    // pdq_node_ptr first;
    // if (l)
    // {
    //     first = l;
    //     e->word.list = l->previous;
    //     first->previous = e->previous;
    //     e->previous = first;
    // }
    // else
    // {
    //     printf("'uncons' expected a list with at least one element\n");
    // }
    return NULL;
};

pdq_node_ptr pf_if_else(stack_instance_ptr s, pdq_instance_ptr p)
{
    pdq_node_ptr e = stack_pop(s);
    if (!e || e->type != LIST_T)
    {
        printf("'ifte' expected an else phrase at the top of the stack\n");
        return NULL;
    }
    pdq_node_ptr t = stack_pop(s);
    if (!t || t->type != LIST_T)
    {
        printf("'ifte' expected a then phrase at the top (-1) of the stack\n");
        return NULL;
    }
    pdq_node_ptr c = stack_pop(s);
    if (!c || c->type != BOOL_T)
    {
        printf("'ifte' expected a boolean at the top (-2) of the stack\n");
        return NULL;
    }
    if (c->word.b)
    {
        playback_list(t, p, true);
        pdq_free_node(e);
    }
    else
    {
        playback_list(e, p, true);
        pdq_free_node(t);
    }
    pdq_free_node(c);

    return NULL;
};

#ifdef MICROPROCESSOR
pdq_node_ptr pf_gpioInit(stack_instance_ptr s, pdq_instance_ptr p)
{
    pdq_node_ptr a = stack_pop(s);
    if (a && a->type == STRING_T)
    {
        pdq_node_ptr b = stack_pop(s);
        if (b && b->type == INT_T)
        {
            bool out_dir = (strcmp(a->word.s, "OUT") == 0);
            init_gpio(b->word.i, out_dir, 0);
            pdq_free_node(b);
        }
        else
        {
            printf("gpioInit expected top(-1) to be integer, but got type %c instead\n", b->type);
        }
        pdq_free_node(a);
    }
    else
    {
        printf("gpioInit expected top to be string [IN OUT], but got type %c instead\n", a->type);
    }
    return NULL;
};

pdq_node_ptr pf_gpioSet(stack_instance_ptr s, pdq_instance_ptr p)
{
    pdq_node_ptr a = stack_pop(s);
    if (a && a->type == STRING_T)
    {
        pdq_node_ptr pin = stack_pop(s);
        if (pin && pin->type == INT_T)
        {
            bool val = (strcmp(a->word.s, "HIGH") == 0);
            set_gpio(pin->word.i, val);
            pdq_free_node(pin);
        }
        else
        {
            printf("gpioSet expected top(-1) to be integer, but got type %c instead\n", pin->type);
        }
        pdq_free_node(a);
    }
    else
    {
        printf("gpioSet expected top to be string [LOW HIGH], but got type %c instead\n", a->type);
    }
    // nothing was added to the stack or the pl
    return NULL;
};

pdq_node_ptr pf_gpioGet(stack_instance_ptr s, pdq_instance_ptr p)
{
    pdq_node_ptr pin = stack_pop(s);
    if (pin && pin->type == INT_T)
    {
        bool val;
        if (pin->word.i == -1)
        {
            val = get_bootsel_button();
        }
        else
        {
            val = get_gpio(pin->word.i);
        }
        pdq_free_node(pin);
        stack_push_b(s, val);
    }
    else
    {
        printf("gpioSet expected top(-1) to be integer, but got type %c instead\n", pin->type);
    }
    // nothing was added to the stack or the pl
    return NULL;
};
#endif

pdq_node_ptr pf_play(stack_instance_ptr s, pdq_instance_ptr p)
{
    pdq_node_ptr phrase = stack_pop(s);
    if (!phrase || phrase->type != LIST_T)
    {
        printf("'play' expected a list at the top of the stack\n");
        return NULL;
    }
    playback_list(phrase, p, true);
    return NULL;
};

// for use in crouch and pounce

pdq_node_ptr replace_recursive_in_phrase(pdq_node_ptr l, char *s, pdq_node_ptr v)
{
    pdq_node_ptr ele = l->word.list;
    pdq_node_ptr pre = NULL;
    while (ele)
    {
        if (ele->type == LIST_T)
        {
            replace_recursive_in_phrase(ele, s, v);
        }
        else if (ele->type == STRING_T && strcmp(s, ele->word.s) == 0)
        {
            pdq_node_ptr rep = copy_node(v);
            rep->previous = ele->previous;
            if (pre == NULL)
            {
                l->word.list = rep;
            }
            else
            {
                pre->previous = rep;
            }
            pdq_free_node(ele);
            ele = rep;
        }
        pre = ele;
        ele = ele->previous;
    }
    //pdq_free_node(v);
    return l;
};

pdq_node_ptr replace_each_in_phrase(stack_instance_ptr s, pdq_node_ptr ph, pdq_node_ptr names_list)
{
    pdq_node_ptr p = ph; //copy_node(ph); // rm as excessive copy
    // for each name in names_list
    pdq_node_ptr l = names_list->word.list;
    pdq_node_ptr rev_l = reverse_copy(l);
    pdq_node_ptr n = rev_l;
    pdq_node_ptr stack_value;
    while (n)
    {
        // check name must be a string
        if (n->type != STRING_T)
        {
            printf("Error in crouch or pounce, names must be (unquoted) strings");
            return NULL;
        }
        // pop the stack for a value associated with each name
        stack_value = stack_pop(s);
        if (stack_value == NULL || stack_value->type == LIST_T || stack_value->type == IFUNC_T)
        {
            printf("Error in crouch or pounce, stack values for %s cannot be lists or functions", n->word.s);
            return NULL;
        }
        replace_recursive_in_phrase(p, n->word.s, stack_value);
        pdq_free_node(stack_value);
        n = n->previous;
    }
    pdq_free_list(rev_l);
    return p;
};

pdq_node_ptr pf_crouch(stack_instance_ptr s, pdq_instance_ptr p)
{
    pdq_node_ptr phrase = stack_pop(s);
    pdq_node_ptr stack_names = stack_pop(s);
    if (!phrase || phrase->type != LIST_T)
    {
        printf("'crouch' expected a list at the top of the stack\n");
        return NULL;
    }

    stack_push_node(s, replace_each_in_phrase(s, phrase, stack_names));
    // pdq_free_node(phrase);  // rm as excessive copy
    pdq_free_node(stack_names);
    return NULL;
};

pdq_node_ptr pf_dip(stack_instance_ptr s, pdq_instance_ptr p)
{
    pdq_node_ptr e = stack_pop(s);
    pdq_node_ptr t = stack_pop(s);
    if (!e || e->type != LIST_T)
    {
        printf("'dip' expected a list at the top of the stack\n");
        return NULL;
    }
    pdq_requeue(p, t);
    playback_list(e, p, true);
    return NULL;
};

pdq_node_ptr pf_dip2(stack_instance_ptr s, pdq_instance_ptr p)
{
    pdq_node_ptr e = stack_pop(s);
    pdq_node_ptr t = stack_pop(s);
    pdq_node_ptr t2 = stack_pop(s);
    if (!e || e->type != LIST_T)
    {
        printf("'dip2' expected a list at the top of the stack\n");
        return NULL;
    }
    pdq_requeue(p, t);
    pdq_requeue(p, t2);
    playback_list(e, p, true);
    return NULL;
};

pdq_node_ptr pf_eq(stack_instance_ptr s, pdq_instance_ptr p)
{
    pdq_node_ptr a = stack_pop(s);
    pdq_node_ptr b = stack_pop(s);
    if (a == NULL || b == NULL)
        return NULL;
    else if(a && b) {
        bool result = node_cmp(a, b);
        pdq_free_node(a);
        pdq_free_node(b);
        return make_boolean_node(result);
    }
    else
    {
        printf("'=' expected top(-1) to be a number, but got type %c instead\n", b->type);
        stack_push_node(s, b);
        stack_push_node(s, a);
        return NULL;
    }
};

pdq_node_ptr pf_numGt(stack_instance_ptr s, pdq_instance_ptr p)
{
    pdq_node_ptr a = stack_pop(s);
    if (a == NULL)
        return NULL;
    if (a->type == INT_T || a->type == REAL_T)
    {
        pdq_node_ptr b = stack_pop(s);
        if (b == NULL)
        {
            stack_push_node(s, a);
            return NULL;
        }
        if (b->type == INT_T || b->type == REAL_T)
        {
            if (a->type == INT_T && b->type == INT_T)
            {
                bool prod = (b->word.i > a->word.i);
                pdq_free_node(a);
                pdq_free_node(b);
                return make_boolean_node(prod);
            }
            else if (a->type == REAL_T && b->type == REAL_T)
            {
                bool prod = (b->word.d > a->word.d);
                pdq_free_node(a);
                pdq_free_node(b);
                return make_boolean_node(prod);
            }
            else
            {
                printf("'>' both args to be integer or both to be floating point, but got a mix\n");
                stack_push_node(s, b);
                stack_push_node(s, a);
                return NULL;
            }
        }
        else
        {
            printf("'>' expected top(-1) to be a number, but got type %c instead\n", b->type);
            stack_push_node(s, b);
            stack_push_node(s, a);
            return NULL;
        }
    }
    else
    {
        printf("'>' expected top to be a number, but got type %c instead\n", a->type);
        stack_push_node(s, a);
        return NULL;
    }
};

pdq_node_ptr pf_words(stack_instance_ptr s, pdq_instance_ptr p, dictionary *d)
{
    pdq_node_ptr n = make_string_node(STRING_T, dictionary_words(d));
    stack_push_node(s, n);
    return NULL;
};

dictionary *init_core_word_dictionary()
{
    dictionary *wd = dictionary_new(64);
    dictionary_set(wd, "dup2", parse_list_node("[dup] dip dup [swap] dip"));
    dictionary_set(wd, "strAppend", make_fun_node(pf_strAppend));
    dictionary_set(wd, "+", make_fun_node(pf_numAdd));
    dictionary_set(wd, "-", make_fun_node(pf_numSubtract));
    dictionary_set(wd, "*", make_fun_node(pf_numMult));
    dictionary_set(wd, "/", make_fun_node(pf_numDivide));
    dictionary_set(wd, "||", make_fun_node(pf_boolOr));
    dictionary_set(wd, "&&", make_fun_node(pf_boolAnd));
    dictionary_set(wd, "!", make_fun_node(pf_boolNot));
    dictionary_set(wd, "swap", make_fun_node(pf_swap));
    dictionary_set(wd, "drop", make_fun_node(pf_drop));
    dictionary_set(wd, "size", make_fun_node(pf_size));
    dictionary_set(wd, "dup", make_fun_node(pf_dup));
    dictionary_set(wd, "cons", make_fun_node(pf_cons));
    dictionary_set(wd, "uncons", make_fun_node(pf_uncons));
    dictionary_set(wd, "reverse", make_fun_node(pf_reverse));

    dictionary_set(wd, "play", make_fun_node(pf_play));
    dictionary_set(wd, "dip", make_fun_node(pf_dip));
    // concat binrec split < <= ... preping for quick-sort
    // dictionary_set(wd, "<", make_fun_node(pf_numLt));
    // dictionary_set(wd, "<=", make_fun_node(pf_numLtEq));
    dictionary_set(wd, ">", make_fun_node(pf_numGt));
    dictionary_set(wd, "=", make_fun_node(pf_eq));
    // dictionary_set(wd, ">=", make_fun_node(pf_numGtEq));
    // dictionary_set(wd, "==", make_fun_node(pf_numEq));
    // dictionary_set(wd, "split", make_fun_node(pf_arrSplit));
    dictionary_set(wd, "concat", make_fun_node(pf_concatArr));
    // dictionary_set(wd, "binrec", make_fun_node(pf_binrec));
    // // // const block = toPLOrNull(s?.pop());
    // // //         const item2 = s?.pop();
    // // //         pl = [item2].concat(pl);
    // // //         const item1 = s?.pop();
    // // //         pl = [item1].concat(pl);
    // // //         if (block) {
    // // //             pl = block.concat(pl);
    dictionary_set(wd, "dip", make_fun_node(pf_dip));
    dictionary_set(wd, "dip2", make_fun_node(pf_dip2));
    dictionary_set(wd, "if-else", make_fun_node(pf_if_else));
    // ['dup', 0, '>', [1, '-', 'swap', 'dup', 'dip2', 'swap', 'times'], ['drop', 'drop'], 'if-else']
    dictionary_set(wd, "times", parse_list_node("dup 0 > [1 - swap dup dip2 swap times] [drop drop] if-else"));
    dictionary_set(wd, "rec", parse_list_node("dup 0 > [1 - [a swap cons] dip rec] [drop] if-else"));

    dictionary_set(wd, "crouch", make_fun_node(pf_crouch));
//    dictionary_set(wd, "words", make_fun_node_introspect(pf_words));
#ifdef MICROPROCESSOR
    dictionary_set(wd, "gpioInit", make_fun_node(pf_gpioInit));
    dictionary_set(wd, "gpioSet", make_fun_node(pf_gpioSet));
    dictionary_set(wd, "gpioGet", make_fun_node(pf_gpioGet));
#endif
#if POUNCE_PICO_RP2040
    dictionary_set(wd, "initLED", parse_list_node("25 OUT gpioInit"));
    dictionary_set(wd, "on", parse_list_node("25 HIGH gpioSet"));
    dictionary_set(wd, "off", parse_list_node("25 LOW gpioSet"));
#endif
#if SF_PRO_MICRO_RP2040
    dictionary_set(wd, "initLED", make_fun_node(pf_initWS2812LED));
    dictionary_set(wd, "setLED", make_fun_node(pf_setWS2812LED));
    dictionary_set(wd, "color", parse_list_node("[256 * 256 *] dip [256 *] dip2 + +"));
    // [c+] [dup 16777214 > ! [100 + dup setLED c+] [1 drop] if-else] compose
#endif
    return wd;
}
