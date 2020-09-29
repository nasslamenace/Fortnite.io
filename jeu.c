#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <fmod.h>
#include "CONSTANTE.h"
#include "jeu.h"

void cleanup()
{
    TTF_Quit();
    SDL_Quit();
}


void jeu(SDL_Surface *ecran, int theme)
{
    FMOD_SYSTEM *system;
    FMOD_SOUND *bruitTempete, *wewer;
    FMOD_System_Create(&system);
    FMOD_System_Init(system, 32, FMOD_INIT_NORMAL, NULL);
    FMOD_System_CreateSound(system, "tempete.wav", FMOD_CREATESAMPLE, 0, &bruitTempete);
    FMOD_System_CreateSound(system, "wewer.wav", FMOD_CREATESAMPLE, 0, &wewer);



    int carte[HAUTEUR_TAB][LARGEUR_TAB], i, j, k, l, continuer = 1, nbparticule = 0, timerApparition1 = 0, timerApparition2 = 0;
    int timerDisparition1 = 0, timerDisparition2 = 0, timerTempete1 = 0, timerTempete2 = 0, compteurTempete = 0;

    if(TTF_Init() == -1)
    {
        fprintf(stderr, "Erreur d'initialisation de TTF_Init : %s\n", TTF_GetError());
        exit(EXIT_FAILURE);
    }
    TTF_Font *police = NULL;
    police = TTF_OpenFont("Starjedi.ttf",20);
    SDL_Color noir = {0, 0, 0}, blanc = {98, 126, 235};
    char score[30];
    char* noms[NBJOUEUR] = NOMS;

    ElementCarte elements;

    for(i = 0; i < HAUTEUR_TAB; i++)
    {
        for(j = 0; j < LARGEUR_TAB; j++)
        {
            carte[i][j] = VIDE;
        }
    }



    InfoIa joueur[NBJOUEUR];
    for(i = 0; i < NBJOUEUR; i++)
    {
        joueur[i].nbparticule = 0;
        joueur[i].lvl = 0;
        joueur[i].etat = VIVANT;
        joueur[i].numero = i;
        joueur[i].direction = BAS;
        joueur[i].bonus = NON;
        sprintf(joueur[i].nom, "%s", noms[i]);
        joueur[i].position = placer_joueur(carte, i);
        carte[joueur[i].position.x][joueur[i].position.y] = i;
    }


    SDL_Surface *ia[4] = {NULL};
    SDL_Surface *iaActuel, *vide, *particule, *obstacle, *tempete, *texte, *rectangle, *bonus, *bloc, *piege, *boite;
    SDL_Event event;
    SDL_Rect positionObjet, positionscore, positionBlanc;

    positionscore.x = 0;
    positionscore.y = 0;

    switch(theme)
    {
    case FORTNITE:
        ia[HAUT] = IMG_Load("ia_haut.png");
        ia[BAS] = IMG_Load("ia_bas.png");
        ia[GAUCHE] = IMG_Load("ia_gauche.png");
        ia[DROITE] = IMG_Load("ia_droite.png");
        elements.vide = vide = IMG_Load("vide.png");
        elements.obstacle = obstacle = IMG_Load("obstacle.png");
        elements.particule = particule = IMG_Load("particule.png");
        elements.tempete = tempete = IMG_Load("tempete.jpg");
        elements.bonus = bonus = IMG_Load("bonus.png");
        elements.piege = piege = IMG_Load("piege.png");
        elements.boite = boite = IMG_Load("boite.png");
        elements.bloc = bloc = IMG_Load("bloc.png");
        break;
    case STARWARS:
        ia[HAUT] = IMG_Load("ia_haut_starwars.png");
        ia[BAS] = IMG_Load("ia_bas_starwars.png");
        ia[GAUCHE] = IMG_Load("ia_gauche_starwars.png");
        ia[DROITE] = IMG_Load("ia_droite_starwars.png");
        elements.vide = vide = IMG_Load("vide_starwars.png");
        elements.obstacle = obstacle = IMG_Load("obstacle_starwars.jpg");
        elements.particule = particule = IMG_Load("particule_starwars.png");
        elements.tempete = tempete = IMG_Load("tempete.jpg");
        elements.bonus = bonus = IMG_Load("bonus_starwars.png");
        elements.piege = piege = IMG_Load("piege.png");
        elements.boite = boite = IMG_Load("boite_starwars.png");
        elements.bloc = bloc = IMG_Load("bloc_starwars.png");
        break;
    case NARUTO:
        ia[HAUT] = IMG_Load("ia_haut_naruto.png");
        ia[BAS] = IMG_Load("ia_bas_naruto.png");
        ia[GAUCHE] = IMG_Load("ia_gauche_naruto.png");
        ia[DROITE] = IMG_Load("ia_droite_naruto.png");
        elements.vide = vide = IMG_Load("vide_naruto.png");
        elements.obstacle = obstacle = IMG_Load("obstacle_naruto.bmp");
        elements.particule = particule = IMG_Load("particule_naruto.png");
        elements.tempete = tempete = IMG_Load("tempete.jpg");
        elements.bonus = bonus = IMG_Load("bonus_naruto.png");
        elements.bloc = bloc = IMG_Load("bloc_naruto.png");
        elements.piege = piege = IMG_Load("piege.png");
        elements.boite = boite = IMG_Load("boite_naruto.png");
        break;
    case POKEMON:
        ia[HAUT] = IMG_Load("ia_haut_pokemon.png");
        ia[BAS] = IMG_Load("ia_bas_pokemon.png");
        ia[GAUCHE] = IMG_Load("ia_gauche_pokemon.png");
        ia[DROITE] = IMG_Load("ia_droite_pokemon.png");
        elements.vide = vide = IMG_Load("vide_pokemon.png");
        elements.obstacle = obstacle = IMG_Load("obstacle_pokemon.bmp");
        elements.particule = particule = IMG_Load("particule_pokemon.png");
        elements.tempete = tempete = IMG_Load("tempete.jpg");
        elements.bonus = bonus = IMG_Load("bonus_pokemon.png");
        elements.bloc = bloc = IMG_Load("bloc_pokemon.png");
        elements.piege = piege = IMG_Load("piege.png");
        elements.boite = boite = IMG_Load("boite_pokemon.png");
        break;
    }
    iaActuel = ia[BAS];


    SDL_SetColorKey(bloc, SDL_SRCCOLORKEY, SDL_MapRGB(bloc->format, 255,255, 255));
    SDL_SetColorKey(piege, SDL_SRCCOLORKEY, SDL_MapRGB(piege->format, 255,255, 255));
    SDL_SetColorKey(boite, SDL_SRCCOLORKEY, SDL_MapRGB(boite->format, 255,255, 255));
    SDL_SetColorKey(obstacle, SDL_SRCCOLORKEY, SDL_MapRGB(obstacle->format, 255,255, 255));
    SDL_SetColorKey(particule, SDL_SRCCOLORKEY, SDL_MapRGB(particule->format, 255, 255, 255));
    for(i = 0; i < 4; i++)
    {
        SDL_SetColorKey(ia[i], SDL_SRCCOLORKEY, SDL_MapRGB(ia[i]->format, 255, 255, 255));
    }

    SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 98, 126, 235));
    apparition_particule(carte, 0);
    apparition_obstacle(carte);
    afficher_jeu(carte, ecran, ia,joueur, elements);

    SDL_Rect posrectangle;
    posrectangle.x = HAUTEUR_FENETRE - (MARGE * TAILLE_BLOC);
    posrectangle.y = 190;

    rectangle = SDL_CreateRGBSurface(SDL_HWSURFACE, 500, 200, 32, 0, 0, 0, 0);
    SDL_FillRect(rectangle, NULL, SDL_MapRGB(rectangle->format, 23, 80, 75));
    SDL_BlitSurface(rectangle, NULL, ecran, &posrectangle);

    posrectangle.y = 0;

    rectangle = SDL_CreateRGBSurface(SDL_HWSURFACE, 500, 200, 32, 0, 0, 0, 0);
    SDL_FillRect(rectangle, NULL, SDL_MapRGB(rectangle->format, 23, 80, 75));
    SDL_BlitSurface(rectangle, NULL, ecran, &posrectangle);


    posrectangle.x = HAUTEUR_FENETRE - (MARGE * TAILLE_BLOC);
    posrectangle.y = 200;

    sprintf(score, "évenement :");
    texte = TTF_RenderText_Solid(police, score, noir);
    SDL_BlitSurface(texte, NULL, ecran, &posrectangle);

    SDL_Flip(ecran);

    for(k = 0; k < NBTOURS; k++)
    {
        for(l = 0; l < NBJOUEUR; l++)
        {
            apparition_boite(carte);
            while(joueur[l].etat == MORT)
            {
                afficher_gagnant(carte, ecran, joueur);
                l++;
                if(l == NBJOUEUR)
                {
                    l = 0;
                }
            }
            SDL_WaitEvent(&event);
            while(event.type != SDL_KEYDOWN)
            {
                SDL_WaitEvent(&event);
            }
            switch(event.type)
            {
            case SDL_QUIT:
                cleanup();
                exit(EXIT_SUCCESS);
                break;
            case SDL_KEYDOWN:
                switch(event.key.keysym.sym)
                {
                case SDLK_ESCAPE:
                    exit(EXIT_SUCCESS);
                    break;
                case SDLK_UP:
                    if(carte[joueur[l].position.x] [joueur[l].position.y - 1] == PARTICULE)
                    {
                        joueur[l].nbparticule++;
                    }
                    if(carte[joueur[l].position.x] [joueur[l].position.y - 1] == BONUS)
                    {
                        joueur[l].lvl += 2;
                    }
                    iaActuel = ia[HAUT];
                    deplacer_joueur(carte, HAUT, &joueur[l].position, l, joueur, ecran, 1);
                    joueur[l].direction = HAUT;
                    break;
                case SDLK_DOWN:
                    if(carte[joueur[l].position.x][joueur[l].position.y + 1] == PARTICULE)
                    {
                        joueur[l].nbparticule++;
                    }
                    if(carte[joueur[l].position.x][joueur[l].position.y + 1] == BONUS)
                    {
                        joueur[l].lvl += 2;
                    }
                    iaActuel = ia[BAS];
                    deplacer_joueur(carte, BAS, &joueur[l].position, l, joueur, ecran, 1);
                    joueur[l].direction = BAS;
                    break;
                case SDLK_LEFT:
                    if(carte[joueur[l].position.x - 1][joueur[l].position.y] == PARTICULE)
                    {
                        joueur[l].nbparticule++;
                    }
                    if(carte[joueur[l].position.x - 1][joueur[l].position.y] == BONUS)
                    {
                        joueur[l].lvl += 2;
                    }
                    iaActuel = ia[GAUCHE];
                    deplacer_joueur(carte, GAUCHE, &joueur[l].position, l, joueur, ecran, 1);
                    joueur[l].direction = GAUCHE;
                    break;
                case SDLK_RIGHT:
                    if(carte[joueur[l].position.x + 1][joueur[l].position.y] == PARTICULE)
                    {
                        joueur[l].nbparticule++;
                    }
                    if(carte[joueur[l].position.x + 1][joueur[l].position.y] == BONUS)
                    {
                        joueur[l].lvl += 2;
                    }
                    iaActuel = ia[DROITE];
                    deplacer_joueur(carte, DROITE, &joueur[l].position, l, joueur, ecran, 1);
                    joueur[l].direction = DROITE;
                    break;
                case SDLK_t:
                    tir(carte, joueur[l].position, joueur[l].direction, ecran, joueur, ia, elements);
                    break;
                case SDLK_p:
                    activation_boite(joueur[l].numero, carte, joueur[l].position, joueur[l].direction, ecran, joueur, ia, elements);
                    break;
                case SDLK_c:
                    construire_bloc(carte, joueur[l].position, joueur[l].direction);
                    break;
                }
                break;
            }
            timerApparition1 = SDL_GetTicks();
            if(timerApparition1 - timerApparition2 > 5000)
            {
                apparition_particule(carte, compteurTempete);
                timerApparition2 = timerApparition1;
            }

            timerDisparition1 = SDL_GetTicks();
            if(timerDisparition1 - timerDisparition2 > 5000)
            {
                disparition_particule(carte, compteurTempete);
                timerDisparition2 = timerDisparition1;
            }

            timerTempete1 = SDL_GetTicks();
            if(timerTempete1 - timerTempete2 > 8000)
            {
                FMOD_System_PlaySound(system, FMOD_CHANNEL_FREE, bruitTempete, 0, NULL);
                timerTempete2 = timerTempete1;
                compteurTempete++;
            }

            if(k % 3 == 0)
            {
                apparition_bonus(carte);
            }


            if(joueur[l].nbparticule >= 3)
            {
                joueur[l].lvl++;
                joueur[l].nbparticule = 0;
            }
            positionscore.y = l * texte->h ;
            sprintf(score, "lvl--%s: %d", joueur[l].nom, joueur[l].lvl);
            texte = TTF_RenderText_Shaded(police, score, noir, blanc);
            SDL_BlitSurface(texte, NULL, ecran, &positionscore);
            SDL_Flip(ecran);


            retrecir_carte(carte, compteurTempete, joueur, ecran);
            afficher_jeu(carte, ecran, ia, joueur, elements);
            afficher_top(joueur, ecran);

        }
        SDL_FreeSurface(texte);

    }


    SDL_FreeSurface(iaActuel);
    SDL_FreeSurface(obstacle);
    SDL_FreeSurface(particule);

    for(i = 0; i < 4; i++)
    {
        SDL_FreeSurface(ia[i]);
    }

    TTF_CloseFont(police);
    TTF_Quit();

    FMOD_Sound_Release(bruitTempete);
    FMOD_System_Close(system);
    FMOD_System_Release(system);
}


