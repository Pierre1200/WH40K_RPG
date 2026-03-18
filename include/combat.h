/**
 * @file combat.h
 * @brief Ce fichier sert de "menu". Il annonce aux autres fichiers 
 * les fonctions de combat qui existent et qu'ils ont le droit d'utiliser.
 */

#ifndef COMBAT_H
#define COMBAT_H

#include "combatant.h"

/* * On annonce la fonction start_combat. 
 * Les etoiles (*) signifient qu'on attend l'adresse en memoire (pointeur) 
 * des personnages, pas une simple copie.
 */
void start_combat(Combatant *player, Combatant *enemy);

#endif 