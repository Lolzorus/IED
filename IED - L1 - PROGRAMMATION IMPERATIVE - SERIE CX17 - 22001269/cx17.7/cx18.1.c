#include <stdio.h>
#include <stdlib.h>
#define nil NULL
typedef char * str ; // type string

typedef struct node 
{ int car ; struct node * cdr ; } node, * list ;

void usage(str message) 
{ fprintf(stderr, "%s\n", message) ; exit(1) ; }

list cons(int car, list L)
{ list new = malloc(sizeof(node)) ;
if (! new) usage("cons : manque de RAM") ; // enfin, un peu de sérieux !
new -> car = car ;
new -> cdr = L ;
return new ; }

void putlist(list L)
{ if (! L) return ; // nil : fin de liste
putlist(L -> cdr) ; 
printf("%c \n", L -> car) ;}

int main(void) 
{ list L = nil ;
for (int k = 'a' ; k < 'i' ; k++)
L = cons(k, L) ;
putlist(L) ;
return 0 ; }