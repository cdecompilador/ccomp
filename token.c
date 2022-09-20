#include <stdio.h>

#include "token.h"

void
print_token(const token_t* token) {
    print_token_kind(token->kind);   
}

void
print_token_kind(token_kind kind) {
    switch (kind) {
    case LEFT_PAREN: puts("'('"); break;
    case RIGHT_PAREN: puts("')'"); break;
    case LEFT_BRACE: puts("'{'"); break;
    case RIGHT_BRACE: puts("'}'"); break;
    case LEFT_BRACKET: puts("'['"); break;
    case RIGHT_BRACKET: puts("']'"); break;
    case MINUS: puts("'-'"); break;
    case PLUS: puts("'+'"); break;
    case STAR: puts("'*'"); break;
    case SLASH: puts("'/'"); break;
    case SEMICOLON: puts("';'"); break;

    case IDENTIFIER: puts("identifier"); break;
    case STRING: puts("string"); break;
    case NUMBER: puts("number"); break;

    case IF: puts("if"); break;
    case END_OF_FILE: puts("<eof>"); break;
    case UNKNOWN: puts("<unknown>"); break;
    case UNTERMINATED_STRING: puts("<unterminated string>"); break;
    }
}
