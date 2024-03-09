// BENMBAREK Younes SE3

#include "../Graphique/libgraph.h"
#include "ListeChainee.h"
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>

//-- ajout d'une entité --//
void ajout_entite(list_e *ptrEntite, int x, int y, int image1, int image2, int image3)
{
    Entite *entity = (Entite *) malloc(sizeof(Entite));
    if (entity == NULL)
    {
      exit(EXIT_FAILURE);
    }

    entity->x = x;
    entity->y = y;
    entity->suivant = *ptrEntite;
    entity->direction = 1;
    
    entity->frames[0] = image1;
    if(image2 != -1)
    {
         entity->frames[1] = image2;
         if(image3 != -1)
         {
             entity->frames[2] = image3;
             entity->bombe = false;
         }
    }
    
    *ptrEntite = entity;
}

//-- supprresion d'une entité --//
void supp_entite(list_e *tete, Entite *m, bool IsMonstre) 
{
    if ((m == NULL) || (tete == NULL)) // on vérifie qu'aucun paramètre = NULL
    { 
      return;
    }

    if (*tete == m) // suppression de la tête
    {
        *tete = m->suivant;
        if (IsMonstre)
        {
            afficherLutin(m->frames[2], m->x, m->y);
        }
        free(m);
    }
    else
    {
        Entite *temp = *tete;
        if (temp != NULL)
        {
            while (temp->suivant != NULL && temp->suivant != m) // on cherche où se situe l'entité
            {
                temp = temp->suivant;
            }
            if (temp->suivant == m) // on a trouvé l'entité à supprimer
            {
                temp->suivant = m->suivant; // on chaîne les deux entités qui étaient dans les côtés de l'entité supprimée

                if (IsMonstre)
                {
                    afficherLutin(m->frames[2], m->x, m->y);
                }
                free(m);
            }
        }
    }
}

//-- liberation liste --//
void free_list_entity(list_e *tete)
{
  if(tete == NULL)
  {
    return;
  }

  Entite *temp = *tete;
  while(temp != NULL)
  {
    Entite *temp2 = temp;
    temp = temp->suivant;
    free(temp2);
  }
}
