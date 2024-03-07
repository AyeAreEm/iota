#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"

ParseTuple parser(ParseNode *list, TokenList tokens) {
    list = malloc(sizeof(ParseNode));
    ParseTuple ret = {
        .length = 0,
        .err = 0,
        .nodes = list,
    };

    int has_text_sect = 0;
    int has_data_sect = 0;
    int has_start_func = 0;
    int iterator = 0;
    
    for (size_t i = 0; i < tokens.count; i++) {
        switch (tokens.arr[i].type) {
            case TextSect:;
                if (has_text_sect == 1) {
                    ret.err = 1;
                    return ret;
                }
                has_text_sect += 1;
                ParseNode text_sect = {
                    .type = TextSect,
                    .value = "text section",
                    .is_alloc = 0,
                };
                list[iterator] = text_sect;
                iterator++;
                list = realloc(list, iterator+1 * sizeof(ParseNode));
                break;
            case StartFunc:;
                if (has_start_func == 1) {
                    ret.err = 1;
                    return ret;
                }
                has_start_func += 1;
                ParseNode start_func = {
                    .type = StartFunc,
                    .value = "start func",
                    .is_alloc = 0,
                };
                list[iterator] = start_func;
                iterator++;
                list = realloc(list, iterator+1 * sizeof(ParseNode));
                break;
            case DataSect:;
                if (has_data_sect == 1) {
                    ret.err = 1;
                    return ret;
                }
                has_data_sect += 1;
                ParseNode data_sect = {
                    .type = DataSect,
                    .value = "data sect",
                    .is_alloc = 0,
                };
                list[iterator] = data_sect;
                iterator++;
                list = realloc(list, iterator+1 * sizeof(ParseNode));
                break;
            case Move:;
                if (
                    tokens.arr[i+1].type == Register &&
                    tokens.arr[i+2].type == Comma &&
                    tokens.arr[i+3].type == Number
                ) {
                    size_t cur_size = strlen(tokens.arr[i].value) + 1;
                    size_t reg_size = strlen(tokens.arr[i+1].value) + 1;
                    size_t comma_size = strlen(tokens.arr[i+2].value) + 1;
                    size_t num_size = strlen(tokens.arr[i+3].value) + 1;

                    char *str = malloc(cur_size + reg_size + comma_size + num_size);
                    strncpy(str, tokens.arr[i].value, cur_size);
                    strncat(str, tokens.arr[i+1].value, reg_size);
                    strncat(str, tokens.arr[i+2].value, comma_size);
                    strncat(str, tokens.arr[i+3].value, num_size);

                    ParseNode move = {
                        .type = Move,
                        .value = str,
                        .is_alloc = 1,
                    };
                    move.value[(cur_size-1+reg_size-1+comma_size-1+num_size)] = '\0';
                    list[iterator] = move;
                    iterator++;
                    list = realloc(list, iterator+1 * sizeof(ParseNode));
                } else {
                    ret.err = 1;
                    return ret;
                }
                break;
            case None:;
                break;
            case SysCall:;
                ParseNode syscall = {
                    .type = SysCall,
                    .value = "syscall",
                    .is_alloc = 0,
                };
                list[iterator] = syscall;
                iterator++;
                list = realloc(list, iterator+1 * sizeof(ParseNode));
                break;
            case Register:;
                break;
            case Comma:
                break;
            case Number:
                break;
        }
    }

    ret.length = iterator + 1;
    ret.nodes = malloc(ret.length * sizeof(ParseNode));
    ret.nodes = list;
    return ret;
}
