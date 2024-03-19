#ifndef __MONSTER__
#define __MONSTER__

#include "../include/Deplacement.h"
#include "../include/Mana.h"

#define MAX_WAVES 50
#define H 18


typedef struct {
    int has_effect; // 0 : no effect , 1 : has effect
    int time_left; // temp restant de l effet en 1/60 de secondes
    int interval; 
    int damage;
    double slow_down;
} Effect;

typedef enum {
    NORMAL,
    FOULE,
    AGILE,
    BOSS
} Wave_Name;

typedef struct {
    double speed;
    int hp;
    int color; // 0-359
    int elementary_residue; // 1 = pyro, 2 = dendro, 3 = hydro
    Effect effect;
    int dont_move; // special effect, its the time before monster can move
    double pos_x;
    double pos_y;
    int current_segment;
} Monster;

typedef struct {
    Wave_Name name;
    Monster monsters[24];
    int size;
    int spawn;
    int dead;
} Wave;

typedef struct {
    Wave tab[MAX_WAVES]; // limiter a 50 en meme temp sur la grille
    int nb_current_waves;
    int nb_dead_waves;
    int nb_total_waves;
    int overall_damage;
} Waves;


/**
 * @brief creer une nouvelle vague aleatoire sauf boss sur les 5 premieres
 * 
 * @param waves 
 * @param first_case 
 */
void new_random_wave(Waves* waves, Position first_case);

/**
 * @brief initialise la structure waves qui stocke le tableau de vague, le nb total de vague ,
 * le nb de vague tuées 
 * 
 * @return Waves 
 */
Waves init_waves_tab();

/**
 * @brief effectue les calculs pour les effets avec un timer, ainsi que les calcules necessaire 
 * pour le deplacement des monstres sur le chemin.Effectue les calcules avec les fonctions de Mana.h
 * pour les gains/pertes de mana.
 * 
 * @param waves 
 * @param segments 
 * @param mana 
 * @return int 
 */
int update_monsters(Waves* waves, Segments* segments, Mana* mana);

/**
 * @brief fait spawn les monstres de la vague wave un a un 
 * 
 * @param wave 
 */
void spawn_wave(Wave* wave);

/**
 * @brief deplace le monster de distance dist dans la direction dir
 * 
 * @param dir 
 * @param monster 
 * @param dist 
 */
void move_in_dir(Dir dir, Monster* monster, double dist);

/**
 * @brief initialise les monstres pour les stocker dans la vague monster
 * 
 * @param monster 
 * @param size 
 * @param speed 
 * @param hp 
 * @param first_case 
 */
void init_monster(Monster monster[24],int size, double speed, int hp, Position first_case);

/**
 * @brief initialise une vague entierement
 * 
 * @param type 
 * @param nb_waves 
 * @param first_case 
 * @return Wave 
 */
Wave init_wave(Wave_Name type, int nb_waves, Position first_case);

/**
 * @brief donne un nom Wave_Name aleatoire de vague sauf boss si current_wave est < 5
 * 
 * @param current_wave 
 * @return Wave_Name 
 */
Wave_Name random_wave(int current_wave);

/**
 * @brief supprime la vague de monstre a l'indice indice_w du tableau de vagues
 * 
 * @param w 
 * @param indice_w 
 */
void delete_wave(Waves* w, int indice_w);

#endif
