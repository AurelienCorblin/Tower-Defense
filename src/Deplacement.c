#include "../include/Deplacement.h"

Segment init_seg(Position debut, Position fin){
    Segment seg;
    seg.deb = debut;
    seg.fin = fin;
    return seg; 
}

void addSeg(Segment seg, Segments* segments){
    segments->tab[segments->nb_seg] = seg;
    segments->nb_seg++;
}

Segments init_segments(Grid grid){
    Segments segments;
    segments.nb_seg = 0;

    Segment seg;
    seg.deb.col = grid.path[0].col;
    seg.deb.line = grid.path[0].line;
    for(int i = 0; i < grid.size - 1; i++){
        
        if(seg.deb.col != grid.path[i + 1].col && i < grid.size - 1){ // si le segment est horizontale
            while(grid.path[i].col != grid.path[i + 1].col && i < grid.size - 1){
                i++;
            }
            seg.fin.col = grid.path[i].col;
            seg.fin.line = grid.path[i].line;
            addSeg(seg, &segments);
            seg.deb.col = seg.fin.col;
            seg.deb.line = seg.fin.line;
        }
        if(seg.deb.line  != grid.path[i + 1].line  && i < grid.size - 1){ // si le segment est vertical
            while(grid.path[i].line != grid.path[i + 1].line && i < grid.size - 1){
                i++;
            }
            seg.fin.col = grid.path[i].col;
            seg.fin.line = grid.path[i].line;
            addSeg(seg, &segments);
            seg.deb.col = seg.fin.col;
            seg.deb.line = seg.fin.line;
        }
    }
    return segments;
}

Dir direction_segment(Segment seg) {
    if(seg.deb.line == seg.fin.line) {
        if(seg.fin.col - seg.deb.col < 0) {
            return WEST;
        }
        else {
            return EAST;
        }
    }
    else {
        if(seg.fin.line - seg.deb.line < 0) {
            return NORTH;
        }
        else {
            return SOUTH;
        }
    }
}