void apparition_particule(int carte[HAUTEUR_TAB][LARGEUR_TAB], int ecart)
{
    int nbParticule, posParticule_x, posParticule_y, i, j, k;
    nbParticule = rand() % 100 ;
    for(i = 0; i < nbParticule; i++)
    {
        posParticule_x = rand() % HAUTEUR_TAB;
        posParticule_y = rand() % LARGEUR_TAB;
        while(carte[posParticule_x][posParticule_y] == TEMPETE || carte[posParticule_x][posParticule_y] == OBSTACLE || (carte[posParticule_x][posParticule_y] >= 0 && carte[posParticule_x][posParticule_y] <= NBJOUEUR))
        {
            posParticule_x = rand() % HAUTEUR_TAB;
            posParticule_y = rand() % LARGEUR_TAB;
        }
        for(j = 0; j < HAUTEUR_TAB; j++)
        {
            for (k = 0; k < LARGEUR_TAB; k++)
            {

                if(((j == posParticule_x) && (k == posParticule_y)))
                {
                    carte[j][k] = PARTICULE;
                    j = HAUTEUR_TAB;
                    k = LARGEUR_TAB;
                }
            }
        }
    }
}

void apparition_obstacle(int carte[HAUTEUR_TAB][LARGEUR_TAB])
{
    int i, j, k,  posx, posy;
    for(i = 0; i < 50; i++)
    {
        posx = rand() % HAUTEUR_TAB;
        posy = rand() % LARGEUR_TAB;
        while((carte[posx][posy] >= 0 && carte[posx][posy] <= NBJOUEUR))
        {
            posx = rand() % HAUTEUR_TAB;
            posy = rand() % LARGEUR_TAB;
        }
        for(j = 0; j < HAUTEUR_TAB; j++)
        {
            for(k = 0; k < LARGEUR_TAB; k++)
            {
                if(j == posx && k == posy)
                {
                    carte[j][k] = OBSTACLE;
                }
            }
        }
    }
}


