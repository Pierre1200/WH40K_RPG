#ifndef COMBATANT_H
#define COMBATANT_H

/* Structure représentant n'importe quelle unité sur le champ de bataille */
typedef struct {
    char name[50];
    char weapon_name[50];
    int wounds;         /* Points de Vie */
    int max_wounds;
    int strength;       /* Dégâts d'attaque */
    int toughness;      /* Réduction des dégâts */
    int initiative;     /* Vitesse d'action au tour par tour */
} Combatant;

#endif