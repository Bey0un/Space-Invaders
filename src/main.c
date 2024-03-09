// BENMBAREK Younes SE3

#include "Graphique/libgraph.h"
#include "ListeChainee/ListeChainee.h"
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define X_WINDOW 600
#define Y_WINDOW 800

#define NO_IMAGE (-1)

#define RANDOM_LIMIT 50
#define RANDOM_COEF 1 // coef utilisé pour éviter warning tidy

#define X_SPEED 5 // vaisseau
#define MIDDLE_VAISSEAU 20
#define X_VAISSEAU (X_WINDOW / 2)
#define Y_VAISSEAU (Y_WINDOW - 50)
#define VAISSEAU_WIDTH 50
#define VAISSEAU_HEIGHT 28

#define X_SPEED_MSTR 10
#define X_GAP_MSTR 40
#define Y_GAP_MSTR 40
#define Y_INIT_MSTR_1 200
#define Y_INIT_MSTR_2 (Y_INIT_MSTR_1 + Y_GAP_MSTR)
#define Y_INIT_MSTR_3 (Y_INIT_MSTR_2 + Y_GAP_MSTR)
#define X_MAX_MSTR (X_WINDOW - 40)
#define MSTR_WIDTH 38
#define MSTR_HEIGHT 35

#define X_MIN 0
#define X_MAX (X_WINDOW - 50)
#define Y_MIN 0
#define NB_PER_LINE 12

#define GAP_MISSILE 500
#define SPEED_MISSILE 10
#define MISSILE_WIDTH 7
#define MISSILE_HEIGHT 19

#define LIFE 5
#define LIFE_X (X_WINDOW / 2 + 130)
#define LIFE_DISPLAY_GAP 30

#define BOMB_WIDTH 13
#define BOMB_HEIGHT 23
#define BOMB_SPEED 5

#define SHIELD_HEIGHT 44
#define SHIELD_WIDTH 50
#define S_TAB_SIZE 3
#define NB_SHIELD 2
#define S_LIFE 4
#define X_SHIELD1 100
#define Y_SHIELD1 700
#define X_SHIELD2 440
#define Y_SHIELD2 700

#define DELAY_US 10000
#define SIZE_POLICE 20

#define FILE_SCORE "score.txt"
#define X_SCORE (X_WINDOW / 2 - 250)
#define X_BEST_SCORE (X_WINDOW / 2 + 60)
#define X_BEST_PRINT (X_VAISSEAU - 75)
#define SIZE_SCORE 200
#define SCORE_INC 10

#define CST_TIMER 20

//--prototypes--//

void enregistrer_best_score(int score, int score_to_beat);

int lire_best_score(int *img_best);

void welcomeScreen(int press, int logo);

void monstre_blanc_style(int *frame1, int *frame2);

void monstre_bleu_style(int *frame1, int *frame2);

void monstre_rose_style(int *frame1, int *frame2);

void initialise_rand(void);

int get_rand(int nb_limit);

Entite *player_init(void);

list_e monstres_init(void);

void missile_desert_eagle(char touche, bool *Missile_tire);

void deplacement_joueur(char touche, Entite *Player, int speed_x, evenement e,
                        bool IsTouched, int *life);

bool deplacer_monstre(list_e liste_monstre, int x_inc, int limit_x,
                      list_e *liste_bombe, int bomb_image, int random_syst,
                      int *cycle);

void deplacer_monstre_y(Entite *monstre, int y_inc, bool bord_touche);

void AfficherMonstre(list_e liste_monstres, int cycle);

void AfficherBombe(list_e liste_bombe, int speed_b);

bool test_monstre_touche(Entite *missile, list_e *liste_monstre,
                         list_e *liste_missile);

void missile_control_add(list_e *liste_missile, Entite *Player,
                         bool *Missile_tire, int *missile_delay, int style);

void deplacer_missile_and_test(list_e *liste_missile, list_e *liste_monstre,
                               int *missile_delay, int gap, int *score);

void CleanScreen(void);

void init_shield(int *bouclier, int *bouclier2, int *bouclier3, int *bouclier4);

void init_decor(int *champi, int *print_score);

void handle_decor(int champi, int print_score, int img_best,
                  int best_score_print);

