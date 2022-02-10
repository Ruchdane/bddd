#ifndef  DATASTRUCTUREHEADER
#define DATASTRUCTUREHEADER
//#define TABLEAU

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define toBool(condition) (condition ? true: false)

/*#if defined(TABLEAU)
struct file {
    void **Tab;
    int tail;
};
struct pile {
    void **Tab;
    int head;
};
#else
*/
/**/
struct element
{
    void *value;
    struct element *next;
};

#define foreach(element,list) for(element = list;element != NULL;element = element->next)
#define forreach(element,list) for(element = list;element->next != NULL;element = element->next)
struct element *createElement(void *);
void * freeElement(struct element *);
int elementCount(struct element *);

struct element * createList();
#define isListEmpty(L) (L == NULL)
struct element *prependElement(struct element *,struct element *);
#define prepend(L,value) prependElement(L,createElement(value))
#define append(L,value) appendElement(L,createElement(value))
struct element *appendElement(struct element *,struct element *);
struct element *removeFirst(struct element *,struct element**);
struct element *removeLast(struct element *,struct element**);
struct element *removeAt(struct element *,int ,struct element**);
//Pour inserer apres un pointer il suffit d'utiliser 
// le pointer comme la liste et 1 comme l'index
struct element *insertElementAt(struct element *,struct element *,int );
// struct element *insertAt(struct element *,void *,int ,void *);
#define insertAt(L,value,index) insertElementAt(L,createElement(value),index);
//struct element *remove(struct element *,void * ,struct element**);


struct file { 
    struct element *head; 
    struct element *tail; 
};

struct file *createFile();
#define isFileEmpty(F) (F == NULL || F->head == NULL)
int fileCount(struct file);
void *defile(struct file *);
#define enfile(F,value) enfileElement(F,createElement(value))
void enfileElement(struct file *,struct element *);
void freeFile(struct file *);
struct pile { struct element *head; };
//#endif // MACRO

struct noeud{
  int value;
  struct noeud *fils;
  struct noeud *frere;
};

struct pile *createPile();
#define isPileEmpty(P) (P == NULL || P->head == NULL)
void *depile(struct pile *);
#define enpile(P,value) enpileElement(P,createElement(value))
void enpileElement(struct pile *,struct element *);
void freePile(struct pile *);


struct noeud *creerNoeud(int val);
struct noeud * ajouterFils(struct noeud *noeud,int val);
struct noeud * srcArbre(struct noeud *noeud,int val);
#endif // DATASTRUCTUREHEADER