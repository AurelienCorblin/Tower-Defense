#include "../include/Gem.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

int damage(float damage, Gem g, int color_m){
    return (int)(damage * pow(2, g.level) * (1 - cos(g.color - color_m) / 2));
}

int gemPrice(int level, int mana_qty){
    return 100 * pow(2, level);
}

Gem initGem(int level){
    Gem newGem;
    newGem.level = level;

    int res = rand() % 3;
    int teinte = rand() % 60;
    // 0 : PYRO
    if(res == 0){
        if(teinte > 30){
            teinte += 300;
        }
    }
    // 1 : DENDRO
    if(res == 1){
        teinte += 90;
    }
    // 2 : HYDRO
    if(res == 2){
        teinte += 210;
    }
    newGem.isMixte = 0;
    newGem.color = teinte;
    return newGem;
}

Gem fusion(Gem gem1, Gem gem2){
    Gem gem;
    gem.level = gem1.level + 1;
    gem.color = (gem1.color + gem2.color)/2;
    gem.isMixte = 1;
    return gem;
}

Inventory initInventory(){
    Inventory gems;
    gems.nb = 0;
    return gems; 
}

int addGemInventory(Gem gem, Inventory* gems){
    if(gems->nb < 24){
        gems->inventory[gems->nb] = gem;
        gems->nb++;
        return 1;
    }
    return 0;
}

Inventory deleteGem(Inventory gems, int index){
    Inventory new = initInventory();
    for(int i = 0; i < gems.nb; i++){
        if(i != index){
            addGemInventory(gems.inventory[i], &new);
        }
    }
    return new;
}

Inventory deleteGemFusion(Inventory gems, int index1, int index2){
    Inventory new = initInventory();
    for(int i = 0; i < gems.nb; i++){
        if(i != index1 && i != index2){
            addGemInventory(gems.inventory[i], &new);
        }
    }
    return new;
}

int isType(int teinte){
    if((0 <= teinte && teinte <= 30) || (300 <= teinte && teinte <= 359)){
        return 1; // PYRO
    }else if(90 <= teinte && teinte <= 160){
        return 2;
    }else if(210 <= teinte && teinte <= 270){
        return 3;
    }
    return 0;
}