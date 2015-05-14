#include <stdbool.h>

typedef struct Hash Hash;

Hash * hasht_new(int);
void hasht_destroy(Hash *);
void hasht_put(Hash *, char *, int);
int hasht_get(Hash *, char *, bool *);
void hasht_delete(Hash *, char *);
int hasht_size(Hash *);

int hasht_index(Hash *, char *);
unsigned long hash(unsigned char *str); 
