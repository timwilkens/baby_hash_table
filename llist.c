#include <stdlib.h>
#include <string.h>

#include "llist.h"

Node *
llist_empty_node() {
    Node *node = (Node*)calloc(1, sizeof(Node*));
    return node;
}

Node *
llist_new_node(char *key, int value) {
    Node *node = (Node*)calloc(1, sizeof(Node));
    node->value = value;
    node->key = strdup(key);
    return node;
}

void
llist_destroy(Node *n) {
    free(n->key);
    if (n->next != NULL) {
        llist_destroy(n->next);
    }
    free(n);
}