void deplacer_joueur(int carte[HAUTEUR_TAB][LARGEUR_TAB], int direction, SDL_Rect *position, int joueur, InfoIa joueurs[NBJOUEUR], SDL_Surface *ecran, int ecart)
{
    int numero;

    SDL_Surface *texte, *rectangle;
    if(TTF_Init() == -1)
    {
        fprintf(stderr, "Erreur d'initialisation de TTF_Init : %s\n", TTF_GetError());
        exit(EXIT_FAILURE);
    }
    TTF_Font *police = NULL;
    police = TTF_OpenFont("starjedi.ttf", 8);
    SDL_Color noir = {0, 0, 0};
    char declaration[300];
    SDL_Rect postexte;
    postexte.x = HAUTEUR_FENETRE - (MARGE * TAILLE_BLOC);
    postexte.y = 250;

    switch(direction)
    {
    case HAUT:
        if((position->y - ecart) < 0 || carte[position->x][position->y - ecart] == OBSTACLE || carte[position->x][position->y - ecart] == BLOC)
        {
            break;
        }

        if(carte[position->x][position->y - ecart] == TRAP)
        {
            carte[position->x][position->y - ecart] = VIDE;
            carte[position->x][position->y] = VIDE;
            joueurs[joueur].etat = MORT;
            position->y -= ecart;
            explosion(ecran, *position);
            sprintf(declaration, "%s est tomber dans un piege", joueurs[joueur].nom);
            texte = TTF_RenderText_Solid(police, declaration, noir);
            rectangle = SDL_CreateRGBSurface(SDL_HWSURFACE, texte->w + 50, texte->h, 32, 0, 0, 0, 0);
            SDL_FillRect(rectangle, NULL, SDL_MapRGB(rectangle->format, 23, 80, 75));
            SDL_BlitSurface(rectangle, NULL, ecran, &postexte);
            SDL_BlitSurface(texte, NULL, ecran, &postexte);
            SDL_Flip(ecran);
            break;
        }

        if(carte[position->x][position->y - ecart] == BOITE)
        {
            joueurs[joueur].bonus = rand() % 3;
            carte[position->x][position->y - ecart] = joueur;
            carte[position->x][position->y] = VIDE;
            position->y -= ecart;
            break;
        }

        if(carte[position->x][position->y - ecart] == TEMPETE)
        {
            carte[position->x][position->y] = VIDE;
            joueurs[joueur].etat = MORT;
            sprintf(declaration, "%s s'est suicider dans la tempete", joueurs[joueur].nom);
            texte = TTF_RenderText_Solid(police, declaration, noir);
            rectangle = SDL_CreateRGBSurface(SDL_HWSURFACE, texte->w + 50, texte->h, 32, 0, 0, 0, 0);
            SDL_FillRect(rectangle, NULL, SDL_MapRGB(rectangle->format, 23, 80, 75));
            SDL_BlitSurface(rectangle, NULL, ecran, &postexte);
            SDL_BlitSurface(texte, NULL, ecran, &postexte);
            SDL_Flip(ecran);
        }
        if(carte[position->x][position->y - ecart] <= NBJOUEUR && carte[position->x][position->y - ecart] >= 0)
        {
            numero = carte[position->x][position->y - ecart];
            if(joueurs[joueur].lvl < joueurs[numero].lvl)
            {
                carte[position->x][position->y] = VIDE;
                joueurs[joueur].etat = MORT;
                joueurs[numero].lvl += joueurs[joueur].lvl;
                sprintf(declaration, "%s s'est suicider", joueurs[joueur].nom);
                texte = TTF_RenderText_Solid(police, declaration, noir);
                rectangle = SDL_CreateRGBSurface(SDL_HWSURFACE, texte->w + 50, texte->h, 32, 0, 0, 0, 0);
                SDL_FillRect(rectangle, NULL, SDL_MapRGB(rectangle->format, 23, 80, 75));
                SDL_BlitSurface(rectangle, NULL, ecran, &postexte);
                SDL_BlitSurface(texte, NULL, ecran, &postexte);
                SDL_Flip(ecran);
                break;
            }
            else if(joueurs[joueur].lvl > joueurs[numero].lvl)
            {
                carte[position->x][position->y] = VIDE;
                carte[position->x][position->y - ecart] = joueur;
                joueurs[numero].etat = MORT;
                joueurs[joueur].lvl += joueurs[numero].lvl;
                position->y -= ecart;
                sprintf(declaration, "%s a manger %s", joueurs[joueur].nom, joueurs[numero].nom);
                texte = TTF_RenderText_Solid(police, declaration, noir);
                rectangle = SDL_CreateRGBSurface(SDL_HWSURFACE, texte->w + 50, texte->h, 32, 0, 0, 0, 0);
                SDL_FillRect(rectangle, NULL, SDL_MapRGB(rectangle->format, 23, 80, 75));
                SDL_BlitSurface(rectangle, NULL, ecran, &postexte);
                SDL_BlitSurface(texte, NULL, ecran, &postexte);
                SDL_Flip(ecran);
                break;
            }
            else
            {
                break;
            }
        }
        carte[position->x][position->y] = VIDE;
        carte[position->x][position->y - ecart] = joueur;
        position->y -= ecart;
        break;
    case BAS:
        if((position->y + ecart) > LARGEUR_TAB - 1 || carte[position->x][position->y + ecart] == OBSTACLE || carte[position->x][position->y + ecart] == BLOC)
        {
            break;
        }

        if(carte[position->x][position->y + ecart] == TRAP)
        {
            carte[position->x][position->y + ecart] = VIDE;
            carte[position->x][position->y] = VIDE;
            joueurs[joueur].etat = MORT;
            position->y += ecart;
            explosion(ecran, *position);
            sprintf(declaration, "%s est tomber dans un piege", joueurs[joueur].nom);
            texte = TTF_RenderText_Solid(police, declaration, noir);
            rectangle = SDL_CreateRGBSurface(SDL_HWSURFACE, texte->w + 50, texte->h, 32, 0, 0, 0, 0);
            SDL_FillRect(rectangle, NULL, SDL_MapRGB(rectangle->format, 23, 80, 75));
            SDL_BlitSurface(rectangle, NULL, ecran, &postexte);
            SDL_BlitSurface(texte, NULL, ecran, &postexte);
            SDL_Flip(ecran);
            break;

        }

        if(carte[position->x][position->y + ecart] == BOITE)
        {
            joueurs[joueur].bonus = rand() % 3;
            carte[position->x][position->y + ecart] = joueur;
            carte[position->x][position->y] = VIDE;
            position->y += ecart;
            break;
        }

        if(carte[position->x][position->y + ecart] == TEMPETE)
        {
            carte[position->x][position->y] = VIDE;
            joueurs[joueur].etat = MORT;
            sprintf(declaration, "%s s'est suicider dans la tempete", joueurs[joueur].nom);
            texte = TTF_RenderText_Solid(police, declaration, noir);
            rectangle = SDL_CreateRGBSurface(SDL_HWSURFACE, texte->w + 50, texte->h, 32, 0, 0, 0, 0);
            SDL_FillRect(rectangle, NULL, SDL_MapRGB(rectangle->format, 23, 80, 75));
            SDL_BlitSurface(rectangle, NULL, ecran, &postexte);
            SDL_BlitSurface(texte, NULL, ecran, &postexte);
            SDL_Flip(ecran);

        }
        if(carte[position->x][position->y + ecart] < NBJOUEUR && carte[position->x][position->y + 1] >= 0)
        {
            numero = carte[position->x][position->y + ecart];
            if(joueurs[joueur].lvl < joueurs[numero].lvl)
            {
                carte[position->x][position->y] = VIDE;
                joueurs[joueur].etat = MORT;
                joueurs[numero].lvl += joueurs[joueur].lvl;
                sprintf(declaration, "%s s'est suicider", joueurs[joueur].nom);
                texte = TTF_RenderText_Solid(police, declaration, noir);
                rectangle = SDL_CreateRGBSurface(SDL_HWSURFACE, texte->w + 50, texte->h, 32, 0, 0, 0, 0);
                SDL_FillRect(rectangle, NULL, SDL_MapRGB(rectangle->format, 23, 80, 75));
                SDL_BlitSurface(rectangle, NULL, ecran, &postexte);
                SDL_BlitSurface(texte, NULL, ecran, &postexte);
                SDL_Flip(ecran);
                break;
            }
            else if (joueurs[joueur].lvl > joueurs[numero].lvl)
            {
                carte[position->x][position->y] = VIDE;
                carte[position->x][position->y + ecart] = joueur;
                joueurs[numero].etat = MORT;
                joueurs[joueur].lvl += joueurs[numero].lvl;
                position->y += ecart;
                sprintf(declaration, "%s a manger %s", joueurs[joueur].nom, joueurs[numero].nom);
                texte = TTF_RenderText_Solid(police, declaration, noir);
                rectangle = SDL_CreateRGBSurface(SDL_HWSURFACE, texte->w + 50, texte->h, 32, 0, 0, 0, 0);
                SDL_FillRect(rectangle, NULL, SDL_MapRGB(rectangle->format, 23, 80, 75));
                SDL_BlitSurface(rectangle, NULL, ecran, &postexte);
                SDL_BlitSurface(texte, NULL, ecran, &postexte);
                SDL_Flip(ecran);
                break;
            }
            else
            {
                break;
            }
        }

        carte[position->x][position->y] = VIDE;
        carte[position->x][position->y + ecart] = joueur;
        position->y += ecart;
        break;
    case GAUCHE:
        if((position->x - ecart) < 0 || carte[position->x - ecart ][position->y] == OBSTACLE || carte[position->x - ecart][position->y] == BLOC)
        {
            break;
        }

        if(carte[position->x - ecart][position->y] == TRAP)
        {
            carte[position->x - ecart][position->y] = VIDE;
            carte[position->x][position->y] = VIDE;
            joueurs[joueur].etat = MORT;
            position->x -= ecart;
            explosion(ecran, *position);
            sprintf(declaration, "%s est tomber dans un piege", joueurs[joueur].nom);
            texte = TTF_RenderText_Solid(police, declaration, noir);
            rectangle = SDL_CreateRGBSurface(SDL_HWSURFACE, texte->w + 50, texte->h, 32, 0, 0, 0, 0);
            SDL_FillRect(rectangle, NULL, SDL_MapRGB(rectangle->format, 23, 80, 75));
            SDL_BlitSurface(rectangle, NULL, ecran, &postexte);
            SDL_BlitSurface(texte, NULL, ecran, &postexte);
            SDL_Flip(ecran);
            break;
        }

        if(carte[position->x - ecart ][position->y] == BOITE)
        {
            joueurs[joueur].bonus = rand() % 3;
            carte[position->x - ecart ][position->y] = joueur;
            carte[position->x][position->y] = VIDE;
            position->x -= ecart;
            break;
        }

        if(carte[position->x - ecart ][position->y] == TEMPETE)
        {
            carte[position->x][position->y] = VIDE;
            joueurs[joueur].etat = MORT;
            sprintf(declaration, "%s s'est suicider dans la tempete", joueurs[joueur].nom);
            texte = TTF_RenderText_Solid(police, declaration, noir);
            rectangle = SDL_CreateRGBSurface(SDL_HWSURFACE, texte->w + 50, texte->h, 32, 0, 0, 0, 0);
            SDL_FillRect(rectangle, NULL, SDL_MapRGB(rectangle->format, 23, 80, 75));
            SDL_BlitSurface(rectangle, NULL, ecran, &postexte);
            SDL_BlitSurface(texte, NULL, ecran, &postexte);
            SDL_Flip(ecran);
        }

        if(carte[position->x - ecart][position->y] <= NBJOUEUR && carte[position->x - ecart][position->y] >= 0)
        {
            numero = carte[position->x - ecart][position->y];
            if(joueurs[joueur].lvl < joueurs[numero].lvl)
            {
                carte[position->x][position->y] = VIDE;
                joueurs[joueur].etat = MORT;
                joueurs[numero].lvl += joueurs[joueur].lvl;
                sprintf(declaration, "%s s'est suicider", joueurs[joueur].nom);
                texte = TTF_RenderText_Solid(police, declaration,noir);
                rectangle = SDL_CreateRGBSurface(SDL_HWSURFACE, texte->w + 50, texte->h, 32, 0, 0, 0, 0);
                SDL_FillRect(rectangle, NULL, SDL_MapRGB(rectangle->format, 23, 80, 75));
                SDL_BlitSurface(rectangle, NULL, ecran, &postexte);
                SDL_BlitSurface(texte, NULL, ecran, &postexte);
                SDL_Flip(ecran);
                break;
            }
            else if(joueurs[joueur].lvl > joueurs[numero].lvl)
            {
                carte[position->x][position->y] = VIDE;
                carte[position->x - ecart][position->y] = joueur;
                joueurs[numero].etat = MORT;
                joueurs[joueur].lvl += joueurs[numero].lvl;
                position->x -= ecart;
                sprintf(declaration, "%s a manger %s", joueurs[joueur].nom, joueurs[numero].nom);
                texte = TTF_RenderText_Solid(police, declaration, noir);
                rectangle = SDL_CreateRGBSurface(SDL_HWSURFACE, texte->w + 50, texte->h, 32, 0, 0, 0, 0);
                SDL_FillRect(rectangle, NULL, SDL_MapRGB(rectangle->format, 23, 80, 75));
                SDL_BlitSurface(rectangle, NULL, ecran, &postexte);
                SDL_BlitSurface(texte, NULL, ecran, &postexte);
                SDL_Flip(ecran);
                break;
            }
            else
            {
                break;
            }
        }
        carte[position->x][position->y] = VIDE;
        carte[position->x - ecart][position->y] = joueur;
        position->x -= ecart;
        break;
    case DROITE:
        if((position->x + ecart) > HAUTEUR_TAB - 1 || carte[position->x + ecart ][position->y] == OBSTACLE || carte[position->x + ecart][position->y] == BLOC)
        {
            break;
        }

       if(carte[position->x + ecart][position->y] == TRAP)
        {
            carte[position->x + ecart][position->y] = VIDE;
            carte[position->x][position->y] = VIDE;
            joueurs[joueur].etat = MORT;
            position->x += ecart;
            explosion(ecran, *position);
            sprintf(declaration, "%s est tomber dans un piege", joueurs[joueur].nom);
            texte = TTF_RenderText_Solid(police, declaration, noir);
            rectangle = SDL_CreateRGBSurface(SDL_HWSURFACE, texte->w + 50, texte->h, 32, 0, 0, 0, 0);
            SDL_FillRect(rectangle, NULL, SDL_MapRGB(rectangle->format, 23, 80, 75));
            SDL_BlitSurface(rectangle, NULL, ecran, &postexte);
            SDL_BlitSurface(texte, NULL, ecran, &postexte);
            SDL_Flip(ecran);
            break;
        }

        if(carte[position->x + ecart ][position->y] == BOITE)
        {
            joueurs[joueur].bonus = rand() % 3;
            carte[position->x + ecart ][position->y] = joueur;
            carte[position->x][position->y] = VIDE;
            position->x += ecart;
            break;
        }

        if(carte[position->x + ecart ][position->y] == TEMPETE)
        {
            carte[position->x][position->y] = VIDE;
            joueurs[joueur].etat = MORT;
            sprintf(declaration, "%s s'est suicider dans la tempete", joueurs[joueur].nom);
            texte = TTF_RenderText_Solid(police, declaration,noir);
            rectangle = SDL_CreateRGBSurface(SDL_HWSURFACE, texte->w + 50, texte->h, 32, 0, 0, 0, 0);
            SDL_FillRect(rectangle, NULL, SDL_MapRGB(rectangle->format, 23, 80, 75));
            SDL_BlitSurface(rectangle, NULL, ecran, &postexte);
            SDL_BlitSurface(texte, NULL, ecran, &postexte);
            SDL_Flip(ecran);
        }

        if(carte[position->x + ecart][position->y] <= NBJOUEUR && carte[position->x + ecart][position->y] >= 0)
        {
            numero = carte[position->x + ecart][position->y];
            if(joueurs[joueur].lvl < joueurs[numero].lvl)
            {
                carte[position->x][position->y] = VIDE;
                joueurs[joueur].etat = MORT;
                joueurs[numero].lvl += joueurs[joueur].lvl;
                sprintf(declaration, "%s s'est suicider", joueurs[joueur].nom);
                texte = TTF_RenderText_Solid(police, declaration, noir);
                rectangle = SDL_CreateRGBSurface(SDL_HWSURFACE, texte->w + 50, texte->h, 32, 0, 0, 0, 0);
                SDL_FillRect(rectangle, NULL, SDL_MapRGB(rectangle->format, 23, 80, 75));
                SDL_BlitSurface(rectangle, NULL, ecran, &postexte);
                SDL_BlitSurface(texte, NULL, ecran, &postexte);
                SDL_Flip(ecran);
                break;
            }
            else if(joueurs[joueur].lvl > joueurs[numero].lvl)
            {
                carte[position->x][position->y] = VIDE;
                carte[position->x + ecart][position->y] = joueur;
                joueurs[numero].etat = MORT;
                joueurs[joueur].lvl += joueurs[numero].lvl;
                position->x += ecart;
                sprintf(declaration, "%s a manger %s", joueurs[joueur].nom, joueurs[numero].nom);
                texte = TTF_RenderText_Solid(police, declaration, noir);
                rectangle = SDL_CreateRGBSurface(SDL_HWSURFACE, texte->w + 50, texte->h, 32, 0, 0, 0, 0);
                SDL_FillRect(rectangle, NULL, SDL_MapRGB(rectangle->format, 23, 80, 75));
                SDL_BlitSurface(rectangle, NULL, ecran, &postexte);
                SDL_BlitSurface(texte, NULL, ecran, &postexte);
                SDL_Flip(ecran);
                break;
            }
            if(joueurs[joueur].lvl == joueurs[numero].lvl)
            {
                break;
            }
        }
        carte[position->x + ecart][position->y] = joueur;
        carte[position->x][position->y] = VIDE;
        position->x += ecart;
        break;
    }
}

