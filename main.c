//============================================================================
// Name        : main.c
// Author      : Tondeur Herve
// Version     :
// Copyright   : GPL V3.0
// Description :  centipede clone in C & SDL, Ansi-style
//============================================================================

#include "global_var.h"
#include "AppInit.h"
#include "AppImg.h"

#include <stdio.h>
#include <stdlib.h>


//local events
SDL_Event event;

//const menu game
const int MNPLAY=1;
const int MNSOUND=2;
const int MNSCORE=3;
const int MNQUIT=4;
//const GAME
const int GAMEON=0;
const int GAMEOFF=-1;
const int GAMESCORE=1;
const int GAMESOUND=2;

const int QUITGAME=0;

//frame speed
const long FRAMES_PER_SECOND_MIN=15;




/************
 *   MAIN
 ************/
int main(int argc, char ** argv) {
int runas;

if (argc==1){
	runas=1;
} else
{
	runas=0;
}

//center windows on screen
putenv("SDL_VIDEO_CENTERED=1");

//init SDL & set video mode
if (initSDL()==-1) {printf("Init SDL KO!");exit(1);}
if (setVideo(runas)==-1){printf("set video mode KO!");exit(2);}


//load images in surfaces and sounds
load_img();

load_sound();

if (soundOn==1){Mix_PlayMusic(music,-1);} //play music

//init first level => change it TODO

//for allowed repeating key press
SDL_EnableKeyRepeat(10,5);
SDL_ShowCursor(SDL_DISABLE);

//game loop
while(goout!=QUITGAME) {
	//first tick before drawing for calculate time elapse
	tick=SDL_GetTicks();
	//if there is an event
	if (SDL_PollEvent(&event) == 0 ) {
		//no events => so drawing screen
		//if playing is on
		if (play==GAMEON){
			//DRAW SCREEN HERE
			draw_grass();
			draw_mush_worms();
			move_man(posFireGun);
			move_all();
			score=score+manage_colisions();
			draw_score_life(level,score,life);
		}
		if (play==GAMESOUND){
			//draw_firstScreen();
			draw_sound();
				}
		if (play==GAMESCORE){
			draw_firstScreen();
			draw_best_score();
		}
		if (play==GAMEOFF){
			//MENU SCREEN
			draw_firstScreen();
			menu(menusel);
			draw_copyright();
		}

		//update the screen
		SDL_UpdateRect(screen,0,0,0,0);

		//set frame rate
		spent=SDL_GetTicks()-tick;
		FRAMES_PER_SECOND=FRAMES_PER_SECOND_MIN+level*2;
		if (spent>(1000/FRAMES_PER_SECOND)) {spent=(1000/FRAMES_PER_SECOND);}
		SDL_Delay((1000/FRAMES_PER_SECOND)-spent);
		//-----DEBUG-----
		//printf("play %d \n",play);
		//printf("menu %u \n",menusel);
		//---------------
	} //if
	else {
	// an event?
		switch (event.type)
		{
		// exit if the window is closed
		case SDL_QUIT:
				goout = QUITGAME;
				break;

		// check for keypresses
		case SDL_KEYDOWN:
						{
						if (event.key.keysym.sym == SDLK_q && play==GAMEOFF){goout = QUITGAME;} //quit game
						if (event.key.keysym.sym == SDLK_ESCAPE && play==GAMEON){play = GAMEOFF; if (soundOn==1){Mix_ResumeMusic();};level=1;} //stop playing
						if (event.key.keysym.sym == SDLK_F1 && play==GAMEOFF){life=3;score=0;initLevel(level);play = GAMEON; if (soundOn==1){Mix_PauseMusic();}} //playing
						if (event.key.keysym.sym==SDLK_LEFT && play==GAMEON){posFireGun--; if (posFireGun<0) posFireGun=0;}
						if (event.key.keysym.sym==SDLK_RIGHT && play==GAMEON){posFireGun++; if (posFireGun>39) posFireGun=39;}
						if (event.key.keysym.sym==SDLK_SPACE && play==GAMEON){SDL_EnableKeyRepeat(150,100);fire(posFireGun);SDL_EnableKeyRepeat(10,5);} //fire in game
						if (event.key.keysym.sym==SDLK_RETURN && play==GAMEOFF && menusel==MNPLAY) {life=3;score=0;initLevel(level);play=GAMEON; if (soundOn==1){Mix_PauseMusic();}} //playing
						if (event.key.keysym.sym==SDLK_BACKSPACE && play==GAMESCORE && menusel==MNSCORE) {play=GAMEOFF;} //playing
						//if (event.key.keysym.sym==SDLK_BACKSPACE && play==GAMESOUND && menusel==MNSOUND) {play=GAMEOFF;} //playing
						if (event.key.keysym.sym==SDLK_RETURN && play==GAMEOFF && menusel==MNSCORE) {;play=GAMESCORE;} //show Scores
						if (event.key.keysym.sym==SDLK_RETURN && play==GAMEOFF && menusel==MNSOUND) {;play=GAMESOUND;} //show Scores
						if (event.key.keysym.sym==SDLK_RETURN && play==GAMEOFF && menusel==MNQUIT) {goout=QUITGAME;} //quit game
						if (event.key.keysym.sym==SDLK_UP && play==GAMEOFF){menusel--;if (menusel<1) menusel=1; if (soundOn==1){Mix_PlayChannel(-1,click,0);}} //move up menu
						if (event.key.keysym.sym==SDLK_DOWN && play==GAMEOFF){menusel++;if (menusel>4) menusel=4; if (soundOn==1){Mix_PlayChannel(-1,click,0);}} //move down menu
						if (event.key.keysym.sym==SDLK_i && play==GAMEON){SHEET30=29; SHEET29=30;}
						}
						break;

		} // end switch
	} //else
}

//free surfaces images
destroy_img();

//quit game est free main SDL screen
quitSDL();
//bye
return 0;
}
