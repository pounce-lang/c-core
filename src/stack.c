#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "pounce.h"

typedef struct ps_instance
{
	pq_node_ptr top; // = NULL;
} * ps_instance_ptr;

// static char *xstrcp(const char *s)
// {
// 	char *t;
// 	size_t len;
// 	if (!s)
// 		return NULL;

// 	len = strlen(s) + 1;
// 	t = (char *)malloc(len);
// 	if (t)
// 	{
// 		memcpy(t, s, len);
// 	}
// 	return t;
// }

// word_ptr word_init()
// {
// 	word_ptr new_word = (word_ptr)malloc(sizeof(struct word_struct));

// 	if (new_word == NULL)
// 	{
// 		printf("could not malloc word_struct union\n");
// 		return NULL;
// 	}
// 	// printf("returning new word %d\n", &new_word);
// 	return new_word;
// };
pq_node_ptr ps_init_node() {
	pq_node_ptr item = (pq_node_ptr)malloc(sizeof(struct pq_node));
	if (item == NULL)
	{
		return false;
	}
	word_ptr wp = word_init();
	if (wp == NULL)
	{
		return false;
	}
	item->data = wp;
	item->previous = NULL;
	return item;
}

void ps_free_node(pq_node_ptr node)
{
	if (node->type == 's')
	{
		free(node->data->w.s);
	}
	free(node);
}

ps_instance_ptr ps_init()
{
	ps_instance_ptr new_ps = (ps_instance_ptr)malloc(sizeof(struct ps_instance));

	if (new_ps == NULL)
	{
		printf("could not malloc pq 0\n");
		return NULL;
	}
	new_ps->top = NULL;
	return new_ps;
};

bool ps_delete(ps_instance_ptr old_pq)
{
// not implemented yet?
	return false;
};

bool is_ps_empty(ps_instance_ptr ps)
{
	return (ps->top == NULL);
}


bool ps_push_l(ps_instance_ptr ps, pq_node_ptr value)
{
	pq_node_ptr item = ps_init_node();
	item->data->w.list = value;
	item->type = 'l';

	if (ps->top == NULL)
		ps->top = item;
	else
	{
		item->previous = ps->top;
		ps->top = item;
	}
	return true;
};

bool ps_push_s(ps_instance_ptr ps, char *value)
{
	pq_node_ptr item = ps_init_node();
	item->data->w.s = xstrcp(value);
	item->type = 's';

	if (ps->top == NULL)
		ps->top = item;
	else
	{
		item->previous = ps->top;
		ps->top = item;
	}
	return true;
}

pq_node_ptr ps_pop(ps_instance_ptr ps)
{

	if (is_ps_empty(ps))
	{
		return NULL;
	}
	pq_node_ptr ret_val = ps->top;
	ps->top = ps->top->previous;

	return ret_val;
}


bool ps_clear(ps_instance_ptr ps)
{

	if (is_ps_empty(ps))
	{
		return false;
	}

	pq_node_ptr current = ps->top;
	pq_node_ptr previous = NULL;

	while (current != NULL)
	{
		previous = current->previous;
        ps_free_node(current);
		current = previous;
	}
	ps->top = NULL;

	return is_ps_empty(ps);
}
