/*
 * AppInit.h
 *
 *  Created on: Aug 9, 2014
 *      Author: herve
 */

#ifndef APPINIT_H_
#define APPINIT_H_

#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_gfxPrimitives.h>
#include <SDL/SDL_ttf.h>

#include <time.h>
#include <unistd.h>
#include <string.h>


//Global déclarations
SDL_Surface * screen;

struct items{
	int value;
	int direction;
	int speed;
};

struct strScore{
	char name[20];
	long score;
};

struct items tabField[40][30];
struct items tabFieldtmp[40][30];

struct strScore tabScore[10];


//images
SDL_Surface* grass;
SDL_Surface* tempIN;
SDL_Surface* mush;
SDL_Surface* mush2;
SDL_Surface* mush3;
SDL_Surface* lifeSpot;
SDL_Surface* backScreen;

//The music that will be played
Mix_Music *music;

//The sound effects that will be used
Mix_Chunk *click;
Mix_Chunk *bow;
Mix_Chunk *hit;
Mix_Chunk *blop;


/*
 * 0 vide
 * 11-10 mushroom1
 * 22-20 mushroom2
 * 33-30 mushroom3
 * 4 corps d'un worms
 * 5 head d'un worms
 * 62-60 spider
 * 73-70 bee
 * 8 DTT
 * 9 Arrow
 */



int initSDL();
int setVideo(int tvideo);
void initLevel(int level);
char* get_file(char* file);
int load_sound();
void quitSDL();

#endif /* APPINIT_H_ */
