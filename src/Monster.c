#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "../include/Path.h"
#include "../include/Deplacement.h"
#include "../include/Monster.h"
#include "../include/Mana.h"


Wave_Name random_wave(int current_wave) {
    Wave_Name wave_name;
    int wave[4] = {50, 20, 20, 10};
    int random;
    if(current_wave <= 5) {
        random = rand() % 90;
    }
    else {
        random = rand() % 100;
    }
    int cpt = 0;
    for(int i = 0; i < 4; i++) {
        cpt += wave[i];
        if(cpt > random) {
            switch(i) {
                case 1: wave_name = FOULE; break;
                case 2: wave_name = AGILE; break;
                case 3: wave_name = BOSS; break;
                default : wave_name = NORMAL;
            }
            break;
        }
    }
    return wave_name;
}

Wave init_wave(Wave_Name type, int nb_waves, Position first_case) {
    Wave new_wave;
    new_wave.spawn = 0;
    new_wave.dead = 0;
    new_wave.name = type;
    switch(type) {
        case NORMAL : 
            new_wave.size = 12;
            init_monster(new_wave.monsters, new_wave.size, 1., (int) H * pow(1.2, (double) nb_waves), first_case);
            break;
        case FOULE :
            new_wave.size = 24;
            init_monster(new_wave.monsters, new_wave.size, 1., (int) H * pow(1.2, (double) nb_waves), first_case);
            break;
        case AGILE :
            new_wave.size = 12;
            init_monster(new_wave.monsters, new_wave.size, 2., (int) H * pow(1.2, (double) nb_waves), first_case);
            break;
        case BOSS :
            new_wave.size = 2;
            init_monster(new_wave.monsters, new_wave.size, 1., (int) 12 * H * pow(1.2, (double) nb_waves), first_case);
            break;
    }
    return new_wave;
}

void init_monster(Monster monster[24],int size, double speed, int hp, Position first_case) {
    for(int i = 0; i < size; i++) {
        monster[i].current_segment = 0;
        monster[i].speed = speed;
        monster[i].hp = hp;
        monster[i].color = rand() % 360;
        monster[i].elementary_residue = 0;
        monster[i].pos_x = first_case.col;
        monster[i].pos_y = first_case.line;
        monster[i].effect.has_effect = 0;
        monster[i].dont_move = 0;
    }
}

void new_random_wave(Waves* waves, Position first_case) {
    if(waves->nb_current_waves != MAX_WAVES) {
        waves->tab[waves->nb_current_waves] = init_wave(random_wave(waves->nb_total_waves), waves->nb_total_waves, first_case);
        waves->nb_current_waves++;
        waves->nb_total_waves++;
    }
}

void delete_wave(Waves* w, int indice_w) {
    if (indice_w >= 0 && indice_w < w->nb_current_waves) {
        w->nb_current_waves--;
        for(int i = 0; i < w->tab[indice_w].size; i++) {
            if(w->tab[indice_w].monsters[i].hp > 0)
                return;
        }
        for (int i = indice_w; i < w->nb_current_waves; i++) {
            w->tab[i] = w->tab[i + 1];
        }
    }
}

Waves init_waves_tab() {
    Waves waves;
    waves.nb_current_waves = 0;
    waves.nb_dead_waves = 0;
    waves.nb_total_waves = 0;
    waves.overall_damage = 0;
    for(int i = 0; i < MAX_WAVES; i++) {
        waves.tab[i].size = 0;
        waves.tab[i].spawn = 0;
    }
    return waves;
} 

