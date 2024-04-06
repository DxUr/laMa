#include <laMa.h>

#include "frontend/tokenizer.h"
#include <stdio.h>

char get_char(void *p_ctx) {
    return (char)getc((FILE*)p_ctx);
}

int main(int argc, char *argv[]) {

    FILE *f = fopen(argv[1], "rb");

    // Init the tokenizer
    Tokenizer *tk = tokenizerInit(get_char, (void*)f);

    while (tokenizerAdvance(tk) != NULL) {
        printf("TOKEN: %s\n", tokenizerGetTokenName(tokenizerGetCurrent(tk)->type));
        if (tokenizerGetCurrent(tk)->type == TK_LITERAL) {
            Literal *lt = (Literal*)tokenizerGetCurrent(tk)->value;
            switch (lt->type) {
                case LT_INT:
                    printf("int: %ld\n", lt->value);
                    break;
                case LT_FLOAT:
                    printf("float: %f\n", *((float*)lt->value));
                    break;
                case LT_STRING:
                    printf("string: %s\n", (char*)lt->value);
            }
        }
    }
    

    tokenizerTerminate(tk);

    return 0;
}
