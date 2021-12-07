#ifndef _POUNCE_H_
#define _POUNCE_H_

#ifdef __cplusplus
extern "C" {
#endif

#define BOOL_T 'b'
#define STRING_T 's'
#define INT_T 'i'
#define REAL_T 'd'
#define LIST_T 'l'
#define IFUNC_T 'f'

// MACRO to test for a string type node
#define type_s(n) (n->type == STRING_T || n->type == '\'' || n->type == '"' || n->type == '`')

typedef struct word_struct
{
	union
	{
		bool b;
		char *s;
		long i;
		double d;
		struct pounce_node *list;
		struct pounce_node *(*fun)(stack_instance_ptr, pounce_instance_ptr, dictionary_ptr); 
	} w;
} * word_ptr;

typedef struct pounce_node
{
	char type; // (b)oolean, (s)tring, (i)nt (long), (l)ist, (f)unction (d)ouble presision float
	word_ptr data;
	struct pounce_node *previous;

} * pounce_node_ptr;

typedef struct stack_instance
{
	pounce_node_ptr top; // = NULL;
} * stack_instance_ptr;

void pounce_display_word(pounce_node_ptr node);


#ifdef __cplusplus
}
#endif

#endif