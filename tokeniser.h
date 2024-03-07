typedef enum {
    None,
    DataSect,
    TextSect,
    StartFunc,
    Move,
    Register,
    SysCall,
    Comma,
    Number,
} TokenType;

typedef struct Token {
    TokenType type;
    int is_alloc;
    char *value;
} Token;

typedef struct TokenList TokenList;
struct TokenList {
    size_t count;
    size_t len;
    void (*init)(TokenList*);
    void (*append)(TokenList*, Token t);
    Token *arr;
};

TokenList tokenise(char *str);
void initTokenList(TokenList *self);
void appendToken(TokenList *self, Token t);
