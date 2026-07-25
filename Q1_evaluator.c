#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct {
    char tokens[100][16];
    int  count;
} TokenList;

int precedence(char op) {
    if (op == '+' || op == '-') {
        return 1;
    }
    if (op == '*' || op == '/') {
        return 2;
    }
    return 0;
}

int tokenizee(const char *line, TokenList *tl) {
    tl->count = 0;
    char buffer[200 + 1];
    strncpy(buffer, line, 200);
    buffer[200] = '\0';

    char *tok = strtok(buffer, " \t\n");
    while (tok != NULL) {
        if (tl->count >= 100) {
            return 0;
        }
        strncpy(tl->tokens[tl->count], tok, 15);
        tl->tokens[tl->count][15] = '\0';
        tl->count++;
        tok = strtok(NULL, " \t\n");
    }
    return 1;
}

int is_number(const char *t) {
    if (!isdigit((unsigned char)t[0])) {
        return 0;
    }
    for (int i = 0; t[i] != '\0'; i++) {
        if (!isdigit((unsigned char)t[i])) {
            return 0;
        }
    }
    return 1;
}

int infix_to_postfix(TokenList *infix, TokenList *postfix) {
    char opStack[100][16];
    int  top = -1;

    postfix->count = 0;

    for (int i = 0; i < infix->count; i++) {
        char *t = infix->tokens[i];

        if (is_number(t)) {

            strcpy(postfix->tokens[postfix->count++], t);

        } else if (strcmp(t, "(") == 0) {

            strcpy(opStack[++top], t);

        } else if (strcmp(t, ")") == 0) {

            while (top >= 0 && strcmp(opStack[top], "(") != 0) {
                strcpy(postfix->tokens[postfix->count++], opStack[top]);
                top--;
            }
            if (top < 0) {
                return 0;
            }
            top--;

        } else {

            char t0 = t[0];
            while (top >= 0 &&
                   strcmp(opStack[top], "(") != 0 &&
                   precedence(opStack[top][0]) >= precedence(t0)) {
                strcpy(postfix->tokens[postfix->count++], opStack[top]);
                top--;
            }
            strcpy(opStack[++top], t);
        }
    }

    while (top >= 0) {
        if (strcmp(opStack[top], "(") == 0) {
            return 0;
        }
        strcpy(postfix->tokens[postfix->count++], opStack[top]);
        top--;
    }

    return 1;
}

int evaluate_postfix(TokenList *postfix, int *result) {
    int stack[100];
    int top = -1;

    for (int i = 0; i < postfix->count; i++) {
        char *t = postfix->tokens[i];

        if (is_number(t)) {
            stack[++top] = atoi(t);
        } else {

            int b = stack[top--];
            int a = stack[top--];
            int r;

            switch (t[0]) {
                case '+': {
                    r = a + b;
                    break;
                }
                case '-': {
                    r = a - b;
                    break;
                }
                case '*': {
                    r = a * b;
                    break;
                }
                case '/': {
                    if (b == 0) {
                        return 0;
                    }
                    r = a / b;
                    break;
                }
                default: {
                    r = 0;
                    break;
                }
            }
            stack[++top] = r;
        }
    }

    *result = stack[top];
    return 1;
}

int main(void) {
    char line[200 + 1];

    if (fgets(line, sizeof(line), stdin) == NULL) {
        printf("ERROR:UNBALANCED\n");
        return 0;
    }

    TokenList infix, postfix;
    tokenizee(line, &infix);

    if (!infix_to_postfix(&infix, &postfix)) {
        printf("ERROR:UNBALANCED\n");
        return 0;
    }

    int result;
    if (!evaluate_postfix(&postfix, &result)) {
        printf("ERROR:DIV_BY_ZERO\n");
        return 0;
    }

    printf("%d\n", result);
    return 0;
}