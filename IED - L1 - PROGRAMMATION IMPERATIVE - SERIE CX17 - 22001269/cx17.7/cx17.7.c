//adapter votre solution de[cx17.6] pour utiliser la biblioth�que cr��e pour[cx18.6].

#include "indexation.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, const str argv[])
{
    if (argc < 2) // V�rifie s'il y a au moins un argument (nom du fichier) pass� en ligne de commande
    {
        fprintf(stderr, "manque le nom du fichier...\n");
        exit(1);
    }

    const str filename = argv[1]; // R�cup�re le nom du fichier � partir des arguments

    if (argc < 3) {
        stoplist("stopliste.txt"); // Construit la stoplist en ajoutant les mots du fichier stopliste.txt
    }
    else {
        stoplist(argv[2]); // Utilise le fichier de stopliste pass� en argument
    }

    char ligne[maximum];

    FILE* flux = fopen(filename, "r");
    if (!flux)
    {
        fprintf(stderr, "fichier pas bon du tout...\n");
        exit(1);
    }

    idx x = 0; // � partir de l'index 0

    while (fgets(ligne, maximum, flux))
        indexe(ligne, ++x); // Indexe chaque ligne du fichier avec un num�ro de r�f�rence unique

    fclose(flux);

    dump(mot_libre); // Affiche le contenu de la table de mots avec leurs r�f�rences

    return 0;
}
