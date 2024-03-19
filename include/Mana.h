#ifndef __MANA__
#define __MANA__

typedef struct{
    int level;
    int qty;
    int max;
}Mana;

/*
initialize mana at :
level 0
qty 150
max 2000
*/

Mana initMana();

/*
if mana.qty <= 0 return 1
else return 0
*/
int GameOver(Mana mana);

/**
 * @brief calcule le prix necessaire pour augmenter le niveau de la reserve de mana
 * 
 * @param mana 
 * @return int 
 */
int priceToUpgrade(Mana mana);

/*
upgrade the maximum amount of mana if the player have 
25% of the current maximum.
*/
int upgradeMana(Mana* mana);

/*
when a monster is killed, you collect mana.
if the mana collected make the mana quantity superior to 
the maximum amount allowed, the mana quantity will be equal
to the maximum amount.
*/
void addMana(int hp_init, Mana* mana);

/*
when a monster enter your base, you loose mana.
if your mana quantity is inferior or equal to 0, you lost the function return 1;
else mana quantity is now lower, returns 0;
*/
int manaLoss(int hp_init, Mana* mana);

/**
 * @brief verifie si le joueur peut payer ce qu il a demander ou pas 
 * 
 * @param priceToPay 
 * @param mana 
 * @return int 
 */
int canPay(int priceToPay, Mana mana);

/**
 * @brief depense le mana priceToPay de la reserve du joueur
 * 
 * @param priceToPay 
 * @param mana 
 * @return int 
 */
int pay(int priceToPay, Mana* mana);
#endif
