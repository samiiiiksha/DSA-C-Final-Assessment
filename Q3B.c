#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
    int id;
    struct Node *left;
    struct Node *right;
} Node;

Node *make_node(int id) {
    Node *n = malloc(sizeof(Node));
    if (n == NULL) {
        exit(1);
    }
    n->id = id;
    n->left = NULL;
    n->right = NULL;
    return n;
}

Node *insert(Node *root, int id) {
    if (root == NULL) {
        return make_node(id);
    }
    if (id < root->id) {
        root->left = insert(root->left, id);
    } else if (id > root->id) {
        root->right = insert(root->right, id);
    }
    return root;
}

void inorder(Node *root, int *first) {
    if (root == NULL) {
        return;
    }
    inorder(root->left, first);
    if (!(*first)) {
        printf(" ");
    }
    printf("%d", root->id);
    *first = 0;
    inorder(root->right, first);
}

int search(Node *root, int x) {
    if (root == NULL) {
        return 0;
    }
    if (root->id == x) {
        return 1;
    }
    if (x < root->id) {
        return search(root->left, x);
    }
    return search(root->right, x);
}

int height(Node *root) {
    if (root == NULL) {
        return -1;
    }
    int lh = height(root->left);
    int rh = height(root->right);
    if (lh > rh) {
        return lh + 1;
    }
    return rh + 1;
}

Node *find_min(Node *root) {
    if (root == NULL) {
        return NULL;
    }
    while (root->left != NULL) {
        root = root->left;
    }
    return root;
}

Node *find_max(Node *root) {
    if (root == NULL) {
        return NULL;
    }
    while (root->right != NULL) {
        root = root->right;
    }
    return root;
}

void free_tree(Node *root) {
    if (root == NULL) {
        return;
    }
    free_tree(root->left);
    free_tree(root->right);
    free(root);
}

int main(void) {
    int n;
    if (scanf("%d", &n) != 1) {
        return 0;
    }

    Node *root = NULL;
    for (int i = 0; i < n; i++) {
        int id;
        scanf("%d", &id);
        root = insert(root, id);
    }

    int q;
    if (scanf("%d", &q) != 1) {
        q = 0;
    }

    for (int i = 0; i < q; i++) {
        char cmd[16];
        scanf("%s", cmd);

        if (strcmp(cmd, "INORDER") == 0) {
            int first = 1;
            inorder(root, &first);
            printf("\n");
        } else if (strcmp(cmd, "SEARCH") == 0) {
            int x;
            scanf("%d", &x);
            if (search(root, x)) {
                printf("FOUND\n");
            } else {
                printf("NOTFOUND\n");
            }
        } else if (strcmp(cmd, "HEIGHT") == 0) {
            printf("%d\n", height(root));
        } else if (strcmp(cmd, "MIN") == 0) {
            Node *m = find_min(root);
            printf("%d\n", m->id);
        } else if (strcmp(cmd, "MAX") == 0) {
            Node *m = find_max(root);
            printf("%d\n", m->id);
        }
    }

    free_tree(root);
    return 0;
}