#include "tokeniser.h"

typedef struct ParseNode {
    TokenType type;
    int is_alloc;
    char *value;
} ParseNode;

typedef struct ParseTuple {
    int err;
    size_t length;
    ParseNode *nodes;
} ParseTuple;

ParseTuple parser(ParseNode *list, TokenList tokens);
