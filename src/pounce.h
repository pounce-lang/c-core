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
		struct pdq_node *list;
		struct pdq_node *(*fun)(stack_instance_ptr, pdq_instance_ptr, dictionary_ptr); 
	};
} * word_ptr;

typedef struct pdq_node
{
	char type; // (b)oolean, (s)tring, (i)nt (long), (l)ist, (f)unction (d)ouble presision float
	struct word_struct word;
	struct pdq_node *previous;

} * pdq_node_ptr;

typedef struct stack_instance
{
	pdq_node_ptr top; // = NULL;
} * stack_instance_ptr;

void pdq_display_word(pdq_node_ptr node);


typedef struct pdq_instance
{
	pdq_node_ptr front; // = NULL;
	pdq_node_ptr rear;  // = NULL;
} * pdq_instance_ptr;

void pdq_free_node(pdq_node_ptr node);

#ifdef __cplusplus
}
#endif

#endif