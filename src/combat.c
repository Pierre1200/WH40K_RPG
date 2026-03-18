/**
 * @file combat.c
 * @brief C'est ici que se font tous les calculs : les lancers de des, 
 * les degats, et la gestion des tours de jeu.
 */

#include <stdio.h>
#include <stdlib.h>
#include "combat.h"

/* * Fonction qui gere une attaque (Tir ou Corps-a-Corps).
 * Comme on utilise des pointeurs (*), on doit utiliser la fleche (->) 
 * pour lire ou modifier les stats des personnages.
 */
void execute_attack(Combatant *attacker, Combatant *defender, Weapon *weapon)
{
    printf("\n> %s attaque avec %s !\n", attacker->name, weapon->name);
    
    /* On boucle pour chaque attaque que l'arme permet de faire */
    for (int i = 0; i < weapon->nb_attacks; i++)
    {
        /* Calcul des chances de toucher (Base de l'arme + Dexterite de l'attaquant) */
        int total_hit_chance = weapon->base_hit_chance + attacker->dex;
        int bonus_dmg = 0;

        /* Si les chances depassent 100%, le surplus se transforme en degats bonus */
        if (total_hit_chance > 100)
        {
            bonus_dmg = (total_hit_chance - 100) / 10;
            total_hit_chance = 100;
        }

        /* Lancer du de (un nombre aleatoire entre 1 et 100) */
        int roll = (rand() % 100) + 1; 

        /* Si le de est inferieur ou egal aux chances de toucher, l'attaque reussit */
        if (roll <= total_hit_chance)
        {
            /* Calcul des degats de base (Force arme - Endurance defenseur) */
            int raw_dmg = weapon->strength - defender->end;
            if (raw_dmg < 1) raw_dmg = 1; /* Minimum 1 degat avant armure */

            /* L'armure encaisse une partie des degats */
            int final_dmg = raw_dmg - defender->equipped_armor.armor_value + bonus_dmg;
            if (final_dmg < 0) final_dmg = 0; /* Si l'armure est trop forte, 0 degat */

            /* On retire les degats aux PV du defenseur */
            defender->pv -= final_dmg;
            printf("  [Coup %d] TOUCHE. (De: %d) | Degats : %d (Armure ennemie a absorbe: %d)\n", 
                   i+1, roll, final_dmg, defender->equipped_armor.armor_value);
        }
        else
        {
            printf("  [Coup %d] RATE. (De: %d / Fallait faire moins de: %d)\n", i+1, roll, total_hit_chance);
        }
    }
}

/* Fonction qui gere l'utilisation d'un pouvoir psychique */
void execute_psy(Combatant *caster, Combatant *target)
{
    printf("\n> %s invoque le pouvoir [%s]...\n", caster->name, caster->psychic_power.name);
    
    /* Si le personnage n'a pas de stats Psy, il ne peut rien faire */
    if (caster->psy <= 0)
    {
        printf("  ECHEC : Ce personnage ne maitrise pas le Warp.\n");
        return;
    }

    /* On lance un de a 20 faces (D20) et on ajoute la stat Psy du lanceur */
    int roll = (rand() % 20) + 1 + caster->psy;
    
    /* On verifie si le jet depasse la difficulte (warp_charge) du sort */
    if (roll >= caster->psychic_power.warp_charge)
    {
        printf("  [SUCCES] (Jet: %d / Requis: %d)\n", roll, caster->psychic_power.warp_charge);
        
        /* Si c'est un sort de soin */
        if (caster->psychic_power.is_heal)
        {
            caster->pv += caster->psychic_power.power_value;
            /* On s'assure de ne pas depasser les PV maximums */
            if (caster->pv > caster->max_pv) caster->pv = caster->max_pv;
            printf("  Magie ! %s se soigne de %d PV.\n", caster->name, caster->psychic_power.power_value);
        }
        /* Si c'est un sort d'attaque */
        else
        {
            target->pv -= caster->psychic_power.power_value;
            printf("  Magie offensive ! %s subit %d degats qui ignorent l'armure !\n", target->name, caster->psychic_power.power_value);
        }
    }
    else
    {
        printf("  [ECHEC] Le pouvoir echoue. (Jet: %d)\n", roll);
    }
}

/* Fonction principale qui fait tourner la boucle de combat */
void start_combat(Combatant *player, Combatant *enemy)
{
    int choice;

    /* Tant que les deux combattants sont en vie, on continue */
    while (player->pv > 0 && enemy->pv > 0)
    {
        /* --- TOUR DU JOUEUR --- */
        printf("\n========================================\n");
        printf("ETAT : %s [%d/%d PV] | %s [%d/%d PV]\n", 
               player->name, player->pv, player->max_pv, 
               enemy->name, enemy->pv, enemy->max_pv);
        
        printf("\n1. Attaquer a distance (%s)\n", player->ranged_weapon.name);
        printf("2. Attaquer au corps-a-corps (%s)\n", player->melee_weapon.name);
        printf("3. Utiliser un pouvoir psychique (%s)\n", player->psychic_power.name);
        printf("Que voulez-vous faire ? : ");
        scanf("%d", &choice);

        /* Selon le choix, on appelle la bonne fonction en envoyant les bonnes armes */
        if (choice == 1) execute_attack(player, enemy, &player->ranged_weapon);
        else if (choice == 2) execute_attack(player, enemy, &player->melee_weapon);
        else if (choice == 3) execute_psy(player, enemy);
        else printf("> Choix invalide.\n");

        /* Si l'ennemi meurt pendant notre tour, on arrete tout de suite le combat */
        if (enemy->pv <= 0) break;

        /* --- TOUR DE L'ENNEMI --- */
        printf("\n--- TOUR ENNEMI ---\n");
        /* Pour l'instant, l'ennemi est bete : il attaque toujours a distance */
        execute_attack(enemy, player, &enemy->ranged_weapon);
    }

    /* --- FIN DU COMBAT --- */
    if (player->pv > 0)
        printf("\n[VICTOIRE] L'ennemi a ete ecrase pour la gloire de Nurgle !\n");
    else
        printf("\n[DEFAITE] Vous etes mort...\n");
}