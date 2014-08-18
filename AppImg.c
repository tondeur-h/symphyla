/*
 * AppImg.c
 *
 *  Created on: Aug 9, 2014
 *      Author: herve
 */

/*
 * 0 vide
 * 11-10 mushroom1
 * 22-20 mushroom2
 * 33-30 mushroom3
 * 4 corps d'un worms
 * 5 head Left d'un worms
 * 6 head right d'un worms
 * 62-60 spider
 * 73-70 bee
 * 8 DTT
 * 9 Arrow
 */

#include "AppImg.h"
#include "global_var.h"
#include "AppInput.h"

//local déclarations
const int RESOLUTION=16;

SDL_Surface* imgGrass=NULL;
SDL_Surface* tempIN;
SDL_Surface* imgMush1=NULL;
SDL_Surface* imgMush2=NULL;
SDL_Surface* imgMush3=NULL;
SDL_Surface* imgArrow=NULL;
SDL_Surface* imgMan=NULL;
SDL_Surface* imgLifeSpot=NULL;
SDL_Surface* imgBackScreen=NULL;
SDL_Surface* imgwormb=NULL;
//SDL_Surface* imgwormhl=NULL;
//SDL_Surface* imgwormhr=NULL;
TTF_Font *fontSmall;
TTF_Font *fontGame;
TTF_Font *fontMenu;
SDL_Surface *txtMessage;
char buffer[50];
SDL_Rect rm;
SDL_Rect rd;

//tmp_variables
int value, direction;




/***************
 * fire a arrow
 ***************/
void fire(int pos){
	if (soundOn==1) {Mix_PlayChannel(-1,bow,0);};
	tabField[pos][28].value=9;
}




/*************************
 * move man left and right
 *************************/
void move_man(int pos){
	boxRGBA(screen,0,464,640,480,255,255,255,100);
	rm.x=pos*RESOLUTION;rm.y=29*RESOLUTION;
	SDL_SetColorKey(imgMan,SDL_SRCCOLORKEY, SDL_MapRGB(screen->format, 255, 255, 255));
	SDL_BlitSurface(imgMan, NULL, screen,&rm);
}




/******************************
 * move all elements on the tab
 ******************************/
void move_all(){
int x,y;

//vider tmp tout mettre a zero
for (x=0;x<40;x++){for (y=0;y<30;y++){
	tabFieldtmp[x][y].value=0;
	tabFieldtmp[x][y].direction=0;
	tabFieldtmp[x][y].speed=0;
}}


//make all moves and modify tmp
for (x=0;x<40;x++){
	for (y=0;y<30;y++){

		 direction=tabField[x][y].direction;
		 value=tabField[x][y].value;


		 //recopier les items non worms
		if (tabField[x][y].value>=9) {tabFieldtmp[x][y].value=value;tabFieldtmp[x][y].direction=direction;}

		//move fired object
		if (tabField[x][y].value==9 ){
			//if not out off screen
			if (y>0){
				if (tabFieldtmp[x][y-1].value==0){
					tabFieldtmp[x][y-1].value=9;
					tabFieldtmp[x][y].value=0;
				} //else do nothing, calculate in colission routine
			} else
			{ //out of screen
				tabFieldtmp[x][y].value=0;
			}//if
		}//if


		//move worms body to the direction

		//peut bouger dans la direction indiqué...
		if (value==4){

			if (((x+direction)>=0) && ((x+direction)<40) && (tabField[x+direction][y].value<10) ){
				tabFieldtmp[x+direction][y].value=value;
				tabFieldtmp[x+direction][y].direction=direction;
				//tabField[x][y].value=0;
				//tabField[x][y].direction=0;
			}
			else
			{
				//ne peut pas bouger dans la direction indiqué
		 	 	//changer de direction, baisser d'un palier si possible, sinon monter d'un palier si possible
				if (tabField[x][y+1].value<10){
					tabFieldtmp[x][y+1].direction=direction*-1;
					tabFieldtmp[x][y+1].value=4;
				}else
				{
					tabField[x][y+1].direction=direction*-1;
					tabField[x][y+1].value=0;
					tabFieldtmp[x][y+1].direction=direction*-1;
					tabFieldtmp[x][y+1].value=4;
				}
				//tabField[x][y].value=0;
				//tabField[x][y-1].direction=0;
			} //else

		}//if



	}//for
}//for



//copy tmp to current
for (x=0;x<40;x++){for (y=0;y<30;y++){
	tabField[x][y].value=tabFieldtmp[x][y].value;
	tabField[x][y].direction=tabFieldtmp[x][y].direction;
	tabField[x][y].speed=tabFieldtmp[x][y].speed;
}}

}




