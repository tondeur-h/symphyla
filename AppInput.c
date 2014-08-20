
#include "AppInput.h"
#include "AppInit.h"




//The surfaces
SDL_Surface *message;
SDL_Surface *msgscore;
SDL_Surface *screen;
SDL_Surface *background;

TTF_Font *font;

//The event structure
SDL_Event Localevent;



//The color of the font
SDL_Color textColor = { 0xFF, 0xFF, 0xFF };
SDL_Color scoreColor = { 0xFF, 0xFF, 0x00 };

    //The storage string
    char str[20];
    int idx=0;

    //The text surface
    SDL_Surface *text;

    char buffer[15];




void beginInput()
{
	//Open the font
	font = TTF_OpenFont(get_file("/gfx/font.fnt"), 40 );
	if (font==NULL){printf("Erreur loading font...\n");exit(3);}


    //Initialize the surface
    text = NULL;

    //Enable Unicode
    SDL_EnableUNICODE( SDL_ENABLE );
}



void apply_surface( int x, int y, int h, int w, SDL_Surface* source, SDL_Surface* destination )
{
    //Holds offsets
    SDL_Rect offset;

    //Get offsets
    offset.x = x;
    offset.y = y;
    offset.w=w;
    offset.h=h;

    //Blit
    SDL_BlitSurface( source, NULL, destination, &offset );
}



void killInput()
{
    //Free text surface
    SDL_FreeSurface( text );

    //Disable Unicode
    SDL_EnableUNICODE( SDL_DISABLE );
}




void handle_input()
{
    //If a key was pressed
    if( Localevent.type == SDL_KEYDOWN )
    {


        //If the string less than maximum size
        if( idx <= 16 )
        {
            //If the key is a space
            if( Localevent.key.keysym.unicode == (Uint16)' ' )
            {
                //Append the character
                str[idx]= (char)Localevent.key.keysym.unicode;
                str [idx+1]='\0';
                idx++;
            }


            //If the key is a number
            else if( ( Localevent.key.keysym.unicode >= (Uint16)'0' ) && ( Localevent.key.keysym.unicode <= (Uint16)'9' ) )
            {
            	 //Append the character
            	                str[idx]= (char)Localevent.key.keysym.unicode;
            	                str [idx+1]='\0';
            	                idx++;

            }
            //If the key is a uppercase letter
            else if( ( Localevent.key.keysym.unicode >= (Uint16)'A' ) && ( Localevent.key.keysym.unicode <= (Uint16)'Z' ) )
            {
            	 //Append the character
            	                str[idx]= (char)Localevent.key.keysym.unicode;
            	                str [idx+1]='\0';
            	                idx++;
            }
            //If the key is a lowercase letter
            else if( ( Localevent.key.keysym.unicode >= (Uint16)'a' ) && ( Localevent.key.keysym.unicode <= (Uint16)'z' ) )
            {
            	 //Append the character
            	                str[idx]= (char)Localevent.key.keysym.unicode;
            	                str [idx+1]='\0';
            	                idx++;
            }

        }

        //If backspace was pressed and the string isn't blank
        if( ( Localevent.key.keysym.sym == SDLK_BACKSPACE ) && ( idx > 0 ) )
        {
            //Remove a character from the end
        	idx--;
            str[idx]='\0';

        }

        //If the string was changed
        if( idx>=0 )
        {
            //Free the old surface
            SDL_FreeSurface( text );

            //Render a new text surface
            text = TTF_RenderText_Solid( font, str, textColor );
        }
    }
}


void show_centered()
{
    //If the surface isn't blank
    if( text != NULL )
    {
        //Show the name
        apply_surface( ( 640 - text->w ) / 2, ( 500 - text->h ) / 2,text->w,text->h, text, screen );
    }
}


char* enter_name(SDL_Surface* scrn, SDL_Surface* back, long score){
	screen=scrn;
	background=back;


    //Quit flag
    int quit = 0;


   //Keep track if whether or not the user has entered their name
    int nameEntered = 0;


    //The gets the user's name
    beginInput();

    SDL_EnableKeyRepeat(0,0);

    //Set the message
    sprintf(buffer,"%ld",score);
    message = TTF_RenderText_Solid( font, "New High Score! Enter Name:", textColor );
    msgscore = TTF_RenderText_Solid( font,buffer, scoreColor );

    //While the user hasn't quit
    while( quit == 0 )
    {
        //While there's events to handle
        while( SDL_PollEvent( &Localevent ) )
        {
            //If the user hasn't entered their name yet
            if( nameEntered == 0 )
            {
                //Get user input
                handle_input();

                //If the enter key was pressed
                if( ( Localevent.type == SDL_KEYDOWN ) && ( Localevent.key.keysym.sym == SDLK_RETURN ) )
                {
                    //Change the flag
                    nameEntered = 1;

                    //Free the old message surface
                    SDL_FreeSurface( message );

                    return str;
                }
            }


        }

        //Apply the background
        apply_surface( 0, 0,0,0, back, scrn );


        //Show the message
        apply_surface( ( 640 - message->w ) / 2, ( 340 - message->h ) / 2, message->w,message->h,message, scrn );
        apply_surface( ( 640 - msgscore->w ) / 2, ( 400 - msgscore->h ) / 2, msgscore->w,msgscore->h,msgscore, scrn );

        //Show the name on the screen
        show_centered();

        //Update the screen
        SDL_UpdateRect(scrn,0,0,0,0);

    }

    killInput();

    SDL_EnableKeyRepeat(10,5);

    return str;
}
