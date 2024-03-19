#ifndef __GEM__
#define __GEM__

#define DAMAGE 2
#include "../include/Path.h"

typedef struct{
    int color; // 0 - 359
    int level;
    int isMixte;
}Gem;

typedef struct{
    Gem inventory[24];
    int nb; 
}Inventory;


int damage(float damage, Gem g, int teinte_m);

int gemPrice(int level, int mana_qty);

Gem initGem(int level);

Inventory initInventory();

int addGemInventory(Gem gem, Inventory* gems);

Gem fusion(Gem gem1, Gem gem2);

Inventory deleteGem(Inventory gems, int index);

Inventory deleteGemFusion(Inventory gems, int index1, int index2);

int isType(int teinte);

#endif