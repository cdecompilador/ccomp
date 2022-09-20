#ifndef ARRAY_STACK_H
#define ARRAY_STACK_H

#include <stddef.h>
#include <inttypes.h>

/* A fake FIFO stack array. It does not shrink, it just reduces it's count
 * when `as_pop` is called */
typedef struct {
    /* Size of each allocated element */
    size_t size;

    /* Count of existing elements of the array stack */
    uint32_t count;

    /* Allocated capacity for the array stack. Always cap >= count */
    uint32_t cap;

    /* The data */
    void* data;
} array_stack_t;

/* Create an `array_stack_t` on the heap and return a pointer to it */
array_stack_t*
as_create(size_t size);

/* Destroy an `array_stack_t`, removing it from the heap */
void 
as_destroy(array_stack_t* as);

/* Pop the first element from the stack */
void* 
as_pop(array_stack_t* as);

/* Push data on the stack */
void 
as_push(array_stack_t* as, void* data);

#endif
