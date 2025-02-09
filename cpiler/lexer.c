#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "lexer.h"


token_t lex_single_token(const char * code, size_t start, slice_t * slice) {
    // in most (one character) cases, this is good, so doing here and overwritting in other cases means it doesnt have to be in every single case
    if (slice)
        *slice = SLICE_TO(code[start]);
    
    switch (code[start]) {
        case '&':
            return (token_t){.start=start, .type=AND};

        case '|':
            return (token_t){.start=start, .type=AND};

        case '+':
            return (token_t){.start=start, .type=PLUS};

        case '*':
            return (token_t){.start=start, .type=MULT};

        case ',':
            return (token_t){.start=start, .type=COMMA};

        case '^':
            return (token_t){.start=start, .type=POWER};

        case '%':
            return (token_t){.start=start, .type=MODULUS};

        case ';':
            return (token_t){.start=start, .type=SEMICOLON};

        case '(':
            return (token_t){.start=start, .type=BRACKET_OPEN};

        case ')':
            return (token_t){.start=start, .type=BRACKET_CLOSE};

        case '[':
            return (token_t){.start=start, .type=SQR_BRACKET_OPEN};

        case ']':
            return (token_t){.start=start, .type=SQR_BRACKET_CLOSE};

        case '{':
            return (token_t){.start=start, .type=CURLY_BRACKET_OPEN};

        case '}':
            return (token_t){.start=start, .type=CURLY_BRACKET_CLOSE};

        case '-':
            if (code[start+1] == '>') {
                if (slice)
                    *slice = SLICE_ARRAY(code+start, 2);
                return (token_t){.start=start, .type=ARROW};
            } else {
                return (token_t){.start=start, .type=MINUS};
            }

        case '!':
            if (code[start+1] == '=') {
                if (slice)
                    *slice = SLICE_ARRAY(code+start, 2);
                return (token_t){.start=start, .type=NOT_EQUALS};
            } else {
                return (token_t){.start=start, .type=NOT};
            }

        case '=':
            if (code[start+1] == '=') {
                if (slice)
                    *slice = SLICE_ARRAY(code+start, 2);
                return (token_t){.start=start, .type=EQUALS};
            } else {
                return (token_t){.start=start, .type=ASSIGN};
            }

        case '>':
            if (code[start+1] == '=') {
                if (slice)
                    *slice = SLICE_ARRAY(code+start, 2);
                return (token_t){.start=start, .type=GREATER_EQUAL};
            } else {
                return (token_t){.start=start, .type=GREATER};
            }

        case '<':
            if (code[start+1] == '=') {
                if (slice)
                    *slice = SLICE_ARRAY(code+start, 2);
                return (token_t){.start=start, .type=LESSER_EQUAL};
            } else {
                return (token_t){.start=start, .type=LESSER};
            }

        case '/':
            if (code[start+1] == '/') {
                // skip comments (kinda horrendous code) B^)
                slice->pointer = code+start;
                for (; code[start] != '\n'; start++);
                slice->length = (code+start)-(char*)slice->pointer;
                return (token_t){.start=(char*)slice->pointer-code, .type=COMMENT};
            } else {
                return (token_t){.start=start, .type=DIVISION};
            }

        // TODO: escape " with \ (\")
        case '"':
            // cur++ is to skip the "
            slice->pointer = code+start;
            start++;
            for (; code[start] != '"'; start++);
            start++;
            slice->length = (code+start)-(char*)slice->pointer;
            return (token_t){.start=(char*)slice->pointer-code, .type=STRING};

        case ' ': case '\t': case '\n':
            return (token_t){.start=start, .type=WHITESPACE};

        default:
            // numbers
            // TODO: floats
            if (isdigit(code[start])) {
                size_t id_start = start;
                while (isdigit(code[start])) start++;
                size_t id_length = start - id_start;
                *slice = SLICE_ARRAY(code+id_start, id_length);
                return (token_t){.start=id_start, .type=INT};
            }
            // identifiers
            // start with either alpha or _
            else if (isalpha(code[start]) || code[start] == '_') {
                size_t id_start = start;
                while (isalnum(code[start]) || code[start] == '_') start++;
                size_t id_length = start - id_start;

                *slice = SLICE_ARRAY(code+id_start, id_length);

                // keywords
                if (id_length == 3 && !memcmp(&code[id_start], "mut", 3))
                    return (token_t){.start=id_start, .type=MUT};
                else if (id_length == 3 && !memcmp(&code[id_start], "ref", 3))
                    return (token_t){.start=id_start, .type=REF};
                else if (id_length == 5 && !memcmp(&code[id_start], "while", 5))
                    return (token_t){.start=id_start, .type=WHILE};
                else if (id_length == 3 && !memcmp(&code[id_start], "for", 3))
                    return (token_t){.start=id_start, .type=FOR};
                else if (id_length == 2 && !memcmp(&code[id_start], "if", 2))
                    return (token_t){.start=id_start, .type=IF};
                else if (id_length == 4 && !memcmp(&code[id_start], "else", 4))
                    return (token_t){.start=id_start, .type=ELSE};
                else if (id_length == 8 && !memcmp(&code[id_start], "function", 8))
                    return (token_t){.start=id_start, .type=FUNCTION};
                else if (id_length == 6 && !memcmp(&code[id_start], "return", 6))
                    return (token_t){.start=id_start, .type=RETURN};
                else if (id_length == 5 && !memcmp(&code[id_start], "break", 5))
                    return (token_t){.start=id_start, .type=BREAK};
                else if (id_length == 4 && !memcmp(&code[id_start], "skip", 4))
                    return (token_t){.start=id_start, .type=SKIP};
                // identifrier
                else
                    return (token_t){.start=id_start, .type=IDENTIFIER};
            } else {
                fprintf(stderr, "No known token @ %li", start);
                exit(-1);
            }
    }
}


token_array_t lex(const char * code) {
    token_array_t tokens = create_token_array();

    int cur = 0;
    while (code[cur]) {
        slice_t slice;
        token_t token = lex_single_token(code, cur, &slice);
        push_token(&tokens, token);
        cur += slice.length;
    }

    drop_comment_whitespace(&tokens);

    return tokens;
}