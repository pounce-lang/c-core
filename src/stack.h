#ifndef _P_STACK_H_
#define _P_STACK_H_

#ifdef __cplusplus
extern "C" {
#endif



pdq_node_ptr stack_init_node();
bool stack_delete(stack_instance_ptr old_pq);
pdq_node_ptr stack_pop(stack_instance_ptr ps);
bool stack_clear(stack_instance_ptr ps);


#ifdef __cplusplus
}
#endif

#endif