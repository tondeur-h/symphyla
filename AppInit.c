/*
 * AppInit.c
 *
 *  Created on: Aug 9, 2014
 *      Author: herve
 */

#include "AppInit.h"
#include "global_var.h"


const int SCREEN_HEIGHT=480;
const int SCREEN_WIDTH=640;
const int BITS_PER_PIXEL=0;


/****************
 * concat current directory with relatif path and file name...
 *****************/
char* get_file(char* file){
char* retour;
	retour=(char*)malloc(150*sizeof(char));
	strcpy(retour,(char*)get_current_dir_name());
	strcat(retour,file);
return retour;
}


/*******************
 * init SDL library
 *******************/
int initSDL(){
		//init SDL
	if (SDL_Init(SDL_INIT_EVERYTHING)==-1){
		printf("erreur init SDL");
		printf(SDL_GetError());
		return -1;
	}//if
	//Initialize SDL_mixer
	if( Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 1024 ) == -1 ){
		printf("erreur init SDL");
		printf(SDL_GetError());
		return -1;
	}//if

	//lire fichier best score
	int num;
	char xstr[20];

	FILE* f=fopen(get_file("/gfx/score.scr"),"r");
			if (f){
				for (num=0;num<10;num++){

				fscanf(f,"%s",xstr);
				strcpy(tabScore[num].name,xstr);

				fscanf(f,"%s",xstr);
				tabScore[num].score=atoi(xstr);
				}
			fclose(f);
			}

return 0;
}




/***********************
 * load sounds and music
 ************************/
int load_sound(){
	 //Load the music


	    music = Mix_LoadMUS(get_file("/gfx/music1.mus"));

	    //If there was a problem loading the music
	    if( music == NULL )
	    {
	        return -1;
	    }//if

	    //Load the sound effects
	    click = Mix_LoadWAV(get_file("/gfx/tick.snd"));
	    bow = Mix_LoadWAV(get_file("/gfx/bow.snd"));
	    hit = Mix_LoadWAV(get_file("/gfx/hit.snd"));
	    blop = Mix_LoadWAV(get_file("/gfx/blop.snd"));

	    //If there was a problem loading the sound effects
	    if( click == NULL )
	    {
	        return -1;
	    }//if

	    //If everything loaded fine
	    return 0;
}



/*****************************
 * Quit SDL & free main surface
 *****************************/
void quitSDL(){
//free surface and quit
SDL_FreeSurface(screen);
//Quit SDL_mixer
Mix_CloseAudio();
SDL_ShowCursor(SDL_ENABLE);
atexit(SDL_Quit);
printf("Terminating normally.\n");
}




/***********************
 * set video resolution
 ***********************/
int setVideo(int tvideo){
//set video mode
if (tvideo==0){
	screen=SDL_SetVideoMode(SCREEN_WIDTH,SCREEN_HEIGHT,BITS_PER_PIXEL, SDL_HWSURFACE | SDL_FULLSCREEN);
}
else
{
	screen=SDL_SetVideoMode(SCREEN_WIDTH,SCREEN_HEIGHT,BITS_PER_PIXEL, SDL_HWSURFACE);
}

if (screen == NULL ) {
printf("SetVideoMode failed. %s ",SDL_GetError());
return -1;
}//if
//caption windows
SDL_WM_SetCaption("Symphyla Game Version 1.0",NULL);
return 0;
}




/*********************
 * init a level
 *
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
 *********************/
void initLevel(int level){
	//emptying the board
	leftWorm=lenghtWorm+(level*2);


	int x,y;
	for (x=0;x<40;x++){
		for (y=0;y<30;y++){
			tabField[x][y].value=0;
			tabField[x][y].speed=0;
			tabField[x][y].direction=0;
		}//for
	}//for


	//take 50+more differents mushrooms
	int max_mush=50 +level*8;
	int iter;
	srand(time(NULL));

	for (iter=0;iter<max_mush;iter++){tabField[rand()%40][rand()%28].value=((rand()%3) +1)*11;}

	//set worm
	//can max 40 units lenght
	if (leftWorm>40) leftWorm=40;
	for (iter=0;iter<=leftWorm;iter++){tabField[iter][level-1].value=4;tabField[iter][level-1].direction=1;}
}
