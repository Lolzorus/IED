//modifier le programme réalisé pour[cx17.4] de sorte que les références soient gérées sous
//forme de listes – et que la référence courante ne soit pas rajoutée si elle est déjà présente
//dans la liste des références pour ce mot.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned idx; // type index
typedef char* str;   // type string

struct reference {
    idx ref;
    struct reference* suivant;
};

struct ndex {
    str mot;
    struct reference* references;
};

void usage(str message) {
    fprintf(stderr, "%s\n", message);
    exit(1);
}

#define max_mots 5096 // nombre maximum d'éléments dans la table mots
struct ndex mots[max_mots]; // table de max_mots
idx mot_libre = 0; // où le premier est libre

#define maximum 5096 // peut-être insuffisant ?

const str split_chars = "(., ;'\n!)\t<>/=\":-\r"; // pour commencer
const char* stopliste = "stopliste.txt"; // stoplist

int compare_mots(const void* a, const void* b);
void ajoute_ref(idx x, idx ref);

void ajoute_mot(idx x, str mot, idx ref)
{
    mots[x].mot = mot; // Assigner le mot à l'indice x dans la table mots
    ajoute_ref(x, ref); // Ajouter la référence à la liste de références pour le mot à l'indice x
    mot_libre++; // Augmenter le nombre de mots libres dans la table
}

typedef enum { False, True } bool; // nouveau type

bool pareil(str x, str y)
{
    return strcasecmp(x, y) ? False : True;
}

int indice(str mot)
{
    idx x;
    for (x = 0; x < mot_libre; ++x)
    {
        if (pareil(mot, mots[x].mot)) // Vérifie si le mot à l'indice x dans la table mots est identique au mot recherché
            return x; // Retourne l'indice x si le mot est trouvé
    }
    return -1; // Retourne -1 si le mot n'est pas trouvé dans la table
}

void indexe(str ligne, idx ref)
{
    str mot = strtok(strdup(ligne), split_chars); // Découpe la ligne en mots en utilisant les caractères de séparation définis dans split_chars
    while (mot)
    {
        int x = indice(mot); // Recherche l'indice du mot dans la table mots
        if (x < 0)
            ajoute_mot(mot_libre, strdup(mot), ref); // Si le mot n'existe pas dans la table, l'ajoute avec sa référence
        else
            ajoute_ref(x, ref); // Si le mot existe déjà dans la table, ajoute la référence à sa liste de références
        mot = strtok(NULL, split_chars); // Passe au mot suivant dans la ligne
    }
}


void dump(idx k)
{
    idx x;
    for (x = 0; x < k; ++x)
    {
        if (mots[x].mot) // Vérifie si le mot existe
        {
            printf("%s :", mots[x].mot); // Affiche le mot suivi de ":"
            struct reference* current = mots[x].references; // Pointe vers la liste des références du mot
            while (current != NULL) // Parcourt la liste des références
            {
                printf(" %i", current->ref); // Affiche la référence
                current = current->suivant; // Passe à la référence suivante
            }
            printf("\n"); // Passe à la ligne suivante après avoir affiché toutes les références du mot
        }
    }
}


void stoplist()
{
    FILE* fichier = fopen(stopliste, "r"); // Ouvre le fichier de la stoplist en mode lecture
    if (!fichier)
    {
        fprintf(stderr, "fichier pas tres bon.\n"); // Affiche un message d'erreur si le fichier ne peut pas être ouvert
        exit(1); // Quitte le programme avec un code d'erreur
    }
    str mot = (str)malloc(maximum * sizeof(char)); // Alloue de la mémoire pour stocker un mot de la stoplist
    while (fgets(mot, maximum, fichier))
    {
        mot[strcspn(mot, "\n")] = '\0'; // Supprime le saut de ligne du mot lu depuis le fichier
        int x = indice(mot); // Recherche l'indice du mot dans la table des mots
        if (x < 0)
        {
            ajoute_mot(mot_libre, strdup(mot), 0); // Ajoute le mot à la table des mots s'il n'est pas déjà présent
        }
    }
    fclose(fichier); // Ferme le fichier de la stoplist
}


int compare_mots(const void* a, const void* b)
{
    const str mot1 = (*(const struct ndex*)a).mot; // Obtient le mot du premier élément
    const str mot2 = (*(const struct ndex*)b).mot; // Obtient le mot du deuxième élément
    return strcmp(mot1, mot2); // Compare les deux mots et renvoie le résultat de la comparaison
}


void ajoute_ref(idx x, idx ref)
{
    struct reference* current = mots[x].references;
    struct reference* prec = NULL;
    while (current != NULL)
    {
        if (current->ref == ref)
        {
            return; // référence est déjà présente donc on ignore
        }
        prec = current;
        current = current->suivant;
    }
    struct reference* new_reference = (struct reference*)malloc(sizeof(struct reference));
    new_reference->ref = ref;
    new_reference->suivant = NULL;

    if (prec == NULL)
    {
        mots[x].references = new_reference; // Premier élément de la liste des références
    }
    else
    {
        prec->suivant = new_reference;
    }
}

int main(int k, const str ldc[])
{
    if (k < 2)
        usage("manque le nom du fichier..."); // Vérifie si le nom du fichier est passé en argument, sinon affiche un message d'erreur

    char ligne[maximum]; // Déclare un tableau de caractères pour stocker une ligne du fichier
    FILE* flux = fopen(ldc[1], "r"); // Ouvre le fichier en mode lecture
    if (!flux)
        usage("fichier pas bon du tout..."); // Affiche un message d'erreur si le fichier ne peut pas être ouvert

    idx x = 0; // Variable pour compter les références
    stoplist(); // Charge la stoplist depuis le fichier

    while (fgets(ligne, maximum, flux))
        indexe(ligne, ++x); // Indexe chaque ligne du fichier en appelant la fonction indexe avec la ligne et la référence

    fclose(flux); // Ferme le fichier

    qsort(mots, mot_libre, sizeof(struct ndex), compare_mots); // Trie la table des mots par ordre alphabétique

    dump(mot_libre); // Affiche le contenu de la table des mots avec les références associées

    return 0; // Fin du programme
}

