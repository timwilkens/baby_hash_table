#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "llist.h"
#include "hasht.h"

struct Hash {
    int num_keys;
    int num_buckets;
    Node **entries;
};

Hash *
hasht_new(int size) {
    if (size <= 0) {
        return NULL;
    }
	Hash *h = (Hash*)malloc(sizeof(Hash));
    h->num_buckets = size;
    h->num_keys = 0;
    h->entries = calloc(size, sizeof(Node*));
    return h;
}

void
hasht_destroy(Hash *h) {
    int i = 0;
    for (i = 0; i < h->num_buckets; i++) {
        if (h->entries[i] != NULL) {
            llist_destroy(h->entries[i]);
        }
    }
    free(h);

    return;
}

void
hasht_put(Hash *h, char *key, int value) {
     int index = hasht_index(h, key);
     Node *n = h->entries[index];
     if (n ==  NULL) {
         h->entries[index] = llist_new_node(key, value);
         h->num_keys++;
     } else {
         for (;;) {
             if (strcmp(n->key, key) == 0) {
                 n->value = value; 
                 return;
             }
             if (n->next == NULL) {
                 n->next = llist_new_node(key, value);
                 h->num_keys++;
                 return;
             }
             n = n->next;
         }
     }
}

int
hasht_get(Hash *h, char *key, bool *found) {
    int index = hasht_index(h, key);
    Node *n = h->entries[index];
    if (n == NULL) {
        *found = false;
        return 0;
    } else {
        for (;;) {
            if (strcmp(n->key, key) == 0) {
                *found = true;
                return n->value;
            }
            if (n->next == NULL) {
                *found = false;
                return 0;
            }
            n = n->next;
        }
    }
}

void
hasht_delete(Hash *h, char *key) {
    int index = hasht_index(h, key);
    Node *n = h->entries[index];
    Node *prev = NULL;
    if (n == NULL) {
        return;
    }

    for (;;) {
        if (strcmp(n->key, key) == 0) {
            if (prev == NULL) {
                h->entries[index] = n->next;
            } else {
                prev->next = n->next;
            }
            h->num_keys--;
            return;
        }
        if (n->next == NULL) {
            return;
        }
        prev = n;
        n = n->next;
    }
}

int
hasht_size(Hash *h) {
    return h->num_keys;
}

int
hasht_index(Hash *h, char *key) {
    return ((unsigned int)hash((unsigned char*)key) % h->num_buckets);
}

// From Dan Bernstein
// http://www.cse.yorku.ca/~oz/hash.html
unsigned long
hash(unsigned char *str) {
    unsigned long hash = 5381;
    int c;

    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    }

    return hash;
}