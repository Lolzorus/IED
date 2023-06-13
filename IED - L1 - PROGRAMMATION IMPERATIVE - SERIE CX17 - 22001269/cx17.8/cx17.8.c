//adapter votre solution de[cx17.6] pour utiliser la bibliothèque créée pour[cx18.7].

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "indexer.h"

int main(int argc, const str argv[]) {
    if (argc < 2) {
        fprintf(stderr, "manque le nom du fichier...\n");
        exit(1);
    }

    const str filename = argv[1];

    if (argc < 3) {
        stoplist("stopliste.txt");
    }
    else {
        stoplist(argv[2]);
    }

    char ligne[maximum];

    FILE* flux = fopen(filename, "r");
    if (!flux) {
        fprintf(stderr, "fichier pas bon du tout...\n");
        exit(1);
    }

    idx x = 0;

    while (fgets(ligne, maximum, flux))
        indexe(ligne, ++x);

    fclose(flux);

    dump(mot_libre);

    return 0;
}
