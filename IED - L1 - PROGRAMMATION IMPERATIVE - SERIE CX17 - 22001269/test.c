//coder et compiler ce programme!; voyez-vous l'opportunité de rajouter d'autres caractères
// à la valeur de split_chars!? La tabulation!? Les symboles '<' et '>'!? Essayez - le sur un fichier
// html, et voyez ce que vous pouvez filtrer ainsi...

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned idx ; // type index
typedef char * str ; // type string

void usage(str message) 
{ 
    fprintf(stderr, "%s\n", message) ; 
    exit(1) ; 
}

#define max_mots 5096 // nombre maximum d'éléments dans la table mots
char *mots[max_mots] ; // table de max_mots
idx mot_libre = 0 ; // où le premier est libre

#define max_refs 1000 // nombre maximum de références par mot
idx refs[max_mots][max_refs] ; // matrice de max_mots par max_refs
idx ref_libre[max_mots] ; // table de max_mots

#define maximum 5096 // peut-être insuffisant ?

const str split_chars = "(., ;'\n!)\t<>/=\":-\r" ; // pour commencer

void ajoute_ref(idx x, idx ref)
{ 
    idx r = ref_libre[x] ;
    refs[x][r] = ref ;
    ++ref_libre[x] ; 
}
    
void ajoute_mot(idx x, str mot, idx ref) // un index, un mot, une référence
{ 
    mots[x] = mot ; // met le mot là où on te dit
    refs[x][0] = ref ; // met la référence correspondante
    ++ref_libre[x] ; // décale pour la réf suivante
    ++mot_libre ; // décale le mot libre
} 

typedef enum {False, True} bool ;
bool pareil(str x, str y) { return strcasecmp(x, y) ? False : True ; }

int indice(str mot)
{ 
    idx x ;
    for (x = 0; mots[x] ; ++x) // Parcourt les indices de la table de mots
    if (pareil(mot, mots[x])) // Vérifie si le mot à l'index x est identique à mot
            return x ; // Retourne l'index x si le mot est trouvé
    return -1 ; //Retourne -1 si le mot n'est pas trouvé
}


void indexe(str ligne, idx ref)
{ 
    str mot = strtok(strdup(ligne), split_chars) ; // Utilise strtok pour diviser
    while (mot)
    { 
        int x = indice(mot) ; // Vérifie si le mot existe déjà dans la table de mots
        if (x < 0) 
            ajoute_mot(mot_libre, mot, ref) ; //Si le mot n'existe pas, l'ajoute
        else 
            ajoute_ref(x, ref) ; //Si le mot existe déjà, ajoute simplement la référence
        mot = strtok(NULL, split_chars) ;  // Passe au mot suivant dans la ligne
    } 
}

void dump(idx k)
{  
    idx x, z ;
    for (x = 0 ; x < k ; ++x)
    { 
        if (mots[x]) // Vérifie si le mot à l'index x existe
        { 
            printf("%s :", mots[x]) ; // Affiche le mot à l'index x
            for (z = 0 ; refs[x][z] ; ++z) // Parcourt les références du mot à l'index x
                printf(" %i", refs[x][z]) ;// Affiche chaque référence du mot
            printf("\n") ; // Passe à la ligne suivante
        } 
    } 
}

int main(int k, const str ldc[])
{ 
    if (k < 2) 
        usage("manque le nom du fichier...") ;
    char ligne[maximum] ;
    FILE * flux = fopen(ldc[1], "r") ;
    if (! flux) 
        usage("fichier pas bon du tout...") ;
    idx x = 0 ;
    while (fgets(ligne, maximum, flux))
        indexe(ligne, ++x) ;
    fclose(flux) ;
    dump(mot_libre) ;
return 0 ; 
}