void disparition_particule(int carte[HAUTEUR_TAB][LARGEUR_TAB], int ecart)
{
    int i, j, k,  posx, posy, nbparticule;
    nbparticule = rand() % 100;
    for(i = 0; i < nbparticule; i++)
    {
        posx = rand() % HAUTEUR_TAB;
        posy = rand() % LARGEUR_TAB;
        while(carte[posx][posy] == TEMPETE)
        {
            posx = rand() % HAUTEUR_TAB;
            posy = rand() % LARGEUR_TAB;
        }
        for(j = posx; j < HAUTEUR_TAB; j++)
        {
            for(k = posy; k < LARGEUR_TAB; k++)
            {
                if(carte[j][k] == PARTICULE)
                {
                    carte[j][k] = VIDE;
                    j = HAUTEUR_TAB;
                    k = LARGEUR_TAB;
                }
            }
        }
    }
}


void retrecir_carte(int carte[HAUTEUR_TAB][LARGEUR_TAB], int ecart, InfoIa joueur[NBJOUEUR], SDL_Surface *ecran)
{
    int i,j, numero;

    SDL_Surface *texte, *rectangle;
    if(TTF_Init() == -1)
    {
        fprintf(stderr, "Erreur d'initialisation de TTF_Init : %s\n", TTF_GetError());
        exit(EXIT_FAILURE);
    }
    TTF_Font *police = NULL;
    police = TTF_OpenFont("Starjedi.ttf", 8);
    SDL_Color noir = {0, 0, 0}, blanc = {255, 255, 255};
    char declaration[300];
    SDL_Rect postexte;
    postexte.x = HAUTEUR_FENETRE - (MARGE * TAILLE_BLOC);
    postexte.y = 250;

    for(i = 0; i < ecart; i++)
    {
        for(j = 0; j < HAUTEUR_TAB; j++)
        {
            if(carte[j][i] >= 0 && carte[j][i] < NBJOUEUR)
            {
                numero = carte[j][i];
                joueur[numero].etat = MORT;
                sprintf(declaration, "la tempete a englouti %s", joueur[numero].nom);
                texte = TTF_RenderText_Solid(police, declaration, noir);
                rectangle = SDL_CreateRGBSurface(SDL_HWSURFACE, texte->w + 50, texte->h, 32, 0, 0, 0, 0);
                SDL_FillRect(rectangle, NULL, SDL_MapRGB(rectangle->format, 23, 80, 75));
                SDL_BlitSurface(rectangle, NULL, ecran, &postexte);
                SDL_BlitSurface(texte, NULL, ecran, &postexte);
                SDL_Flip(ecran);
                SDL_FreeSurface(texte);
                SDL_FreeSurface(rectangle);
            }
            carte[j][i] = TEMPETE;
        }
    }

    for(i = 0; i < ecart; i++)
    {
        for(j = 0; j < LARGEUR_TAB; j++)
        {
            if(carte[i][j] >= 0 && carte[i][j] <= NBJOUEUR)
            {
                numero = carte[i][j];
                joueur[numero].etat = MORT;
            }
            carte[i][j] = TEMPETE;
        }
    }

    for(i = 0; i < ecart; i++)
    {
        for(j = 0; j < HAUTEUR_TAB; j++)
        {
            if(carte[j][LARGEUR_TAB - (i + 1)] >= 0 && carte[j][LARGEUR_TAB - (i + 1)] <= NBJOUEUR)
            {
                numero = carte[j][LARGEUR_TAB - (i + 1)];
                joueur[numero].etat = MORT;
            }
            carte[j][LARGEUR_TAB - (i + 1)] = TEMPETE;
        }
    }

    for(i = 0; i < ecart; i++)
    {
        for(j = 0; j < LARGEUR_TAB; j++)
        {
            if(carte[HAUTEUR_TAB - (i + 1)][j] >= 0 && carte[HAUTEUR_TAB - (i + 1)][j] <= NBJOUEUR)
            {
                numero = carte[HAUTEUR_TAB - (i + 1)][j];
                joueur[numero].etat = MORT;
            }
            carte[HAUTEUR_TAB - (i + 1)][j] = TEMPETE;
        }
    }
}


