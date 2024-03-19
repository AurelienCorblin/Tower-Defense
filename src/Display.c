#include "../include/Display.h"
#include "../include/Mana.h"
#include "../include/Gem.h"

#include <string.h>

///// SCREEN PARAM /////

Screen screenSize(){
  Screen screen;
  MLV_get_desktop_size(&(screen.width), &(screen.height));
  screen.block_x  = (screen.width - (6 * screen.width/ABS))/ABS;
  screen.block_y  = (screen.height - (3 * screen.height/ORD))/ORD;
  return screen;
}

///// MOUSE /////
void currentMousePosition(Position p, Screen screen, MLV_Font* font){
  MLV_draw_text_box_with_font(screen.block_x, screen.block_y, screen.block_x * 8, screen.block_y, 
		" Current position(%d , %d)",font,
		0,
		MLV_COLOR_BLACK, MLV_COLOR_BLACK, MLV_COLOR_WHITE,
		MLV_TEXT_CENTER,
		MLV_HORIZONTAL_CENTER,
		MLV_VERTICAL_CENTER,
    p.col, p.line);
}

///// GAME WINDOW /////
void displayEmptyGrid(Screen screen){
  MLV_draw_filled_rectangle(0,  screen.block_y * 3, screen.block_x * 28, screen.block_y * 22, MLV_COLOR_GREY);
}

///// MANA /////
MLV_Color nuanceColorRedToGreen(int quantity_mana, int max_mana){
  if (quantity_mana <= max_mana/2){
    return MLV_rgba(255, ((quantity_mana * 255)*2)/max_mana, 0,MLV_ALPHA_OPAQUE );
  }
  return MLV_rgba(255 - ((quantity_mana * 255)*2)/max_mana, 255, 0,MLV_ALPHA_OPAQUE );
}

MLV_Color nuanceColor(int teinte){
  if(0 <= teinte && teinte < 60 ){
    return MLV_rgba(255, teinte * (255/60), 0,MLV_ALPHA_OPAQUE );
  }else if(60 <= teinte && teinte < 120){
    return MLV_rgba(255 - teinte * (255/60), 255, 0,MLV_ALPHA_OPAQUE );
  }else if(120 <= teinte && teinte < 180){
    return MLV_rgba(0, 255, teinte * (255/60),MLV_ALPHA_OPAQUE );
  }else if(180 <= teinte && teinte < 240){
    return MLV_rgba(0, 255 - teinte * (255/60),255 ,MLV_ALPHA_OPAQUE );
  }else if(240 <= teinte && teinte < 300){
    return MLV_rgba(teinte * (255/60), 0, 255,MLV_ALPHA_OPAQUE );
  }
  return MLV_rgba(255, 0, 255 - teinte * (255/60),MLV_ALPHA_OPAQUE );
}

void displayUpgrade(Screen screen, MLV_Color color, MLV_Font* font){
    MLV_draw_text_box_with_font(screen.block_x * 11, 0, screen.block_x * 5, screen.block_y, 
		"Upgrade",font,
		0,
		MLV_COLOR_BLACK, MLV_COLOR_BLACK, color,
		MLV_TEXT_CENTER,
		MLV_HORIZONTAL_CENTER,
		MLV_VERTICAL_CENTER);
}

void displayMana(Screen screen, int quantity, int max,int level, MLV_Font* font){
  // text : quantity / max
  MLV_draw_text_box_with_font(screen.block_x * 17, 0, screen.block_x * 11, screen.block_y, 
		"%d / %d",font,
		0,
		MLV_COLOR_BLACK, MLV_COLOR_BLACK, MLV_COLOR_WHITE,
		MLV_TEXT_CENTER,
		MLV_HORIZONTAL_CENTER,
		MLV_VERTICAL_CENTER,
    quantity, max);
  // mana level
  MLV_draw_text_box_with_font(screen.block_x * 11, screen.block_y, screen.block_x * 5, screen.block_y, 
		"level : %d",font,
		0,
		MLV_COLOR_BLACK, MLV_COLOR_BLACK, MLV_COLOR_WHITE,
		MLV_TEXT_CENTER,
		MLV_HORIZONTAL_CENTER,
		MLV_VERTICAL_CENTER,
    level);

  // max mana gauge
  MLV_draw_filled_rectangle(screen.block_x * 17, screen.block_y, screen.block_x * 11, screen.block_y, MLV_COLOR_BLUE);
  MLV_draw_rectangle(screen.block_x * 17, screen.block_y, screen.block_x * 11, screen.block_y, MLV_COLOR_BLACK);
  
  // quantity mana gauge
  MLV_draw_filled_rectangle(screen.block_x * 17, screen.block_y, screen.block_x * (11.*quantity)/max, screen.block_y, nuanceColorRedToGreen(quantity, max));
  MLV_draw_rectangle(screen.block_x * 17, screen.block_y,screen.block_x * (11.*quantity)/max, screen.block_y, MLV_COLOR_BLACK);
}

