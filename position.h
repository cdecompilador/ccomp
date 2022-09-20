#ifndef POSITION_H
#define POSITION_H

#include <stdlib.h>
#include "defines.h"

/* The position of a character on the file, designed to handle multibyte 
 * characters */
// NOTE: For the moment unused its multibyte capabilities, if I don't feel like
// adding support for them I may remove it
typedef struct {
    size_t value;
} bytepos_t;

/* Like a value++, on the future it may take in a character and increment the
 * character count based on the bytes that character occupies */
void
shift_bytepos(bytepos_t* bytepos);

/* Span on a file of something, not inclusive. May be used for tokens, expr, 
 * stmt, etc .. */
typedef struct {
    bytepos_t start;
    bytepos_t end;
} span_t;

/* Create a empty span */
inline span_t
empty_span() {
    return (span_t) {
        .start = (bytepos_t) { .value = 0 },
        .end   = (bytepos_t) { .value = 0 }
    };
}

/* Join the span of two spans into a new one that equal to both */
inline span_t
union_span(const span_t* a, const span_t* b) {
    return (span_t) {
        .start = { min(a->start.value, b->start.value) },
        .end   = { max(a->end.value,   b->end.value) }
    };
}

/* A spanned element, it may be a token, expr, stmt, .... The access of
 * `value` is context dependent and bug prone but no generics in C :P */
typedef struct {
    void* value;
    span_t span;
} with_span_t;

/* Utility to keep track of the line a a element, In the future used by the
 * reporting system to resolve the line of a element just giving the span */
typedef struct {
    /* Offsets of each line number, the index 1 corresponds to line 1, so index
     * 0 just contains a 0 as correctness */
    size_t* offsets;

    /* Number of bytes of the file it represents, used to check that the 
     * line one is trying to extract from a bytepos_t is bounds checked */
    size_t len;
} line_offsets_t;

/* Create it from a source */
line_offsets_t
create_line_offsets(const char* data);

/* Get the line of a certain bytepos on the file */
size_t
get_line(const line_offsets_t* line_offsets, bytepos_t pos);

#endif
