
#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <fmod.h>
#include "../jeu.h"
#include "../CONSTANTE.h"

void Menu(SDL_Surface *ecran, FMOD_CHANNEL *canalintro, FMOD_BOOL *etat);
void regle(SDL_Surface *ecran);

int main(int argc, char* argv[])
{
    FMOD_BOOL etat;
    FMOD_CHANNEL *canalintro;
    FMOD_SYSTEM *system;
    FMOD_SOUND *intro;
    FMOD_System_Create(&system);
    FMOD_System_Init(system, 32, FMOD_INIT_NORMAL, NULL);
    FMOD_System_CreateSound(system, "intro.wav", FMOD_SOFTWARE|FMOD_2D|FMOD_CREATESTREAM, 0,&intro);
    FMOD_System_PlaySound(system, 4, intro, 0, NULL);
    FMOD_System_GetChannel(system, 4, &canalintro);


    srand(time(NULL));
    int continuer = 1;
    SDL_Surface *ecran, *menu;
    SDL_Event event;
    SDL_Rect position;
    position.x = 0;
    position.y = 0;

    SDL_Init(SDL_INIT_VIDEO);
    ecran = SDL_SetVideoMode(HAUTEUR_FENETRE , LARGEUR_FENETRE, 32, SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_FULLSCREEN);
    menu = IMG_Load("menu.png");
    SDL_BlitSurface(menu, NULL, ecran, &position);
    SDL_Flip(ecran);

    while(continuer)
    {
        SDL_WaitEvent(&event);
        switch(event.type)
        {
        case SDL_QUIT:
            continuer = 0;
            break;
        case SDL_KEYDOWN:
            switch(event.key.keysym.sym)
            {
            case SDLK_ESCAPE:
                exit(EXIT_SUCCESS);
                break;
            case SDLK_SPACE:
                regle(ecran);
                Menu(ecran, canalintro, &etat);
                break;
            }
        }
    }
    SDL_FreeSurface(menu);
    cleanup();
    return EXIT_SUCCESS;
}



