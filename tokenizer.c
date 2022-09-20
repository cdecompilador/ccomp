#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "tokenizer.h"

scanner_t
create_scanner(char* data) {
    return (scanner_t) {
        .curr_pos = (bytepos_t) { .value = 0 },
        .data = data,
        .len = strlen(data),
    };
}

char*
next(scanner_t* scanner) {
    char* next = NULL;
    if (scanner->curr_pos.value < scanner->len) {
        next = (scanner->data + scanner->curr_pos.value);
        shift_bytepos(&scanner->curr_pos);
    }

    return next;
}

char*
peek(scanner_t* scanner) {
    char* next = NULL;
    if (scanner->curr_pos.value < scanner->len) {
        next = (scanner->data + scanner->curr_pos.value);
    }

    return next;
}

bool
consume_if(scanner_t* scanner, bool (*x)(char ch)) {
    char* c = peek(scanner);
    if (c != NULL) {
        if (x(*c)) {
            c = next(scanner);
            if (c == NULL) {
                fprintf(stderr, "Error: Could not retrieve next token\n");
                exit(1);
            }

            return true;
        } else {
            return false;
        }
    } else {
        return false;
    }
}

bool
consume_if_next(scanner_t* scanner, bool (*x)(char ch)) {
    /* Advance the current character on a temporal scanner */
    scanner_t scanner_copy = *scanner;
    if (next(&scanner_copy) == NULL) {
        return false;
    }
    
    /* Peek the next character and try to apply the condition */
    char* c = peek(&scanner_copy);
    if (c != NULL) {
        if (x(*c)) {
            /* The temporal scanner is now the real scanner */
            *scanner = scanner_copy;
            if (c == NULL) {
                fprintf(stderr, "Error: Could not retrieve next token\n");
                exit(1);
            }

            return true;
        } else {
            return false;
        }
    } else {
        return false;
    }
}

char*
consume_while(scanner_t* scanner, bool (*x)(char ch)) {
    /* Call next on the `scanner` while the condition provided by the function
     * pointer `x` is fulfilled */
    size_t consumed = 0;
    char* c = peek(scanner);
    while (c != NULL) {
        consumed++;

        if (x(*c)) {
            c = next(scanner);
            if (c == NULL) {
                fprintf(stderr, "Error: Could not retrieve next token\n");
                exit(1);
            }
        } else {
            break;
        }
    }

    /* Using `consumed` we return a pointer to a heap allocated array with the
     * consumed chars, might be useful for parsing some tokens like strings
     * or identifiers */
    char* chars = (char*)malloc(consumed);
    if (memcpy(chars,
               scanner->data + scanner->curr_pos.value - consumed, 
               consumed) == NULL) {
        fprintf(stderr, "Error: Failed to copy chars\n");
        exit(1);
    }

    return chars;
}

static bool
is_slash(char ch) {
    return ch == '/';
}

int
match_token(lexer_t* lexer, char ch, token_t* out_token) {
    switch (ch) {
    case ' ':
    case '\n':
    case '\t':
    case '\r':
        return -1;
    case '+':
        *out_token = (token_t) {
            .kind = PLUS,
        };
        break;
    case '-':
        *out_token = (token_t) {
            .kind = MINUS
        };
        break;
    case '*':
        *out_token = (token_t) {
            .kind = STAR
        };
        break;
    case '/':
        *out_token = (token_t) {
            .kind = SLASH
        };
        break;
    default:
        char* allocated_char = malloc(1);
        *allocated_char = ch;
        *out_token = (token_t) {
            .kind = UNKNOWN,
            .value = allocated_char
        };
    }

    return 0;
}
