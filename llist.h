typedef struct Node {
    struct Node *next;
    char *key;
    int  value;
} Node;

Node *llist_empty_node();
Node *llist_new_node(char *, int);
void llist_destroy(Node *);