void Menu(SDL_Surface *ecran, FMOD_CHANNEL *canalintro, FMOD_BOOL *etat)
{
    int continuer = 1, i, temps1, temps2;

    FMOD_CHANNEL *canalStarwars, *canalost, *canalnaruto, *canalpokemon;
    FMOD_SOUND *starwars, *ost, *naruto, *pokemon;
    FMOD_SYSTEM *system;
    FMOD_System_Create(&system);
    FMOD_System_Init(system, 32, FMOD_INIT_NORMAL, NULL);
    FMOD_System_CreateSound(system, "starwars.wav", FMOD_SOFTWARE|FMOD_2D |FMOD_CREATESTREAM, 0, &starwars);
    FMOD_System_CreateSound(system, "ost.mp3", FMOD_SOFTWARE|FMOD_2D|FMOD_CREATESTREAM, 0, &ost);
    FMOD_System_CreateSound(system, "naruto.wav", FMOD_SOFTWARE |FMOD_2D|FMOD_CREATESTREAM, 0, &naruto);
    FMOD_System_CreateSound(system, "pokemon.wav", FMOD_SOFTWARE |FMOD_2D|FMOD_CREATESTREAM, 0, &pokemon);
    FMOD_System_GetChannel(system, 1, &canalStarwars);
    FMOD_System_GetChannel(system, 3, &canalost);
    FMOD_System_GetChannel(system, 4, &canalnaruto);
    FMOD_System_GetChannel(system, 5, &canalpokemon);


    SDL_Event event;
    SDL_Surface *texte, *fond, *dance[14];
    SDL_Rect position;
    position.x = 0;
    position.y = 0;
    char chaine[50];
    SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 5, 89, 139));
    if(TTF_Init() == -1)
    {
        fprintf(stderr, "Erreur d'initialisation de TTF_Init : %s\n", TTF_GetError());
        exit(EXIT_FAILURE);
    }
    TTF_Font *police = NULL;
    police = TTF_OpenFont("Starjedi.ttf",26);
    SDL_Color blanc = {255, 255, 255};

    fond = IMG_Load("fond.png");
    dance[0] = IMG_Load("dance1.png");
    dance[1] = IMG_Load("dance2.png");
    dance[2] = IMG_Load("dance3.png");
    dance[3] = IMG_Load("dance4.png");
    dance[4] = IMG_Load("dance5.png");
    dance[5] = IMG_Load("dance6.png");
    dance[6] = IMG_Load("dance7.png");
    dance[7] = IMG_Load("dance8.png");
    dance[8] = IMG_Load("dance9.png");
    dance[9] = IMG_Load("dance10.png");
    dance[10] = IMG_Load("dance11.png");
    dance[11] = IMG_Load("dance12.png");
    dance[12] = IMG_Load("dance13.png");
    dance[13] = IMG_Load("dance14.png");


    SDL_BlitSurface(fond, NULL, ecran, &position);


    sprintf(chaine, "choix du theme :");
    texte = TTF_RenderText_Solid(police, chaine, blanc);
    SDL_BlitSurface(texte, NULL, ecran, &position);
    position.y += texte->h;
    SDL_FreeSurface(texte);

    sprintf(chaine, "F1 STAR WARS");
    texte = TTF_RenderText_Solid(police, chaine, blanc);
    SDL_BlitSurface(texte, NULL, ecran, &position);
    position.y += texte->h;
    SDL_FreeSurface(texte);

    sprintf(chaine, "F2 fortnite");
    texte = TTF_RenderText_Solid(police, chaine, blanc);
    SDL_BlitSurface(texte, NULL, ecran, &position);
    position.y += texte->h;
    SDL_FreeSurface(texte);

    sprintf(chaine, "F3 Naruto");
    texte = TTF_RenderText_Solid(police, chaine, blanc);
    SDL_BlitSurface(texte, NULL, ecran, &position);
    position.y += texte->h;
    SDL_FreeSurface(texte);

    sprintf(chaine, "F4 Pokemon");
    texte = TTF_RenderText_Solid(police, chaine, blanc);
    SDL_BlitSurface(texte, NULL, ecran, &position);
    position.y += texte->h;
    SDL_FreeSurface(texte);


    SDL_Flip(ecran);
    position.x = 700;
    position.y = 100;
    temps1 = 0;
    temps2 = 0;
    i = 0;

    while(continuer)
    {
        SDL_PollEvent(&event);

        switch(event.type)
        {
        case SDL_KEYDOWN:
            switch(event.key.keysym.sym)
            {
            case SDLK_ESCAPE:
                exit(EXIT_SUCCESS);
                break;
            case SDLK_F1:
                FMOD_System_PlaySound(system, 1, starwars, 0, NULL);
                FMOD_Channel_SetVolume(canalStarwars, 0.5);
                FMOD_Sound_SetMode(starwars, FMOD_LOOP_NORMAL);
                FMOD_Channel_GetPaused(canalintro, etat);
                if(etat == 1)
                {
                    FMOD_Channel_SetPaused(canalintro, 0);
                }
                else
                {
                    FMOD_Channel_SetPaused(canalintro, 1);
                }
                jeu(ecran, STARWARS);
                break;
            case SDLK_F2:
                FMOD_System_PlaySound(system, 3, ost, 0, NULL);
                FMOD_Channel_SetVolume(canalost, 0.1);
                FMOD_Sound_SetMode(ost, FMOD_LOOP_NORMAL);
                FMOD_Channel_GetPaused(canalintro, etat);
                if(etat == 1)
                {
                    FMOD_Channel_SetPaused(canalintro, 0);
                }
                else
                {
                    FMOD_Channel_SetPaused(canalintro, 1);
                }
                jeu(ecran, FORTNITE);
                break;
            case SDLK_F3:
                FMOD_System_PlaySound(system, 4, naruto, 0, NULL);
                FMOD_Channel_SetVolume(canalnaruto, 0.2);
                FMOD_Sound_SetMode(naruto, FMOD_LOOP_NORMAL);
                FMOD_Channel_GetPaused(canalintro, etat);
                if(etat == 1)
                {
                    FMOD_Channel_SetPaused(canalintro, 0);
                }
                else
                {
                    FMOD_Channel_SetPaused(canalintro, 1);
                }
                jeu(ecran, NARUTO);
                break;
            case SDLK_F4:
                FMOD_System_PlaySound(system, 5, pokemon, 0, NULL);
                FMOD_Channel_SetVolume(canalpokemon, 0.1);
                FMOD_Sound_SetMode(starwars, FMOD_LOOP_NORMAL);
                FMOD_Sound_SetLoopCount(pokemon, 10);
                FMOD_Channel_GetPaused(canalintro, etat);
                if(etat == 1)
                {
                    FMOD_Channel_SetPaused(canalintro, 0);
                }
                else
                {
                    FMOD_Channel_SetPaused(canalintro, 1);
                }
                jeu(ecran, POKEMON);
                break;
            }
            break;
        }
        temps1 = SDL_GetTicks();
        if(temps1 - temps2 > 100)
        {
            SDL_BlitSurface(dance[i], NULL, ecran, &position);
            SDL_Flip(ecran);
            i++;
            if(i == 14)
                i = 0;
            temps2 = temps1;
        }
    }
    SDL_FreeSurface(fond);
    SDL_FreeSurface(texte);
    for(i = 0; i < 14; i++)
    {
        SDL_FreeSurface(dance[i]);
    }

    FMOD_Sound_Release(naruto);
    FMOD_Sound_Release(ost);
    FMOD_Sound_Release(pokemon);
    FMOD_Sound_Release(starwars);
    FMOD_System_Close(system);
    FMOD_System_Release(system);
}


void regle(SDL_Surface *ecran)
{
    SDL_Surface *regle;
    SDL_Event event;
    SDL_Rect position;
    position.x = 0;
    position.y = 0;

    regle = IMG_Load("regles.png");

    int continuer = 1;

    SDL_BlitSurface(regle, NULL, ecran, &position);
    SDL_Flip(ecran);

    while(continuer)
    {
        SDL_WaitEvent(&event);
        switch(event.type)
        {
        case SDL_KEYDOWN:
            switch(event.key.keysym.sym)
            {
            case SDLK_ESCAPE:
                cleanup();
                exit(EXIT_SUCCESS);
                break;
            case SDLK_SPACE:
                continuer = 0;
                break;
            }
        }
    }
}