SDL_Rect placer_joueur(int carte[HAUTEUR_TAB][LARGEUR_TAB], int numero)
{
    int posx, posy;
    SDL_Rect position;

    posx = rand() % HAUTEUR_TAB;
    posy = rand() % LARGEUR_TAB;
    while((carte[posx - 1][posy] >= 0 && carte[posx - 1][posy] <= NBJOUEUR) || (carte[posx + 1][posy] >= 0 && carte[posx + 1][posy] <= NBJOUEUR) || (carte[posx][posy - 1] >= 0 && carte[posx][posy - 1] <= NBJOUEUR) || (carte[posx][posy + 1] >= 0 && carte[posx][posy + 1] <= NBJOUEUR))
    {
        posx = rand() % HAUTEUR_TAB;
        posy = rand() % LARGEUR_TAB;
    }

    carte[posx][posy] = numero;
    position.x = posx;
    position.y = posy;
    return position;
}


void afficher_gagnant(int carte[HAUTEUR_TAB][LARGEUR_TAB], SDL_Surface *ecran, InfoIa joueur[NBJOUEUR])
{
    int i, j, joueursRestant = 0, numero, numeroBis, lvlmax, nbegalite = 1;
    TTF_Font *police = NULL;
    SDL_Color fond = {255, 255, 255}, blanc = {0, 0, 0};
    SDL_Surface *texte;
    SDL_Rect position;
    char gagnant[300];
    police = TTF_OpenFont("Starjedi.ttf", 20);


    for(i = 0; i < HAUTEUR_TAB; i++)
    {
        for(j = 0; j < LARGEUR_TAB; j++)
        {
            if(carte[i][j] <= NBJOUEUR && carte[i][j] >= 0)
            {
                joueursRestant++;
            }
        }
    }
    if(joueursRestant == 1)
    {
        for(i = 0; i < HAUTEUR_TAB; i++)
        {
            for(j = 0; j < LARGEUR_TAB; j++)
            {
                if(carte[i][j] <= NBJOUEUR && carte[i][j] >= 0)
                {
                    numero = carte[i][j];
                }
            }
        }
        SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 0, 0, 0));
        sprintf(gagnant, "Partie terminée !!! le combatant qui a gagné cette partie est %s", joueur[numero].nom);
        texte = TTF_RenderText_Shaded(police, gagnant, fond, blanc);
        position.x = (ecran->w / 2) - (texte->w / 2);
        position.y = (ecran->h / 2) - (texte->h / 2);
        SDL_BlitSurface(texte, NULL, ecran, &position);
        SDL_Flip(ecran);
        SDL_Delay(5000);
        SDL_FreeSurface(texte);
        cleanup();
        exit(EXIT_SUCCESS);
    }
    if(joueursRestant == 0)
    {
        lvlmax = joueur[0].lvl;
        for(i = 0; i < NBJOUEUR; i++)
        {
            if(joueur[i].lvl > lvlmax)
            {
                lvlmax = joueur[i].lvl;
                numero = i;
            }
        }
        for(i = 0; i < NBJOUEUR; i++)
        {
            if(joueur[i].lvl == lvlmax && i != numero)
            {
                nbegalite++;
                numeroBis = i;
            }
        }

        if(nbegalite == 2)
        {
            TTF_CloseFont(police);
            police = TTF_OpenFont("Starjedi.ttf", 18);
            if(joueur[numero].nbparticule > joueur[numeroBis].nbparticule)
            {
                sprintf(gagnant, "%s et %s on le meme niveau, cependant %s a manger %d particule et %s %d, le gagnant est %s", joueur[numero].nom, joueur[numeroBis].nom, joueur[numero].nom, joueur[numero].nbparticule, joueur[numeroBis].nom, joueur[numeroBis].nbparticule, joueur[numero].nom);
            }
            else if(joueur[numero].nbparticule < joueur[numeroBis].nbparticule)
            {
                sprintf(gagnant, "%s et %s on le meme niveau, cependant %s a manger %d particule et %s %d, le gagnant est %s", joueur[numeroBis].nom, joueur[numero].nom, joueur[numeroBis].nom, joueur[numeroBis].nbparticule, joueur[numero].nom, joueur[numero].nbparticule, joueur[numeroBis].nom);
            }
            else if (joueur[numero].nbparticule == joueur[numeroBis].nbparticule)
            {
                sprintf(gagnant, "%s et %s sont completement ex aequo. Veuillez refaire une partie pour les departager", joueur[numero].nom, joueur[numeroBis].nom);
            }
        }
        else if(nbegalite > 2)
        {
            sprintf(gagnant, "il y'a plus de deux personne qui sont du meme niveau, veuillez recommencer une partie");
        }
        else if(nbegalite < 2)
        {
            sprintf(gagnant, "il ne reste aucun survivant : le gagnant est %s avec un niveau de %d", joueur[numero].nom, joueur[numero].lvl);
        }
        SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 0, 0, 0));
        texte = TTF_RenderText_Shaded(police, gagnant, fond, blanc);
        position.x = (ecran->w / 2) - (texte->w / 2);
        position.y = (ecran->h / 2) - (texte->h / 2);
        SDL_BlitSurface(texte, NULL, ecran, &position);
        SDL_Flip(ecran);
        SDL_Delay(5000);
        SDL_FreeSurface(texte);
        cleanup();
        exit(EXIT_SUCCESS);
    }
    TTF_CloseFont(police);
}


void afficher_jeu(int carte[HAUTEUR_TAB][LARGEUR_TAB], SDL_Surface *ecran, SDL_Surface *ia[4], InfoIa joueur[NBJOUEUR], ElementCarte element)
{
    int i,j, numero;
    SDL_Rect positionObjet;
    SDL_SetColorKey(element.bonus, SDL_SRCCOLORKEY, SDL_MapRGB(element.bonus->format, 255, 255, 255));
    for(i = 0; i < HAUTEUR_TAB; i++)
    {
        for(j = 0; j < LARGEUR_TAB; j++)
        {
            positionObjet.x = i * TAILLE_BLOC + (MARGE * TAILLE_BLOC);
            positionObjet.y = j * TAILLE_BLOC;
            switch(carte[i][j])
            {
            case VIDE:
                SDL_BlitSurface(element.vide, NULL, ecran, &positionObjet);
                break;
            case OBSTACLE:
                SDL_BlitSurface(element.vide, NULL, ecran, &positionObjet);
                SDL_BlitSurface(element.obstacle, NULL, ecran, &positionObjet);
                break;
            case PARTICULE:
                SDL_BlitSurface(element.vide, NULL, ecran, &positionObjet);
                SDL_BlitSurface(element.particule, NULL, ecran, &positionObjet);
                break;
            case TEMPETE:
                SDL_BlitSurface(element.vide, NULL, ecran, &positionObjet);
                SDL_BlitSurface(element.tempete, NULL, ecran, &positionObjet);
                break;
            case BONUS:
                SDL_BlitSurface(element.vide, NULL, ecran, &positionObjet);
                SDL_BlitSurface(element.bonus, NULL, ecran, &positionObjet);
                break;
            case TRAP:
                SDL_BlitSurface(element.vide, NULL, ecran, &positionObjet);
                SDL_BlitSurface(element.piege, NULL, ecran, &positionObjet);
                break;
            case BLOC:
                SDL_BlitSurface(element.vide, NULL, ecran, &positionObjet);
                SDL_BlitSurface(element.bloc, NULL, ecran, &positionObjet);
                break;
            case BOITE:
                SDL_BlitSurface(element.vide, NULL, ecran, &positionObjet);
                SDL_BlitSurface(element.boite, NULL, ecran, &positionObjet);
                break;
            }
            if(carte[i][j] >= 0 && carte[i][j] < NBJOUEUR)
            {
                numero = carte[i][j];
                switch(joueur[numero].direction)
                {
                case HAUT:
                    SDL_BlitSurface(element.vide, NULL, ecran, &positionObjet);
                    SDL_BlitSurface(ia[HAUT], NULL, ecran, &positionObjet);
                    break;
                case BAS:
                    SDL_BlitSurface(element.vide, NULL, ecran, &positionObjet);
                    SDL_BlitSurface(ia[BAS], NULL, ecran, &positionObjet);
                    break;
                case GAUCHE:
                    SDL_BlitSurface(element.vide, NULL, ecran, &positionObjet);
                    SDL_BlitSurface(ia[GAUCHE], NULL, ecran, &positionObjet);
                    break;
                case DROITE:
                    SDL_BlitSurface(element.vide, NULL, ecran, &positionObjet);
                    SDL_BlitSurface(ia[DROITE], NULL, ecran, &positionObjet);
                    break;
                }
            }
        }
    }
    SDL_Flip(ecran);
}



