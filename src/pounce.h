#ifndef _POUNCE_H_
#define _POUNCE_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef struct word_struct
{
	union
	{
		char *s;
		long i;
		double d;
		struct pq_node *list;
        //struct pq_node * (*fun)(struct ps_instance *);
		struct pq_node *(*fun)(ps_instance_ptr); 
	} w;
} * word_ptr;

typedef struct pq_node
{
	char type; // (s)tring, (i)nt, (l)ist, (f)unction (d)ouble presision float
	word_ptr data;
	struct pq_node *previous;

} * pq_node_ptr;

typedef struct ps_instance
{
	pq_node_ptr top; // = NULL;
} * ps_instance_ptr;

void pq_display_word(pq_node_ptr node);

// typedef union {
//     char * phrase;
//     pq_node_ptr (*fun)(ps_instance_ptr);
//   } defined;
// // } definition ;

#ifdef __cplusplus
}
#endif

#endif