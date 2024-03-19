#include "../include/Path.h"
#include "../include/Monster.h"
#include "../include/Display.h"
#include "../include/Mana.h"
#include "../include/Tower.h"
#include "../include/Deplacement.h"
#include "../include/Damage.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "MLV/MLV_all.h"

int main(int args, char* argv[]){
  srand(time(NULL));
  
  // mana
  Mana mana = initMana();

  // the path
  Grid grid;
  generate_path(&grid);

  // the towers
  Towers t = initTowers(grid);

  // the monsters
  Waves waves = init_waves_tab();

  // segments
  Segments segments = init_segments(grid);

  // Inventory
  Inventory gems = initInventory();

  Shots shots = init_shots();

  // screen size and block size
  Screen screen = screenSize();

  MLV_create_full_screen_window("projet", "tower defense", screen.width, screen.height);
  MLV_clear_window(MLV_COLOR_GREY);

  // ORBITRON fonts
  MLV_Font* font = MLV_load_font( "src/Orbitron/static/Orbitron-Medium.ttf", 30);
  MLV_Font* font2 = MLV_load_font( "src/Orbitron/static/Orbitron-Medium.ttf", 20);

  MLV_Keyboard_button touche;
  MLV_Button_state state;
  int x,y; // mouse 
  int level_gem = 0;
  int addGemTower = 0;
  int indiceGemToAdd = -1;
  int indiceTower = -1;

  int cpt = 0;

  int endtime;
  int newtime;
  double extratime;
  double frametime;

  int game = 0;

  while(MLV_get_event (&touche, NULL, NULL, NULL, NULL, NULL, NULL, NULL, &state) == MLV_NONE || touche != MLV_KEYBOARD_ESCAPE){ 

    if(touche == MLV_KEYBOARD_SPACE && state == MLV_PRESSED) {
      if(!game) {
        game = 1;
      }
      else if(waves.nb_current_waves == 0) {
        if(mana.qty + ((((double)(35-(cpt/60)%35)/100)* mana.max)) >= mana.max){
            mana.qty = mana.max;
        }else{
            mana.qty += ((((double)(35-(cpt/60)%35)/100)* mana.max));
        }
        cpt = 35 * 60;
      }
      touche = MLV_KEYBOARD_NONE;
    }
    endtime = MLV_get_time();

    MLV_get_mouse_position( &x, &y );

    displayGrid(screen, grid, mana.qty, mana.max, mana.level, font);
    displayTowers(t, screen, font2);
    
    displayTD(screen, font);

    Position mouse;
    mouse.col = (int)(x / (screen.block_x)); // x
    mouse.line = (int)(y / (screen.block_y)) - 3; // y  

    displayWave(screen , font, waves.nb_total_waves);
    displayNextWave(screen, font, 35 - (cpt / 60) % 35);

    if(cpt % 60 == 0 && waves.nb_current_waves != 0 && (waves.tab[waves.nb_current_waves - 1].name == NORMAL 
    || waves.tab[waves.nb_current_waves - 1].name == BOSS)) {
        spawn_wave(&(waves.tab[waves.nb_current_waves - 1]));
    }
    else if(cpt % 30 == 0 && waves.nb_current_waves != 0 && (waves.tab[waves.nb_current_waves - 1].name == FOULE 
    || waves.tab[waves.nb_current_waves - 1].name == AGILE)) {
        spawn_wave(&(waves.tab[waves.nb_current_waves - 1]));
    }
    if(cpt % (35 * 60) == 0 && game) {
        new_random_wave(&waves, segments.tab[0].deb);
    }
    
    // ADD A TOWER OR GEM
    if(0 < x && x < screen.block_x * 28
      && y > screen.block_y * 3  && y < screen.block_y * 25){
      currentMousePosition(mouse, screen, font);
      if(freeCase(mouse, grid, t) == 1 && addGemTower == 0){
        displayRay(mouse, screen);
        displayTower(mouse, screen);

        // si on clique pour ajouter une tour
        if( MLV_get_mouse_button_state( MLV_BUTTON_LEFT ) == MLV_PRESSED &&
          pay(t.priceToPay, &mana)){
          addTower(mouse, grid, &t);
        }
      }else if(addGemTower == 1 && 
              (indiceTower = isTower(t, mouse)) != -1 && 
               MLV_get_mouse_button_state( MLV_BUTTON_LEFT ) == MLV_PRESSED){
        addGemInTower(&t.tab[indiceTower], gems, indiceGemToAdd);
        gems = deleteGem(gems, indiceGemToAdd);
        indiceGemToAdd = -1;
        addGemTower = 0;
      }else if(addGemTower == 1 && isTowerFree(t) == 0){
        addGemTower = 0;
      }
    }

    // GEM SELECTION 
    if(screen.block_x * 29 < x && x < screen.block_x * 35 &&
       screen.block_y * 10 < y && y < screen.block_y * 13){
         if(gemSelection(screen, x, y, gems) != -1 && addGemTower == 0 ){
          if(gems.inventory[gemSelection(screen, x, y, gems)].isMixte == 0){
            switch(isType(gems.inventory[gemSelection(screen, x, y, gems)].color)){
            case 1 : 
              MLV_draw_text_with_font(screen.block_x * 29, screen.block_y * 14, "TYPE PYRO", font, MLV_COLOR_BLACK);break;
            case 2 : 
              MLV_draw_text_with_font(screen.block_x * 29, screen.block_y * 14, "TYPE DENDRO", font, MLV_COLOR_BLACK);break;
            case 3 :
              MLV_draw_text_with_font(screen.block_x * 29, screen.block_y * 14, "TYPE HYDRO", font, MLV_COLOR_BLACK);break;
            }
          }
        }
      if(gemSelection(screen, x, y, gems) != -1 && addGemTower == 0 &&
      MLV_get_mouse_button_state( MLV_BUTTON_LEFT ) == MLV_PRESSED){
        MLV_draw_text_with_font(screen.block_x * 29, screen.block_y * 15, "%d", font, MLV_COLOR_BLACK, indiceGemToAdd);
        addGemTower = 1;
        indiceGemToAdd = gemSelection(screen, x, y, gems);
      }else if(gemSelection(screen, x, y, gems) != -1 && 
              gemSelection(screen, x, y, gems) != indiceGemToAdd &&
              addGemTower == 1 &&
              mana.qty >= 100 &&
              gems.inventory[indiceGemToAdd].level == gems.inventory[gemSelection(screen, x, y, gems)].level &&
              MLV_get_mouse_button_state( MLV_BUTTON_LEFT ) == MLV_RELEASED &&
              state == 1 &&
              pay(100, &mana) == 1
              ){
        Gem f = fusion(gems.inventory[indiceGemToAdd], gems.inventory[gemSelection(screen, x, y, gems)]);
        gems = deleteGemFusion(gems, indiceGemToAdd, gemSelection(screen, x, y, gems));
        addGemInventory(f, &gems);
        addGemTower = 0;
        indiceGemToAdd = -1;
      }else if(MLV_get_mouse_button_state( MLV_BUTTON_LEFT ) == MLV_PRESSED && 
               gemSelection(screen, x, y, gems) == -1){
                addGemTower = 0;
                indiceGemToAdd = -1;
               }
    }

    displayUpgrade(screen, MLV_COLOR_BLUE, font);
    // MANA UPGRADE
    if(screen.block_x * 11 < x && x < screen.block_x * 16
    && 0 < y && y < screen.block_y){
      if(priceToUpgrade(mana) <= mana.qty){
        displayUpgrade(screen, MLV_COLOR_GREEN, font);
        if(MLV_get_mouse_button_state( MLV_BUTTON_LEFT ) == MLV_RELEASED
        && state == 1){
        upgradeMana(&mana);
      }
      }else{
        displayUpgrade(screen, MLV_COLOR_RED, font);
      }
    }

    // GEM LEVEL MODIFICATION 
    int gem_price = gemPrice(level_gem, mana.qty);
    displayGemLevel(screen, font, level_gem);
    // if the mouse is over the +
    if(screen.block_x * 29 < x && x < screen.block_x * 30 &&
    screen.block_y * 5 < y && y < screen.block_y * 6 ){
      // if a click is detected
      if(MLV_get_mouse_button_state( MLV_BUTTON_LEFT ) == MLV_RELEASED
      && state == 1){
        if(gemPrice(level_gem + 1, mana.qty) <= mana.qty){
        level_gem++;
        }
      }
    }
    // if the mouse is over the -
    if(screen.block_x * 29 < x && x < screen.block_x * 30 &&
    screen.block_y * 6 < y && y < screen.block_y * 7 ){
      // if a click is detected
      if(MLV_get_mouse_button_state( MLV_BUTTON_LEFT ) == MLV_RELEASED
      && state == 1){
        if(level_gem - 1 >= 0 && gemPrice(level_gem - 1, mana.qty)){
        level_gem--;
        }
      }
    }

    // PAYMENT FOR GEMS
    displayInventoryGrid(screen, font, gems, font2);
    displayNewGem(screen, font, MLV_COLOR_BLUE);
    if(screen.block_x * 29 < x && x < screen.block_x * 35 &&
    screen.block_y * 3 < y && y < screen.block_y * 5 ){
      if(gemPrice(level_gem, mana.qty) <= mana.qty){
        displayNewGem(screen, font, MLV_COLOR_GREEN);
        // player wants to pay for a gem
        if(MLV_get_mouse_button_state( MLV_BUTTON_LEFT ) == MLV_RELEASED 
        && state ==1 && gems.nb < 18){
          // enough mana
          pay(gem_price, &mana);
          Gem gem = initGem(level_gem);
          addGemInventory(gem, &gems);
        }
      }else{
        // not enough mana
        displayNewGem(screen, font, MLV_COLOR_RED);
      }
    }

    update_towers(waves, &t, &shots);
    update_shots(&shots, &waves, &mana);
    if(update_monsters(&waves, &segments, &mana)) {
      displayGameOver(screen, waves.nb_dead_waves, waves.overall_damage, font);
      MLV_actualise_window();
      MLV_wait_event(&touche,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
      break;
    }
    displayMonster(&waves, screen);
    display_shot(&shots, screen);

    MLV_actualise_window();
    if(game) {
      cpt++;
    }
    
    state = -1;

    newtime = MLV_get_time();
    frametime = newtime - endtime;
    frametime /= 1000;
    extratime = 1.0 / 60 - frametime;
    if(extratime > 0 ){
      MLV_wait_milliseconds((int)(extratime * 1000));
      
    }
  }
  MLV_free_window();
  free(shots.tab);
  free(t.tab);
  return 0;
}
