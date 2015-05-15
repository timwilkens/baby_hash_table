#include <stdlib.h>
#include <stdio.h>
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

Hash *
hasht_clone(Hash *h) {
    Hash *copy = hasht_new(h->num_buckets);
    hasht_copy(copy, h);
    return copy;
}

Hash *
hasht_grow_clone(Hash *h) {
    // Double the number of buckets
    Hash *copy = hasht_new(h->num_buckets*2);
    hasht_copy(copy, h);
    return copy;
}

void
hasht_copy(Hash *dest, Hash *src) {
    int i;
    for (i = 0; i < src->num_buckets; i++) {
        Node *n = src->entries[i];
        for (;;) {
            if (n == NULL) {
                break;
            }
            hasht_put(dest, n->key, n->value);
            n = n->next;
        }
    }
}

int
hasht_avg_collisions(Hash *h) {

    int i, found, buckets;
    found = 0;
    buckets = 0;

    for (i = 0; i < h->num_buckets; i++) {
        Node *n = h->entries[i];
        if (n == NULL) {
            continue;
        }
        buckets++;
        for (;;) {
            found++;
            if (n->next == NULL) {
                break;
            }
            n = n->next;
        }
    }

    if (buckets == 0) {
        return 0;
    }

    return (int)found/buckets;
}

void
hasht_put(Hash *h, char *key, int value) {
     int collisions = hasht_avg_collisions(h);
     // Grow
     if (collisions > 5) {
         Hash *copy = hasht_grow_clone(h);
         h->num_buckets = copy->num_buckets;
         free(h->entries);
         h->entries = copy->entries;
         copy->entries = NULL;
         hasht_destroy(copy);
     }

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

char **
hasht_keys(Hash *h) {

    char **keys = calloc(h->num_keys, sizeof(char*));
    int i, index;
    index = 0;

    for (i = 0; i < h->num_buckets; i++) {
        Node *n = h->entries[i];
        for (;;) {
            if (n == NULL) {
                break;
            }
            keys[index++] = strdup(n->key);
            n = n->next;
        }
    }

    return keys;
}

void
hasht_destroy_keys(Hash *h, char **ks) {
    int i;
    for (i = 0; i < h->num_keys; i++) {
        free(ks[i]);
    }
    free(ks);
}

void
hasht_pretty_print(Hash *h) {
    printf("{\n");

    int i;
    for (i = 0; i < h->num_buckets; i++) {
        Node *n = h->entries[i];
        for (;;) {
            if (n == NULL) {
                break;
            }
            printf("  %s => %d\n", n->key, n->value);
            n = n->next;
        }
    }

    printf("}\n");
}

bool
hasht_equal(Hash *h1, Hash *h2) {

    if (h1->num_keys != h2->num_keys) {
        return false;
    }

    char **ks = hasht_keys(h1);
    int i;
    bool f1,f2;

    for (i = 0; i < h1->num_keys; i++) {
        int v1 = hasht_get(h1, ks[i], &f1);
        int v2 = hasht_get(h2, ks[i], &f2);

        if (!f2 || !f1) {
            hasht_destroy_keys(h1, ks);
            return false;
        }
        if (v1 != v2) {
            hasht_destroy_keys(h1, ks);
            return false;
        }
    }

    hasht_destroy_keys(h1, ks);
    return true;
}

int
hasht_size(Hash *h) {
    return h->num_keys;
}

int
hasht_buckets(Hash *h) {
    return h->num_buckets;
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
