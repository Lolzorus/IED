#ifndef INDEXER_H
#define INDEXER_H

#include <stdbool.h>

typedef unsigned int idx; // type index
typedef char* str;      // type string

#define max_mots 5096    // nombre maximum d'éléments dans la table mots
#define max_refs 1000    // nombre maximum de références par mot
#define maximum 5096     // peut-être insuffisant ?

struct ndex {
    str mot;
    idx ref_libre;
    idx refs[max_refs];
};

extern struct ndex mots[max_mots]; // table de max_mots
extern idx mot_libre;          // où le premier est libre

extern const char split_chars[]; // pour commencer

void ajoute_mot(idx x, str mot, idx ref);
void ajoute_ref(idx x, idx ref);
bool pareil(str x, str y);
int indice(str mot);
void indexe(str ligne, idx ref);
void dump(idx k);
void stoplist(const char* filename);

#endif // INDEXER_H
