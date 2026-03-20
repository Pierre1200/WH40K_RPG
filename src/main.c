/**
 * @file main.c
 * @brief Point d'entree du jeu, gere desormais l'exploration de la carte
 * et le declenchement des combats.
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "combatant.h"
#include "combat.h"

/* Dimensions de notre carte de test */
#define MAP_WIDTH 15
#define MAP_HEIGHT 7

int main(void)
{
    srand(time(NULL));

    /* --- 1. INITIALISATION DES ACTEURS --- */
    
    /* Frere Vorx commence en haut a gauche (x=1, y=1) */
    Combatant plague_marine = {
        "Frere Vorx", 
        1, 1, /* x, y */
        100, 100, 8, 20, 5,
        {"Bolter de la Peste", 2, 14, 50, 0},
        {"Couteau de la Peste", 3, 16, 60, 1},
        {"Armure Energetique", 3},
        {"Miasmes de Pestilence", 12, 20, 1}
    };

    /* Le Garde Imperial est positionne plus loin (x=10, y=3) */
    Combatant guardsman = {
        "Conscrit Cadian", 
        10, 3, /* x, y */
        30, 30, 3, 30, 0, 
        {"Fusil Laser", 4, 6, 40, 0},
        {"Baionnette Fixee", 2, 4, 30, 1},
        {"Gilet Pare-Eclats", 1},
        {"Priere", 99, 0, 1} 
    };

    /* --- 2. CREATION DE LA CARTE --- */
    /* Un tableau de chaines de caracteres. '#' = Mur, '.' = Sol */
    char map[MAP_HEIGHT][MAP_WIDTH + 1] = {
        "###############",
        "#.............#",
        "#.###.........#",
        "#...#.........#",
        "#.............#",
        "#.#####.......#",
        "###############"
    };

    char move;
    int playing = 1;

    /* --- 3. BOUCLE D'EXPLORATION --- */
    while (playing)
    {
        /* Astuce pour Mac/Linux : on efface le terminal a chaque tour pour faire une vraie image fixe */
        system("clear"); 

        printf("==== SECTEUR SANCTUS - ZONE DE QUARANTAINE ====\n");
        printf("Deplacement : Z (Haut), S (Bas), Q (Gauche), D (Droite) | Quitter : X\n\n");

        /* DESSIN DE LA CARTE */
        for (int y = 0; y < MAP_HEIGHT; y++)
        {
            for (int x = 0; x < MAP_WIDTH; x++)
            {
                /* Si c'est la position de Vorx, on dessine un 'V' */
                if (plague_marine.x == x && plague_marine.y == y) {
                    printf("V");
                }
                /* Si c'est le Garde (et qu'il est vivant !), on dessine un 'G' */
                else if (guardsman.pv > 0 && guardsman.x == x && guardsman.y == y) {
                    printf("G");
                }
                /* Sinon, on dessine le decor normal */
                else {
                    printf("%c", map[y][x]);
                }
            }
            printf("\n"); /* Fin de la ligne, on passe a la suivante */
        }

        /* ACTIONS DU JOUEUR */
        printf("\nOrdre de mouvement : ");
        /* L'espace avant %c est CRUCIAL, il vide la touche 'Entree' precedente de la memoire */
        scanf(" %c", &move); 

        /* On prepare les futures coordonnees selon la touche appuyee */
        int next_x = plague_marine.x;
        int next_y = plague_marine.y;

        if (move == 'z' || move == 'Z') next_y--;
        else if (move == 's' || move == 'S') next_y++;
        else if (move == 'q' || move == 'Q') next_x--;
        else if (move == 'd' || move == 'D') next_x++;
        else if (move == 'x' || move == 'X') playing = 0;

        /* COLLISION : Si la prochaine case n'est pas un mur '#', on avance */
        if (map[next_y][next_x] != '#')
        {
            plague_marine.x = next_x;
            plague_marine.y = next_y;
        }

        /* DECLENCHEMENT DU COMBAT : Si on marche sur le Garde VIVANT */
        if (guardsman.pv > 0 && plague_marine.x == guardsman.x && plague_marine.y == guardsman.y)
        {
            system("clear");
            printf(">> CONTACT ENNEMI IMINENT <<\n");
            
            start_combat(&plague_marine, &guardsman);
            
            printf("\nAppuyez sur n'importe quelle touche + Entree pour retourner a la carte...");
            char pause;
            scanf(" %c", &pause);
        }
    }

    printf("\nDeconnexion du reseau vox. Gloire a Nurgle.\n");
    return (0);
}