void tir(int carte[HAUTEUR_TAB][LARGEUR_TAB], SDL_Rect pos, int direction, SDL_Surface *ecran, InfoIa joueur[NBJOUEUR], SDL_Surface *ia[4], ElementCarte element)
{
    int i,j, numero;
    SDL_Rect position;
    switch(direction)
    {
    case HAUT:
        for(i = 1; i <= PORTEE_TIR; i++)
        {
            if(carte[pos.x][pos.y - i] == PARTICULE || carte[pos.x][pos.y - i] == BONUS)
            {
                carte[pos.x][pos.y - i] = VIDE;
            }

            if(carte[pos.x][pos.y - i] == BLOC)
            {
                carte[pos.x][pos.y - i] = VIDE;
                position.x = pos.x;
                position.y = pos.y - i;
                break;
            }


            if(pos.y - i < 0)
            {
                position.x = pos.x;
                position.y = ((pos.y - i) + 1);
                break;
            }

            if(carte[pos.x][pos.y - i] < NBJOUEUR && carte[pos.x][pos.y - i] >= 0)
            {
                numero = carte[pos.x][pos.y - i];
                if(joueur[numero].lvl >= 2)
                {
                    joueur[numero].lvl -= 2;
                }
                position.x = pos.x;
                position.y = pos.y - i;
                break;
            }
            if(carte[pos.x][pos.y - i] == OBSTACLE || carte[pos.x][pos.y - i] == TEMPETE)
            {
                position.x = pos.x;
                position.y = pos.y - i;
                break;
            }
            position.x = pos.x;
            position.y = pos.y - PORTEE_TIR;
        }
        break;
    case BAS:
        for(i = 1; i <= PORTEE_TIR; i++)
        {
            if(carte[pos.x][pos.y + i] == PARTICULE || carte[pos.x][pos.y + i] == BONUS)
            {
                carte[pos.x][pos.y + 1] = VIDE;
            }

            if(carte[pos.x][pos.y + i] == BLOC)
            {
                carte[pos.x][pos.y + i] = VIDE;
                position.x = pos.x;
                position.y = pos.y + i;
                break;
            }

            if(pos.y + i > LARGEUR_TAB)
            {
                position.x = pos.x;
                position.y = ((pos.y + i) - 1);
                break;
            }

            if(carte[pos.x][pos.y + i] < NBJOUEUR && carte[pos.x][pos.y + i] >= 0)
            {
                numero = carte[pos.x][pos.y + i];
                if(joueur[numero].lvl >= 2)
                {
                    joueur[numero].lvl -= 2;
                }
                position.x = pos.x;
                position.y = pos.y + i;
                break;
            }
            if(carte[pos.x][pos.y + i] == OBSTACLE || carte[pos.x][pos.y + i] == TEMPETE)
            {
                position.x = pos.x;
                position.y = pos.y + i;
                break;
            }
            position.x = pos.x;
            position.y = pos.y + PORTEE_TIR;
        }

        break;
    case GAUCHE:
        for(i = 1; i <= PORTEE_TIR; i++)
        {
            if(carte[pos.x - i][pos.y] == PARTICULE || carte[pos.x - i][pos.y] == BONUS)
            {
                carte[pos.x - i][pos.y] = VIDE;
            }

            if(carte[pos.x - i][pos.y] == BLOC)
            {
                carte[pos.x - i][pos.y] = VIDE;
                position.x = pos.x - i;
                position.y = pos.y;
                break;
            }

            if(pos.x - i < 0)
            {
                position.x = (pos.x - i) + 1;
                position.y = pos.y;
                break;
            }

            if(carte[pos.x - i][pos.y] < NBJOUEUR && carte[pos.x - i][pos.y] >= 0)
            {
                numero = carte[pos.x - i][pos.y];
                if(joueur[numero].lvl >= 2)
                {
                    joueur[numero].lvl -= 2;
                }
                position.x = pos.x - i;
                position.y = pos.y;
                break;
            }
            if(carte[pos.x - i][pos.y] == OBSTACLE || carte[pos.x - i][pos.y] == TEMPETE)
            {
                position.x = pos.x - i;
                position.y = pos.y;
                break;
            }
            position.x = pos.x - PORTEE_TIR;
            position.y = pos.y;
        }
        break;
    case DROITE:
        for(i = 1; i <= PORTEE_TIR; i++)
        {
            if(carte[pos.x + i][pos.y] == PARTICULE || carte[pos.x + i][pos.y] == BONUS)
            {
                carte[pos.x + i][pos.y] = VIDE;
            }

            if(carte[pos.x + i][pos.y] == BLOC)
            {
                carte[pos.x + i][pos.y] = VIDE;
                position.x = pos.x + i;
                position.y = pos.y;
                break;
            }

            if(pos.x + i > HAUTEUR_TAB)
            {
                position.x = (pos.x + i) - 1;
                position.y = pos.y;
                break;
            }

            if(carte[pos.x + i][pos.y] < NBJOUEUR && carte[pos.x + i][pos.y] >= 0)
            {
                numero = carte[pos.x + i][pos.y];
                if(joueur[numero].lvl >= 2)
                {
                    joueur[numero].lvl -= 2;
                }
                position.x = pos.x + i;
                position.y = pos.y;
                break;
            }
            if(carte[pos.x + i][pos.y] == OBSTACLE || carte[pos.x + i][pos.y] == TEMPETE)
            {
                position.x = pos.x + i;
                position.y = pos.y;
                break;
            }
            position.x = pos.x + PORTEE_TIR;
            position.y = pos.y;
        }

        break;
    }
    animation_tir(carte, ecran, ia, joueur, pos, element, position, direction);

}


void animation_tir(int carte[HAUTEUR_TAB][LARGEUR_TAB], SDL_Surface *ecran, SDL_Surface *ia[4], InfoIa joueur[NBJOUEUR], SDL_Rect posJoueur, ElementCarte element,  SDL_Rect posCible, int direction)
{
    int i;
    SDL_Surface *tir[4], *explosions[7];

    FMOD_SYSTEM *system;
    FMOD_SOUND *missile, *impact;
    FMOD_System_Create(&system);
    FMOD_System_Init(system, 32, FMOD_INIT_NORMAL, NULL);
    FMOD_System_CreateSound(system, "missile.wav", FMOD_CREATESAMPLE, 0, &missile);
    FMOD_System_CreateSound(system, "explosion.wav", FMOD_CREATESAMPLE, 0, &impact);

    tir[BAS] = IMG_Load("tir_bas.png");
    tir[HAUT] = IMG_Load("tir_haut.png");
    tir[GAUCHE] = IMG_Load("tir_gauche.png");
    tir[DROITE] = IMG_Load("tir_droite.png");
    explosions[0] = IMG_Load("explosion0.png");
    explosions[1] = IMG_Load("explosion1.png");
    explosions[2] = IMG_Load("explosion2.png");
    explosions[3] = IMG_Load("explosion3.png");
    explosions[4] = IMG_Load("explosion4.png");
    explosions[5] = IMG_Load("explosion5.png");
    explosions[6] = IMG_Load("explosion6.png");

    for(i = 0; i < 4; i++)
    {
        SDL_SetColorKey(tir[i], SDL_SRCCOLORKEY, SDL_MapRGB(tir[i]->format, 255, 255, 255));
    }

    for(i = 0; i < 7; i++)
    {
        SDL_SetColorKey(explosions[i], SDL_SRCCOLORKEY, SDL_MapRGB(explosions[i]->format, 255, 255, 255));
    }

    SDL_Rect position;
    position.x = posJoueur.x * TAILLE_BLOC + (MARGE * TAILLE_BLOC);
    position.y = posJoueur.y * TAILLE_BLOC;
    posCible.x = posCible.x * TAILLE_BLOC + (MARGE * TAILLE_BLOC);
    posCible.y *= TAILLE_BLOC;
    FMOD_System_PlaySound(system, FMOD_CHANNEL_FREE, missile, 0, NULL);
    switch(direction)
    {
    case HAUT:
        while(position.y > posCible.y)
        {
            SDL_BlitSurface(tir[HAUT], NULL, ecran, &position);
            SDL_Flip(ecran);
            position.y -= 2;
        }
        break;
    case BAS:
        while(position.y < posCible.y)
        {
            SDL_BlitSurface(tir[BAS], NULL, ecran, &position);
            SDL_Flip(ecran);
            position.y += 2;
        }
        break;
    case GAUCHE:
        while(position.x > posCible.x)
        {
            SDL_BlitSurface(tir[GAUCHE], NULL, ecran, &position);
            SDL_Flip(ecran);
            position.x -= 2;
        }
        break;
    case DROITE:
        while(position.x < posCible.x)
        {
            SDL_BlitSurface(tir[DROITE], NULL, ecran, &position);
            SDL_Flip(ecran);
            position.x += 2;
        }
        break;
    }
    FMOD_System_PlaySound(system, FMOD_CHANNEL_FREE, impact, 0, NULL);
    for(i = 0; i < 7; i++)
    {
        afficher_jeu(carte, ecran, ia, joueur, element);
        SDL_BlitSurface(explosions[i], NULL, ecran, &position);
        SDL_Flip(ecran);
        SDL_Delay(10);
    }
    for(i = 0; i < 4; i++)
    {
        SDL_FreeSurface(tir[i]);
    }
    for(i = 0; i < 7; i++)
    {
        SDL_FreeSurface(explosions[i]);
    }

    FMOD_Sound_Release(missile);
    FMOD_Sound_Release(impact);
    FMOD_System_Close(system);
    FMOD_System_Release(system);

}