/*********************************
 * mage_colisions netween elements
 *********************************/
int manage_colisions(){

	int x,y;
	int out=0;
	for (x=0;x<40;x++){
		for (y=0;y<30;y++){

			if (tabField[x][y].value==9){
			//tester si y-& est soit 11 10 22 21 20 33 32 31 30
				if (tabField[x][y-1].value==11 || tabField[x][y-1].value==22 || tabField[x][y-1].value==21 || tabField[x][y-1].value==33 || tabField[x][y-1].value==32 || tabField[x][y-1].value==31){tabField[x][y-1].value--;tabField[x][y].value=0;if (soundOn==1){Mix_PlayChannel(-1,blop,0);}}
				if (tabField[x][y-1].value==10){tabField[x][y-1].value=0;tabField[x][y].value=0;out=10;if (soundOn==1){Mix_PlayChannel(-1,blop,0);}}
				if (tabField[x][y-1].value==20){tabField[x][y-1].value=0;tabField[x][y].value=0;out=50;if (soundOn==1){Mix_PlayChannel(-1,blop,0);}}
				if (tabField[x][y-1].value==30){tabField[x][y-1].value=0;tabField[x][y].value=0;out=100;if (soundOn==1){Mix_PlayChannel(-1,blop,0);}}
				//hit worm
				if (tabField[x][y-1].value==4){tabField[x][y].value=0;tabField[x][y-1].value=0;tabField[x][y-1].direction=0;out=500; leftWorm--;if (soundOn==1){Mix_PlayChannel(-1,hit,0);}}
			}//if

				//colision with player si y==29 alors ok colision
				if ((tabField[x][y].value==4) && y==SHEET29) {
					life--;
					if (life==0){
						if (soundOn==1){Mix_ResumeMusic();}
						level=1;
						draw_game_over();
						//ask name if in 10 best score
						int bs=isBestScore(score);
						if (bs>-1){
						strcpy(tabScore[bs].name,enter_name(screen,imgBackScreen,score));
						tabScore[bs].score=score;
						save_bestScore();
						pauseScreen(10);
						}
						play = -1;
						return 0;
					}
					draw_hit();
					initLevel(level);
				}

		}//for
	}//for

	//count number au worm left si y<30 alors test derniere ligne donc OK
	int i=0;
	for (x=0;x<40;x++){for (y=0;y<SHEET30;y++){if (tabField[x][y].value==4) i++;leftWorm=i;}}

	if (leftWorm==0) {level++;initLevel(level);}

	return out;
}




int isBestScore(long score){
int pos;

//inferieur a la postion 9
if (score<=tabScore[9].score) return -1;


for (pos=9;pos>=1;pos--){
	//position exacte
	if (score==tabScore[pos].score) {slide_bestScore(pos);return pos;}
	//compris entre
	if (score>tabScore[pos].score && score<tabScore[pos-1].score){slide_bestScore(pos);return pos;}
}

//superieur a 0
if (score>tabScore[0].score) {slide_bestScore(0);return 0;}

return -1;
}


void slide_bestScore(int pos){
int num;
	if (pos==9) return;
	for (num=8;num>=pos;num--){
		strcpy(tabScore[num+1].name,tabScore[num].name);
		tabScore[num+1].score=tabScore[num].score;
	}
}




void save_bestScore(){
	int num=0;
	FILE* f=fopen(get_file("/gfx/score.scr"),"w");
			if (f){
				for (num=0;num<10;num++){
					fprintf(f,"%s\n",tabScore[num].name);
					fprintf(f,"%ld\n",tabScore[num].score);
				}
			fclose(f);
			}
}




void pauseScreen(long duration){
	SDL_Delay(duration);
	tick=SDL_GetTicks();
}


/***********************
 * load images from disk
 ***********************/
