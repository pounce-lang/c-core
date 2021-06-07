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

pq_node_ptr top = NULL;
pq_node_ptr front = NULL;
pq_node_ptr rear = NULL;

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

bool is_pq_empty()
{

	return (front == NULL || rear == NULL);
}

bool pq_requeue(char t, const char *value)
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

	if (front == NULL)
	{
		front = rear = item;
		item->previous = NULL;
	}
	else
	{
		item->previous = front;
		front = item;
	}

	return true;
}

bool pq_enqueue(char t, const char *value)
{

	pq_node_ptr item = (pq_node_ptr)malloc(sizeof(struct pq_node));

	if (item == NULL)
		return false;

	if (t == 's')
	{
		item->data.s = xstrcp(value);
	}
	item->previous = NULL;

	if (rear == NULL)
		front = rear = item;
	else
	{
		rear->previous = item;
		rear = item;
	}

	return true;
}

const char *pq_dequeue()
{

	if (is_pq_empty())
	{

		return "";
	}
	pq_node_ptr temp = front;
	const char *ret_val = front->data.s;
	if (temp->previous == NULL)
	{
		rear = NULL;
	}
	front = front->previous;
	if (temp->type == 's')
	{
		free(temp->data.s);
	}
	free(temp);

	return ret_val;
}

bool pq_clear()
{

	if (is_pq_empty())
	{

		return false;
	}

	pq_node_ptr current = front;
	pq_node_ptr previous = NULL;

	while (current != NULL)
	{

		previous = current->previous;
		free(current);
		current = previous;
	}

	front = NULL;
	rear = NULL;

	return is_pq_empty();
}