void apparition_bonus(int carte[HAUTEUR_TAB][LARGEUR_TAB])
{
    int posx, posy, i, j;
    for(i = 0; i < 2; i++)
    {
        posx = rand() % HAUTEUR_TAB;
        posy = rand() % LARGEUR_TAB;
        while(carte[posx][posy] == TEMPETE || carte[posx][posy] == OBSTACLE || (carte[posx][posy] >= 0 && carte[posx][posy] < NBJOUEUR))
        {
            posx = rand() % HAUTEUR_TAB;
            posy = rand() % LARGEUR_TAB;
        }
        for(i = 0; i < HAUTEUR_TAB; i++)
        {
            for (j = 0; j < LARGEUR_TAB; j++)
            {
                if(((i == posx) && (j == posy)))
                {
                    carte[i][j] = BONUS;
                    i = HAUTEUR_TAB;
                    j = LARGEUR_TAB;
                }
            }
        }
    }

}

void afficher_top(InfoIa joueurs[NBJOUEUR], SDL_Surface *ecran)
{
    SDL_Surface *texte, *rectangle;
    if(TTF_Init() == -1)
    {
        fprintf(stderr, "Erreur d'initialisation de TTF_Init : %s\n", TTF_GetError());
        exit(EXIT_FAILURE);
    }
    TTF_Font *police = NULL;
    police = TTF_OpenFont("Starjedi.ttf", 20);
    SDL_Color noir = {98, 126, 235}, blanc = {255, 255, 255};
    char declaration[300];
    SDL_Rect postexte;

    int i, j, max, maxid, top[3];
    InfoIa copie[NBJOUEUR];

    for(i = 0; i < NBJOUEUR; i++)
    {
        copie[i] = joueurs[i];
    }

    for(i = 0; i < 3; i++)
    {
        max = 0;
        maxid = 0;
        for(j = 0; j < NBJOUEUR; j++)
        {
            if(copie[j].lvl > max)
            {
                max = copie[j].lvl;
                maxid = j;
            }
        }
        copie[maxid].lvl = 0;
        top[i] = maxid;
    }

    postexte.x = HAUTEUR_FENETRE - (MARGE * TAILLE_BLOC);
    postexte.y = 20;

    sprintf(declaration, "top 3 :");
    texte = TTF_RenderText_Solid(police, declaration, blanc);
    SDL_BlitSurface(texte, NULL, ecran, &postexte);

    for(i = 0; i < 3; i++)
    {
    postexte.y += 40;
    texte = TTF_RenderText_Solid(police, joueurs[top[i]].nom, blanc);
    rectangle = SDL_CreateRGBSurface(SDL_HWSURFACE, texte->w + 50, texte->h, 32, 0, 0, 0, 0);
    SDL_FillRect(rectangle, NULL, SDL_MapRGB(rectangle->format, 23, 80, 75));
    SDL_BlitSurface(rectangle, NULL, ecran, &postexte);
    SDL_BlitSurface(texte, NULL, ecran, &postexte);
    SDL_Flip(ecran);
    }

}




void apparition_boite(int carte[HAUTEUR_TAB][LARGEUR_TAB])
{
    int posx, posy, i, j;
    for(i = 0; i < 2; i++)
    {
        posx = rand() % HAUTEUR_TAB;
        posy = rand() % LARGEUR_TAB;
        while(carte[posx][posy] == TEMPETE || carte[posx][posy] == OBSTACLE || (carte[posx][posy] >= 0 && carte[posx][posy] < NBJOUEUR))
        {
            posx = rand() % HAUTEUR_TAB;
            posy = rand() % LARGEUR_TAB;
        }
        for(i = 0; i < HAUTEUR_TAB; i++)
        {
            for (j = 0; j < LARGEUR_TAB; j++)
            {
                if(((i == posx) && (j == posy)))
                {
                    carte[i][j] = BOITE;
                    i = HAUTEUR_TAB;
                    j = LARGEUR_TAB;
                }
            }
        }
    }
}


void tir_bonus(int carte[HAUTEUR_TAB][LARGEUR_TAB], SDL_Rect pos, int direction, SDL_Surface *ecran, InfoIa joueur[NBJOUEUR], SDL_Surface *ia[4], ElementCarte element)
{
    int i,j, numero;
    SDL_Rect position;
    switch(direction)
    {
    case HAUT:
        for(i = 1; i <= PORTEE_TIR_BONUS; i++)
        {
            if(carte[pos.x][pos.y - i] == PARTICULE || carte[pos.x][pos.y - i] == BONUS)
            {
                carte[pos.x][pos.y - i] = VIDE;
            }

            if(carte[pos.x][pos.y - i] == BLOC)
            {
                carte[pos.x][pos.y - i] = VIDE;
                position.x = pos.x;
                position.y = pos.y - i;
                break;
            }

            if(pos.y - i < 0)
            {
                position.x = pos.x;
                position.y = ((pos.y - i) + 1);
                break;
            }

            if(carte[pos.x][pos.y - i] < NBJOUEUR && carte[pos.x][pos.y - i] >= 0)
            {
                numero = carte[pos.x][pos.y - i];
                if(joueur[numero].lvl >= 2)
                {
                    joueur[numero].lvl -= 2;
                }
                position.x = pos.x;
                position.y = pos.y - i;
                break;
            }
            if(carte[pos.x][pos.y - i] == OBSTACLE || carte[pos.x][pos.y - i] == TEMPETE)
            {
                position.x = pos.x;
                position.y = pos.y - i;
                break;
            }
            position.x = pos.x;
            position.y = pos.y - PORTEE_TIR_BONUS;
        }
        break;
    case BAS:
        for(i = 1; i <= PORTEE_TIR_BONUS; i++)
        {
            if(carte[pos.x][pos.y + i] == PARTICULE || carte[pos.x][pos.y + i] == BONUS)
            {
                carte[pos.x][pos.y + 1] = VIDE;
            }

            if(carte[pos.x][pos.y + i] == BLOC)
            {
                carte[pos.x][pos.y + i] = VIDE;
                position.x = pos.x;
                position.y = pos.y + i;
                break;
            }

            if(pos.y + i > LARGEUR_TAB)
            {
                position.x = pos.x;
                position.y = ((pos.y + i) - 1);
                break;
            }

            if(carte[pos.x][pos.y + i] < NBJOUEUR && carte[pos.x][pos.y + i] >= 0)
            {
                numero = carte[pos.x][pos.y + i];
                if(joueur[numero].lvl >= 2)
                {
                    joueur[numero].lvl -= 2;
                }
                position.x = pos.x;
                position.y = pos.y + i;
                break;
            }
            if(carte[pos.x][pos.y + i] == OBSTACLE || carte[pos.x][pos.y + i] == TEMPETE)
            {
                position.x = pos.x;
                position.y = pos.y + i;
                break;
            }
            position.x = pos.x;
            position.y = pos.y + PORTEE_TIR_BONUS;
        }

        break;
    case GAUCHE:
        for(i = 1; i <= PORTEE_TIR_BONUS; i++)
        {
            if(carte[pos.x - i][pos.y] == PARTICULE || carte[pos.x - i][pos.y] == BONUS)
            {
                carte[pos.x - i][pos.y] = VIDE;
            }

            if(carte[pos.x - i][pos.y] == BLOC)
            {
                carte[pos.x - i][pos.y] = VIDE;
                position.x = pos.x - i;
                position.y = pos.y;
                break;
            }

            if(pos.x - i < 0)
            {
                position.x = (pos.x - i) + 1;
                position.y = pos.y;
                break;
            }

            if(carte[pos.x - i][pos.y] < NBJOUEUR && carte[pos.x - i][pos.y] >= 0)
            {
                numero = carte[pos.x - i][pos.y];
                if(joueur[numero].lvl >= 2)
                {
                    joueur[numero].lvl -= 2;
                }
                position.x = pos.x - i;
                position.y = pos.y;
                break;
            }
            if(carte[pos.x - i][pos.y] == OBSTACLE || carte[pos.x - i][pos.y] == TEMPETE)
            {
                position.x = pos.x - i;
                position.y = pos.y;
                break;
            }
            position.x = pos.x - PORTEE_TIR_BONUS;
            position.y = pos.y;
        }
        break;
    case DROITE:
        for(i = 1; i <= PORTEE_TIR_BONUS; i++)
        {
            if(carte[pos.x + i][pos.y] == PARTICULE || carte[pos.x + i][pos.y] == BONUS)
            {
                carte[pos.x + i][pos.y] = VIDE;
            }

            if(carte[pos.x + i][pos.y] == BLOC)
            {
                carte[pos.x + i][pos.y] = VIDE;
                position.x = pos.x + i;
                position.y = pos.y;
                break;
            }

            if(pos.x + i > HAUTEUR_TAB)
            {
                position.x = (pos.x + i) - 1;
                position.y = pos.y;
                break;
            }

            if(carte[pos.x + i][pos.y] < NBJOUEUR && carte[pos.x + i][pos.y] >= 0)
            {
                numero = carte[pos.x + i][pos.y];
                if(joueur[numero].lvl >= 2)
                {
                    joueur[numero].lvl -= 2;
                }
                position.x = pos.x + i;
                position.y = pos.y;
                break;
            }
            if(carte[pos.x + i][pos.y] == OBSTACLE || carte[pos.x + i][pos.y] == TEMPETE)
            {
                position.x = pos.x + i;
                position.y = pos.y;
                break;
            }
            position.x = pos.x + PORTEE_TIR_BONUS;
            position.y = pos.y;
        }

        break;
    }
    animation_tir(carte, ecran, ia, joueur, pos, element, position, direction);


}




