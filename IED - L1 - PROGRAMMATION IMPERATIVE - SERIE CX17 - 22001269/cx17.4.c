//modifier le programme[cx17.3] pour trier la table mots avant d'appeler dump().
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned int idx; // type index
typedef char* str; // type string

#define max_mots 5096 // nombre maximum d'éléments dans la table mots
#define max_refs 1000 // nombre maximum de références par mot
#define maximum 5096 // peut-être insuffisant ?

typedef struct ndex {
    str mot;
    idx ref_libre;
    idx refs[max_refs];
} ndex;

ndex mots[max_mots]; // table de max_mots
idx mot_libre = 0;     // où le premier est libre

const str split_chars = "(., ;'\n!)\t<>/=\":-\r"; // pour commencer
const char* stopliste = "stopliste.txt";         // stoplist

void usage(str message)
{
    fprintf(stderr, "%s\n", message);
    exit(1);
}

int compare_mots(const void* a, const void* b)
{
    const ndex* mot1 = (const ndex*)a; // Conversion des pointeurs en pointeurs ndex*
    const ndex* mot2 = (const ndex*)b;
    return strcmp(mot1->mot, mot2->mot); // Comparaison des chaînes de caractères des mots
}

void ajoute_mot(idx x, str mot, idx ref)
{
    mots[x].mot = mot;  // Assigner le mot à la structure ndex à l'index x
    mots[x].ref_libre = 1;  // Initialiser le compteur de références libres à 1
    mots[x].refs[0] = ref;  // Ajouter la référence à la position 0 du tableau de références
    mot_libre++;  // Incrémenter le compteur de mots libres
}

void ajoute_ref(idx x, idx ref)
{
    mots[x].refs[mots[x].ref_libre] = ref;  // Ajouter la référence à la position indiquée par le compteur de références libres
    mots[x].ref_libre++;  // Incrémenter le compteur de références libres
}


typedef enum
{
    False,
    True
} bool; // nouveau type

bool pareil(str x, str y)
{
    return strcasecmp(x, y) ? False : True;
}

int indice(str mot) // soit un mot
{
    idx x; // soit un index dans la table des mots
    for (x = 0; x < mot_libre; ++x) // ce mot est-il déjà mémorisé ?
    {
        if (pareil(mot, mots[x].mot))
            return x; // oui : retourne cet index
    }
    return -1; // non : retourne négatif
}

void indexe(str ligne, idx ref) // étant donnée une ligne de texte
{
    str mot = strtok(strdup(ligne), split_chars); // attrape le 1er mot
    while (mot)                                    // si ce n'est pas la chaîne vide
    {
        int x = indice(mot); // regarde s'il n'y est pas déjà
        if (x < 0)
            ajoute_mot(mot_libre, strdup(mot), ref); // et rajoute-le à la suite
        else
            ajoute_ref(x, ref); // sinon ajoute une nouvelle référence
        mot = strtok(NULL, split_chars);            // attrape le mot suivant
    }
}

void dump(idx k) // déverse le contenu de la mémoire
{
    idx x, z;
    for (x = 0; x < k; ++x) // scanne les mots un par un
    {
        if (mots[x].mot) // test redondant, en l'état
        {
            printf("%s :", mots[x].mot); // affiche le mot
            for (z = 0; z < mots[x].ref_libre; ++z) // scanne ses références
                printf(" %d", mots[x].refs[z]);    // affiche-les sur la même ligne
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
        mot[strcspn(mot, "\n")] = '\0'; // ignore le caractere nouvelle ligne
        int x = indice(mot);            //verifie si le mot est dans la table
        if (x < 0)
        {
            ajoute_mot(mot_libre, strdup(mot), 0);
        }
    }
    fclose(fichier);
}

int main(int k, const str ldc[])
{
    if (k < 2)
        usage("manque le nom du fichier...");
    char ligne[maximum];                 // espace pour une ligne
    FILE* flux = fopen(ldc[1], "r");      // ouvre le texte
    if (!flux)
        usage("fichier pas bon du tout...");
    idx x = 0;                            // à partir de l'index 0
    stoplist();
    while (fgets(ligne, maximum, flux))    // tant qu'il y a du texte
        indexe(ligne, ++x);                // indexe la ligne suivante
    fclose(flux);                         // ferme
    qsort(mots, mot_libre, sizeof(ndex), compare_mots);
    dump(mot_libre);
    return 0;
}
