/**
 * @file combatant.h
 * @brief Définition des structures de données pour le système de combat.
 * * Ce fichier contient les modèles de base représentant les entités physiques
 * et leur équipement dans l'univers de Warhammer 40,000.
 */

#ifndef COMBATANT_H
#define COMBATANT_H

/**
 * @struct Weapon
 * @brief Caractéristiques d'une arme équipée.
 */
typedef struct {
    char name[50];         /**< Nom de l'arme (ex: Bolter de la Peste) */
    int nb_attacks;        /**< Nombre de jets de touche par phase de tir */
    int strength;          /**< Force brute de l'arme, comparée à l'Endurance de la cible */
    int base_hit_chance;   /**< Pourcentage de chance de base de toucher la cible (0-100) */
} Weapon;

/**
 * @struct Armor
 * @brief Caractéristiques d'une armure équipée.
 */
typedef struct {
    char name[50];         /**< Nom de l'armure (ex: Armure Energétique) */
    int armor_value;       /**< Valeur de mitigation : points de dégâts absorbés par attaque */
} Armor;

/**
 * @struct Combatant
 * @brief Représente une entité complète sur le champ de bataille.
 */
typedef struct {
    char name[50];         /**< Identifiant ou nom du personnage */
    
    /* Statistiques de vitalité */
    int pv;                /**< Points de vie actuels */
    int max_pv;            /**< Points de vie maximums de l'entité */
    
    /* Attributs physiques et mentaux */
    int end;               /**< Endurance : Résistance naturelle avant application de l'armure */
    int dex;               /**< Dextérité : Modificateur ajouté à la précision de l'arme */
    int psy;               /**< Puissance psychique : Utilisée pour les tests du Warp (WIP) */
    
    /* Équipement */
    Weapon equipped_weapon; /**< L'arme actuellement maniée par l'entité */
    Armor equipped_armor;   /**< L'armure actuellement portée par l'entité */
} Combatant;

#endif