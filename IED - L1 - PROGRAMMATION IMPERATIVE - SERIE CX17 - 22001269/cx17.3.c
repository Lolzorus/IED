//utiliser votre solution de [cx15.3] pour rajouter au programme la capacité de lire et d'exploiter une STOPLIST comme dans le prototype PYTHON.
// adapter le programme[cx17.2] pour qu'il manipule une table de structures – en utilisant, bien sûr, le script[px17] ou[sx17] pour comparer les résultats.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned int idx; // type index
typedef char* str;      // type string

#define max_mots 5096    // nombre maximum d'éléments dans la table mots
#define max_refs 1000    // nombre maximum de références par mot
#define maximum 5096     // peut-être insuffisant ?

struct ndex {
    str mot; // Pointeur vers chaîne de caractères
    idx ref_libre; // Nombre de références libres pour ce mot
    idx refs[max_refs]; // Tableau des références pour ce mot
};

struct ndex mots[max_mots]; // table de max_mots
idx mot_libre = 0;          // où le premier est libre

const char split_chars[] = "(., ;'\n!)\t<>/=\":-\r"; // pour commencer
const char* stopliste = "stopliste.txt"; // stoplist

typedef enum { False, True } bool; // nouveau type

void ajoute_mot(idx x, str mot, idx ref)
{
    mots[x].mot = mot;
    mots[x].ref_libre = 1; // Initialise le nombre de références à 1
    mots[x].refs[0] = ref;
    ++mot_libre;
}

void ajoute_ref(idx x, idx ref)
{
    idx r = mots[x].ref_libre; // Récupère le nombre de références libres pour le mot à l'index x
    mots[x].refs[r] = ref; // Ajoute la référence ref à la position r dans le tableau
    ++mots[x].ref_libre; // Incrémente le nombre de références
}

bool pareil(str x, str y)
{
    return (strcasecmp(x, y) == 0) ? True : False; // Compare les chaînes de caractères insensibles à la casse
}

int indice(str mot)
{
    idx x;
    for (x = 0; x < mot_libre; ++x)
    {
        if (pareil(mot, mots[x].mot))
            return x; // Retourne l'index du mot s'il est trouvé dans la table
    }
    return -1; // Retourne -1 si le mot n'est pas trouvé
}

void indexe(str ligne, idx ref)
{
    str mot = strtok(strdup(ligne), split_chars); // Découpe la ligne en mots
    while (mot != NULL)
    {
        int x = indice(mot);
        if (x < 0)
            ajoute_mot(mot_libre, strdup(mot), ref); // Ajoute le mot à la table s'il n'est pas déjà présent
        else
            ajoute_ref(x, ref); // Ajoute une nouvelle référence au mot existant
        mot = strtok(NULL, split_chars); // Passe au mot suivant
    }
}

void dump(idx k)
{
    idx x, z;
    for (x = 0; x < k; ++x)
    {
        if (mots[x].mot != NULL) // Vérifie si le mot à l'index x existe
        {
            printf("%s :", mots[x].mot); // Affiche le mot
            for (z = 0; z < mots[x].ref_libre; ++z)
                printf(" %d", mots[x].refs[z]); // Affiche les références du mot
            printf("\n");
        }
    }
}

void stoplist()
{
    FILE* fichier = fopen(stopliste, "r");
    if (!fichier)
    {
        fprintf(stderr, "fichier pas tres bon.\n");
        exit(1);
    }
    str mot = (str)malloc(maximum * sizeof(char));
    while (fgets(mot, maximum, fichier))
    {
        mot[strcspn(mot, "\n")] = '\0'; // Ignore le caractère de nouvelle ligne
        int x = indice(mot); // Vérifie si le mot est dans la table
        if (x < 0)
        {
            ajoute_mot(mot_libre, strdup(mot), 0); // Ajoute le mot à la table
        }
    }
    fclose(fichier);
}

int main(int argc, const str argv[])
{
    if (argc < 2) // Vérifie s'il y a au moins un argument (nom du fichier) passé en ligne de commande
    {
        fprintf(stderr, "manque le nom du fichier...\n");
        exit(1);
    }

    const str filename = argv[1]; // Récupère le nom du fichier à partir des arguments
    char ligne[maximum];

    FILE* flux = fopen(filename, "r");
    if (!flux)
    {
        fprintf(stderr, "fichier pas bon du tout...\n");
        exit(1);
    }

    idx x = 0; // à partir de l'index 0
    stoplist(); // Construit la stoplist en ajoutant les mots du fichier stopliste.txt

    while (fgets(ligne, maximum, flux))
        indexe(ligne, ++x); // Indexe chaque ligne du fichier avec un numéro de référence unique

    fclose(flux);

    dump(mot_libre); // Affiche le contenu de la table de mots avec leurs références

    return 0;
}
