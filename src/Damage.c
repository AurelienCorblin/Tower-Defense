#include "../include/Damage.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "../include/Mana.h"


Shots init_shots() {
    Shots shots;
    shots.nb_shots = 0;
    shots.tab = (shot*) malloc(sizeof(shot) * 10);
    if(shots.tab == NULL) {
        fprintf(stderr, "not enough memory for the shots\n");
        exit(1);
    }
    shots.size = 10;
    return shots;
}


void add_shot(Shots* s, double pos_x, double pos_y, int color, int niv_gem, int w_target, int m_target) {
    if(s->nb_shots == s->size) {
        s->tab = (shot*) realloc(s->tab, sizeof(shot) * (s->size * 2));
        if(s->tab == NULL) {
            fprintf(stderr, "not enough memory for the shots\n");
            exit(1);
        }
        s->size *= 2;
    }
    s->tab[s->nb_shots].wave_targeted = w_target;
    s->tab[s->nb_shots].monster_targeted = m_target;
    s->tab[s->nb_shots].color = color;
    s->tab[s->nb_shots].niv_gem = niv_gem;
    s->tab[s->nb_shots].pos_x = pos_x;
    s->tab[s->nb_shots].pos_y = pos_y;
    s->nb_shots++;
}


int damage_to_monster(int m_color, int s_color, int niv_gem) {
    return (int) D * pow(2, niv_gem) * (1. - cos(s_color - m_color) / 2);
}


void delete_shot(Shots* s, int w_target, int m_target) {
    int index = 0;
    for(int i = 0; i < s->nb_shots; i++) {
        if(s->tab[i].wave_targeted != w_target && s->tab[i].monster_targeted != m_target) {
            s->tab[index] = s->tab[i];
            index++;
        }
    }
    s->nb_shots = index;
}


void update_shots(Shots* s, Waves* w, Mana* mana) {
    double dist;
    int damage;
    double v_x, v_y;
    for(int i = 0; i < s->nb_shots; i++) {
        if(w->tab[s->tab[i].wave_targeted].monsters[s->tab[i].monster_targeted].hp > 0) {
            
            dist = dist_shot_monster(&(s->tab[i]), &(w->tab[s->tab[i].wave_targeted].monsters[s->tab[i].monster_targeted]));
            if(dist <= (3. / 60)) {
                damage = damage_to_monster(w->tab[s->tab[i].wave_targeted].monsters[s->tab[i].monster_targeted].color, s->tab[i].color, s->tab[i].niv_gem);
                if((s->tab[i].color >= 0 && s->tab[i].color <= 30) || (s->tab[i].color >= 330 && s->tab[i].color <= 359)) {
                    manage_effect(w, 1, s->tab[i].wave_targeted, s->tab[i].monster_targeted, damage, mana);
                }
                else if(s->tab[i].color >= 90 && s->tab[i].color <= 150) {
                    manage_effect(w, 2, s->tab[i].wave_targeted, s->tab[i].monster_targeted, damage, mana);    
                }
                else if(s->tab[i].color >= 210 && s->tab[i].color <= 270) {
                    manage_effect(w, 3, s->tab[i].wave_targeted, s->tab[i].monster_targeted, damage, mana);
                }
                
                w->tab[s->tab[i].wave_targeted].monsters[s->tab[i].monster_targeted].hp -= damage;
                w->overall_damage += damage;
                
                if(w->tab[s->tab[i].wave_targeted].monsters[s->tab[i].monster_targeted].hp <= 0) {
                    w->tab[s->tab[i].wave_targeted].dead++;
                    int hp_init = (int) H * pow(1.2, (double) w->nb_total_waves - (w->nb_current_waves - s->tab[i].wave_targeted));
                    if(w->tab[s->tab[i].wave_targeted].name == BOSS) {
                        hp_init *= 12;
                    }
                    addMana(hp_init, mana);
                    fprintf(stderr, "%d / %d \n",w->tab[s->tab[i].wave_targeted].dead, w->tab[s->tab[i].wave_targeted].size);///////////////////////////////////////////////
                    if(w->tab[s->tab[i].wave_targeted].dead == w->tab[s->tab[i].wave_targeted].size) {
                        delete_wave(w, s->tab[i].wave_targeted);
                        w->nb_dead_waves++;
                        delete_shot(s, s->tab[i].wave_targeted, s->tab[i].monster_targeted);
                    }
                }
                for(int j = i; j < s->nb_shots - 1; j++) {
                    s->tab[j] = s->tab[j + 1];
                }
                s->nb_shots--;
                i--;
            }
            else {
                v_x = (w->tab[s->tab[i].wave_targeted].monsters[s->tab[i].monster_targeted].pos_x - s->tab[i].pos_x) / dist;
                v_y = (w->tab[s->tab[i].wave_targeted].monsters[s->tab[i].monster_targeted].pos_y - s->tab[i].pos_y) / dist;
                s->tab[i].pos_x += v_x * (3. / 60);
                s->tab[i].pos_y += v_y * (3. / 60);
            }
        }
        else {
            delete_shot(s, s->tab[i].wave_targeted, s->tab[i].monster_targeted);
        }
    }
}


