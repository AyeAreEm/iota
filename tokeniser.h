typedef enum {
    None,
    DataSect,
    TextSect,
    StartFunc,
    Move,
    RAX,
    RDI,
    SysCall,
} Token;

typedef struct tokenList tokenList;
struct tokenList {
    size_t count;
    size_t len;
    void (*init)(tokenList*);
    void (*append)(tokenList*, Token t);
    Token *arr;
};

void tokenise(char *str);
void initTokenList(tokenList *self);
void appendToken(tokenList *self, Token t);
