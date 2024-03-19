#ifndef __PATH__
#define __PATH__

#define LINE 22
#define COL 28

typedef enum {
    NORTH,
    EAST,
    SOUTH,
    WEST
} Dir;

typedef struct {
    int line;
    int col;
} Position;

typedef struct {
    Position path[LINE * COL];
    int size;
} Grid;

/**
 * @brief tire une position aleatoirement a distance min_dist du bord de la grille
 * 
 * @param min_dist 
 * @return Position 
 */
Position random_location(int min_dist);

/**
 * @brief calcule la distance de manhattan entre deux position sur la grille
 * 
 * @param first 
 * @param second 
 * @return int 
 */
int manhattan_dist(Position first, Position second);

/**
 * @brief choisi une direction aleatoirement 
 * 
 * @param extend 
 * @return Dir 
 */
Dir random_dir(int extend[4]);

/**
 * @brief calcule l'etendu dans la direction donnée
 * 
 * @param grid 
 * @param direction 
 * @return int 
 */
int calculate_extend(Grid* grid, Dir direction);

/**
 * @brief genere un chemin random sur la grille respectant une distance de 2 cases entres les bords et les autres segments du
 * chemin que le chemin croise lui meme
 * 
 * @param grid 
 */
void generate_path(Grid* grid);

#endif
