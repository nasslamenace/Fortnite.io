#include "CONSTANTE.h"

struct ElementCarte
{
    SDL_Surface *vide;
    SDL_Surface *obstacle;
    SDL_Surface *particule;
    SDL_Surface *tempete;
    SDL_Surface *bonus;
    SDL_Surface *boite;
    SDL_Surface *piege;
    SDL_Surface *bloc;
};

typedef struct ElementCarte ElementCarte;

struct InfoIa
{
    int numero;
    int nbparticule;
    int lvl;
    int etat;
    int direction;
    int bonus;
    char nom[100];
    SDL_Rect position;
    SDL_Surface *ia[4];

};

typedef struct InfoIa InfoIa;

void cleanup();
void jeu(SDL_Surface *ecran, int theme);
void apparition_particule(int carte[HAUTEUR_TAB][LARGEUR_TAB], int ecart);
void apparition_obstacle(int carte[HAUTEUR_TAB][LARGEUR_TAB]);
void deplacer_joueur(int carte[HAUTEUR_TAB][LARGEUR_TAB], int direction, SDL_Rect *position, int joueur, InfoIa joueurs[NBJOUEUR], SDL_Surface *ecran, int ecart);
void disparition_particule(int carte[HAUTEUR_TAB][LARGEUR_TAB], int ecart);
void retrecir_carte(int carte[HAUTEUR_TAB][LARGEUR_TAB], int ecart, InfoIa joueur[NBJOUEUR], SDL_Surface *ecran);
SDL_Rect placer_joueur(int carte[HAUTEUR_TAB][LARGEUR_TAB], int numero);
void afficher_gagnant(int carte[HAUTEUR_TAB][LARGEUR_TAB], SDL_Surface *ecran, InfoIa joueur[NBJOUEUR]);
void afficher_jeu(int carte[HAUTEUR_TAB][LARGEUR_TAB], SDL_Surface *ecran, SDL_Surface *ia[4], InfoIa joueur[NBJOUEUR] , ElementCarte element);
void tir(int carte[HAUTEUR_TAB][LARGEUR_TAB], SDL_Rect pos, int direction, SDL_Surface *ecran, InfoIa joueur[NBJOUEUR], SDL_Surface *ia[4], ElementCarte element);
void tir_bonus(int carte[HAUTEUR_TAB][LARGEUR_TAB], SDL_Rect pos, int direction, SDL_Surface *ecran, InfoIa joueur[NBJOUEUR], SDL_Surface *ia[4], ElementCarte element);
void animation_tir(int carte[HAUTEUR_TAB][LARGEUR_TAB], SDL_Surface *ecran, SDL_Surface *ia[4], InfoIa joueur[NBJOUEUR], SDL_Rect posJoueur, ElementCarte element,  SDL_Rect posCible, int direction);
void apparition_bonus(int carte[HAUTEUR_TAB][LARGEUR_TAB]);
void afficher_top(InfoIa joueurs[NBJOUEUR], SDL_Surface *ecran);
void apparition_boite(int carte[HAUTEUR_TAB][LARGEUR_TAB]);
void activation_boite( int numero, int carte[HAUTEUR_TAB][LARGEUR_TAB], SDL_Rect pos, int direction, SDL_Surface *ecran, InfoIa joueur[NBJOUEUR], SDL_Surface *ia[4], ElementCarte element);
void explosion(SDL_Surface *ecran, SDL_Rect position);
void construire_bloc(int carte[HAUTEUR_TAB][LARGEUR_TAB], SDL_Rect position, int direction);