void display_life(int p_life, int life_lutin);

void test_bombe(list_e *liste_bombe, Entite *Joueur, bool *vaisseau_touche,
                int shield[][S_TAB_SIZE]);

void handle_shield(int shield_life[][S_TAB_SIZE], int bouclier, int bouclier2,
                   int bouclier3, int bouclier4);

bool test_monstre_player(Entite *monstre);

int main(void) {
  if (creerSurface(X_WINDOW, Y_WINDOW, "SPACE INVADERS") == 0) {
    return 1;
  }
  initialiserTexte();
  choisirPolice("../Lutins/RetroGaming.ttf", SIZE_POLICE); // police "rétro"

  evenement evt = toucheBas;
  int player_life = LIFE;

  //=== nb aléatoires ===//
  initialise_rand();

  //=== images ===//
  const int missile =
      chargerLutin("../Lutins/invader_missile.bmp", COULEUR_NOIR);
  const int bomb_image =
      chargerLutin("../Lutins/invader_bombe.bmp", COULEUR_NOIR);
  const int life = chargerLutin("../Lutins/invader_vies.bmp", COULEUR_NOIR);
  const int logo = chargerLutin("../Lutins/logo.bmp", COULEUR_NOIR);

  const int over = lutinTexte("GAME OVER", COULEUR_BLANC);
  const int press = lutinTexte("PRESS ANY BUTTON", COULEUR_BLANC);
  const int best_score_print = lutinTexte("HI-SCORE", COULEUR_VERT);

  //=== initialisation des entités ===//
  Entite *Player = player_init();
  list_e liste_monstres = monstres_init();
  list_e liste_missile = NULL;
  list_e liste_bombe = NULL;

  //=== initialisation décor ===//
  int bouclier = 0;
  int bouclier2 = 0;
  int bouclier3 = 0;
  int bouclier4 = 0;
  int champi = 0;
  int print_score = 0;
  init_decor(&champi, &print_score);
  init_shield(&bouclier, &bouclier2, &bouclier3, &bouclier4);

  //=== initialisation cycle de frames et tirs de missile ===//
  bool Missile_tire = false;
  bool IsTouched = false;
  int cycle = 0;
  int missile_delay = 0;

  int img_best = 0;
  int score = init_score();
  int best = lire_best_score(&img_best);

  int shields[NB_SHIELD][S_TAB_SIZE] = {{S_LIFE, X_SHIELD1, Y_SHIELD1},
                                        {S_LIFE, X_SHIELD2, Y_SHIELD2}};

  // ecran d'acceuil
  welcomeScreen(press, logo);
  initialise_rand();

  while (evt != quitter && player_life > 0) {

    ///////////////=== effacer l'écran ===////////////////

    CleanScreen();

    //////////////////=== évenements ===//////////////////
    char touche = 'a';
    lireEvenement(&evt, &touche, NULL);

    //--- controle du vaisseau avec touches s et d (s ==> gauche et d ==>
    // droite)
    deplacement_joueur(touche, Player, X_SPEED, evt, IsTouched, &player_life);

    //--- Tir de missile (avec la touche m)

    missile_desert_eagle(touche, &Missile_tire);
    IsTouched = false;

    ////////////////=== déplacements/affichages/contrôles ===///////////////////

    //--- monstres
    bool test_x = deplacer_monstre(liste_monstres, X_SPEED_MSTR, X_MAX_MSTR,
                                   &liste_bombe, bomb_image, 1, &cycle);
    deplacer_monstre_y(liste_monstres, Y_GAP_MSTR, test_x);
    AfficherMonstre(liste_monstres, cycle);
    if (test_monstre_player(liste_monstres)) // test pour savoir si les monstres
                                             // arrivent au niveau du vaisseau
    {
      break;
    }

    //--- missiles
    missile_control_add(&liste_missile, Player, &Missile_tire, &missile_delay,
                        missile);
    deplacer_missile_and_test(&liste_missile, &liste_monstres, &missile_delay,
                              GAP_MISSILE, &score);

    //--- bombes
    test_bombe(&liste_bombe, Player, &IsTouched, shields);
    AfficherBombe(liste_bombe, BOMB_SPEED);

    ////////////////=== décor ===///////////////////
    handle_decor(champi, score, img_best, best_score_print);
    handle_shield(shields, bouclier, bouclier2, bouclier3, bouclier4);
    display_life(player_life, life);

    //////////////=== délai ===/////////////////
    usleep(DELAY_US);

    ////////////=== MAJ ===//////////////
    majSurface();
  }

  // Enregistrement du score meme en cas d'appui sur la touche fermer
  enregistrer_best_score(score, best);

  if (evt == quitter) {
    return 0;
  }

  while (1) {
    char touche = 'a';
    lireEvenement(&evt, &touche, NULL);

    majSurface();
    afficherLutin(over, Y_INIT_MSTR_1, X_SHIELD1); // GAME OVER
    //--- fermeture de la fenetre
    if (evt == quitter) {
      break;
    }
  }
  // libération des listes
  free_list_entity(&liste_monstres);
  free_list_entity(&liste_missile);
  free_list_entity(&liste_bombe);
  fermerSurface();

  return 0;
}

