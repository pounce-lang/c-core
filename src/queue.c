#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct word
{
	union
	{
		char *s;
		long i;
		double f;
		struct pq_node *list;
	} w;
} * word_ptr;

typedef struct pq_node
{
	char type; // (w)ord, (v)alue, (l)ist, (")wrapped_string, (')wrapped_string, (`)wrapped_string
	word_ptr data;
	struct pq_node *previous;

} * pq_node_ptr;

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
	word_ptr new_word = (word_ptr)malloc(sizeof(struct word));

	if (new_word == NULL)
	{
		printf("could not malloc word union\n");
		return NULL;
	}
	return new_word;
};

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
	printf("returning pq 0 %d\n", &new_pq);
	return new_pq;
};

bool pq_delete(pq_instance_ptr old_pq)
{
	return false;
};

bool is_pq_empty(pq_instance_ptr pq)
{
	return (pq->front == NULL || pq->rear == NULL);
}

bool pq_requeue(pq_instance_ptr pq, char t, word_ptr value)
{

	pq_node_ptr item = (pq_node_ptr)malloc(sizeof(struct pq_node));

	if (item == NULL)
	{
		return false;
	}

	if (t == 's')
	{
		item->data->w.s = xstrcp(value->w.s);
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

bool pq_enqueue(pq_instance_ptr pq, char t, word_ptr value)
{

	pq_node_ptr item = (pq_node_ptr)malloc(sizeof(struct pq_node));

	if (item == NULL)
		return false;

	if (t == 's')
	{
		item->data->w.s = xstrcp(value->w.s);
	}
	item->previous = NULL;

	if (pq->rear == NULL)
		pq->front = pq->rear = item;
	else
	{
		pq->rear->previous = item;
		pq->rear = item;
	}

	return true;
}

const char *pq_dequeue(pq_instance_ptr pq)
{

	if (is_pq_empty(pq))
	{

		return "";
	}
	pq_node_ptr temp = pq->front;
	const char *ret_val = pq->front->data->w.s;
	if (temp->previous == NULL)
	{
		pq->rear = NULL;
	}
	pq->front = pq->front->previous;
	if (temp->type == 's')
	{
		free(temp->data->w.s);
	}
	free(temp);

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
