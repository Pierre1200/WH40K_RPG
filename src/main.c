/**
 * @file main.c
 * @brief Point d'entrée principal et exécution de la Game Loop.
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "combatant.h"

int main(void)
{
    /* Initialisation du générateur de nombres pseudo-aléatoires (RNG) */
    srand(time(NULL));

    /* -------------------------------------------------------------------------
     * INITIALISATION DU CONTEXTE DE JEU (GAME STATE)
     * ------------------------------------------------------------------------- */
    
    Combatant plague_marine = {
        "Frere Vorx", 
        100, 100,
        8, 45, 0,
        {"Bolter de la Peste", 2, 14, 50},
        {"Armure Energetique Corrompue", 3}
    };

    Combatant guardsman = {
        "Conscrit Cadian", 
        30, 30,
        3, 20, 0,
        {"Fusil Laser", 1, 6, 40},
        {"Gilet Pare-Eclats", 1}
    };

    int choice;
    int i;

    printf("==== WARHAMMER 40,000 : ENGINE INITIATED ====\n\n");

    /* -------------------------------------------------------------------------
     * BOUCLE DE COMBAT (MAIN GAME LOOP)
     * ------------------------------------------------------------------------- */
    while (plague_marine.pv > 0 && guardsman.pv > 0)
    {
        /* -- PHASE DU JOUEUR -- */
        printf("\n========================================\n");
        printf("ETAT : %s [%d/%d PV] | %s [%d/%d PV]\n", 
               plague_marine.name, plague_marine.pv, plague_marine.max_pv, 
               guardsman.name, guardsman.pv, guardsman.max_pv);
        
        printf("\n1. Ouvrir le feu avec %s (%d tirs)\n", plague_marine.equipped_weapon.name, plague_marine.equipped_weapon.nb_attacks);
        printf("2. Posture defensive (Passer le tour)\n");
        printf("Action requise : ");
        scanf("%d", &choice);

        if (choice == 1)
        {
            printf("\n> Resolution des tirs pour : %s\n", plague_marine.name);
            
            for (i = 0; i < plague_marine.equipped_weapon.nb_attacks; i++)
            {
                /* Calcul du seuil de réussite (Précision Arme + Modificateur Dextérité) */
                int total_hit_chance = plague_marine.equipped_weapon.base_hit_chance + plague_marine.dex;
                int bonus_dmg = 0;

                /* Conversion de la précision excédentaire en dégâts critiques */
                if (total_hit_chance > 100)
                {
                    bonus_dmg = (total_hit_chance - 100) / 10;
                    total_hit_chance = 100;
                }

                int roll = (rand() % 100) + 1; 

                if (roll <= total_hit_chance)
                {
                    /* Calcul de mitigation : Force vs Endurance, puis prise en compte de l'Armure */
                    int raw_dmg = plague_marine.equipped_weapon.strength - guardsman.end;
                    if (raw_dmg < 1) raw_dmg = 1;

                    int final_dmg = raw_dmg - guardsman.equipped_armor.armor_value + bonus_dmg;
                    if (final_dmg < 0) final_dmg = 0;

                    guardsman.pv -= final_dmg;
                    printf("  [Tir %d] IMPACT CONFIRME. (Roll: %d) | Degats nets : %d (Mitigation armure: %d)\n", 
                           i+1, roll, final_dmg, guardsman.equipped_armor.armor_value);
                }
                else
                {
                    printf("  [Tir %d] ECHEC DU VERROUILLAGE. (Roll: %d / Seuil: %d)\n", i+1, roll, total_hit_chance);
                }
            }
        }
        
        /* Vérification de la condition de victoire prématurée */
        if (guardsman.pv <= 0) break;

        /* -- PHASE DE L'ENTITE IA -- */
        printf("\n--- PHASE ENNEMIE ---\n");
        printf("> Resolution des tirs pour : %s\n", guardsman.name);
        
        for (i = 0; i < guardsman.equipped_weapon.nb_attacks; i++)
        {
            int total_hit_chance = guardsman.equipped_weapon.base_hit_chance + guardsman.dex;
            int roll = (rand() % 100) + 1;

            if (roll <= total_hit_chance)
            {
                int raw_dmg = guardsman.equipped_weapon.strength - plague_marine.end;
                if (raw_dmg < 1) raw_dmg = 1;

                int final_dmg = raw_dmg - plague_marine.equipped_armor.armor_value;
                if (final_dmg < 0) final_dmg = 0;

                plague_marine.pv -= final_dmg;
                printf("  [Tir %d] IMPACT. Degats nets : %d.\n", i+1, final_dmg);
            }
            else
            {
                printf("  [Tir %d] ECHEC.\n", i+1);
            }
        }
    }

    /* -------------------------------------------------------------------------
     * RESOLUTION DE FIN DE PARTIE (ENDGAME STATE)
     * ------------------------------------------------------------------------- */
    if (plague_marine.pv > 0)
        printf("\n[RESULTAT] : Cible eliminee. Gloire a Nurgle.\n");
    else
        printf("\n[RESULTAT] : Entite detruite. Echec critique.\n");

    return (0);
}