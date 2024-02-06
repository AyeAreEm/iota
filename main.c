#include <corecrt.h>
#include <stdio.h>
#include <stdlib.h>
#include "tokeniser.h"
#include "dynam.h"

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
    
    tokenise(words.str);

    free(words.str);
    return 0;
}
