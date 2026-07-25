#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
    char url[51];
    struct Node *prev;
    struct Node *next;
} Node;

Node *current = NULL;

Node *make_node(const char *url) {
    Node *n = malloc(sizeof(Node));
    if (n == NULL) {
        exit(1);
    }
    strncpy(n->url, url, 50);
    n->url[50] = '\0';
    n->prev = NULL;
    n->next = NULL;
    return n;
}

void free_forward(Node *from) {
    Node *n = from;
    while (n != NULL) {
        Node *next = n->next;
        free(n);
        n = next;
    }
}

void visit(const char *url) {
    if (current != NULL) {
        free_forward(current->next);
        current->next = NULL;
    }

    Node *n = make_node(url);
    n->prev = current;
    if (current != NULL) {
        current->next = n;
    }
    current = n;
}

void go_back(void) {
    if (current == NULL || current->prev == NULL) {
        printf("NO_BACK\n");
        return;
    }
    current = current->prev;
    printf("%s\n", current->url);
}

void go_forward(void) {
    if (current == NULL || current->next == NULL) {
        printf("NO_FORWARD\n");
        return;
    }
    current = current->next;
    printf("%s\n", current->url);
}

void show_current(void) {
    if (current == NULL) {
        printf("NONE\n");
        return;
    }
    printf("%s\n", current->url);
}

void free_all(void) {
    if (current == NULL) {
        return;
    }
    Node *n = current;
    while (n->prev != NULL) {
        n = n->prev;
    }
    free_forward(n);
    current = NULL;
}

int main(void) {
    int q;
    if (scanf("%d", &q) != 1) {
        return 0;
    }

    for (int i = 0; i < q; i++) {
        char cmd[16];
        scanf("%s", cmd);

        if (strcmp(cmd, "VISIT") == 0) {
            char url[51];
            scanf("%s", url);
            visit(url);
        } else if (strcmp(cmd, "BACK") == 0) {
            go_back();
        } else if (strcmp(cmd, "FORWARD") == 0) {
            go_forward();
        } else if (strcmp(cmd, "CURRENT") == 0) {
            show_current();
        }
    }

    free_all();
    return 0;
}