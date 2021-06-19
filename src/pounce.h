#ifndef _POUNCE_H_
#define _POUNCE_H_

#ifdef __cplusplus
extern "C" {
#endif

// MACRO to test for a string type node
#define type_s(n) (n->type == 's' || n->type == '\'' || n->type == '"' || n->type == '`')

typedef struct word_struct
{
	union
	{
		char *s;
		long i;
		double d;
		struct pq_node *list;
		struct pq_node *(*fun)(ps_instance_ptr, pq_instance_ptr); 
	} w;
} * word_ptr;

typedef struct pq_node
{
	char type; // (s)tring, (i)nt (long), (l)ist, (f)unction (d)ouble presision float
	word_ptr data;
	struct pq_node *previous;

} * pq_node_ptr;

typedef struct ps_instance
{
	pq_node_ptr top; // = NULL;
} * ps_instance_ptr;

void pq_display_word(pq_node_ptr node);


#ifdef __cplusplus
}
#endif

#endif