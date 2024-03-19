#ifndef __DISPLAY__
#define __DISPLAY__

#define ABS 28
#define ORD 22

#include "../include/Path.h"
#include "../include/Tower.h"
#include "../include/Mana.h"
#include "../include/Monster.h"

#include "MLV/MLV_all.h"

typedef struct{
  unsigned int width;
  unsigned int height;
  int block_x;
  int block_y;
}Screen;

/**
 * @brief cette fonction initialise le type écran
 * 
 * @return Screen 
 */
Screen screenSize();
/**
 * @brief cette fonction affiche le background de l'espace de jeu en gris.
 * 
 * @param screen 
 */
void displayEmptyGrid(Screen screen);

/**
 * @brief cette fonction return une MLV_Color selon la quantité de Mana
 * 
 * @param quantity_mana 
 * @param max_mana 
 * @return MLV_Color 
 */
MLV_Color nuanceColorRedToGreen(int quantity_mana, int max_mana);

/**
 * @brief cette fonction return une MLV_Color selon la couleur
 * 
 * @param teinte 
 * @return MLV_Color 
 */
MLV_Color nuanceColor(int teinte);

/**
 * @brief cette fonction affiche la case upgrade (en vert ou en bleu ou en rouge)
 * 
 * @param screen 
 * @param color 
 * @param font 
 */
void displayUpgrade(Screen screen, MLV_Color color, MLV_Font* font);


/**
 * @brief cette fonction affiche la jauge de mana, son niveau, et sa quantité textuelle
 * 
 * @param screen 
 * @param quantity 
 * @param max 
 * @param level 
 * @param font 
 */
void displayMana(Screen screen, int quantity, int max,int level, MLV_Font* font);

/**
 * @brief cette fonction affiche le path
 * 
 * @param screen 
 * @param grid 
 */
void displayPath(Screen screen, Grid path);

/**
 * @brief cette fonction affiche le background de l'espace de jeu en gris, le mana et le path
 * 
 * @param screen 
 * @param grid 
 * @param quantity_mana 
 * @param max_mana 
 * @param level 
 * @param font 
 */
void displayGrid(Screen screen, Grid path, int quantity_mana, int max_mana, int level, MLV_Font* font);

/**
 * @brief cette fonction affiche le rayon autour de la tour
 * 
 * @param p 
 * @param screen 
 */
void displayRay(Position p, Screen screen);

/**
 * @brief cette fonction affiche la position de la souris 
 * 
 * @param p 
 * @param screen 
 * @param font 
 */
void currentMousePosition(Position p, Screen screen, MLV_Font* font);

/**
 * @brief cette fonction affiche une tour
 * 
 * @param p 
 * @param screen 
 */
void displayTower(Position p, Screen screen);

/**
 * @brief cette fonction affiche un Block(meme chose que display Tower mais sans le adapt)
 * 
 * @param p 
 * @param screen 
 */
void displayBlock(Position p, Screen screen);

/**
 * @brief cette fonction affichetoutes les tours
 * 
 * @param t 
 * @param screen 
 * @param font2 
 */
void displayTowers(Towers t, Screen screen, MLV_Font* font2);

/**
 * @brief cette fonction affiche la gemme dans la tour
 * 
 * @param t 
 * @param g 
 * @param screen 
 * @param font 
 */
void displayGemInTower(Tower t, Gem g, Screen screen, MLV_Font* font);

/**
 * @brief cette fonction affiche la grille de l'inventaire de gemme
 * 
 * @param gem 
 * @param screen 
 * @param nb 
 * @param font 
 */
void displayGemInventory(Gem gem, Screen screen, int nb, MLV_Font* font);

/**
 * @brief cette fonction affiche la séléction d'une nouvelle gemme(affichage en bleu ou en rouge ou en vert)
 * 
 * @param screen 
 * @param font 
 * @param color 
 */
void displayNewGem(Screen screen, MLV_Font* font, MLV_Color color);

/**
 * @brief cette fonction affiche le niveau d'une gemme et le + et le - pour modifier le niveau
 * 
 * @param screen 
 * @param font 
 * @param gemLevel 
 */
void displayGemLevel(Screen screen, MLV_Font* font, int gemLevel);


/**
 * @brief cette fonction affiche les gemmes présentes dans l'inventaires
 * 
 * @param screen 
 * @param font 
 * @param gems 
 * @param font2 
 */
void displayInventoryGrid(Screen screen, MLV_Font* font, Inventory gems, MLV_Font* font2);


/**
 * @brief cette fonction affiche de la déco
 * 
 * @param screen 
 * @param font 
 */
void displayTD(Screen screen, MLV_Font* font);

/**
 * @brief cette fonction retourne -1 si la gemme n'est pas dans l'inventaire, 
 sinon elle retourne son indice
 * 
 * @param screen 
 * @param x 
 * @param y 
 * @param inventory 
 * @return int 
 */
int gemSelection(Screen screen, int x, int y, Inventory inventory);

/**
 * @brief cette fonction affiche les monstre
 * 
 * @param waves 
 * @param screen 
 */
void displayMonster(Waves* waves, Screen screen);

/**
 * @brief cette fonction affiche le numéro de vague courant
 * 
 * @param screen 
 * @param font 
 * @param nb_w 
 */
void displayWave(Screen screen , MLV_Font* font, int nb_w);

/**
 * @brief cette fonction affiche le temps restant avant la prochaine vague
 * 
 * @param screen 
 * @param font 
 * @param nb_s 
 */
void displayNextWave(Screen screen , MLV_Font* font, int nb_s);

/**
 * @brief cette fonction affiche les tirs
 * 
 * @param s 
 * @param screen 
 */
void display_shot(Shots* s, Screen screen);

/**
 * @brief cette fonction affiche le menu de GameOver
 * 
 * @param screen 
 * @param nbWaveKilled 
 * @param nbDamage 
 * @param font 
 */
void displayGameOver(Screen screen,int nbWaveKilled, int nbDamage, MLV_Font* font);

#endif