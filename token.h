#ifndef TOKEN_H
#define TOKEN_H

#include "position.h"

typedef enum {
    /* Single character tokens */
    LEFT_PAREN, RIGHT_PAREN,
    LEFT_BRACE, RIGHT_BRACE,
    LEFT_BRACKET, RIGHT_BRACKET,
    MINUS, PLUS, STAR, SLASH,
    SEMICOLON,

    /* Literals */
    IDENTIFIER,
    STRING,
    NUMBER,

    /* Keywords */
    IF,

    /* Other */
    END_OF_FILE,
    UNKNOWN,
    UNTERMINATED_STRING
} token_kind;

typedef struct {
    token_kind kind;
    void* value;
} token_t;

void
print_token(const token_t* token);

void
print_token_kind(token_kind kind);

#endif