///// PATH /////
void displayPath(Screen screen, Grid grid){
  int adapt = screen.block_y * 3;
  // affichage du nid des monstres, indice 0
  MLV_draw_filled_rectangle(screen.block_x * grid.path[0].col, + screen.block_y * grid.path[0].line + adapt, screen.block_x, screen.block_y, MLV_COLOR_RED);
  MLV_draw_rectangle(screen.block_x * grid.path[0].col, + screen.block_y * grid.path[0].line + adapt, screen.block_x, screen.block_y, MLV_COLOR_BLACK);

  // affichage du grid, indice i
  for(int i = 1; i < grid.size - 1; i++){
    MLV_draw_filled_rectangle(screen.block_x * grid.path[i].col, + screen.block_y * grid.path[i].line + adapt, screen.block_x, screen.block_y, MLV_COLOR_WHITE);
  }

  // affichage du camp des joueurs, indice taille -1 
  MLV_draw_filled_rectangle(screen.block_x * grid.path[grid.size - 1].col, + screen.block_y * grid.path[grid.size - 1].line + adapt, screen.block_x, screen.block_y, MLV_COLOR_GREEN);
  MLV_draw_rectangle(screen.block_x * grid.path[grid.size - 1].col, + screen.block_y * grid.path[grid.size - 1].line + adapt, screen.block_x, screen.block_y, MLV_COLOR_BLACK);
}


///// TOWER /////
void displayTower(Position p, Screen screen){
  int adapt = screen.block_y * 3;
  MLV_draw_filled_rectangle(screen.block_x * p.col, screen.block_y * p.line + adapt,
                            screen.block_x, screen.block_y, MLV_COLOR_BLACK);
  MLV_draw_filled_rectangle(screen.block_x * p.col + screen.block_x/10, screen.block_y * p.line + adapt + screen.block_y/10,
                            screen.block_x - 2*screen.block_x/10, screen.block_y - 2*screen.block_y/10, MLV_COLOR_GREY);
  MLV_draw_rectangle(screen.block_x * p.col, screen.block_y * p.line + adapt,
                            screen.block_x, screen.block_y, MLV_COLOR_GREY50);
}

void displayTowers(Towers t, Screen screen, MLV_Font* font2){
  for(int i = 0; i < t.number; i++){
    displayTower(t.tab[i].p, screen);
    if(t.tab[i].g.color != -1){
      displayGemInTower(t.tab[i], t.tab[i].g, screen, font2);
    }
  }
}

void displayRay(Position p, Screen screen){
  int adapt = screen.block_y * 3;
  MLV_draw_ellipse(screen.block_x * p.col + screen.block_x/2, screen.block_y * p.line + adapt + screen.block_y/2, 3 * screen.block_x, 3 * screen.block_y, MLV_COLOR_BLACK);
}

///// GEM /////

/*
return -1 if gem cannot be in the inventory
return the index of the gem if it is
*/
int gemSelection(Screen screen, int x, int y, Inventory inventory){
  for(int i = 0; i < 6; i++){
    for(int j = 0; j < 3; j++){
      if(screen.block_x * (29 + i) < x && x < screen.block_x * (30 + i) &&
         screen.block_y * (10 + j) < y && y < screen.block_y * (11 + j) &&
         inventory.nb > (6 * j) + i){
        return (6 * j) + i;
      }
    }
  }
  return -1;
}

void displayGemInTower(Tower t, Gem g, Screen screen, MLV_Font* font){
  int adapt = screen.block_y * 3;
  MLV_draw_filled_ellipse(screen.block_x * t.p.col + screen.block_x/2,
                          screen.block_y * t.p.line + adapt + screen.block_y/2,
                          screen.block_x/3,
                          screen.block_y/3, nuanceColor(g.color));
  MLV_draw_text_with_font(screen.block_x * t.p.col + screen.block_x/2,
                        screen.block_y * t.p.line + adapt + screen.block_y/2,
                        "%d",
                        font,
                        nuanceColor((g.color + 180) % 360), // inverse de nuanceColor(gem.color)
                        g.level);
}

void displayGemInventory(Gem gem, Screen screen, int nb, MLV_Font* font){
  MLV_draw_filled_ellipse(screen.block_x * 29 + screen.block_x *(nb%6) + screen.block_x/2,
                          screen.block_y * 10 + screen.block_y*(nb/6) + screen.block_y/2,
                          screen.block_x/3,
                          screen.block_y/3, nuanceColor(gem.color));
  MLV_draw_text_with_font(screen.block_x * 29 + screen.block_x *(nb%6) + screen.block_x/2,
                        screen.block_y * 10 + screen.block_y*(nb/6) + screen.block_y/2,
                        "%d",
                        font,
                        nuanceColor((gem.color + 180) % 360), // inverse de nuanceColor(gem.color)
                        gem.level);
}

