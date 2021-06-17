#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "pounce.h"

typedef struct pq_instance
{
	pq_node_ptr front; // = NULL;
	pq_node_ptr rear;  // = NULL;
} * pq_instance_ptr;

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
pq_node_ptr pq_init_node()
{
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
};

void pq_free_node(pq_node_ptr node);

void pq_free_list(pq_node_ptr le) {
	pq_node_ptr next;
	while(le) {
		next = le->previous;
		pq_free_node(le);
		le = next;
	}
};

void pq_free_node(pq_node_ptr node)
{
	if (node->type == 's')
	{
		free(node->data->w.s);
	}
	if (node->type == 'l')
	{
		pq_free_list(node->data->w.list);
	}
	// if (node->type == 'f')
	// {
	// 	free(node->data->w.fun);
	// }
	free(node->data);
	free(node);
}

pq_instance_ptr pq_init()
{
	pq_instance_ptr new_pq = (pq_instance_ptr)malloc(sizeof(struct pq_instance));

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

// bool pq_delete(pq_instance_ptr old_pq)
// {
// 	return false;
// };

bool is_pq_empty(pq_instance_ptr pq)
{
	return (pq->front == NULL || pq->rear == NULL);
}

bool pq_requeue_s(pq_instance_ptr pq, char *value)
{
	pq_node_ptr item = pq_init_node();
	if (!item)
	{
		return false;
	}

	item->data->w.s = xstrcp(value);
	item->type = 's';

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
bool pq_requeue(pq_instance_ptr pq, pq_node_ptr item)
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

bool pq_enqueue_l(pq_instance_ptr pq, pq_node_ptr value)
{
	pq_node_ptr item = pq_init_node();
	if (!item)
	{
		return false;
	}
	item->data->w.list = value;
	item->type = 'l';

	if (pq->rear == NULL)
		pq->front = pq->rear = item;
	else
	{
		pq->rear->previous = item;
		pq->rear = item;
	}
	return true;
};

bool pq_enqueue_i(pq_instance_ptr pq, long value)
{
	pq_node_ptr item = pq_init_node();
	if (!item)
	{
		return false;
	}
	item->data->w.i = value;
	item->type = 'i';

	if (pq->rear == NULL)
		pq->front = pq->rear = item;
	else
	{
		pq->rear->previous = item;
		pq->rear = item;
	}
	return true;
};

bool pq_enqueue_d(pq_instance_ptr pq, double value)
{
	pq_node_ptr item = pq_init_node();
	if (!item)
	{
		return false;
	}
	item->data->w.d = value;
	item->type = 'd';

	if (pq->rear == NULL)
		pq->front = pq->rear = item;
	else
	{
		pq->rear->previous = item;
		pq->rear = item;
	}
	return true;
};

bool pq_enqueue_s(pq_instance_ptr pq, char *value)
{
	pq_node_ptr item = pq_init_node();
	if (!item)
	{
		return false;
	}
	item->data->w.s = xstrcp(value);
	item->type = 's';

	if (pq->rear == NULL)
		pq->front = pq->rear = item;
	else
	{
		pq->rear->previous = item;
		pq->rear = item;
	}
	return true;
}

pq_node_ptr pq_dequeue(pq_instance_ptr pq)
{

	if (is_pq_empty(pq))
	{
		return NULL;
	}
	pq_node_ptr temp = pq->front;
	pq_node_ptr ret_val = pq->front;
	if (temp->previous == NULL)
	{
		pq->rear = NULL;
	}
	pq->front = pq->front->previous;
	ret_val->previous = NULL;
	return ret_val;
}

bool pq_clear(pq_instance_ptr pq)
{

	if (is_pq_empty(pq))
	{

		return false;
	}

	pq_node_ptr current = pq->front;
	pq_node_ptr previous = NULL;

	while (current != NULL)
	{

		previous = current->previous;
		free(current);
		current = previous;
	}

	pq->front = NULL;
	pq->rear = NULL;

	return is_pq_empty(pq);
}