void load_img(){
IMG_Init(IMG_INIT_PNG);
//prepare score
TTF_Init();

//data_path=initializeDataPath();
//printf("chemin %s ",data_path);

//Open the font
fontSmall = TTF_OpenFont(get_file("/gfx/font.fnt"), 18 );
if (fontSmall==NULL){printf("Erreur loading font...\n");exit(3);}

//Open the font
fontGame = TTF_OpenFont(get_file("/gfx/font.fnt"), 28 );
if (fontGame==NULL){printf("Erreur loading font...\n");exit(3);}

//Open the font
fontMenu = TTF_OpenFont(get_file("/gfx/font.fnt"), 48 );
if (fontMenu==NULL){printf("Erreur loading font...\n");exit(3);}

//load grass & convert it to right format
tempIN=IMG_Load(get_file("/gfx/symphylaBack.png"));
imgBackScreen = SDL_DisplayFormat(tempIN);
if (imgBackScreen==NULL){printf("error loading firstScreen...\n");exit(4);}

//load grass & convert it to right format
tempIN=IMG_Load("/home/herve/workspace/symphyla/gfx/grass.png");
imgGrass = SDL_DisplayFormat(tempIN);
if (imgGrass==NULL){printf("error loading grass...\n");exit(4);}

//load mushroom 1
tempIN=IMG_Load(get_file("/gfx/mushroom.png"));
imgMush1 = SDL_DisplayFormat(tempIN);
if (imgMush1==NULL){printf("error loading mushroom1...\n");exit(4);}

//load mushroom 2
tempIN=IMG_Load(get_file("/gfx/mushroom2.png"));
imgMush2 = SDL_DisplayFormat(tempIN);
if (imgMush2==NULL){printf("error loading mushroom2...\n");exit(4);}

//load mushroom 3
tempIN=IMG_Load(get_file("/gfx/mushroom3.png"));
imgMush3 = SDL_DisplayFormat(tempIN);
if (imgMush3==NULL){printf("error loading mushroom3...\n");exit(4);}

//load life cartoon
tempIN=IMG_Load(get_file("/gfx/life.png"));
imgLifeSpot = SDL_DisplayFormat(tempIN);
if (imgLifeSpot==NULL){printf("error loading life cartoon...\n");exit(4);}

//load arrow
tempIN=IMG_Load(get_file("/gfx/arrow.png"));
imgArrow = SDL_DisplayFormat(tempIN);
if (imgArrow==NULL){printf("error loading arrow...\n");exit(4);}

//load man
tempIN=IMG_Load(get_file("/gfx/man.png"));
imgMan = SDL_DisplayFormat(tempIN);
if (imgMan==NULL){printf("error loading man...\n");exit(4);}

//load wormb
tempIN=IMG_Load(get_file("/gfx/wormb.png"));
imgwormb = SDL_DisplayFormat(tempIN);
if (imgwormb==NULL){printf("error loading wormb...\n");exit(4);}

//load wormhl
/*
tempIN=IMG_Load(get_file("/gfx/wormhl.png"));
imgwormhl = SDL_DisplayFormat(tempIN);
if (imgwormhl==NULL){printf("error loading wormhl...\n");exit(4);}

//load wormhr
tempIN=IMG_Load(get_file("/gfx/wormhr.png"));
imgwormhr = SDL_DisplayFormat(tempIN);
if (imgwormhr==NULL){printf("error loading wormhr...\n");exit(4);}
*/

//free unused surfaces
SDL_FreeSurface(tempIN);

}




/**********************
 * draw bacgroud grass
 **********************/
void draw_grass(){
int x,y;
for (x=0;x<4;x++){
	for (y=0;y<4;y++){
		rd.x=x*160;rd.y=y*120;
		SDL_BlitSurface(imgGrass, NULL, screen,&rd);
	} //for
} //for
}




/**************************
 * draw diffrents mushroom
 **************************/