void displayBlock(Position p, Screen screen){
  MLV_draw_filled_rectangle(screen.block_x * p.col, screen.block_y * p.line,
                            screen.block_x, screen.block_y, MLV_COLOR_BLACK);
  MLV_draw_filled_rectangle(screen.block_x * p.col + screen.block_x/10, screen.block_y * p.line + screen.block_y/10,
                            screen.block_x - 2*screen.block_x/10, screen.block_y - 2*screen.block_y/10, MLV_COLOR_GREY);
  MLV_draw_rectangle(screen.block_x * p.col, screen.block_y * p.line,
                            screen.block_x, screen.block_y, MLV_COLOR_GREY50);
}

void displayNewGem(Screen screen, MLV_Font* font, MLV_Color color){
    MLV_draw_text_box_with_font(screen.block_x * 29, screen.block_y * 3, screen.block_x * 6, screen.block_y *2,
		"New Gem",font,
		0,
		MLV_COLOR_BLACK, MLV_COLOR_BLACK, color,
		MLV_TEXT_CENTER,
		MLV_HORIZONTAL_CENTER,
		MLV_VERTICAL_CENTER);
  }

void displayGemLevel(Screen screen, MLV_Font* font, int gemLevel){
  MLV_draw_text_box_with_font(screen.block_x * 29, screen.block_y * 5, screen.block_x , screen.block_y,
		"+",font,
		0,
		MLV_COLOR_BLACK, MLV_COLOR_BLACK, MLV_COLOR_WHITE,
		MLV_TEXT_CENTER,
		MLV_HORIZONTAL_CENTER,
		MLV_VERTICAL_CENTER);
  MLV_draw_text_box_with_font(screen.block_x * 29, screen.block_y * 6, screen.block_x, screen.block_y ,
		"-",font,
		0,
		MLV_COLOR_BLACK, MLV_COLOR_BLACK, MLV_COLOR_WHITE,
		MLV_TEXT_CENTER,
		MLV_HORIZONTAL_CENTER,
		MLV_VERTICAL_CENTER);
  MLV_draw_text_box_with_font(screen.block_x * 30, screen.block_y * 5, screen.block_x*5, screen.block_y * 2 ,
		"GEM LEVEL\n%d",font,
		0,
		MLV_COLOR_BLACK, MLV_COLOR_BLACK, MLV_COLOR_WHITE,
		MLV_TEXT_CENTER,
		MLV_HORIZONTAL_CENTER,
		MLV_VERTICAL_CENTER,
    gemLevel);
}

void displayInventoryGrid(Screen screen, MLV_Font* font, Inventory gems, MLV_Font* font2){
    MLV_draw_text_box_with_font(screen.block_x * 29, screen.block_y * 8, screen.block_x*6, screen.block_y,
		"GEM INVENTORY",font,
		0,
		MLV_COLOR_BLACK, MLV_COLOR_BLACK, MLV_COLOR_WHITE,
		MLV_TEXT_CENTER,
		MLV_HORIZONTAL_CENTER,
		MLV_VERTICAL_CENTER);

  for(int i = 29; i < 35; i++){
    for(int j = 10; j < 13; j++){
      Position p;
      p.col = i;
      p.line= j;
      displayBlock(p, screen);
    }
  }
  for(int i = 0; i < gems.nb; i++){
    displayGemInventory(gems.inventory[i], screen, i, font2);
  }
}

