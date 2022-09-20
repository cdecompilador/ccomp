#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

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
        if (x(*c)) {
            c = next(scanner);
            consumed++;
            if (c == NULL) {
                scanner->unterminated_string = true;
                break;
            }
        } else {
            break;
        }
    }

    if (consumed == 0) {
        return NULL;
    }

    /* Using `consumed` we return a pointer to a heap allocated array with the
     * consumed chars, might be useful for parsing some tokens like strings
     * or identifiers */
    char* chars = (char*)calloc(1, consumed + 2);
    printf("data = \"%s\"\n", scanner->data);
    printf("curr_pos = %d\n", scanner->curr_pos.value);
    printf("consumed = %d\n", consumed);
    printf("ident = \"%s\"\n", scanner->data + scanner->curr_pos.value - consumed);
    if (strncpy(chars,
               scanner->data + scanner->curr_pos.value - consumed,
               consumed - 1
        ) == NULL) {
        fprintf(stderr, "Error: Failed to copy chars\n");
        exit(1);
    }
    printf("chars = \"%s\"\n", chars);

    return chars;
}

static bool
is_slash(char ch) {
    return ch == '/';
}

static bool
is_alpha(char ch) {
    uint8_t n = (uint8_t)ch;
    return n >= 65 && n <= 122;
}

static bool
is_numeric(char ch) {
    uint8_t n = (uint8_t)ch;
    return n >= 48 && n <= 57;
}

static bool
is_alphanumeric(char ch) {
    return is_alpha(ch) || is_numeric(ch);
}

static bool
isnt_quote(char ch) {
    return ch != '"';
}

static bool
is_identbody(char ch) {
    return is_alphanumeric(ch) || ch == '_';
}

static int
lex_string(lexer_t* lexer, token_t* out_token) {
    /* Skip the starting delimiter */
    next(&lexer->it);

    /* Take chars until end delimeter */
    char* str = consume_while(&lexer->it, isnt_quote);
    if (str == NULL) {
        return -1;
    }
    if (lexer->it.unterminated_string) {
        lexer->it.unterminated_string = false;
        *out_token = (token_t) {
            .kind = UNTERMINATED_STRING,
            .value = str
        };
    } else {
        *out_token = (token_t) {
            .kind  = STRING,
            .value = str,
        };
    }

    return 0;
}

static bool
lex_keyword(const char* str, token_t* out_token) {
    if (strcmp(str, "if") == 0) {
        *out_token = (token_t) {
            .kind = IF
        };
        return true;
    } else if (strcmp(str, "else") == 0) {
        *out_token = (token_t) {
            .kind = ELSE
        };
        return true;
    }

    return false;
}

static int
lex_identifier(lexer_t* lexer, char c, token_t* out_token) {
    char* str = consume_while(&lexer->it, is_identbody);
    if (str == NULL) {
        str = calloc(sizeof(char), 2);
        str[0] = c;
    } else {
        /* Build the identifier */
        size_t len = strlen(str);
        for (int i = len; i >= 0; i--) {
            str[i + 1] = str[i];
        }
        str[0] = c;
    }

    /* Check if its a keyword */
    if (lex_keyword(str, out_token)) {
        return 0;
    }

    *out_token = (token_t) {
        .kind  = IDENTIFIER,
        .value = str,
    };

    return 0;
}

static int
lex_number(lexer_t* lexer, char c, token_t* out_token) {
    uint64_t num = (uint64_t)(c - '0');
    
    while (1) {
        c = *peek(&lexer->it);
        if (!consume_if(&lexer->it, is_numeric)) {
            break;
        }
        num = 10 * num + (uint64_t)(c - '0');
    }

    *out_token = (token_t) {
        .kind  = NUMBER,
        .value = (void*)num,
    };

    return 0;
}

int
match_token(lexer_t* lexer, char ch, token_t* out_token) {
    /* Tokens with range valid start character */
    if (is_alpha(ch) || ch == '_') {
        if (lex_identifier(lexer, ch, out_token) != 0) {
            fprintf(stderr, "Error: Failed to lex identifier\n");
            exit(1);
        }
        return 0;
    } else if (is_numeric(ch)) {
        if (lex_number(lexer, ch, out_token) != 0) {
            fprintf(stderr, "Error: Failed to lex identifier\n");
            exit(1);
        }
        return 0;
    }
    
    /* Tokens with fixed start character */
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
    case ';':
        *out_token = (token_t) {
            .kind = SEMICOLON
        };
        break;
    case '(':
        *out_token = (token_t) {
            .kind = LEFT_PAREN
        };
        break;
    case ')':
        *out_token = (token_t) {
            .kind = RIGHT_PAREN
        };
        break;
    case '{':
        *out_token = (token_t) {
            .kind = RIGHT_BRACE
        };
        break; 
    case '}':
        *out_token = (token_t) {
            .kind = RIGHT_BRACE
        };
        break;
    case '"':
        if (lex_string(lexer, out_token) != 0) {
            fprintf(stderr, "Error: Couldn't parse string literal\n");
            exit(1);
        }
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
