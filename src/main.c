/**
 * @file main.c
 * @brief Le point d'entree du jeu. C'est ici qu'on cree les acteurs 
 * et qu'on les envoie sur la scene (le combat).
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "combatant.h"
#include "combat.h"

int main(void)
{
    /* On initialise le generateur de hasard pour que les des soient differents a chaque partie */
    srand(time(NULL));

    /* --- CREATION DES COMBATTANTS --- */
    
    Combatant plague_marine = {
        "Frere Vorx",         /* Nom */
        100, 100,             /* PV et Max PV */
        8, 45, 5,             /* Endurance, Dexterite, Psy */
        {"Bolter de la Peste", 2, 14, 50, 0},    /* Arme de Tir */
        {"Couteau de la Peste", 3, 16, 60, 1},   /* Arme de Corps-a-Corps */
        {"Armure Energetique", 3},               /* Armure */
        {"Miasmes de Pestilence", 12, 20, 1}     /* Sort de Soin */
    };

    Combatant guardsman = {
        "Conscrit Cadian", 
        30, 30, 
        3, 20, 0, 
        {"Fusil Laser", 1, 6, 40, 0},
        {"Baionnette Fixee", 1, 4, 30, 1},
        {"Gilet Pare-Eclats", 1},
        {"Priere a l'Empereur", 99, 0, 1} /* Sort impossible a reussir (demande 99 au de) */
    };

    printf("==== WARHAMMER 40,000 : DEMARRAGE DU JEU ====\n\n");

    /* * On lance la fonction de combat qui se trouve dans combat.c
     * Le symbole '&' permet d'envoyer l'adresse du personnage.
     * Ainsi, si le personnage prend des degats dans la fonction, 
     * il les garde pour la suite du jeu !
     */
    start_combat(&plague_marine, &guardsman);

    return (0);
}