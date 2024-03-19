#ifndef __TOWER__
#define __TOWER__

#include "../include/Gem.h"
#include "../include/Path.h"
#include "../include/Monster.h"
#include <math.h>
#include "../include/Damage.h"

typedef struct{
    Position p;
    Gem g; 
    int time_to_shoot;
}Tower;

typedef struct{
    Tower* tab;
    int number;
    int size;
    int priceToPay;
}Towers;

/**
 * @brief initialisation du tableau de tours
 * 
 * @param grid 
 * @return Towers 
 */
Towers initTowers(Grid grid);

/**
 * @brief calcule la distance entres une tour et un monstre
 * 
 * @param tour_x 
 * @param tour_y 
 * @param monster_x 
 * @param monster_y 
 * @return double 
 */
double distance_monster_tower(double tour_x, double tour_y,double monster_x, double monster_y);

/**
 * @brief verifie qu une case est libre pour y poser une tours
 * 
 * @param newTower 
 * @param grid 
 * @param t 
 * @return int 
 */
int freeCase(Position newTower, Grid grid, Towers t);

/**
 * @brief ajoute une nouvelle tour sur la grille (dans le tableau de tours)
 * 
 * @param newTower 
 * @param grid 
 * @param t 
 */
void addTower(Position newTower, Grid grid, Towers* t);

/**
 * @brief permet l'ajout d une gem dans une tour
 * 
 * @param t 
 * @param gems 
 * @param index 
 */
void addGemInTower(Tower* t, Inventory gems, int index);

/**
 * @brief verifie si il y a une tour a la position pos
 * 
 * @param towers 
 * @param pos 
 * @return int 
 */
int isTower(Towers towers, Position pos);

/**
 * @brief verifie que l on peut mettre une gem dans cette tour 
 * 
 * @param towers 
 * @return int 
 */
int isTowerFree(Towers towers);

/**
 * @brief permet de trouver le monstre avec le plus de vie dans un rayon de 3 cases
 * 
 * @param w 
 * @param t 
 * @param s 
 */
void findBiggestHpMonster(Waves w, Tower* t, Shots* s);

/**
 * @brief fait les calcule pour savoir quand tirer sur les monstres et a quelle frequence 
 * 
 * @param w 
 * @param t 
 * @param s 
 */
void update_towers(Waves w, Towers* t, Shots* s);

#endif
