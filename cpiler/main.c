#include <stdio.h>
#include <stdlib.h>
#include "lexer.h"
#include "parser.h"
#include "generator.h"

char * read_stdin() {
    // allocate char array of file size
    char * code = malloc(1);
    size_t capacity = 1;
    size_t size = 0;

    // fill char array
    int c = 0;
    while ((c = fgetc(stdin)) != EOF) {
        code[size] = (char)c;
        size++;
        if (size==capacity) {
            capacity *= 2;
            code = realloc(code, capacity);
        }
    }
    code[size] = '\0';

    // check for errors
    if (ferror(stdin)) {
        fputs("boohoo, some error did come across me", stderr);
        exit(EXIT_FAILURE);
    }

    return code;
}

int main() {
    char * code = read_stdin("../test.p2");

    token_array_t tokens = lex(code);
    
    parse_error_t error;
    program_t * program = parse_program(&tokens, &error);

    if (program) {
        printf("\n%s", generate_code(program, code));
        free_program(program);
    }
    else
        fprintf(stderr, "Error %s @ char %lu\n", error_type_string(error), error.token.start);

    free(code);
    free(tokens.tokens);
}