void draw_mush_worms(){
int x,y;
for (x=0;x<40;x++){
	for (y=0;y<30;y++){

rm.x=x*RESOLUTION;rm.y=y*RESOLUTION;

if (tabField[x][y].value==11 || tabField[x][y].value==10 ){
SDL_SetColorKey(imgMush1,SDL_SRCCOLORKEY, SDL_MapRGB(screen->format, 255, 255, 255));
SDL_BlitSurface(imgMush1, NULL, screen,&rm);
} //if

if (tabField[x][y].value==22 || tabField[x][y].value==21 || tabField[x][y].value==20){
SDL_SetColorKey(imgMush2,SDL_SRCCOLORKEY, SDL_MapRGB(screen->format, 255, 255, 255));
SDL_BlitSurface(imgMush2, NULL, screen,&rm);
} //if

if (tabField[x][y].value==33 || tabField[x][y].value==32 || tabField[x][y].value==31 || tabField[x][y].value==30){
SDL_SetColorKey(imgMush3,SDL_SRCCOLORKEY, SDL_MapRGB(screen->format, 255, 255, 255));
SDL_BlitSurface(imgMush3, NULL, screen,&rm);
} //if

if (tabField[x][y].value==9){
SDL_SetColorKey(imgArrow,SDL_SRCCOLORKEY, SDL_MapRGB(screen->format, 255, 255, 255));
SDL_BlitSurface(imgArrow, NULL, screen,&rm);
} //if

if (tabField[x][y].value==4){
SDL_SetColorKey(imgwormb,SDL_SRCCOLORKEY, SDL_MapRGB(screen->format, 255, 255, 255));
SDL_BlitSurface(imgwormb, NULL, screen,&rm);
} //if

/*
if (tabField[x][y].value==5){
SDL_SetColorKey(imgwormhl,SDL_SRCCOLORKEY, SDL_MapRGB(screen->format, 255, 255, 255));
SDL_BlitSurface(imgwormhl, NULL, screen,&rm);
} //if

if (tabField[x][y].value==6){
SDL_SetColorKey(imgwormhr,SDL_SRCCOLORKEY, SDL_MapRGB(screen->format, 255, 255, 255));
SDL_BlitSurface(imgwormhr, NULL, screen,&rm);
} //if
*/

}//for
}//for
}




/*************************
* Draw menu
**************************/
void menu(int pos){
	int left=200;
	int top=200;
	int width=285;
	int height=170;
	SDL_Rect rm;
	SDL_Color playcolor;
	SDL_Color quitcolor;
	SDL_Color soundcolor;
	SDL_Color scorecolor;
	SDL_Surface* playTxt;
	SDL_Surface* soundTxt;
	SDL_Surface* scoreTxt;
	SDL_Surface* quitTxt;

	roundedBoxRGBA(screen,left-10,top-10,left+width+10,top+height+10,20,0,0,0,200);


	if (pos==1){
	playcolor.r=255;playcolor.g=255;playcolor.b=0;
	soundcolor.r=255;soundcolor.g=255;soundcolor.b=255;
	scorecolor.r=255;scorecolor.g=255;scorecolor.b=255;
	quitcolor.r=255;quitcolor.g=255;quitcolor.b=255;
	}
	if (pos==2)
	{
		playcolor.r=255;playcolor.g=255;playcolor.b=255;
		soundcolor.r=255;soundcolor.g=255;soundcolor.b=0;
		scorecolor.r=255;scorecolor.g=255;scorecolor.b=255;
		quitcolor.r=255;quitcolor.g=255;quitcolor.b=255;
	}
	if (pos==3)
		{
			playcolor.r=255;playcolor.g=255;playcolor.b=255;
			soundcolor.r=255;soundcolor.g=255;soundcolor.b=255;
			scorecolor.r=255;scorecolor.g=255;scorecolor.b=0;
			quitcolor.r=255;quitcolor.g=255;quitcolor.b=255;
		}
	if (pos==4)
		{
			playcolor.r=255;playcolor.g=255;playcolor.b=255;
			soundcolor.r=255;soundcolor.g=255;soundcolor.b=255;
			scorecolor.r=255;scorecolor.g=255;scorecolor.b=255;
			quitcolor.r=255;quitcolor.g=255;quitcolor.b=0;
		}
	playTxt=TTF_RenderText_Blended(fontMenu,"PLAY [F1]",playcolor);
	soundTxt=TTF_RenderText_Blended(fontMenu,"SOUND",soundcolor);
	scoreTxt=TTF_RenderText_Blended(fontMenu,"SCORE",scorecolor);
	quitTxt=TTF_RenderText_Blended(fontMenu,"QUIT [Q]",quitcolor);

	rm.x=left;rm.y=top;
	SDL_BlitSurface(playTxt,NULL,screen,&rm);

	rm.x=left;rm.y=top+40;
	SDL_BlitSurface(soundTxt,NULL,screen,&rm);

	rm.x=left;rm.y=top+80;
	SDL_BlitSurface(scoreTxt,NULL,screen,&rm);

	rm.x=left;rm.y=top+120;
	SDL_BlitSurface(quitTxt,NULL,screen,&rm);

	SDL_FreeSurface(playTxt);
	SDL_FreeSurface(soundTxt);
	SDL_FreeSurface(scoreTxt);
	SDL_FreeSurface(quitTxt);
}




