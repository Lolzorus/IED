#include <stdio.h>
#include "listes.h"

int main(void) {
    list L = nil;

    for (int k = 'a'; k < 'i'; k++)
        L = cons(k, L);

    putlist(L);

    return 0;
}