///// MONSTER /////
void displayMonster(Waves* waves, Screen screen){
  int adapt = screen.block_y * 3;
  for(int i = 0; i < waves->nb_current_waves; i++) {
    for(int j = 0; j < waves->tab[i].spawn; j++) {
      if(waves->tab[i].monsters[j].hp > 0){
        int hp_init = (int) H * pow(1.2, (double) waves->nb_total_waves - (waves->nb_current_waves - i));
        if(waves->tab[i].name == BOSS) {
          hp_init *= 12;
        }
        int hp = waves->tab[i].monsters[j].hp;

        MLV_draw_filled_circle( waves->tab[i].monsters[j].pos_x * screen.block_x + screen.block_x/2, waves->tab[i].monsters[j].pos_y * screen.block_y + screen.block_y/2 + adapt , 10, nuanceColor(waves->tab[i].monsters[j].color));
        MLV_draw_circle( waves->tab[i].monsters[j].pos_x * screen.block_x + screen.block_x/2, waves->tab[i].monsters[j].pos_y * screen.block_y + screen.block_y/2 + adapt , 10, MLV_COLOR_BLACK); 

        // max hp
        MLV_draw_filled_rectangle(waves->tab[i].monsters[j].pos_x * screen.block_x + screen.block_x/6,
                                  waves->tab[i].monsters[j].pos_y * screen.block_y + screen.block_y + adapt ,
                                   screen.block_x - screen.block_x/3, screen.block_y/5, MLV_COLOR_BLACK);
        MLV_draw_rectangle(waves->tab[i].monsters[j].pos_x * screen.block_x + screen.block_x/6,
                                  waves->tab[i].monsters[j].pos_y * screen.block_y + screen.block_y + adapt ,
                                   screen.block_x - screen.block_x/3, screen.block_y/5, MLV_COLOR_BLACK);
        // current
        MLV_draw_filled_rectangle(waves->tab[i].monsters[j].pos_x * screen.block_x + screen.block_x/6,
                                  waves->tab[i].monsters[j].pos_y * screen.block_y + screen.block_y + adapt ,
                                   (screen.block_x- screen.block_x/3) * ((double) hp/hp_init), screen.block_y/5, nuanceColorRedToGreen(hp, hp_init));
        MLV_draw_rectangle(waves->tab[i].monsters[j].pos_x * screen.block_x + screen.block_x/6,
                           waves->tab[i].monsters[j].pos_y * screen.block_y + screen.block_y + adapt ,
                           (screen.block_x- screen.block_x/3) * ((double) hp/hp_init), screen.block_y/5, MLV_COLOR_BLACK);
      }
    }
  }
}

///// WAVE /////

void displayWave(Screen screen , MLV_Font* font, int nb_w){
  MLV_draw_text_box_with_font(screen.block_x * 29, screen.block_y * 15, screen.block_x * 6, screen.block_y *2,
		"Wave\n%d",font,
		0,
		MLV_COLOR_BLACK, MLV_COLOR_BLACK, MLV_COLOR_WHITE,
		MLV_TEXT_CENTER,
		MLV_HORIZONTAL_CENTER,
		MLV_VERTICAL_CENTER,
    nb_w);
}

void displayNextWave(Screen screen , MLV_Font* font, int nb_s){
  MLV_draw_text_box_with_font(screen.block_x * 29, screen.block_y * 20, screen.block_x * 6, screen.block_y *3,
		"Next wave in\n%d\nseconds",font,
		0,
		MLV_COLOR_BLACK, MLV_COLOR_BLACK, MLV_COLOR_WHITE,
		MLV_TEXT_CENTER,
		MLV_HORIZONTAL_CENTER,
		MLV_VERTICAL_CENTER,
    nb_s);
}


///// GRID /////
void displayGrid(Screen screen, Grid grid, int quantity_mana, int max_mana, int level, MLV_Font* font){
  MLV_clear_window(MLV_COLOR_GREY50);
  displayEmptyGrid(screen);
  displayMana(screen, quantity_mana, max_mana, level, font);
  displayPath(screen, grid);
}

///// DECORATION /////
void displayTD(Screen screen, MLV_Font* font){
  MLV_draw_text_box_with_font(screen.block_x * 29, screen.block_y * 0, screen.block_x * 6, screen.block_y *2,
		"TOWER DEFENSE",font,
		0,
		MLV_COLOR_BLACK, MLV_COLOR_WHITE, MLV_COLOR_BLACK,
		MLV_TEXT_CENTER,
		MLV_HORIZONTAL_CENTER,
		MLV_VERTICAL_CENTER);
  }


void display_shot(Shots* s, Screen screen) {
  int adapt = screen.block_y * 3;
  for(int i = 0; i < s->nb_shots; i++) {
      MLV_draw_filled_circle( s->tab[i].pos_x * screen.block_x + screen.block_x/2, s->tab[i].pos_y * screen.block_y + screen.block_y/2 + adapt , 5, MLV_COLOR_BLACK);
  }
}

void displayGameOver(Screen screen,int nbWaveKilled, int nbDamage, MLV_Font* font){
  MLV_draw_text_box_with_font(0, screen.block_y * 3, screen.block_x * 28, screen.block_y *22,
        "GAME OVER\nWAVES KILLED : %d\nDAMAGE DONE : %d\n\n(press any key to exit)",font,
        0,
        MLV_COLOR_BLACK, MLV_COLOR_WHITE, MLV_COLOR_BLACK,
        MLV_TEXT_CENTER,
        MLV_HORIZONTAL_CENTER,
        MLV_VERTICAL_CENTER,
    nbWaveKilled, 
    nbDamage);
}