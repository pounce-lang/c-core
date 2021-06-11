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

void pq_display_word(pq_node_ptr node);

typedef union {
    char * phrase;
    pq_node_ptr (*fun)(pq_node_ptr);
  } defined;
// } definition ;

#ifdef __cplusplus
}
#endif

#endif