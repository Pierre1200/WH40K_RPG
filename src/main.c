#include <stdio.h>
/* On inclut notre structure pour pouvoir l'utiliser */
#include "../include/combatant.h" 

int main(void)
{
    /* Création du champion de Nurgle (Lent, très résistant, frappe fort) */
    Combatant plague_marine = {
        "Frere Vorx", 
        "Bolter de la Peste", 
        100, 100, /* Wounds */
        25,       /* Strength (Force surhumaine) */
        30,       /* Toughness (Béni par Nurgle) */
        1         /* Initiative (Lent mais inéluctable) */
    };

    /* Création de la chair à canon (Fragile mais rapide) */
    Combatant guardsman = {
        "Conscrit Cadian", 
        "Fusil Laser", 
        30, 30,   /* Wounds */
        5,        /* Strength (Bras d'humain normal) */
        5,        /* Toughness (Gilet pare-éclats en carton) */
        5         /* Initiative (Rapide, sous l'effet de la panique) */
    };

    /* --- DEBUT DU JEU --- */
    printf("==== WARHAMMER 40,000 : BATTLE LOG ====\n\n");
    
    printf("L'air s'emplit de l'odeur de la rouille et de la pourriture...\n");
    printf("%s avance lourdement, levant son %s.\n", plague_marine.name, plague_marine.weapon_name);
    
    printf("Face a lui, un %s prie le Faux Empereur en serrant son %s.\n", guardsman.name, guardsman.weapon_name);
    printf("Wounds du Garde : %d/%d\n", guardsman.wounds, guardsman.max_wounds);
    
    return (0);
}