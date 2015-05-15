#include <stdbool.h>

typedef struct Hash Hash;

Hash * hasht_new(int);
void hasht_destroy(Hash *);
void hasht_put(Hash *, char *, int);
int hasht_get(Hash *, char *, bool *);
void hasht_delete(Hash *, char *);
int hasht_size(Hash *);
void hasht_pretty_print(Hash *h);
char **hasht_keys(Hash *);
void hasht_destroy_keys(Hash *, char **);

int hasht_index(Hash *, char *);
unsigned long hash(unsigned char *str); 
