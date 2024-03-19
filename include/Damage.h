#ifndef __DAMAGE__
#define __DAMAGE__

#include "../include/Monster.h"
#include "../include/Mana.h"
#define D 12


typedef struct {
    int wave_targeted; // index of targeted wave in tab of waves
    int monster_targeted; // index of targeted monster in wave
    int color; // color of the gem at the origin of the shot
    int niv_gem; // level of the gem at the origin of the shot
    double pos_x;
    double pos_y;
} shot;

typedef struct {
    shot* tab;
    int size;
    int nb_shots;
} Shots;

/**
 * @brief fonction pour initialiser la structure qui stocke le tableau de tirs allouée dynamiquement 
 * 
 * @return Shots 
 */
Shots init_shots();

/**
 * @brief fonction qui ajoute un nouveau tir avec sa position , sa teinte, le niveau de la gemme qui la tirer, ainsi que ce qu
 * il faut pour savoir sur qu elle monstre elle doit aller.La fonction utilise realloc si elle a besoin de plus de place.
 * Quitte directement le programme en cas d echec d allocation de memoire.
 * 
 * @param s 
 * @param pos_x 
 * @param pos_y 
 * @param color 
 * @param niv_gem 
 * @param w_target 
 * @param m_target 
 */
void add_shot(Shots* s, double pos_x, double pos_y, int color, int niv_gem, int w_target, int m_target);

/**
 * @brief fonction qui calcule les degats devant etre infligé a un monstre de teinte m_color en fonction de la teinte s_color
 * de la gemme a l'origine du tir et de son niveau niv_gem. Calculer en double puis cast en int et renvoyer en int.
 * 
 * @param m_color 
 * @param s_color 
 * @param niv_gem 
 * @return int 
 */
int damage_to_monster(int m_color, int s_color, int niv_gem);

/**
 * @brief Supprime tout les tirs stocker dans le tab de la structure Shots qui comme cible le monstre n°m_target de la vague
 * n°w_target.
 * 
 * @param s 
 * @param w_target 
 * @param m_target 
 */
void delete_shot(Shots* s, int w_target, int m_target);

/**
 * @brief utilise damage_to_monster, delete_shot, dist_shot_monster et manage_effect pour effectuer les calculent necessaire 
 * sur les tirs (deplacement, touche un monstre etc..) et mettre ces tirs a jour en fonction de ces calcul.
 * 
 * @param s 
 * @param w 
 * @param mana 
 */
void update_shots(Shots* s, Waves* w, Mana* mana);

/**
 * @brief calcule la distance entre le tir s et le monstre m et la renvoie
 * 
 * @param s 
 * @param m 
 * @return double 
 */
double dist_shot_monster(shot* s, Monster* m);

/**
 * @brief gere les effets appliqués aux vagues selon les types des gems elementary_residue (1: pyro, 2:dendro, 3:hydro)
 * et fait une reaction selon l'element residuel accroché au monstre
 * 
 * @param w 
 * @param elementary_residue 
 * @param w_target 
 * @param m_target 
 * @param damage 
 * @param mana 
 */
void manage_effect(Waves* w, int elementary_residue, int w_target, int m_target, int damage, Mana* mana);
#endif
