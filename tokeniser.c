#include <corecrt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tokeniser.h"
#include "dynam.h"

void initTokenList(TokenList *self) {
    self->count = 0;
    self->len = 0;
    self->append = appendToken;
}

void appendToken(TokenList *self, Token t) {
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
    Token t = {
        .type = None,
        .value = "none",
        .is_alloc = 0,
    };

    if (strlen(str) == 0) {
        return t;
    }

    char *endptr;
    long num = strtol(str, &endptr, 10);
    if (*endptr == '\0') {
        t.type = Number;
        t.value = str;
        t.is_alloc = 0;
        return t;
    }
    
    size_t length = strlen(str);
    t.value = malloc(length + 1);
    strncpy(t.value, str, length + 1);
    t.value[length+1] = '\0';

    if (strcmp(str, ".text") == 0) {
        t.type = TextSect;
        t.is_alloc = 1;
        return t;
    } else if (strcmp(str, "_main():") == 0) {
        t.type = StartFunc;
        t.is_alloc = 1;
        return t;
    } else if (strcmp(str, "mov") == 0) {
        t.type = Move;
        t.is_alloc = 1;
        return t;
    } else if (strcmp(str, "rax") == 0) {
        t.type = Register;
        t.is_alloc = 1;
        return t;
    } else if (strcmp(str, "rdi") == 0) {
        t.type = Register;
        t.is_alloc = 1;
        return t;
    } else if (strcmp(str, "syscall") == 0) {
        t.type = SysCall;
        t.is_alloc = 1;
        return t;
    }

    t.value = "none";
    return t;
}

TokenList tokenise(char *str) {
    TokenList t_list;
    t_list.init = initTokenList;
    t_list.init(&t_list);

    Dynam buffer;
    buffer.init = initDynam;
    buffer.init(&buffer);

    for (size_t i = 0; i < strlen(str); i++) {
        switch (str[i]) {
            case ',':;
                char *first_half = malloc(buffer.len - (sizeof(str[i]) * 2));
                for (size_t i = 0; i <= buffer.count-1; i++) {
                    first_half[i] = buffer.str[i];
                    first_half[i+1] = '\0';
                }

                Token first_token = keyword_checker(first_half);
                Token sec_token = {
                    .type = Comma,
                    .value = ",",
                    .is_alloc = 0,
                };
                if (first_token.type != 0) {
                    t_list.append(&t_list, first_token);
                }

                t_list.append(&t_list, sec_token);
                buffer.count = 0;
                buffer.len = 0;
                buffer.str[0] = '\0';
                free(first_half);
                break;
            case '\n':;
            case ' ':;
                Token new_token = keyword_checker(buffer.str);
                if (new_token.type != 0) {
                    t_list.append(&t_list, new_token);
                }

                buffer.count = 0;
                buffer.len = 0;
                buffer.str[0] = '\0';
                break;
            default:;
                buffer.append(&buffer, str[i]);
        }
    }

    for (size_t i = 0; i < t_list.count; i++) {
        switch (t_list.arr[i].type) {
            case TextSect:;
                printf("type: TextSect, value: %s\n", t_list.arr[i].value);
                break;
            case StartFunc:;
                printf("type: StartFunc, value: %s\n", t_list.arr[i].value);
                break;
            case DataSect:;
                printf("type: DataSect, value: %s\n", t_list.arr[i].value);
                break;
            case Move:;
                printf("type: Move, value: %s\n", t_list.arr[i].value);
                break;
            case None:;
                printf("type: None, value: %s\n", t_list.arr[i].value);
                break;
            case SysCall:;
                printf("type: SysCall, value: %s\n", t_list.arr[i].value);
                break;
            case Register:;
                printf("type: RAX, value: %s\n", t_list.arr[i].value);
                break;
            case Comma:
                printf("type: Comma, value: %s\n", t_list.arr[i].value);
                break;
            case Number:
                printf("type: Number, value: %s\n", t_list.arr[i].value);
                break;
        }
    }

    free(buffer.str);
    return t_list;
}