double dist_shot_monster(shot* s, Monster* m) {
    return sqrt(pow(m->pos_x - s->pos_x, 2.) + pow(m->pos_y - s->pos_y, 2.));
}

static double dist(Monster* m, Monster* m2) {
    return sqrt(pow(m->pos_x - m2->pos_x, 2.) + pow(m->pos_y - m2->pos_y, 2.));
}

void manage_effect(Waves* w, int elementary_residue, int w_target, int m_target, int damage, Mana* mana) {
    if(w->tab[w_target].monsters[m_target].elementary_residue == elementary_residue) {
        if(elementary_residue == 2 || elementary_residue == 3) {
            w->tab[w_target].monsters[m_target].effect.time_left = 10 * 60;
        }
    }
    if(w->tab[w_target].monsters[m_target].elementary_residue == 0) {
        w->tab[w_target].monsters[m_target].elementary_residue = elementary_residue;
        switch(elementary_residue) {
            case 1: 
                // damage 2 cases range
                for(int i = 0; i < w->nb_current_waves; i++) {
                    for(int j = 0; j < w->tab[i].size; j++) {
                        if(dist(&(w->tab[w_target].monsters[m_target]), &(w->tab[i].monsters[j])) <= 2
                         && w->tab[i].monsters[j].hp > 0 ) {
                            w->tab[i].monsters[j].hp -= damage * 0.15;
                            w->overall_damage += damage * 0.15;
                            if(w->tab[i].monsters[j].hp <= 0 ) {
                                w->tab[i].dead++;
                                int hp_init = (int) H * pow(1.2, (double) w->nb_total_waves - (w->nb_current_waves - i));
                                if(w->tab[i].name == BOSS) {
                                    hp_init *= 12;
                                }
                                addMana(hp_init, mana);
                                if(w->tab[i].dead == w->tab[i].size) {
                                    delete_wave(w, i);
                                    w->nb_dead_waves++;
                                }
                            }
                        }
                    }
                }
            case 2:
                w->tab[w_target].monsters[m_target].effect.has_effect = 1;
                w->tab[w_target].monsters[m_target].effect.damage = damage * 0.025;
                w->tab[w_target].monsters[m_target].effect.interval = 30;
                w->tab[w_target].monsters[m_target].effect.slow_down = 0;
                w->tab[w_target].monsters[m_target].effect.time_left = 10 * 60;
                break;
            case 3:
                w->tab[w_target].monsters[m_target].effect.has_effect = 1;
                w->tab[w_target].monsters[m_target].effect.damage = 0;
                w->tab[w_target].monsters[m_target].effect.interval = 0;
                w->tab[w_target].monsters[m_target].effect.slow_down = 1.5;
                w->tab[w_target].monsters[m_target].effect.time_left = 10 * 60;
                break;
        }
    }
    if((w->tab[w_target].monsters[m_target].elementary_residue == 1 && elementary_residue == 3)
        || (w->tab[w_target].monsters[m_target].elementary_residue == 3 && elementary_residue == 1)) {
        w->tab[w_target].monsters[m_target].effect.has_effect = 1;
        w->tab[w_target].monsters[m_target].effect.damage = 0;
        w->tab[w_target].monsters[m_target].effect.interval = 0;
        w->tab[w_target].monsters[m_target].effect.slow_down = 1.25;
        w->tab[w_target].monsters[m_target].effect.time_left = 5 * 60;
        // 5% damage in 3.5 cases range:
        for(int i = 0; i < w->nb_current_waves; i++) {
            for(int j = 0; j < w->tab[i].size; j++) {
                if(dist(&(w->tab[w_target].monsters[m_target]), &(w->tab[i].monsters[j])) <= 3.5
                && w->tab[i].monsters[j].hp > 0 ) {
                    w->tab[i].monsters[j].hp -= damage * 0.05;
                    w->overall_damage += damage * 0.05;
                    if(w->tab[i].monsters[j].hp <= 0) {
                        w->tab[i].dead++;
                        int hp_init = (int) H * pow(1.2, (double) w->nb_total_waves - (w->nb_current_waves - i));
                        if(w->tab[i].name == BOSS) {
                            hp_init *= 12;
                        }
                        addMana(hp_init, mana);
                        if(w->tab[i].dead == w->tab[i].size) {
                            delete_wave(w, i);
                            w->nb_dead_waves++;
                        }
                    }
                }
            }
        }
        w->tab[w_target].monsters[m_target].elementary_residue = 0;
    }
    else if((w->tab[w_target].monsters[m_target].elementary_residue == 1 && elementary_residue == 2)
        || (w->tab[w_target].monsters[m_target].elementary_residue == 2 && elementary_residue == 1)) {
        w->tab[w_target].monsters[m_target].hp -= damage * 3;
        w->overall_damage += damage * 3;
        w->tab[w_target].monsters[m_target].elementary_residue = 0;
    }
    else if((w->tab[w_target].monsters[m_target].elementary_residue == 3 && elementary_residue == 2)
        || (w->tab[w_target].monsters[m_target].elementary_residue == 2 && elementary_residue == 3)) {
        w->tab[w_target].monsters[m_target].dont_move = 3 * 60;
        w->tab[w_target].monsters[m_target].elementary_residue = 0;
    }
}
