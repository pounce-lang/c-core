#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "pounce.h"


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
	item->type = LIST_T;

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
	item->type = STRING_T;

	if (ps->top == NULL)
		ps->top = item;
	else
	{
		item->previous = ps->top;
		ps->top = item;
	}
	return true;
}

bool ps_push_node(ps_instance_ptr ps, pq_node_ptr n)
{
	if (ps->top == NULL)
		ps->top = n;
	else
	{
		n->previous = ps->top;
		ps->top = n;
	}
	return true;
}

bool ps_push_b(ps_instance_ptr ps, bool value)
{
	pq_node_ptr item = ps_init_node();
	item->data->w.b = value;
	item->type = BOOL_T;

	if (ps->top == NULL)
		ps->top = item;
	else
	{
		item->previous = ps->top;
		ps->top = item;
	}
	return true;
}

bool ps_push_i(ps_instance_ptr ps, long value)
{
	pq_node_ptr item = ps_init_node();
	item->data->w.i = value;
	item->type = INT_T;

	if (ps->top == NULL)
		ps->top = item;
	else
	{
		item->previous = ps->top;
		ps->top = item;
	}
	return true;
}

bool ps_push_d(ps_instance_ptr ps, double value)
{
	pq_node_ptr item = ps_init_node();
	item->data->w.d = value;
	item->type = REAL_T;

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
	ret_val->previous = NULL;
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
        pq_free_node(current);
		current = previous;
	}
	ps->top = NULL;

	return is_ps_empty(ps);
}
