#include <string.h>
#include <assert.h>

#include "position.h"

void
shift_bytepos(bytepos_t* bytepos) {
    bytepos->value++;
}

line_offsets_t
create_line_offsets(const char* data) {
    size_t len = strlen(data);
    size_t* offsets = (size_t*)malloc(sizeof(size_t) * len);
    offsets[0] = 0;
    size_t offsets_idx = 1;
    
    /* Iterate the source and store each offset to a newline */
    for (size_t i = 0; i < len; i++) {
        char val = data[i];
        if (val == '\n') {
             offsets[offsets_idx++] = i + 1;
        }
    }

    return (line_offsets_t) {
        .offsets = offsets,
        .len = len
    };
}

size_t
get_line(const line_offsets_t* line_offsets, bytepos_t pos) {
    /* Bounds check */
    size_t offset = pos.value;
    assert(offset <= line_offsets->len);

    /* Iterate all line offsets and return the the line that may contain that
     * position */
    size_t* curr_offset;
    size_t line = 0;
    for (
        curr_offset = line_offsets->offsets;
        *curr_offset != 0 && curr_offset != line_offsets->offsets;
        curr_offset++
    ) {
        if (*curr_offset == offset) {
            return line;
        } else if (*curr_offset > offset) {
            return line - 1;
        }
    }

    return line;
}