//-- fonctions de chargement d'images --//
void monstre_blanc_style(int *frame1, int *frame2) {
  *frame1 = chargerLutin("../Lutins/invader_monstre1_1.bmp", COULEUR_NOIR);
  *frame2 = chargerLutin("../Lutins/invader_monstre1_2.bmp", COULEUR_NOIR);
}

void monstre_bleu_style(int *frame1, int *frame2) {
  *frame1 = chargerLutin("../Lutins/invader_monstre2_1.bmp", COULEUR_NOIR);
  *frame2 = chargerLutin("../Lutins/invader_monstre2_2.bmp", COULEUR_NOIR);
}

void monstre_rose_style(int *frame1, int *frame2) {
  *frame1 = chargerLutin("../Lutins/invader_monstre3_1.bmp", COULEUR_NOIR);
  *frame2 = chargerLutin("../Lutins/invader_monstre3_2.bmp", COULEUR_NOIR);
}

void CleanScreen(void) {
  rectanglePlein(0, 0, X_WINDOW, Y_WINDOW, COULEUR_NOIR);
}

void initialise_rand(void) {
  long k = RANDOM_COEF * time(NULL); // pour éviter warning tidy
  srand(k);
}

int get_rand(int nb_limit) {
  // retourne un nombre au hasard entre 0 et le nombre limite exclu
  return rand() % nb_limit; // warning tidy
}

//-- joueur : initialisation --//
Entite *player_init(void) {
  Entite *joueur = NULL;
  const int image_vaisseau =
      chargerLutin("../Lutins/invader_canon.bmp", COULEUR_NOIR);
  const int image_vaisseau_killed =
      chargerLutin("../Lutins/invader_canon_ferraille.bmp", COULEUR_NOIR);

  ajout_entite(&joueur, X_VAISSEAU, Y_VAISSEAU, image_vaisseau,
               image_vaisseau_killed, NO_IMAGE);

  return joueur;
}

//-- monstres : initialisation --//
list_e monstres_init(void) {
  list_e liste_monstre = NULL;
  int blanc1 = 0;
  int blanc2 = 0;
  int bleu1 = 0;
  int bleu2 = 0;
  int rose1 = 0;
  int rose2 = 0;
  monstre_bleu_style(&bleu1, &bleu2);
  monstre_rose_style(&rose1, &rose2);
  monstre_blanc_style(&blanc1, &blanc2);
  const int bouillie =
      chargerLutin("../Lutins/invader_monstre_bouillie.bmp", COULEUR_NOIR);
  for (int i = 1; i < NB_PER_LINE;
       i++) // crée des monstres avec un écart x de 40
  {
    ajout_entite(&liste_monstre, i * X_GAP_MSTR, Y_INIT_MSTR_1, bleu1, bleu2,
                 bouillie);
    ajout_entite(&liste_monstre, i * X_GAP_MSTR, Y_INIT_MSTR_2, rose1, rose2,
                 bouillie);
    ajout_entite(&liste_monstre, i * X_GAP_MSTR, Y_INIT_MSTR_3, blanc1, blanc2,
                 bouillie);
  }

  return liste_monstre;
}

//-- missile : tir --//
void missile_desert_eagle(char touche, bool *Missile_tire) {
  if (touche == 'm') {
    *Missile_tire = true;
  }
}

