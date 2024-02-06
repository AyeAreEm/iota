typedef struct Dynam Dynam;

struct Dynam {
    char *str;
    size_t count;
    size_t len;
    void (*init)(Dynam*);
    void (*append)(Dynam*, char);
};

void initDynam(Dynam *self);
void appendDynam(Dynam *self, char c);
