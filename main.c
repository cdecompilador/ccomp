#include <stdio.h>
#include <assert.h>

#include "tokenizer.h"

int
main(void) {
    lexer_t lexer = {
        // .it = create_scanner("+ if else 10 ola holita_ \"string")
        .it = create_scanner("+ if else")
    };
    token_t curr_token;

    while (true) {
        /* Retrieve character */
        char* ch = next(&lexer.it);
        if (ch == NULL) 
            break;

        /* Try extract a token using the retrieved character as pivot */
        if (match_token(&lexer, *ch, &curr_token) == 0) {
            print_token(&curr_token);
        }
    }

    /* testing
    scanner_t scanner = create_scanner("+-*");
    char c;

    c = *peek(&scanner);
    putchar(c);
    c = *peek(&scanner);
    putchar(c);

    c = *next(&scanner);
    putchar(c);
    c = *next(&scanner);
    putchar(c);
    c = *next(&scanner);
    putchar(c);

    c = *peek(&scanner);
    putchar(c);
    c = *peek(&scanner);
    putchar(c);

    c = *next(&scanner);
    putchar(c);

    assert(peek(&scanner) == NULL);
    assert(next(&scanner) == NULL);
    */

    return 0;
}
