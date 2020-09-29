#ifndef DEF_CONSTANTES
#define DEF_CONSTANTES

#define MARGE 8
#define TAILLE_BLOC 25
#define HAUTEUR_TAB (43 - MARGE)
#define LARGEUR_TAB 28
#define LARGEUR_FENETRE LARGEUR_TAB * TAILLE_BLOC
#define HAUTEUR_FENETRE HAUTEUR_TAB * TAILLE_BLOC + ((MARGE * 2) * TAILLE_BLOC)


#define IA 12398
#define PARTICULE 1234
#define OBSTACLE 190
#define VIDE 130
#define TEMPETE 456
#define BONUS 999
#define BOITE 1287
#define BLOC 2736

#define NBTOURS 120
#define NBJOUEUR 10
#define PORTEE_TIR 4
#define PORTEE_TIR_BONUS 8
#define NON 12345
#define TRAP 1908


#define VIVANT 1
#define MORT 0
#define NOMS {"nassim", "emile", "amine", "nathan", "sami", "michael", "elise", "justine", "maria", "ema"}

enum {HAUT, BAS, GAUCHE, DROITE};
enum {STARWARS, FORTNITE, NARUTO, POKEMON};
enum {VITESSE, TIR, PIEGE};

#endif
