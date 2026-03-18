/**
 * @file combatant.h
 * @brief Ce fichier définit de quoi sont faits 
 * les armes, les armures, les pouvoirs et les personnages.
 */

#ifndef COMBATANT_H
#define COMBATANT_H

/* --- LES EQUIPEMENTS --- */

typedef struct {
    char name[50];         /* Le nom de l'arme */
    int nb_attacks;        /* Combien de coups cette arme donne par tour */
    int strength;          /* La force des degats infliges */
    int base_hit_chance;   /* Les chances (en %) de toucher la cible de base */
    int is_melee;          /* Vaut 1 si c'est une arme de corps-a-corps, 0 si c'est une arme de tir */
} Weapon;

typedef struct {
    char name[50];         /* Le nom de l'armure */
    int armor_value;       /* Combien de points de degats l'armure absorbe a chaque coup */
} Armor;

typedef struct {
    char name[50];         /* Le nom du sort ou du pouvoir */
    int warp_charge;       /* Le score a atteindre avec les des pour reussir a lancer le pouvoir */
    int power_value;       /* La puissance du soin ou l'intensite des degats */
    int is_heal;           /* Vaut 1 si c'est un sort de soin, 0 si c'est une attaque psychique */
} PsyPower;


/* --- LE PERSONNAGE --- */

typedef struct {
    char name[50];         /* Nom du combattant */
    
    int x;                 /**< Position horizontale (Colonne) */
    int y;                 /**< Position verticale (Ligne) */
    
    /* Stats de Vie */
    int pv;                /* Points de vie actuels */
    int max_pv;            /* Points de vie maximums */
    
    /* Stats de Combat */
    int end;               /* Endurance : reduit l'impact de la force de l'arme ennemie */
    int dex;               /* Dexterite : s'ajoute aux chances de toucher de l'arme */
    int psy;               /* Puissance psychique : bonus pour reussir a lancer un pouvoir du Warp */
    
    /* Inventaire equipe */
    Weapon ranged_weapon;  /* Son arme de tir */
    Weapon melee_weapon;   /* Son arme de corps-a-corps */
    Armor equipped_armor;  /* Son armure */
    PsyPower psychic_power;/* Son pouvoir magique / psychique */
} Combatant;

#endif