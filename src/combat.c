#include <stdio.h>
#include <stdlib.h>
#include <string.h> /* Pour snprintf */
#include "combat.h"

/* Fonction d'attaque adaptee pour envoyer son resultat dans un log */
void execute_attack(Combatant *attacker, Combatant *defender, Weapon *weapon, char *log_buffer)
{
    int total_hits = 0;
    int total_dmg = 0;
    int was_crit = 0;

    /* On garde votre logique de tirs multiples */
    for (int i = 0; i < weapon->nb_attacks; i++)
    {
        int total_hit_chance = weapon->base_hit_chance + attacker->dex;
        int bonus_dmg = 0;

        if (total_hit_chance > 100) {
            bonus_dmg = (total_hit_chance - 100) / 10;
            total_hit_chance = 100;
        }

        int seuil_a_depasser = 100 - total_hit_chance + 1;
        if (seuil_a_depasser < 1) seuil_a_depasser = 1;

        int roll = (rand() % 100) + 1; 

        if (roll >= seuil_a_depasser)
        {
            total_hits++;
            int raw_dmg = weapon->strength - defender->end;
            if (raw_dmg < 1) raw_dmg = 1;

            int final_dmg = raw_dmg - defender->equipped_armor.armor_value + bonus_dmg;
            if (final_dmg < 0) final_dmg = 0;

            if (roll == 100) {
                final_dmg = (int)((float)final_dmg * 1.5f);
                was_crit = 1;
            }

            defender->pv -= final_dmg;
            total_dmg += final_dmg;
        }
    }

    /* --- NOUVEAUTE : Construction de la phrase pour l'interface graphique --- */
    if (total_hits > 0) {
        if (was_crit) {
            snprintf(log_buffer, 150, "CRITIQUE ! %s touche %d fois avec %s et inflige %d degats !", attacker->name, total_hits, weapon->name, total_dmg);
        } else {
            snprintf(log_buffer, 150, "%s touche %d fois avec %s et inflige %d degats.", attacker->name, total_hits, weapon->name, total_dmg);
        }
    } else {
        snprintf(log_buffer, 150, "%s rate totalement son attaque avec %s !", attacker->name, weapon->name);
    }
}

/* Fonction psychique adaptee */
void execute_psy(Combatant *caster, Combatant *target, char *log_buffer)
{
    if (caster->psy <= 0) {
        snprintf(log_buffer, 150, "%s ne maitrise pas le Warp !", caster->name);
        return;
    }

    int roll = (rand() % 20) + 1 + caster->psy;
    
    if (roll >= caster->psychic_power.warp_charge)
    {
        if (caster->psychic_power.is_heal) {
            caster->pv += caster->psychic_power.power_value;
            if (caster->pv > caster->max_pv) caster->pv = caster->max_pv;
            snprintf(log_buffer, 150, "Magie Warp ! %s se soigne de %d PV.", caster->name, caster->psychic_power.power_value);
        } else {
            target->pv -= caster->psychic_power.power_value;
            snprintf(log_buffer, 150, "Assaut Psy ! %s inflige %d degats purs a %s.", caster->name, caster->psychic_power.power_value, target->name);
        }
    }
    else {
        snprintf(log_buffer, 150, "Echec de l'invocation psy pour %s...", caster->name);
    }
}