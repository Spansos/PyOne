#include "token.h"
#include "slice.h"

token_t lex_single_token(const char * code, size_t start, slice_t * slice);
token_array_t lex(const char * code);