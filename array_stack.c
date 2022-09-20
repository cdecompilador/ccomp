#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "array_stack.h"

array_stack_t*
as_create(size_t size)
{
    array_stack_t* as = (array_stack_t*)malloc(sizeof(array_stack_t));
    as->size = size;
    as->data = malloc(size);
    as->cap = 1;
    as->count = 0;

    return as;
}

void 
as_destroy(array_stack_t* as)
{
    free(as->data);
    free(as);
}

void* 
as_pop(array_stack_t* as)
{
    uint8_t* ret = (uint8_t*)as->data;
    ret += (--as->count) * as->size;

    return ret;
}

void 
as_push(array_stack_t* as, void* data)
{
    uint8_t* ptr;

    /* Check if its needed to extend the allocated data capacity */
    if (as->count == as->cap) {
        void* data = realloc(as->data, as->cap * 2 * as->size);
        if (data == NULL) {
            printf("Error: Failed to reallocate memory %s:%d\n", 
                    __FILE__, __LINE__);
            exit(1);
        } else {
            as->data = data;
            as->cap *= 2;
        }
    }

    /* Copy the data to the end */
    ptr = (uint8_t*)as->data;
    ptr += (as->count++) * as->size;
    memcpy(ptr, data, as->size);
}
