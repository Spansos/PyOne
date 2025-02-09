#include <stdio.h>
#include <stdlib.h>
#include "lexer.h"
#include "parser.h"
#include "generator.h"

char * read_file(const char * filename) {
    // open file
    FILE * file = fopen(filename, "r");
    if (!file) {
        perror("hur hur, no file not done, not");
        exit(-1);
    }

    // get size of file
    size_t size;
    for (size=0; fgetc(file) != EOF; size++) ;
    rewind(file);

    // allocate char array of file size
    char * code = malloc(size+1);
    code[size] = 0;

    // fill char array
    int c = 0;
    for (size=0; (c = fgetc(file)) != EOF; size++)
        code[size] = (char)c;

    // check for errors
    if (ferror(file)) {
        fputs("boohoo, some error did come across me", stderr);
        exit(-1);
    }

    // close file
    fclose(file);

    return code;
}

int main() {
    char * code = read_file("../test.p2");

    token_array_t tokens = lex(code);
    
    parse_error_t error;
    program_t * program = parse_program(&tokens, &error);

    printf("Program: %p\n", (void*)program);

    if (program) {
        print_program(program);
        printf("\n%s", generate_code(program, code));
        free_program(program);
    }
    else
        printf("Error %s @ char %lu", error_type_string(error), error.token.start);
    printf("\n");

    free(code);
    free(tokens.tokens);
}