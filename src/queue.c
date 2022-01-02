#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "pounce.h"

/* A dequeue (double ended) implementation.
 * Standard Queue methods are: 
 *  pdq_enqueue() puts an element at the rear of the queue.
 *  pdq_dequeue() pulls out the first element at the front of the queue.
 * Less standard methods are:
 *  pdq_requeue() puts an element into the front of the queue.
 *  pdq_popout() pulls the last element off of the rear of the queue.
 */
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
pdq_node_ptr pdq_init_node()
{
	pdq_node_ptr item = (pdq_node_ptr)malloc(sizeof(struct pdq_node));
	if (item == NULL)
	{
		return NULL;
	}
	word_ptr wp = word_init();
	if (wp == NULL)
	{
		return NULL;
	}
	item->data = wp;
	item->previous = NULL;
	return item;
};

void pdq_free_node(pdq_node_ptr node);

void pdq_free_list(pdq_node_ptr le) {
	pdq_node_ptr next;
	while(le) {
		next = le->previous;
		pdq_free_node(le);
		le = next;
	}
};

void pdq_free_node(pdq_node_ptr node)
{
	if (type_s(node))
	{
		free(node->data->w.s);
	}
	if (node->type == LIST_T)
	{
		pdq_free_list(node->data->w.list);
	}
	// if (node->type == IFUNC_T)
	// {
	// 	free(node->data->w.fun);
	// }
	free(node->data);
	free(node);
}

pdq_instance_ptr pdq_init()
{
	pdq_instance_ptr new_pq = (pdq_instance_ptr)malloc(sizeof(struct pdq_instance));

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

// bool pdq_delete(pdq_instance_ptr old_pq)
// {
// 	return false;
// };

bool is_pdq_empty(pdq_instance_ptr pq)
{
	return (pq->front == NULL || pq->rear == NULL);
}

bool pdq_requeue_s(pdq_instance_ptr pq, char *value)
{
	pdq_node_ptr item = pdq_init_node();
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
bool pdq_requeue(pdq_instance_ptr pq, pdq_node_ptr item)
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

bool pdq_enqueue(pdq_instance_ptr pq, pdq_node_ptr item)
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

bool pdq_enqueue_l(pdq_instance_ptr pq, pdq_node_ptr value)
{
	pdq_node_ptr item = pdq_init_node();
	if (!item)
	{
		return false;
	}
	item->data->w.list = value;
	item->type = LIST_T;

	return pdq_enqueue(pq, item);
};
bool pdq_enqueue_b(pdq_instance_ptr pq, bool value)
{
	pdq_node_ptr item = pdq_init_node();
	if (!item)
	{
		return false;
	}
	item->data->w.b = value;
	item->type = BOOL_T;

	return pdq_enqueue(pq, item);
};
bool pdq_enqueue_i(pdq_instance_ptr pq, long value)
{
	pdq_node_ptr item = pdq_init_node();
	if (!item)
	{
		return false;
	}
	item->data->w.i = value;
	item->type = INT_T;

	return pdq_enqueue(pq, item);
};
bool pdq_enqueue_d(pdq_instance_ptr pq, double value)
{
	pdq_node_ptr item = pdq_init_node();
	if (!item)
	{
		return false;
	}
	item->data->w.d = value;
	item->type = REAL_T;

	return pdq_enqueue(pq, item);
};
bool pdq_enqueue_s(pdq_instance_ptr pq, char t, char *value)
{
	pdq_node_ptr item = pdq_init_node();
	if (!item)
	{
		return false;
	}
	item->data->w.s = xstrcp(value);
	item->type = t;

	return pdq_enqueue(pq, item);
}

pdq_node_ptr pdq_dequeue(pdq_instance_ptr pq)
{

	if (is_pdq_empty(pq))
	{
		return NULL;
	}
	pdq_node_ptr temp = pq->front;
	pdq_node_ptr ret_val = pq->front;
	if (temp->previous == NULL)
	{
		pq->rear = NULL;
	}
	pq->front = pq->front->previous;
	ret_val->previous = NULL;
	return ret_val;
}

// popback is not very efficient, but it is assumed to be only used in non-time critical processes
pdq_node_ptr pdq_popout(pdq_instance_ptr pq)
{
	if (is_pdq_empty(pq))
	{
		return NULL;
	}
	pdq_node_ptr temp = pq->front;
	pdq_node_ptr ret_val = pq->rear;
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

bool pdq_clear(pdq_instance_ptr pq)
{

	if (is_pdq_empty(pq))
	{

		return false;
	}

	pdq_node_ptr current = pq->front;
	pdq_node_ptr previous = NULL;

	while (current != NULL)
	{

		previous = current->previous;
		free(current);
		current = previous;
	}

	pq->front = NULL;
	pq->rear = NULL;

	return is_pdq_empty(pq);
}
