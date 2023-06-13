#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define nil NULL
typedef char* str ; // type string

typedef struct node 
{ int car ; struct node * cdr ; } node, * list ;

void usage(str message) 
{ fprintf(stderr, "%s\n", message) ; exit(1) ; }

list cons(str car, list L)
{ list new = malloc(sizeof(node)) ;
if (! new) usage("cons : manque de RAM") ; // enfin, un peu de sérieux !
new -> car = car ;
new -> cdr = L ;
return new ; }

void putlist(list L)
{ if (! L) return ; // nil : fin de liste
putlist(L -> cdr) ; 
printf("%s ", L -> car) ;}

int main(int argc, char* argv[]) {
	if (argc <2) {
		usage("Usage: Entrez des arguments");
	}

list L = nil ;
for (int k = argc - 1; k > 0 ; k--){
	char* word = argv[k];
	L = cons(word, L) ; }
	putlist(L) ;
	printf("\n");
return 0 ; }