//-- joueur : déplacement/affichage --//
void deplacement_joueur(char touche, Entite *Player, int speed_x, evenement e,
                        bool IsTouched, int *life) {
  // on vérifie si le joueur est dans la fenetre et que les touches de
  // déplacement sont enfoncées
  if (e == toucheBas && Player->x < X_MAX && Player->x > X_MIN) {
    if (touche == 'd' && Player->x < X_MAX) {
      Player->direction = speed_x;
    } else if (touche == 's' && Player->x > X_MIN) {
      Player->direction = -1 * (speed_x);
    }
  } else {
    Player->direction = 0;
  }

  // déplacement
  if (Player->x + Player->direction > X_MIN &&
      Player->x + Player->direction < X_MAX) {
    Player->x += Player->direction;
  }

  // test sur la vie restante
  if (IsTouched) {
    (*life)--;
  }

  // affichage en fonction de la vie
  if ((*life) <= 0) {
    afficherLutin(Player->frames[1], Player->x, Player->y);
  } else {
    afficherLutin(Player->frames[0], Player->x, Player->y);
  }
}

//-- monstre : déplacement en y si bord touché --//
void deplacer_monstre_y(list_e liste_monstre, int y_inc, bool bord_touche) {
  Entite *monstre = liste_monstre;

  if (!bord_touche) {
    return;
  }

  if (monstre == NULL) {
    return;
  }

  while (monstre != NULL) {
    monstre->y += y_inc;
    monstre->direction *= -1; // inversion
    monstre = monstre->suivant;
  }
}

//-- monstre : déplacement/test bombes --//
bool deplacer_monstre(list_e liste_monstre, int x_inc, int limit_x,
                      list_e *liste_bombe, int bomb_image, int random_syst,
                      int *cycle) {
  Entite *monstre = liste_monstre;

  if (monstre == NULL) {
    return false;
  }

  static int TIMER = 0;

  // timer pour alterner frames et diminuer la vitesse
  if (TIMER++ == CST_TIMER) {
    TIMER = 0;
  } else {
    return false;
  }

  // cycle pour frames
  if ((*cycle) == 0) {
    *cycle = 1;
  } else {
    *cycle = 0;
  }

  int touched = 0;

  // déplacement et test sur les bombes
  while (monstre != NULL) {
    int test_limits = monstre->x + x_inc * monstre->direction;

    if ((test_limits <= 0) || (test_limits >= limit_x)) {
      touched = 1;
    }

    int random_mstr = get_rand(RANDOM_LIMIT);
    if (random_mstr == random_syst && monstre->bombe == false) {
      ajout_entite(liste_bombe, monstre->x, monstre->y, bomb_image, NO_IMAGE,
                   NO_IMAGE);
      monstre->bombe = true;
    }
    if ((monstre->bombe)) {
      monstre->bombe = false; // pour éviter qu'un monstre lache plusieurs
                              // bombes en meme temps
    }
    monstre->x += x_inc * monstre->direction;
    monstre = monstre->suivant;
  }
  if (touched == 1) {
    return true;
  }

  return false;
}

//-- bombe : affichage --//
void AfficherBombe(list_e liste_bombe, int speed_b) {
  Entite *bombe = liste_bombe;

  if (bombe == NULL) {
    return;
  }
  while (bombe != NULL) {
    bombe->y += speed_b;
    afficherLutin(bombe->frames[0], bombe->x, bombe->y);
    bombe = bombe->suivant;
  }
}

