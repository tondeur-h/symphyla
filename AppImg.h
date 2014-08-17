/*
 * AppImg.h
 *
 *  Created on: Aug 9, 2014
 *      Author: herve
 */

#ifndef APPIMG_H_
#define APPIMG_H_



#include "AppInit.h"

void load_img();
void draw_grass();
void draw_mush_worms();
void draw_best_score();
void draw_sound();
void menu(int pos);
void destroy_img();
void fire(int pos);
void draw_firstScreen();
void draw_score_life(int level,int score, int life);
void draw_copyright();
void draw_game_over();
void pauseScreen(long duration);
int isBestScore(long score);
void save_bestScore();
void slide_bestScore(int pos);
void draw_hit();
void move_all();
void move_man(int pos);
int manage_colisions();
#endif /* APPIMG_H_ */
