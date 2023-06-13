//adapter votre solution de[cx17.6] pour utiliser la bibliothèque créée pour[cx18.6].

#include "indexation.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, const str argv[])
{
    if (argc < 2) // Vérifie s'il y a au moins un argument (nom du fichier) passé en ligne de commande
    {
        fprintf(stderr, "manque le nom du fichier...\n");
        exit(1);
    }

    const str filename = argv[1]; // Récupère le nom du fichier à partir des arguments

    if (argc < 3) {
        stoplist("stopliste.txt"); // Construit la stoplist en ajoutant les mots du fichier stopliste.txt
    }
    else {
        stoplist(argv[2]); // Utilise le fichier de stopliste passé en argument
    }

    char ligne[maximum];

    FILE* flux = fopen(filename, "r");
    if (!flux)
    {
        fprintf(stderr, "fichier pas bon du tout...\n");
        exit(1);
    }

    idx x = 0; // à partir de l'index 0

    while (fgets(ligne, maximum, flux))
        indexe(ligne, ++x); // Indexe chaque ligne du fichier avec un numéro de référence unique

    fclose(flux);

    dump(mot_libre); // Affiche le contenu de la table de mots avec leurs références

    return 0;
}
