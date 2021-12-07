#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "pounce.h"

/* A dequeue (double ended) implementation.
 * Standard Queue methods are: 
 *  pounce_enqueue() puts an element at the rear of the queue.
 *  pounce_dequeue() pulls out the first element at the front of the queue.
 * Less standard methods are:
 *  pounce_requeue() puts an element into the front of the queue.
 *  pounce_popout() pulls the last element off of the rear of the queue.
 */

typedef struct pounce_instance
{
	pounce_node_ptr front; // = NULL;
	pounce_node_ptr rear;  // = NULL;
} * pounce_instance_ptr;

static char *xstrcp(const char *s)
{
	char *t;
	size_t len;
	if (!s)
		return NULL;

	len = strlen(s) + 1;
	t = (char *)malloc(len);
	if (t)
	{
		memcpy(t, s, len);
	}
	return t;
}

word_ptr word_init()
{
	word_ptr new_word = (word_ptr)malloc(sizeof(struct word_struct));

	if (new_word == NULL)
	{
		printf("could not malloc word_struct union\n");
		return NULL;
	}
	// printf("returning new word %d\n", &new_word);
	return new_word;
};
pounce_node_ptr pounce_init_node()
{
	pounce_node_ptr item = (pounce_node_ptr)malloc(sizeof(struct pounce_node));
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
};

void pounce_free_node(pounce_node_ptr node);

void pounce_free_list(pounce_node_ptr le) {
	pounce_node_ptr next;
	while(le) {
		next = le->previous;
		pounce_free_node(le);
		le = next;
	}
};

void pounce_free_node(pounce_node_ptr node)
{
	if (type_s(node))
	{
		free(node->data->w.s);
	}
	if (node->type == LIST_T)
	{
		pounce_free_list(node->data->w.list);
	}
	// if (node->type == IFUNC_T)
	// {
	// 	free(node->data->w.fun);
	// }
	free(node->data);
	free(node);
}

pounce_instance_ptr pounce_init()
{
	pounce_instance_ptr new_pq = (pounce_instance_ptr)malloc(sizeof(struct pounce_instance));

	if (new_pq == NULL)
	{
		printf("could not malloc pq 0\n");
		return NULL;
	}
	new_pq->front = NULL;
	new_pq->rear = NULL;
	// printf("returning new pq 0 %d\n", &new_pq);
	return new_pq;
};

// bool pounce_delete(pounce_instance_ptr old_pq)
// {
// 	return false;
// };

bool is_pounce_empty(pounce_instance_ptr pq)
{
	return (pq->front == NULL || pq->rear == NULL);
}

bool pounce_repounce_s(pounce_instance_ptr pq, char *value)
{
	pounce_node_ptr item = pounce_init_node();
	if (!item)
	{
		return false;
	}

	item->data->w.s = xstrcp(value);
	item->type = STRING_T;

	if (pq->front == NULL)
	{
		pq->front = pq->rear = item;
		item->previous = NULL;
	}
	else
	{
		item->previous = pq->front;
		pq->front = item;
	}
	return true;
}
bool pounce_requeue(pounce_instance_ptr pq, pounce_node_ptr item)
{
	if (!item)
	{
		return false;
	}
	if (pq->front == NULL)
	{
		pq->front = pq->rear = item;
		item->previous = NULL;
	}
	else
	{
		item->previous = pq->front;
		pq->front = item;
	}
	return true;
}

bool pounce_enqueue(pounce_instance_ptr pq, pounce_node_ptr item)
{
	if (!item)
	{
		return false;
	}

	if (pq->rear == NULL)
		pq->front = pq->rear = item;
	else
	{
		pq->rear->previous = item;
		pq->rear = item;
	}
	return true;
};

bool pounce_enpounce_l(pounce_instance_ptr pq, pounce_node_ptr value)
{
	pounce_node_ptr item = pounce_init_node();
	if (!item)
	{
		return false;
	}
	item->data->w.list = value;
	item->type = LIST_T;

	return pounce_enqueue(pq, item);
};
bool pounce_enpounce_b(pounce_instance_ptr pq, bool value)
{
	pounce_node_ptr item = pounce_init_node();
	if (!item)
	{
		return false;
	}
	item->data->w.b = value;
	item->type = BOOL_T;

	return pounce_enqueue(pq, item);
};
bool pounce_enpounce_i(pounce_instance_ptr pq, long value)
{
	pounce_node_ptr item = pounce_init_node();
	if (!item)
	{
		return false;
	}
	item->data->w.i = value;
	item->type = INT_T;

	return pounce_enqueue(pq, item);
};
bool pounce_enpounce_d(pounce_instance_ptr pq, double value)
{
	pounce_node_ptr item = pounce_init_node();
	if (!item)
	{
		return false;
	}
	item->data->w.d = value;
	item->type = REAL_T;

	return pounce_enqueue(pq, item);
};
bool pounce_enpounce_s(pounce_instance_ptr pq, char t, char *value)
{
	pounce_node_ptr item = pounce_init_node();
	if (!item)
	{
		return false;
	}
	item->data->w.s = xstrcp(value);
	item->type = t;

	return pounce_enqueue(pq, item);
}

pounce_node_ptr pounce_dequeue(pounce_instance_ptr pq)
{

	if (is_pounce_empty(pq))
	{
		return NULL;
	}
	pounce_node_ptr temp = pq->front;
	pounce_node_ptr ret_val = pq->front;
	if (temp->previous == NULL)
	{
		pq->rear = NULL;
	}
	pq->front = pq->front->previous;
	ret_val->previous = NULL;
	return ret_val;
}

// popback is not very efficient, but it is assumed to be only used in non-time critical processes
pounce_node_ptr pounce_popout(pounce_instance_ptr pq)
{
	if (is_pounce_empty(pq))
	{
		return NULL;
	}
	pounce_node_ptr temp = pq->front;
	pounce_node_ptr ret_val = pq->rear;
	// not a doubly linked list, so we have to walk the list
	while (temp->previous != NULL && temp->previous != ret_val)
	{
		temp = temp->previous;
	}
	if (temp == ret_val)
	{
		pq->rear = NULL;
		pq->front = NULL;
		return ret_val;
	}
	pq->rear = temp;
	temp->previous = NULL;
	return ret_val;
}

bool pounce_clear(pounce_instance_ptr pq)
{

	if (is_pounce_empty(pq))
	{

		return false;
	}

	pounce_node_ptr current = pq->front;
	pounce_node_ptr previous = NULL;

	while (current != NULL)
	{

		previous = current->previous;
		free(current);
		current = previous;
	}

	pq->front = NULL;
	pq->rear = NULL;

	return is_pounce_empty(pq);
}
