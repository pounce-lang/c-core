#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct pq_node
{
	char type; // (w)ord, (v)alue, (l)ist, (")wrapped_string, (')wrapped_string, (`)wrapped_string
	union
	{
		char *s;
		long i;
		double f;
		struct pq_node *list;
	} data;
	struct pq_node *previous;

} * pq_node_ptr;

typedef struct pq_instance
{
	pq_node_ptr front; // = NULL;
	pq_node_ptr rear; // = NULL;
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

pq_instance_ptr pq_init() {
	pq_instance_ptr new_pq = (pq_instance_ptr)malloc(sizeof(struct pq_instance));

	if (new_pq == NULL)
	{
		return NULL;
	}
	new_pq->front = NULL;
	new_pq->rear = NULL;
	return new_pq;
};

bool pq_delete(pq_instance_ptr old_pq) {
	return false;
};

bool is_pq_empty(pq_instance_ptr pq)
{
	return (pq->front == NULL || pq->rear == NULL);
}

bool pq_requeue(pq_instance_ptr pq, char t, const char *value)
{

	pq_node_ptr item = (pq_node_ptr)malloc(sizeof(struct pq_node));

	if (item == NULL)
	{
		return false;
	}

	if (t == 's')
	{
		item->data.s = xstrcp(value);
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

bool pq_enqueue(pq_instance_ptr pq, char t, const char *value)
{

	pq_node_ptr item = (pq_node_ptr)malloc(sizeof(struct pq_node));

	if (item == NULL)
		return false;

	if (t == 's')
	{
		item->data.s = xstrcp(value);
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
	const char *ret_val = pq->front->data.s;
	if (temp->previous == NULL)
	{
		pq->rear = NULL;
	}
	pq->front = pq->front->previous;
	if (temp->type == 's')
	{
		free(temp->data.s);
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