/***********************
 * draw best score page
 ***********************/
void draw_best_score(){
	boxRGBA(screen,100,150,540,400,0,0,0,100);
	SDL_Color fgColor = { 200, 200, 200 };
	txtMessage = TTF_RenderText_Blended( fontMenu, "BEST SCORES", fgColor);
	rm.x=150;rm.y=152;
	SDL_BlitSurface(txtMessage, NULL, screen,&rm);
	//draw score from file score, if not exist => draw empty...

	//draw info back to menu
	SDL_Color rtmColor = { 255, 0, 0 };
		txtMessage = TTF_RenderText_Blended( fontSmall, "Press BackSpace for return to menu", rtmColor);
		rm.x=230;rm.y=460;
		SDL_BlitSurface(txtMessage, NULL, screen,&rm);


		//draw info
		int num;
		for (num=0;num<10;num++){

		 	fgColor.r=250-(num*15); fgColor.g=250; fgColor.b=0 ;
		 	sprintf(buffer,"%s",tabScore[num].name);
			txtMessage = TTF_RenderText_Blended( fontGame, buffer, fgColor);
			rm.x=150;rm.y=190+(num*22);
			SDL_BlitSurface(txtMessage, NULL, screen,&rm);



			sprintf(buffer,"%06ld",tabScore[num].score);
			txtMessage = TTF_RenderText_Blended( fontGame, buffer, fgColor);
			rm.x=380;rm.y=190+(num*22);
			SDL_BlitSurface(txtMessage, NULL, screen,&rm);


		}
}




/********************
 * draw sound page
 ********************/
void draw_sound(){
	int left=200;
				int top=200;
				int width=275;
				int height=90;
				SDL_Rect rm;
				SDL_Color soundoncolor;
				SDL_Color soundoffcolor;
				int pos=1;
				SDL_Surface* ONTxt;
				SDL_Surface* OFFTxt;
				int quitLoop=0;
				SDL_Event menuEvent;
const int QUITMENU=1;

//recall current position
		if (soundOn==1) {pos=1;} else {pos=2;}

//menu loop
while(quitLoop!=QUITMENU) {

	if (SDL_PollEvent(&menuEvent) == 0 ) {
	//draw background
	SDL_BlitSurface(imgBackScreen, NULL, screen,NULL);

	//draw info back to menu
		SDL_Color rtmColor = { 255, 0, 0 };
		txtMessage = TTF_RenderText_Blended( fontSmall, "Press BackSpace for return to menu", rtmColor);
		rm.x=230;rm.y=460;
		SDL_BlitSurface(txtMessage, NULL, screen,&rm);

		roundedBoxRGBA(screen,left-10,top-10,left+width+10,top+height+10,20,0,0,0,200);



		if (pos==1){
					soundoncolor.r=255;soundoncolor.g=255;soundoncolor.b=0;
				soundoffcolor.r=255;soundoffcolor.g=255;soundoffcolor.b=255;

		}
		if (pos==2)
		{
					soundoncolor.r=255;soundoncolor.g=255;soundoncolor.b=255;
					soundoffcolor.r=255;soundoffcolor.g=255;soundoffcolor.b=0;

		}

		ONTxt=TTF_RenderText_Blended(fontMenu,"SOUND ON",soundoncolor);
		OFFTxt=TTF_RenderText_Blended(fontMenu,"SOUND OFF",soundoffcolor);

		rm.x=left;rm.y=top;
		SDL_BlitSurface(ONTxt,NULL,screen,&rm);

		rm.x=left;rm.y=top+40;
		SDL_BlitSurface(OFFTxt,NULL,screen,&rm);

		SDL_UpdateRect(screen,0,0,0,0);

SDL_FreeSurface(ONTxt);
SDL_FreeSurface(OFFTxt);


	} //if
		else {
		// an event?
			switch (menuEvent.type)
			{
			case SDL_KEYDOWN:{
				if (menuEvent.key.keysym.sym==SDLK_BACKSPACE) {if (soundOn==0){Mix_PauseMusic();} else {Mix_ResumeMusic();};quitLoop=QUITMENU;play=-1;} //playing
				if (menuEvent.key.keysym.sym==SDLK_UP){pos=1;soundOn=1;} //move up menu
				if (menuEvent.key.keysym.sym==SDLK_DOWN){pos=2;soundOn=0;} //move down menu
							 }
							 break;

			}//switch

		} //else

}//while

//reset frame rate...
pauseScreen(10);

}




