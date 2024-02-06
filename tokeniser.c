#include <corecrt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tokeniser.h"
#include "dynam.h"

void initTokenList(tokenList *self) {
    self->count = 0;
    self->len = 0;
    self->append = appendToken;
}

void appendToken(tokenList *self, Token t) {
    if (self->len == 0) {
        self->len = 5 * sizeof(t);
        self->arr = malloc(self->len);
    }

    if (self->count == self->len / sizeof(t) - 1) {
        self->len += 5 * sizeof(t);
        self->arr = realloc(self->arr, self->len);
    }

    self->arr[self->count] = t;
    self->count++;
}

Token keyword_checker(char *str) {
    if (strcmp(str, ".text") == 0) {
        return TextSect;
    } else if (strcmp(str, "_main():") == 0) {
        return StartFunc;
    } else if (strcmp(str, "mov") == 0) {
        return Move;
    } else if (strcmp(str, "rax") == 0) {
        return RAX;
    } else if (strcmp(str, "rdi") == 0) {
        return RDI;
    }

    return None;
}

void tokenise(char *str) {
    tokenList t_list;
    t_list.init = initTokenList;
    t_list.init(&t_list);
    
    Dynam buffer;
    buffer.init = initDynam;
    buffer.init(&buffer);

    for (size_t i = 0; i < strlen(str); i++) {
        switch (str[i]) {
            case '\n':;
            case ' ':;
                printf("%s", buffer.str);
                Token new_token = keyword_checker(buffer.str);
                if (new_token != 0) {
                    t_list.append(&t_list, new_token);
                    buffer.count = 0;
                    buffer.len = 0;
                    buffer.str[0] = '\n';
                }
                break;
            default:
                buffer.append(&buffer, str[i]);
        }
    }

    printf("count: %zd\n", t_list.count);
    printf("len: %zd\n", t_list.len);

    for (size_t i = 0; i <= t_list.count; i++) {
        switch (t_list.arr[i]) {
            case TextSect:;
                printf("text section\n");
                break;
            case StartFunc:;
                printf("start function\n");
                break;
            case DataSect:;
                printf("data sect\n");
                break;
            case Move:;
                printf("move\n");
                break;
            case None:;
                printf("none\n");
                break;
            case SysCall:;
                printf("syscall\n");
                break;
            case RAX:;
                printf("rax\n");
                break;
            case RDI:;
                printf("rdi\n");
                break;
        }
    }

    free(buffer.str);
    free(t_list.arr);
}
