#include "indexer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct ndex mots[max_mots]; // table de max_mots
idx mot_libre = 0;          // où le premier est libre

const char split_chars[] = "(., ;'\n!)\t<>/=\":-\r"; // pour commencer

void ajoute_mot(idx x, str mot, idx ref)
{
    mots[x].mot = mot;
    mots[x].ref_libre = 1; // Initialise le nombre de références à 1
    mots[x].refs[0] = ref;
    ++mot_libre;
}

void ajoute_ref(idx x, idx ref)
{
    idx r = mots[x].ref_libre;
    mots[x].refs[r] = ref;
    ++mots[x].ref_libre;
}

bool pareil(str x, str y)
{
    return (strcasecmp(x, y) == 0) ? true : false; // Compare les chaînes de caractères insensibles à la casse
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
        if (mots[x].mot != NULL)
        {
            printf("%s :", mots[x].mot); // Affiche le mot
            for (z = 0; z < mots[x].ref_libre; ++z)
                printf(" %d", mots[x].refs[z]); // Affiche les références du mot
            printf("\n");
        }
    }
}

void stoplist(const char* filename)
{
    FILE* fichier = fopen(filename, "r");
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
