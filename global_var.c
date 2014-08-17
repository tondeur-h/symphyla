/*
 * global_var.c
 *
 *  Created on: Aug 15, 2014
 *      Author: herve
 */

#include "global_var.h"

//boolean for main game loop
int goout=-1;
int play=-1;
int menusel=1;

//local variables game loop
int score=0;
int life=3;
int level=1;
int extraLife=0;
int posFireGun=20;

long tick=0;
long spent=0;

int soundOn=1;

long FRAMES_PER_SECOND=0;

//lenght of a worm
int lenghtWorm=10;
int leftWorm=10;

//sheet variable
int SHEET30=30;
int SHEET29=29;