//-- bombe : tests --//
void test_bombe(list_e *liste_bombe, Entite *Joueur, bool *vaisseau_touche,
                int shield[][S_TAB_SIZE]) {

  Entite *bombe = *liste_bombe;

  if (bombe == NULL) {
    return;
  }

  bool test_x = false;
  bool test_y = false;

  bool test_shield_x = false;
  bool test_shield_y = false;

  bool retire_life = false;
  while (bombe != NULL) {

    // test pour la collision avec le joueur
    test_x = ((bombe->x + BOMB_WIDTH >= Joueur->x - BOMB_WIDTH) &&
              (bombe->x <= Joueur->x + VAISSEAU_WIDTH));
    test_y = (bombe->y + BOMB_HEIGHT >= Joueur->y) &&
             (bombe->y <= Joueur->y + VAISSEAU_HEIGHT);

    if (test_x && test_y) {
      retire_life = true;
      supp_entite(liste_bombe, bombe, false);
    }

    // test pour les boucliers
    if (bombe != NULL) //éviter seg fault si la bombe est déja supprimée
    {
      for (int i = 0; i < NB_SHIELD; i++) {
        if (shield[i][0] > 0) {
          test_shield_x =
              ((bombe->x + BOMB_WIDTH >= shield[i][1] - BOMB_WIDTH) &&
               (bombe->x <= shield[i][1] + SHIELD_WIDTH));
          test_shield_y = (bombe->y + BOMB_HEIGHT >= shield[i][2]) &&
                          (bombe->y <= shield[i][2] + SHIELD_HEIGHT);
          if (test_shield_x && test_shield_y) {
            shield[i][0]--;
            supp_entite(liste_bombe, bombe, false);
          }
        }
      }
    }

    // test fenetre
    if (bombe != NULL) //éviter seg fault si la bombe est déja supprimée
    {
      if (bombe->y > Y_WINDOW) {
        supp_entite(liste_bombe, bombe, false);
      }
    }
    bombe = bombe->suivant;
  }

  if (retire_life) {
    *vaisseau_touche = true;
  }
}

//-- monstre : affichage --//
void AfficherMonstre(list_e liste_monstres, int cycle) {
  Entite *monstre = liste_monstres;

  if (monstre == NULL) {
    return;
  }

  while (monstre != NULL) {
    afficherLutin(monstre->frames[cycle], monstre->x, monstre->y);
    monstre = monstre->suivant;
  }
}

//-- missile : tests fenetre et monstres --//
bool test_monstre_touche(Entite *missile, list_e *liste_monstre,
                         list_e *liste_missile) {
  if ((missile == NULL) || (liste_monstre == NULL)) {
    return false;
  }
  bool killed = false;
  bool test_x = false;
  bool test_y = false;
  Entite *monstreTest = *liste_monstre;

  if (missile->y <= 0) { // delete si bord atteint
    supp_entite(liste_missile, missile, false);
  } else {
    while (monstreTest != NULL) {

      test_x =
          ((missile->x + MISSILE_WIDTH >= monstreTest->x - MISSILE_WIDTH) &&
           (missile->x <= monstreTest->x + MSTR_WIDTH));
      test_y = (missile->y + MISSILE_HEIGHT >= monstreTest->y) &&
               (missile->y <= monstreTest->y + MSTR_HEIGHT);

      if (test_x && test_y) {
        supp_entite(liste_monstre, monstreTest, true);
        supp_entite(liste_missile, missile, false);
        killed = true;
      }
      monstreTest = monstreTest->suivant;
    }
  }
  return killed;
}

//-- missile : controle de l'ajout --//
void missile_control_add(list_e *liste_missile, Entite *Player,
                         bool *Missile_tire, int *missile_delay, int style) {
  if ((*Missile_tire) && ((*missile_delay) == 0)) {
    ajout_entite(liste_missile, Player->x, Player->y, style, NO_IMAGE,
                 NO_IMAGE);
    *missile_delay = 1;
    *Missile_tire = false;

  } else if (*Missile_tire) {
    *Missile_tire = false;
  }
  static int TIMER = 0;

  if (TIMER++ == CST_TIMER) {
    TIMER = 0;
  } else {
    return;
  }
}

//-- missile : déplacement+score --//
void deplacer_missile_and_test(list_e *liste_missile, list_e *liste_monstre,
                               int *missile_delay, int gap, int *score) {
  Entite *ptrMissile = *liste_missile;
  Entite *last_missile_added = *liste_missile;

  if (ptrMissile != NULL) {
    if (((*missile_delay) == 1) && (last_missile_added->y <= gap)) {
      *missile_delay = 0;
    }

    while (ptrMissile != NULL) {
      ptrMissile->y -= SPEED_MISSILE;

      afficherLutin(ptrMissile->frames[0], ptrMissile->x + MIDDLE_VAISSEAU,
                    ptrMissile->y);

      if (test_monstre_touche(ptrMissile, liste_monstre, liste_missile)) {
        *score += SCORE_INC;
      }
      ptrMissile = ptrMissile->suivant;
    }
  }
}

