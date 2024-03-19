#include "../include/Tower.h"
#include "../include/Path.h"
#include "../include/Damage.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

Towers initTowers(Grid grid){
    Towers t;
    t.tab = (Tower*)malloc((28*22 - grid.size)*sizeof(Tower));
    if(t.tab == NULL){
        fprintf(stderr, "not enough memory\n");
        exit(1);
    }
    t.size = 28*22 - grid.size;
    t.number = 0;
    t.priceToPay = 0;
    return t;
}

Tower initTower(Position pos){
    Tower t;
    t.p = pos;
    t.g.color = -1; // inexistant color
    t.g.level = 0; 
    return t;
}

int freeCase(Position newTower, Grid grid, Towers t){
    for(int i = 0; i < grid.size; i++){
        if(newTower.col == grid.path[i].col && 
        newTower.line == grid.path[i].line){
            return 0;
        }
    }
    for(int i = 0; i < t.number; i++){
            if(newTower.col == t.tab[i].p.col && 
            newTower.line == t.tab[i].p.line){
            return 0;
        }
    }

    return 1;
}

void addTower(Position newTower, Grid grid, Towers* t){
    if(freeCase(newTower, grid, *t) == 1 && t->number != t->size){
        t->tab[t->number] = initTower(newTower);
        t->number++;
        if(t->number == 3){
            t->priceToPay = 100;
        }
        if(t->number > 3){
            t->priceToPay *= 2;
        }
    }
}

double distance_monster_tower(double tour_x, double tour_y,double monster_x, double monster_y){
    return sqrt(pow((tour_x - monster_x), 2) + pow((tour_y - monster_y), 2));
}

void addGemInTower(Tower* t, Inventory gems, int index){
    t->g.color = gems.inventory[index].color; 
    t->g.level = gems.inventory[index].level;
    t->time_to_shoot = 2 * 60;
}

int isTower(Towers towers, Position pos){
    for(int i = 0; i< towers.number; i++){
        if(towers.tab[i].p.col == pos.col && towers.tab[i].p.line == pos.line){
            return i;
        }
    }
    return -1;
}

int isTowerFree(Towers towers){
    int cpt = 0;
    for(int i = 0; i < towers.number; i++){
        if(towers.tab[i].g.color == -1){
            cpt++;
        }
    }
    return cpt;
}

void findBiggestHpMonster(Waves w, Tower* t, Shots* s){
    int indice_m = -1;
    int indice_vague = -1; 
    int hp = 0;
    for(int v = 0; v < w.nb_current_waves; v++){
        // est ce que le monstre est à distance de la tour
        for(int m = 0; m < w.tab[v].size; m++){
            if(distance_monster_tower((double)t->p.col, (double)t->p.line, w.tab[v].monsters[m].pos_x, w.tab[v].monsters[m].pos_y) <= 3){
                if(w.tab[v].monsters[m].hp > hp){
                    indice_m = m;
                    indice_vague = v;
                    hp = w.tab[v].monsters[m].hp;
                }
            }
        }
    }
    if(indice_m != -1 && indice_vague != -1 && hp > 0) {
        add_shot(s, t->p.col, t->p.line, t->g.color, t->g.level, indice_vague, indice_m);
    }
    
}


void update_towers(Waves w, Towers* t, Shots* s) {
    for(int i = 0; i < t->number; i++) {
        if(t->tab[i].g.color != -1 && t->tab[i].time_to_shoot == 0) {
            t->tab[i].time_to_shoot = 30;
            findBiggestHpMonster(w, &(t->tab[i]), s);
        }
        else if(t->tab[i].time_to_shoot == 0) {
            t->tab[i].time_to_shoot = 30;
        }
        t->tab[i].time_to_shoot--;
    }
}