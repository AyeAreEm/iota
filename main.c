#include <corecrt.h>
#include <stdio.h>
#include <stdlib.h>
#include "dynam.h"
#include "parser.h"

int main() {
    Dynam words;
    words.init = initDynam;
    words.init(&words);

    FILE *file;
    errno_t err = fopen_s(&file, "hello.io", "r");
    if (err != 0) {
        printf("unable to open file");
        return 1;
    }

    char c;
    while ((c = fgetc(file)) != EOF) {
        words.append(&words, c);
    }
    fclose(file);
    
    TokenList tokens = tokenise(words.str);
    free(words.str);

    ParseNode *nodes;
    ParseTuple parsed_values = parser(nodes, tokens);
    if (parsed_values.err == 1) {
        printf("there was an error");
        return 1;
    }

    for (size_t i = 0; i < parsed_values.length; i++) {
        printf("%s\n", parsed_values.nodes[i].value);
        if (parsed_values.nodes[i].is_alloc == 1) {
            free(parsed_values.nodes[i].value);
        }
    }
    free(nodes);

    for (size_t i = 0; i < tokens.count; i++) {
        if (tokens.arr[i].is_alloc == 1) {
            free(tokens.arr[i].value);
        }
    }
    free(tokens.arr);

    return 0;
}
