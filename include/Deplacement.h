#ifndef __DEPLACEMENT__
#define __DEPLACEMENT__

#include "../include/Path.h"

typedef struct{
    Position deb;
    Position fin;
}Segment;

typedef struct{
    Segment tab[30]; // min 6 ou 7
    int nb_seg;
}Segments;

/**
 * @brief initialise un segment de debut a fin.
 * 
 * @param debut 
 * @param fin 
 * @return Segment 
 */
Segment init_seg(Position debut, Position fin);

/**
 * @brief initialise la structure qui stocke le tableau de segment
 * 
 * @param grid 
 * @return Segments 
 */
Segments init_segments(Grid grid);

/**
 * @brief calcule la direction dans laquelle va le segment seg
 * 
 * @param seg 
 * @return Dir 
 */
Dir direction_segment(Segment seg);

#endif