//-- boucliers : initialisation --//
void init_shield(int *bouclier, int *bouclier2, int *bouclier3,
                 int *bouclier4) {

  *bouclier = chargerLutin("../Lutins/invader_bouclier.bmp", COULEUR_NOIR);
  *bouclier2 = chargerLutin("../Lutins/invader_bouclier2.bmp", COULEUR_NOIR);
  *bouclier3 = chargerLutin("../Lutins/invader_bouclier3.bmp", COULEUR_NOIR);
  *bouclier4 = chargerLutin("../Lutins/invader_bouclier4.bmp", COULEUR_NOIR);
}

//-- décors : initialisation --//
void init_decor(int *champi, int *print_score) {
  // mascotte
  *champi = chargerLutin("../Lutins/millepatte_champi.bmp", COULEUR_BLANC);
  // Texte
  *print_score = lutinTexte("score ", COULEUR_BLANC);
}

//-- décors : affichage --//
void handle_decor(int champi, int print_score, int img_best,
                  int best_score_print) {
  display_score(print_score, X_SCORE, 0);
  afficherLutin(champi, 0, 0); // mascotte
  afficherLutin(img_best, X_BEST_SCORE, 0);
  afficherLutin(best_score_print, X_BEST_PRINT, 0);
}

//-- vie restante : affichage --//
void display_life(int p_life, int life_lutin) {
  for (int i = 0; i < p_life; i++) {
    afficherLutin(life_lutin, LIFE_X + i * LIFE_DISPLAY_GAP, Y_MIN);
  }
}

/*
shield[][0] ==> vie
shield[][1] ==> x
shield[][2] ==> y

*/

//-- boucliers : gestion --//
void handle_shield(int shield_life[][S_TAB_SIZE], int bouclier, int bouclier2,
                   int bouclier3, int bouclier4) {
  for (int i = 0; i < NB_SHIELD; i++) {
    switch (shield_life[i][0]) {
    case 4:
      afficherLutin(bouclier, shield_life[i][1], shield_life[i][2]);
      break;
    case 3:
      afficherLutin(bouclier2, shield_life[i][1], shield_life[i][2]);
      break;
    case 2:
      afficherLutin(bouclier3, shield_life[i][1], shield_life[i][2]);
      break;
    case 1:
      afficherLutin(bouclier4, shield_life[i][1], shield_life[i][2]);
      break;
    default:
      printf("");
    }
  }
}

//-- ecran d'acceuil --//
void welcomeScreen(int press, int logo) {
  afficherLutin(press, Y_INIT_MSTR_1, X_WINDOW);
  afficherLutin(logo, X_SHIELD1, X_SHIELD1);
  majSurface();
  attendreEvenement();
}

//-- monstre : test au niveau du joueur --//
bool test_monstre_player(list_e monstre) {
  Entite *monstreTest = monstre;
  while (monstreTest != NULL) {
    if (monstreTest->y >= Y_VAISSEAU) {
      return true;
    }
    monstreTest = monstreTest->suivant;
  }
  return false;
}

//-- score : écriture dans le fichier score.txt --//
void enregistrer_best_score(int score, int score_to_beat) {
  if (score > score_to_beat) {
    FILE *file = fopen(FILE_SCORE, "we"); // ouverture du fichier en lecture
    if (file == NULL) {
      printf("Erreur. Ouverture du fichier score.txt impossible !\n");
    }

    fprintf(file, "%d\n", score);
    fclose(file);
  }
}

//-- score : lecture dans le fichier score.txt --//
int lire_best_score(int *img_best) {
  FILE *file = fopen(FILE_SCORE, "re"); // ouverture du fichier en lecture
  if (file == NULL) {
    printf("Erreur. Ouverture du fichier score.txt impossible !\n");
  }

  int f_score = 0;
  int temp = 0;
  char content[SIZE_SCORE];
  while (fscanf(file, "%s", content) != EOF) // warning tidy
  {
    temp = atoi(content); // warning tidy
    if (f_score <= temp) {
      f_score = temp;
    }
  }
  fclose(file);
  *img_best = lutinTexte(content, COULEUR_VERT);
  return f_score;
}
