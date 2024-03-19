#include "../include/Mana.h"
#include <math.h>

Mana initMana(){
    Mana mana;
    mana.level = 0;
    mana.qty = 150;
    mana.max = 2000;
    return mana;
}

int GameOver(Mana mana){
    if(mana.qty < 0){
        return 1;
    }
    return 0;
}

int priceToUpgrade(Mana mana){
    return (int)(500 * pow(1.4, (double)mana.level));
}

int upgradeMana(Mana* mana){
    int priceToPay = (int)(500 * pow(1.4, (double)mana->level));
    // if mana->qty == priceToPay we return 0, because you can't have 0 mana, 
    // otherwise it means that the game is over.
    if(mana->qty >= priceToPay){
        mana->level++;
        mana->qty = mana-> qty - priceToPay;
        mana->max = 2000 * pow(1.4, mana->level);
        return 1;
    }
    return 0;
}

void addMana(int hp_init, Mana* mana){
    int add = (int)((hp_init * 0.10) * pow(1.3, (double)mana->level));
    if(mana->qty + add  >= mana->max){
        mana->qty = mana->max;
    }else{
        mana->qty += add;
    }
}


int manaLoss(int hp_init, Mana* mana){
    int loss = (int)(((hp_init*0.15)) * pow(1.3, (double)mana->level));
    mana->qty -= loss;
    if(mana->qty < 0){
        return 1;
    }
    return 0;
}

int pay(int priceToPay, Mana* mana){
    if(priceToPay > mana->qty){
        return 0; // not enough mana
    }
    mana->qty -= priceToPay;
    return 1;
}