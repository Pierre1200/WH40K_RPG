#ifndef COMBAT_H
#define COMBAT_H

#include "combatant.h"

/* * NOUVEAU : Nos fonctions de combat demandent maintenant un "char *log_buffer".
 * C'est le tableau de texte que l'interface graphique va lire pour l'afficher à l'écran.
 */
void execute_attack(Combatant *attacker, Combatant *defender, Weapon *weapon, char *log_buffer);
void execute_psy(Combatant *caster, Combatant *target, char *log_buffer);

/* Note: On a supprime start_combat d'ici, car la boucle de combat sera gérée par la SDL dans le main.c */

#endif