int update_monsters(Waves* waves, Segments* segments, Mana* mana) {
    double random_move[2] = {0.9, 1.1};
    double dist;
    double speed;
    int seg;
    int dif;
    for(int i = 0; i < waves->nb_current_waves; i++) {
        for(int j = 0; j < waves->tab[i].spawn; j++) {
            if(waves->tab[i].monsters[j].dont_move) {
                waves->tab[i].monsters[j].dont_move--;
            }
            else if(waves->tab[i].monsters[j].hp > 0) {
                speed = waves->tab[i].monsters[j].speed;
                if(waves->tab[i].monsters[j].effect.has_effect) {
                    if(waves->tab[i].monsters[j].effect.interval) {
                        if(waves->tab[i].monsters[j].effect.time_left % waves->tab[i].monsters[j].effect.interval == 0) {
                            if(waves->tab[i].monsters[j].effect.damage) {
                                waves->tab[i].monsters[j].hp -= waves->tab[i].monsters[j].effect.damage;
                                waves->overall_damage += waves->tab[i].monsters[j].effect.damage;
                            }
                            else if(waves->tab[i].monsters[j].effect.slow_down) {
                                speed /= waves->tab[i].monsters[j].effect.slow_down;
                            }
                        }
                    }
                    else {
                        if(waves->tab[i].monsters[j].effect.damage) {
                            waves->tab[i].monsters[j].hp -= waves->tab[i].monsters[j].effect.damage;
                        }
                        else if(waves->tab[i].monsters[j].effect.slow_down) {
                            speed /= waves->tab[i].monsters[j].effect.slow_down;
                        }
                    }
                    // if effect kill the monster
                    if(waves->tab[i].monsters[j].hp <= 0) {
                        waves->tab[i].dead++;
                        int hp_init = (int) H * pow(1.2, (double) waves->nb_total_waves - (waves->nb_current_waves - i));
                        if(waves->tab[i].name == BOSS) {
                            hp_init *= 12;
                        }
                        addMana(hp_init, mana);
                        if(waves->tab[i].dead == waves->tab[i].size) {
                            delete_wave(waves, i);
                            waves->nb_dead_waves++;
                        }
                    }
                    waves->tab[i].monsters[j].effect.time_left--;
                    if(waves->tab[i].monsters[j].effect.time_left == 0) {
                        waves->tab[i].monsters[j].effect.has_effect = 0;
                    }
                }

                dist = random_move[rand() % 2] * (speed / 60);
                seg = waves->tab[i].monsters[j].current_segment;

                
                switch(direction_segment(segments->tab[seg])) {
                    case NORTH:
                        if(waves->tab[i].monsters[j].pos_y - dist < (segments->tab[seg].fin.line)) {
                            
                            if(seg == segments->nb_seg-1) {
                                int hp_init = (int) H * pow(1.2, (double) waves->nb_total_waves - (waves->nb_current_waves - i));
                                if(waves->tab[i].name == BOSS) {
                                    hp_init *= 12;
                                }
                                if(manaLoss(hp_init, mana))
                                    return 1;
                            }


                            dif = (segments->tab[seg].fin.line) - (waves->tab[i].monsters[j].pos_y - dist);
                            waves->tab[i].monsters[j].current_segment = (waves->tab[i].monsters[j].current_segment + 1) % segments->nb_seg;
                            seg = waves->tab[i].monsters[j].current_segment;

                            waves->tab[i].monsters[j].pos_y = segments->tab[seg].deb.line;
                            waves->tab[i].monsters[j].pos_x = segments->tab[seg].deb.col;
                            move_in_dir(direction_segment(segments->tab[seg]), &(waves->tab[i].monsters[j]), dif);
                        }
                        else {
                            move_in_dir(direction_segment(segments->tab[seg]), &(waves->tab[i].monsters[j]), dist);
                        }
                        break;
                    case SOUTH:
                        if(waves->tab[i].monsters[j].pos_y + dist > (segments->tab[seg].fin.line)) {

                             if(seg == segments->nb_seg-1) {
                                int hp_init = (int) H * pow(1.2, (double) waves->nb_total_waves - (waves->nb_current_waves - i));
                                if(waves->tab[i].name == BOSS) {
                                    hp_init *= 12;
                                }
                                if(manaLoss(hp_init, mana))
                                    return 1;
                            }


                            dif = (waves->tab[i].monsters[j].pos_y + dist) - (segments->tab[seg].fin.line);
                            waves->tab[i].monsters[j].current_segment = (waves->tab[i].monsters[j].current_segment + 1) % segments->nb_seg;
                            seg = waves->tab[i].monsters[j].current_segment;

                            waves->tab[i].monsters[j].pos_y = segments->tab[seg].deb.line;
                            waves->tab[i].monsters[j].pos_x = segments->tab[seg].deb.col;
                            move_in_dir(direction_segment(segments->tab[seg]), &(waves->tab[i].monsters[j]), dif);
                        }
                        else {
                            move_in_dir(direction_segment(segments->tab[seg]), &(waves->tab[i].monsters[j]), dist);
                        }
                        break;
                    case EAST:
                        if(waves->tab[i].monsters[j].pos_x + dist > (segments->tab[seg].fin.col)) {

                                 if(seg == segments->nb_seg-1) {
                                int hp_init = (int) H * pow(1.2, (double) waves->nb_total_waves - (waves->nb_current_waves - i));
                                if(waves->tab[i].name == BOSS) {
                                    hp_init *= 12;
                                }
                                if(manaLoss(hp_init, mana))
                                    return 1;
                            }


                            dif = (waves->tab[i].monsters[j].pos_x + dist) - (segments->tab[seg].fin.col);
                            waves->tab[i].monsters[j].current_segment = (waves->tab[i].monsters[j].current_segment + 1) % segments->nb_seg;
                            seg = waves->tab[i].monsters[j].current_segment;

                            waves->tab[i].monsters[j].pos_y = segments->tab[seg].deb.line;
                            waves->tab[i].monsters[j].pos_x = segments->tab[seg].deb.col;
                            move_in_dir(direction_segment(segments->tab[seg]), &(waves->tab[i].monsters[j]), dif);
                        }
                        else {
                            move_in_dir(direction_segment(segments->tab[seg]), &(waves->tab[i].monsters[j]), dist);
                        }
                        break;
                    case WEST:
                        if(waves->tab[i].monsters[j].pos_x - dist < (segments->tab[seg].fin.col)) {

                             if(seg == segments->nb_seg-1) {
                                int hp_init = (int) H * pow(1.2, (double) waves->nb_total_waves - (waves->nb_current_waves - i));
                                if(waves->tab[i].name == BOSS) {
                                    hp_init *= 12;
                                }
                                if(manaLoss(hp_init, mana))
                                    return 1;
                            }


                            dif = (segments->tab[seg].fin.col) - (waves->tab[i].monsters[j].pos_x - dist);
                            waves->tab[i].monsters[j].current_segment = (waves->tab[i].monsters[j].current_segment + 1) % segments->nb_seg;
                            seg = waves->tab[i].monsters[j].current_segment;
                            
                            waves->tab[i].monsters[j].pos_y = segments->tab[seg].deb.line;
                            waves->tab[i].monsters[j].pos_x = segments->tab[seg].deb.col;
                            move_in_dir(direction_segment(segments->tab[seg]), &(waves->tab[i].monsters[j]), dif);
                        }
                        else {
                            move_in_dir(direction_segment(segments->tab[seg]), &(waves->tab[i].monsters[j]), dist);
                        }
                        break;
                }
            }
        }
    }
    return 0;
}


void move_in_dir(Dir dir, Monster* monster, double dist) {
    switch(dir) {
        case NORTH:
            monster->pos_y -= dist; break;
        case SOUTH:
            monster->pos_y += dist; break;
        case EAST:
            monster->pos_x += dist; break;
        case WEST:
            monster->pos_x -= dist; break;
        default: ;
    }
}

void spawn_wave(Wave* wave) {
    if(wave->spawn < wave->size) {
        wave->spawn++;
    }
}