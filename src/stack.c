#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "pounce.h"


pdq_node_ptr stack_init_node() {
	pdq_node_ptr item = (pdq_node_ptr)malloc(sizeof(struct pdq_node));
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

stack_instance_ptr stack_init()
{
	stack_instance_ptr new_ps = (stack_instance_ptr)malloc(sizeof(struct stack_instance));

	if (new_ps == NULL)
	{
		printf("could not malloc pq 0\n");
		return NULL;
	}
	new_ps->top = NULL;
	return new_ps;
};

bool stack_delete(stack_instance_ptr old_pq)
{
// not implemented yet?
	return false;
};

bool is_stack_empty(stack_instance_ptr ps)
{
	return (ps->top == NULL);
}


bool stack_push_l(stack_instance_ptr ps, pdq_node_ptr value)
{
	pdq_node_ptr item = stack_init_node();
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

bool stack_push_s(stack_instance_ptr ps, char *value)
{
	pdq_node_ptr item = stack_init_node();
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

bool stack_push_node(stack_instance_ptr ps, pdq_node_ptr n)
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

bool stack_push_b(stack_instance_ptr ps, bool value)
{
	pdq_node_ptr item = stack_init_node();
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

bool stack_push_i(stack_instance_ptr ps, long value)
{
	pdq_node_ptr item = stack_init_node();
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

bool stack_push_d(stack_instance_ptr ps, double value)
{
	pdq_node_ptr item = stack_init_node();
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

pdq_node_ptr stack_pop(stack_instance_ptr ps)
{
	if (is_stack_empty(ps))
	{
		return NULL;
	}
	pdq_node_ptr ret_val = ps->top;
	ps->top = ps->top->previous;
	ret_val->previous = NULL;
	return ret_val;
}

bool stack_clear(stack_instance_ptr ps)
{
	if (is_stack_empty(ps))
	{
		return false;
	}

	pdq_node_ptr current = ps->top;
	pdq_node_ptr previous = NULL;

	while (current != NULL)
	{
		previous = current->previous;
        pdq_free_node(current);
		current = previous;
	}
	ps->top = NULL;

	return is_stack_empty(ps);
}