/***************************
 * Draw presentation screen
 ***************************/
void draw_firstScreen(){
	SDL_BlitSurface(imgBackScreen, NULL, screen,NULL);
	}


void draw_copyright(){
	//draw info copyright
			SDL_Color rtmColor = { 255, 0, 0 };
				txtMessage = TTF_RenderText_Blended( fontSmall, "Tondeur Herve copyright(c) 2014 GPL v3.0", rtmColor);
				rm.x=200;rm.y=462;
				SDL_BlitSurface(txtMessage, NULL, screen,&rm);

}


void draw_hit(){
	//draw hit
				SDL_Color rtmColor = { 80, 255, 120 };
				sprintf(buffer,"LIFE:%02u [RETRY]",life);
					txtMessage = TTF_RenderText_Blended( fontMenu, buffer, rtmColor);
					rm.x=320 -(txtMessage->w/2);rm.y=240 -(txtMessage->h/2);
					roundedBoxRGBA(screen,rm.x-1,rm.y-1,rm.x+txtMessage->w+1,rm.y+txtMessage->h+1,20,0,0,0,180);
					SDL_BlitSurface(txtMessage, NULL, screen,&rm);
					SDL_UpdateRect(screen,0,0,0,0);

				pauseScreen(2000);
}


void draw_game_over(){
	//draw hit
						SDL_Color rtbColor = { 0, 0, 0 };
						txtMessage = TTF_RenderText_Blended( fontMenu, "...GAME OVER...", rtbColor);
						rm.x=316 -(txtMessage->w/2);rm.y=236 -(txtMessage->h/2);
						SDL_BlitSurface(txtMessage, NULL, screen,&rm);

						SDL_Color rtmColor = { 255, 0, 0 };
						txtMessage = TTF_RenderText_Blended( fontMenu, "...GAME OVER...", rtmColor);
						rm.x=320 -(txtMessage->w/2);rm.y=240 -(txtMessage->h/2);


						roundedBoxRGBA(screen,rm.x-1,rm.y-1,rm.x+txtMessage->w+1,rm.y+txtMessage->h+1,20,0,0,0,180);

						SDL_BlitSurface(txtMessage, NULL, screen,&rm);

						SDL_UpdateRect(screen,0,0,0,0);
	pauseScreen(5000);
}

/***************************
 * Drawscore, life and level
 ***************************/
void draw_score_life(int level,int score,int lif){

//compute extra life...
int tmpLife=0;
if (score>0) {tmpLife=score/15000;}
life=life+(tmpLife-extraLife);
extraLife=tmpLife;

//render score image
boxRGBA(screen,0,0,640,27,0,0,0,80);
SDL_Color fgColor = { 200, 200, 200 };
sprintf(buffer,"SCORE: %06u",score);
txtMessage = TTF_RenderText_Blended( fontGame, buffer, fgColor);
rm.x=25*RESOLUTION;rm.y=0*RESOLUTION;
SDL_BlitSurface(txtMessage, NULL, screen,&rm);


//render score image
sprintf(buffer,"LEVEL: %u",level);
txtMessage = TTF_RenderText_Blended( fontGame, buffer, fgColor);
rm.x=0*RESOLUTION;rm.y=0*RESOLUTION;
SDL_BlitSurface(txtMessage, NULL, screen,&rm);


//render life cartoons
int nblife;
SDL_SetColorKey(imgLifeSpot,SDL_SRCCOLORKEY, SDL_MapRGB(screen->format, 255, 255, 255));
for (nblife=1;nblife<=lif;nblife++){
rm.x=(9+nblife)*16;rm.y=6;
SDL_BlitSurface(imgLifeSpot, NULL, screen,&rm);
} //for
}




/*******************************
 * Destroy elements from memory
 *******************************/
void destroy_img(){
//destroy all surface before quitting
SDL_FreeSurface(imgGrass);
SDL_FreeSurface(imgMush1);
SDL_FreeSurface(imgMush2);
SDL_FreeSurface(imgMush3);
SDL_FreeSurface(imgLifeSpot);
SDL_FreeSurface(imgBackScreen);
SDL_FreeSurface(imgArrow);
SDL_FreeSurface(imgMan);
SDL_FreeSurface(imgwormb);
//SDL_FreeSurface(imgwormhl);
//SDL_FreeSurface(imgwormhr);
}
