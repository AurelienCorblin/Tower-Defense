#include "../include/Path.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


Position random_location(int min_dist) {
    Position random;
    random.line = (rand() % (LINE - min_dist * 2)) + min_dist;
    random.col = (rand() % (COL - min_dist * 2)) + min_dist;
    return random; 
}

int manhattan_dist(Position first, Position second) {
    return (abs(first.line - second.line) + abs(first.col - second.col));
}


Dir random_dir(int extend[4]) {
    int max = extend[0] + extend[1] + extend[2] + extend[3];
    if(max != 0) {
        int random = rand() % max;
        int cpt = 0;
        for(int i = 0; i < 4; i++) {
            cpt += extend[i];
            if(cpt > random) {
                switch(i) {
                    case 0: return NORTH;
                    case 1: return EAST;
                    case 2: return SOUTH;
                    case 3: return WEST;
                }
            }
        }
    }
    return NORTH;
}


int calculate_extend(Grid* grid, Dir direction) {
    int tmp_extend; 
    int real_extend = 0;
    int dir_calcul[4][2] = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};
    Position tmp = grid->path[grid->size - 1];
    switch(direction) {
        case 0: tmp_extend = tmp.line - 1; break;
        case 1: tmp_extend = COL - 1 - tmp.col - 1; break;
        case 2: tmp_extend = LINE - 1 - tmp.line - 1; break;
        case 3: tmp_extend = tmp.col - 1; break; 
    }

    Position top = {0, tmp.col};
    Position bottom = {LINE - 1, tmp.col};
    Position left = {tmp.line, 0};
    Position right = {tmp.line, COL - 1};
    
    for(int j = 0; j < tmp_extend; j++) {
        tmp.line += dir_calcul[direction][0];
        tmp.col += dir_calcul[direction][1];

        if(grid->size < 2) {
            return tmp_extend - 2; // pour pas etre a moins de deux cases des bords
        }
        if(manhattan_dist(tmp, top) <= 2
            || manhattan_dist(tmp, bottom) <= 2
            || manhattan_dist(tmp, left) <= 2
            || manhattan_dist(tmp, right) <= 2) {
                return real_extend;
        }
        for(int index = 0; index < grid->size - 2; index++) { // -2 car case courante en indice size-1 donc pour ne pas prendre la precedente à size-2
            if(manhattan_dist(tmp, grid->path[index]) <= 2
            ) {
                return real_extend;
            }
        }
        real_extend++;
    }
    return real_extend;
}


void generate_path(Grid* grid) {
    int proba[4] = {1, 1, 1, 0};
    int somme;
    int dir_calcul[4][2] = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};
    int corner = 0;
    grid->size = 0;

    grid->path[grid->size] = random_location(3);
    grid->size++;

    int extends[4];
    for(int i = 0; i < 4; i++) {
        extends[i] = calculate_extend(grid, i);
    }

    Dir direction = random_dir(extends);

    while(extends[direction] > 2) {
        somme = 0;
        for(int i = 0; i < extends[direction]; i++) {
            if(proba[rand() % 4] != 0) {
                somme++;
            }
        }
        if(somme < 3) {
            somme = 3;
        }

        Position new_pos = grid->path[grid->size - 1];
        for(int i = 0; i < somme; i++) {
            new_pos.line += dir_calcul[direction][0];
            new_pos.col += dir_calcul[direction][1];
            grid->path[grid->size] = new_pos;
            grid->size++;
        }
        
        extends[0] = 0;
        extends[1] = 0;
        extends[2] = 0;
        extends[3] = 0;
        switch(direction) {
            case 0:case 2: extends[EAST] = calculate_extend(grid, EAST);
                        extends[WEST] = calculate_extend(grid, WEST);
                        direction = random_dir(extends);
                        corner++;
                        break;
            case 1:case 3: extends[NORTH] = calculate_extend(grid, NORTH);
                        extends[SOUTH] = calculate_extend(grid, SOUTH);
                        direction = random_dir(extends);
                        corner++;
                        break;
        }
    }
    
    if(corner >= 7 && grid->size >= 75) {
        return;
    }
    generate_path(grid);
}

