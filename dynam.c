#include <stdlib.h>
#include "dynam.h"

void initDynam(Dynam *self) {
    self->count = 0;
    self->len = 0;
    self->append = appendDynam;
}

void appendDynam(Dynam *self, char c) {
    if (self->len == 0) {
        self->len = 5 * sizeof(c);
        self->str = malloc(self->len);
    }

    // self->len - sizeof(c) - 2; minus the new character, minus one to match for indexes, minus one more for null character
    if (self->count == self->len - sizeof(c) - 2) {
        self->len += 5 * sizeof(c);
        self->str = realloc(self->str, self->len);
    }

    self->str[self->count] = c;
    self->count++;
    self->str[self->count] = '\0';
}
