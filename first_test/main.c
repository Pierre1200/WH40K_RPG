#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <string.h>
#include "combat.h" /* On inclut vos regles de combat ! */

/* --- CONSTANTES SYSTEME --- */
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define FRAME_WIDTH 256
#define FRAME_HEIGHT 256
#define DELAI_ANIMATION 2500 /* 2.5 secondes de pause pour lire le journal */

/* --- LA MACHINE A ETATS --- */
typedef enum {
    ETAT_CHOIX_JOUEUR,
    ETAT_ANIM_JOUEUR,
    ETAT_TOUR_ENNEMI,
    ETAT_ANIM_ENNEMI,
    ETAT_FIN_COMBAT
} EtatCombat;

/* --- FONCTION UTILITAIRE : Rendu de texte --- */
void DessinerTexte(SDL_Renderer* renderer, TTF_Font* font, const char* texte, int x, int y, SDL_Color couleur) {
    SDL_Surface* surface = TTF_RenderText_Solid(font, texte, couleur);
    if (surface) {
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_Rect rect = { x, y, surface->w, surface->h };
        SDL_RenderCopy(renderer, texture, NULL, &rect);
        SDL_DestroyTexture(texture);
        SDL_FreeSurface(surface);
    }
}

int main(int argc, char* argv[]) {
    
    /* 1. INITIALISATION DES DONNEES DE COMBAT (Vos vraies structures !) */
    Combatant vorx = {
        "Frere Vorx", 1, 1, 150, 150, 8, 20, 5,
        {"Bolter de la Peste", 2, 14, 50, 0},
        {"Couteau de la Peste", 3, 16, 60, 1},
        {"Armure Energetique", 3},
        {"Miasmes", 12, 20, 1}
    };

    Combatant catachan = {
        "Garde Catachan", 10, 3, 80, 80, 3, 30, 0, 
        {"Fusil Laser", 4, 6, 40, 0},
        {"Baionnette", 2, 4, 30, 1},
        {"Gilet Pare-Eclats", 1},
        {"Priere", 99, 0, 1} 
    };

    /* 2. INITIALISATION SDL */
    if (SDL_Init(SDL_INIT_VIDEO) < 0 || !(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) || TTF_Init() == -1) {
        printf("Erreur critique d'initialisation.\n");
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Warhammer 40k: Tactical Combat", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");

    /* 3. CHARGEMENT ASSETS */
    SDL_Texture* bgTexture = IMG_LoadTexture(renderer, "assets/fond_battle.png");
    SDL_Texture* vorxTexture = IMG_LoadTexture(renderer, "assets/vorx_idle.png");
    SDL_Texture* catachanTexture = IMG_LoadTexture(renderer, "assets/catachan_idle.png");
    TTF_Font* uiFont = TTF_OpenFont("assets/font.ttf", 20);

    SDL_Color colorWhite = {255, 255, 255, 255};
    SDL_Color colorGreen = {50, 200, 50, 255};
    SDL_Color colorRed = {255, 50, 50, 255};
    SDL_Color colorYellow = {255, 255, 50, 255};

    /* 4. VARIABLES DU MOTEUR DE JEU */
    int running = 1;
    SDL_Event event;
    char textBuffer[150];
    
    /* Variables de la Machine a Etats */
    EtatCombat etat_actuel = ETAT_CHOIX_JOUEUR;
    Uint32 chrono_animation = 0; /* Va servir de chronometre */
    char journalCombat[150] = "Le combat commence ! En attente de vos ordres...";

    /* --- BOUCLE PRINCIPALE --- */
    while (running) {
        
        /* A. GESTION DES EVENEMENTS (Clavier) */
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = 0;

            /* Si c'est notre tour, on ecoute les touches 1, 2 et 3 */
            if (etat_actuel == ETAT_CHOIX_JOUEUR && event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_1) {
                    execute_attack(&vorx, &catachan, &vorx.ranged_weapon, journalCombat);
                    etat_actuel = ETAT_ANIM_JOUEUR;
                    chrono_animation = SDL_GetTicks(); /* On lance le chrono ! */
                }
                else if (event.key.keysym.sym == SDLK_2) {
                    execute_attack(&vorx, &catachan, &vorx.melee_weapon, journalCombat);
                    etat_actuel = ETAT_ANIM_JOUEUR;
                    chrono_animation = SDL_GetTicks();
                }
                else if (event.key.keysym.sym == SDLK_3) {
                    execute_psy(&vorx, &catachan, journalCombat);
                    etat_actuel = ETAT_ANIM_JOUEUR;
                    chrono_animation = SDL_GetTicks();
                }
            }
        }

        /* B. MISE A JOUR DE LA LOGIQUE (Machine a Etats) */
        Uint32 temps_actuel = SDL_GetTicks();

        /* Le temps de pause de Vorx est ecoule */
        if (etat_actuel == ETAT_ANIM_JOUEUR && (temps_actuel - chrono_animation > DELAI_ANIMATION)) {
            if (catachan.pv <= 0) {
                strcpy(journalCombat, "VICTOIRE ! L'ennemi a ete purge.");
                etat_actuel = ETAT_FIN_COMBAT;
            } else {
                etat_actuel = ETAT_TOUR_ENNEMI;
            }
        }

        /* C'est au tour de l'IA (Instantane) */
        if (etat_actuel == ETAT_TOUR_ENNEMI) {
            execute_attack(&catachan, &vorx, &catachan.ranged_weapon, journalCombat);
            etat_actuel = ETAT_ANIM_ENNEMI;
            chrono_animation = SDL_GetTicks(); /* On lance le chrono pour lire l'attaque de l'ennemi */
        }

        /* Le temps de pause de l'ennemi est ecoule */
        if (etat_actuel == ETAT_ANIM_ENNEMI && (temps_actuel - chrono_animation > DELAI_ANIMATION)) {
            if (vorx.pv <= 0) {
                strcpy(journalCombat, "DEFAITE... Vous avez peri.");
                etat_actuel = ETAT_FIN_COMBAT;
            } else {
                strcpy(journalCombat, "A vous de jouer, Commandant.");
                etat_actuel = ETAT_CHOIX_JOUEUR;
            }
        }

        /* C. RENDU GRAPHIQUE */
        int currentFrame = (temps_actuel / 500) % 2;
        SDL_Rect srcRect = { currentFrame * FRAME_WIDTH, 0, FRAME_WIDTH, FRAME_HEIGHT };

        SDL_RenderClear(renderer);

        /* Couches Decor et Personnages (idem qu'avant) */
        SDL_Rect bgDestRect = { 0, 0, SCREEN_WIDTH, 440 };
        SDL_RenderCopy(renderer, bgTexture, NULL, &bgDestRect);

        SDL_Rect menuBg = { 0, 440, SCREEN_WIDTH, 160 };
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderFillRect(renderer, &menuBg);

        SDL_Rect menuBorder = { 10, 450, SCREEN_WIDTH - 20, 140 };
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderDrawRect(renderer, &menuBorder);

        int renderSize = 384; 
        
        /* Ne dessiner les personnages que s'ils sont vivants ! */
        if (vorx.pv > 0) {
            SDL_Rect vorxDestRect = { 20, 80, renderSize, renderSize }; 
            SDL_RenderCopy(renderer, vorxTexture, &srcRect, &vorxDestRect);
        }
        if (catachan.pv > 0) {
            SDL_Rect catDestRect = { 420, 80, renderSize, renderSize };
            SDL_RenderCopyEx(renderer, catachanTexture, &srcRect, &catDestRect, 0.0, NULL, SDL_FLIP_HORIZONTAL);
        }

        /* HUD - LE JOURNAL DE COMBAT */
        SDL_Rect logBox = { 15, 455, SCREEN_WIDTH - 30, 35 };
        SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
        SDL_RenderDrawRect(renderer, &logBox);
        DessinerTexte(renderer, uiFont, journalCombat, 25, 462, colorYellow);

        /* HUD - VORX */
        snprintf(textBuffer, sizeof(textBuffer), "%s HP : %d/%d", vorx.name, vorx.pv, vorx.max_pv);
        DessinerTexte(renderer, uiFont, textBuffer, 30, 500, colorGreen);
        
        /* HUD - CATACHAN */
        snprintf(textBuffer, sizeof(textBuffer), "%s HP : %d/%d", catachan.name, catachan.pv, catachan.max_pv);
        DessinerTexte(renderer, uiFont, textBuffer, 440, 500, colorRed);

        /* --- LE MENU D'ACTIONS DYNAMIQUE --- */
        if (etat_actuel == ETAT_CHOIX_JOUEUR) {
            snprintf(textBuffer, sizeof(textBuffer), "[1] %s   |   [2] %s   |   [3] %s", vorx.ranged_weapon.name, vorx.melee_weapon.name, vorx.psychic_power.name);
            DessinerTexte(renderer, uiFont, textBuffer, 30, 545, colorWhite);
        } else if (etat_actuel == ETAT_FIN_COMBAT) {
            DessinerTexte(renderer, uiFont, "COMBAT TERMINE. Appuyez sur la croix pour quitter.", 30, 545, colorWhite);
        }

        SDL_RenderPresent(renderer);
    }

    /* NETTOYAGE */
    TTF_CloseFont(uiFont);
    SDL_DestroyTexture(catachanTexture);
    SDL_DestroyTexture(vorxTexture);
    SDL_DestroyTexture(bgTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit(); IMG_Quit(); SDL_Quit();

    return 0;
}