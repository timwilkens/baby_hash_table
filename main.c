#include <assert.h>
#include <stdio.h>

#include "llist.h"
#include "hasht.h"

int
main() {

    Hash *h = hasht_new(100);
    // Check initial size
    assert(hasht_size(h) == 0);

    bool found = false;
    int v;

    hasht_put(h, "One", 1);
    v = hasht_get(h, "One", &found);
    assert(found);
    assert(v == 1);

    v = hasht_get(h, "missing", &found);
    assert(!found);
    // Value for missing key set to 0
    assert(v == 0);

    hasht_put(h, "Two", 2);
    hasht_put(h, "Three", 3);
    hasht_put(h, "Two", 200);
    // Overwrite
    v = hasht_get(h, "Two", &found);
    assert(found);
    assert(v == 200);

    hasht_put(h, "Four", 4);
    hasht_put(h, "Five", 5);
    hasht_put(h, "Six", 6);
    hasht_put(h, "Seven", 7);
    hasht_put(h, "Eight", 8);
    hasht_put(h, "Nine", 9);

    assert(hasht_size(h) == 9);

    hasht_delete(h, "One");
    assert(hasht_size(h) == 8);
    v = hasht_get(h, "One", &found);
    assert(!found);

    hasht_delete(h, "Three");
    assert(hasht_size(h) == 7);
    v = hasht_get(h, "Three", &found);
    assert(!found);

    char *keys[] = {
        "Two",   "Four",  "Five", "Six",
        "Seven", "Eight", "Nine",
    };

    int len = (sizeof(keys)/sizeof(keys[0]));
    int i = 0;
    // Check valid keys
    for (i = 0; i < len; i++) {
        v = hasht_get(h, keys[i], &found);
        assert(found);
    }

    hasht_destroy(h);

    // Force collisions
    h = hasht_new(5);
    for (i = 0; i < 1000; i++) {
        char key[8];
        sprintf(key, "%d", i);
        hasht_put(h, key, i);
        v = hasht_get(h, key, &found);
        assert(found);
    }
    assert(hasht_size(h) == 1000);

    return 0;
}
