#ifndef TOKENIZER_H
#define TOKENIZER_H

#include "defines.h"
#include "position.h"
#include "token.h"

/* Iterator utility over a string */
typedef struct {
    /* The current position in characters on the `data` */
    bytepos_t curr_pos;

    /* The underlying string */
    char* data;

    /* The length in characters of the underlying string */
    size_t len;

    /* Flag to know if a unterminated string was found */
    bool unterminated_string;
} scanner_t;

/* Create a new scanner from a string */
scanner_t
create_scanner(char* data);

/* Get the current character and advance the iterator, NULL represents no more
 * characters */
char*
next(scanner_t* scanner);

/* Just get the current character, NULL represents no more characters */
char*
peek(scanner_t* scanner);

/* Consume the current character if a condition `x` applies over it */
bool
consume_if(scanner_t* scanner, bool (*x)(char ch));

/* Consume the current character if a condition `x` applies over the next 
 * character to the current one */
bool
consume_if_next(scanner_t* scanner, bool (*x)(char ch));

/* Consume chars while the condition `x` is fulfilled, might be useful for 
 * parsing some tokens STRINGs or IDENTIFIERs */
char*
consume_while(scanner_t* scanner, bool (*x)(char ch));

/* Lexer, its purpose is produce tokens */
typedef struct {
    /* Iterator over a string, used instead of directly a `char*` */
    scanner_t it;
} lexer_t;

/* Given a lexer and a starting character tries to extract a token, it may
 * eat more character of the underlying scanner than the provided one */
int
match_token(lexer_t* lexer, char ch, token_t* out_token);

#endif
