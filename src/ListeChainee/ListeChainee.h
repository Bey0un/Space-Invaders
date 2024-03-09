// BENMBAREK Younes SE3
#ifndef __CHAINEE_H__
#define __CHAINEE_H__
#include <stdbool.h>

typedef struct Entite
{
  int x;
  int y;
  int direction;
  int frames[3];
  bool bombe;
  struct Entite *suivant;
}Entite;

typedef Entite *list_e;


/*
Pour toutes les entités : 
frames[0] --> image 1

En plus pour les monstres + feraille canon 
frames[1] --> image 2 (2eme image) (-1 pour vaisseau)
frames[2] --> image 3 (bouillie/feraille)
*/

void ajout_entite(list_e *ptrEntite, int x, int y, int image1, int image2, int image3);

void supp_entite(list_e *tete, Entite *m, bool IsMonstre);

void free_list_entity(list_e *tete);

#endif