void activation_boite( int numero, int carte[HAUTEUR_TAB][LARGEUR_TAB], SDL_Rect pos, int direction, SDL_Surface *ecran, InfoIa joueur[NBJOUEUR], SDL_Surface *ia[4], ElementCarte element)
{
    int i, j;
    switch(joueur[numero].bonus)
    {
    case NON:
        break;
    case TIR:
        tir_bonus(carte, joueur[numero].position, joueur[numero].direction, ecran, joueur, ia, element);
        joueur[numero].bonus = NON;
        break;
    case VITESSE:
        deplacer_joueur(carte, joueur[numero].direction, &joueur[numero].position, numero, joueur, ecran, 2);
        joueur[numero].bonus = NON;
        break;
    case PIEGE:
        switch(joueur[numero].direction)
        {
        case HAUT:
            if(carte[joueur[numero].position.x][joueur[numero].position.y - 1] == OBSTACLE || carte[joueur[numero].position.x][joueur[numero].position.y - 1] == TEMPETE
               || (carte[joueur[numero].position.x][joueur[numero].position.y - 1] < NBJOUEUR && carte[joueur[numero].position.x][joueur[numero].position.y - 1] >= 0) || joueur[numero].position.y - 1 < 0)
            {
                joueur[numero].bonus = NON;
                break;
            }
            else
            {
                i = joueur[numero].position.x;
                j = joueur[numero].position.y;
                deplacer_joueur(carte, joueur[numero].direction, &joueur[numero].position, numero, joueur, ecran, 1);
                carte[i][j] = TRAP;
                joueur[numero].bonus = NON;
            }
            break;
        case BAS:
            if(carte[joueur[numero].position.x][joueur[numero].position.y + 1] == OBSTACLE || carte[joueur[numero].position.x][joueur[numero].position.y + 1] == TEMPETE
               || (carte[joueur[numero].position.x][joueur[numero].position.y + 1] < NBJOUEUR && carte[joueur[numero].position.x][joueur[numero].position.y + 1] >= 0) || joueur[numero].position.y + 1 > LARGEUR_TAB)
            {
                joueur[numero].bonus = NON;
                break;
            }
            else
            {
                i = joueur[numero].position.x;
                j = joueur[numero].position.y;
                deplacer_joueur(carte, joueur[numero].direction, &joueur[numero].position, numero, joueur, ecran, 1);
                carte[i][j] = TRAP;
                joueur[numero].bonus = NON;
            }
            break;
        case GAUCHE:
            if(carte[joueur[numero].position.x - 1][joueur[numero].position.y] == OBSTACLE || carte[joueur[numero].position.x - 1][joueur[numero].position.y] == TEMPETE
               || (carte[joueur[numero].position.x - 1][joueur[numero].position.y] < NBJOUEUR && carte[joueur[numero].position.x - 1][joueur[numero].position.y] >= 0) || joueur[numero].position.x - 1 < 0)
            {
                joueur[numero].bonus = NON;
                break;
            }
            else
            {
                i = joueur[numero].position.x;
                j = joueur[numero].position.y;
                deplacer_joueur(carte, joueur[numero].direction, &joueur[numero].position, numero, joueur, ecran, 1);
                carte[i][j] = TRAP;
                joueur[numero].bonus = NON;
            }
            break;
        case DROITE:
            if(carte[joueur[numero].position.x + 1][joueur[numero].position.y] == OBSTACLE || carte[joueur[numero].position.x + 1][joueur[numero].position.y] == TEMPETE
               || (carte[joueur[numero].position.x + 1][joueur[numero].position.y] < NBJOUEUR && carte[joueur[numero].position.x + 1][joueur[numero].position.y] >= 0) || joueur[numero].position.x + 1 > HAUTEUR_TAB)
            {
                joueur[numero].bonus = NON;
                break;
            }
            else
            {
                i = joueur[numero].position.x;
                j = joueur[numero].position.y;
                deplacer_joueur(carte, joueur[numero].direction, &joueur[numero].position, numero, joueur, ecran, 1);
                carte[i][j] = TRAP;
                joueur[numero].bonus = NON;
            }
            break;
    }
    break;
    }
}


explosion(SDL_Surface *ecran, SDL_Rect position)
{
    int i;
    SDL_Surface*explosions[7];

    FMOD_SYSTEM *system;
    FMOD_SOUND *impact;
    FMOD_System_Create(&system);
    FMOD_System_Init(system, 32, FMOD_INIT_NORMAL, NULL);
    FMOD_System_CreateSound(system, "explosion.wav", FMOD_CREATESAMPLE, 0, &impact);

    explosions[0] = IMG_Load("explosion0.png");
    explosions[1] = IMG_Load("explosion1.png");
    explosions[2] = IMG_Load("explosion2.png");
    explosions[3] = IMG_Load("explosion3.png");
    explosions[4] = IMG_Load("explosion4.png");
    explosions[5] = IMG_Load("explosion5.png");
    explosions[6] = IMG_Load("explosion6.png");


    for(i = 0; i < 7; i++)
    {
        SDL_SetColorKey(explosions[i], SDL_SRCCOLORKEY, SDL_MapRGB(explosions[i]->format, 255, 255, 255));
    }

    position.x = position.x * TAILLE_BLOC + (MARGE * TAILLE_BLOC);
    position.y *= TAILLE_BLOC;

    FMOD_System_PlaySound(system, FMOD_CHANNEL_FREE, impact, 0, NULL);
    for(i = 0; i < 7; i++)
    {
        SDL_BlitSurface(explosions[i], NULL, ecran, &position);
        SDL_Flip(ecran);
        SDL_Delay(10);
    }
    for(i = 0; i < 7; i++)
    {
        SDL_FreeSurface(explosions[i]);
    }
    FMOD_Sound_Release(impact);
    FMOD_System_Close(system);
    FMOD_System_Release(system);

}


void construire_bloc(int carte[HAUTEUR_TAB][LARGEUR_TAB], SDL_Rect position, int direction)
{
    switch(direction)
    {
    case HAUT:
        if(carte[position.x][position.y - 1] == TEMPETE || carte[position.x][position.y - 1] == OBSTACLE || position.y < 0
           || (carte[position.x][position.y - 1] < NBJOUEUR && carte[position.x][position.y - 1] >= 0))
        {
            break;
        }
        carte[position.x][position.y - 1] = BLOC;
        break;
    case BAS:
        if(carte[position.x][position.y + 1] == TEMPETE || carte[position.x][position.y + 1] == OBSTACLE || position.y > LARGEUR_TAB
           || (carte[position.x][position.y + 1] < NBJOUEUR && carte[position.x][position.y + 1] >= 0))
        {
            break;
        }
        carte[position.x][position.y + 1] = BLOC;
        break;
    case GAUCHE:
        if(carte[position.x - 1][position.y] == TEMPETE || carte[position.x - 1][position.y] == OBSTACLE || position.x < 0
           || (carte[position.x - 1][position.y] < NBJOUEUR && carte[position.x - 1][position.y] >= 0))
        {
            break;
        }
        carte[position.x - 1][position.y] = BLOC;
        break;
    case DROITE:
        if(carte[position.x + 1][position.y] == TEMPETE || carte[position.x + 1][position.y] == OBSTACLE || position.x > HAUTEUR_TAB
           || (carte[position.x + 1][position.y] < NBJOUEUR && carte[position.x + 1][position.y] >= 0))
        {
            break;
        }
        carte[position.x + 1][position.y] = BLOC;
        break;
    }
}
