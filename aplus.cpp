//Using SDL, SDL_image, standard IO, and strings
#include <SDL2/SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include "character.h"
#include <iostream>
using namespace std;
const int SCREEN_WIDTH = 1440;  //Screen dimension constants
const int SCREEN_HEIGHT = 810;

enum game_state {				//game states
	start,						//剛進入遊戲 
	explanation,				//背景解說 
	enter_stage,				//進入戰鬥畫面 
	student_attacking,  		//學生出牌 
	professor_attacking,		//教授回擊 
	gatcha,						//抽考卷解答 
	no_school,					//學生還沒打完BOSS就死掉的退學畫面 
	get_f,						//學生打完BOSS但是沒集滿考卷的F畫面 
	get_aplus					//學生成功拿到A+的畫面 
};
game_state state = start;	  //define state as the variable indicates current game state
int stage = 1;				  //stage 表示現在第幾個敵人 
	
bool init();				  //Starts up SDL and creates window

bool loadMedia();     		  //Loads media

void close();				  //Frees media and shuts down SDL

SDL_Texture* loadTexture( std::string path );  //Loads individual image as texture

SDL_Window* gWindow = NULL;	  //The window we'll be rendering to

SDL_Renderer* gRenderer = NULL;//The window renderer

SDL_Texture* start_texture = NULL;  		//texture of start scene
SDL_Texture* explanation_texture = NULL;	//texture of explanation scene
SDL_Texture* burning_texture = NULL;		//texture of burning small icon
SDL_Texture* stunning_texture = NULL; 		//texture of stunning small icon

bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
		//Set texture filtering to linear
	if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
	{
		printf( "Warning: Linear texture filtering not enabled!" );
	}
		//Create window
	gWindow = SDL_CreateWindow( "A+ or F", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
	if( gWindow == NULL )
	{
		printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	
	//Create renderer for window
	gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED /*| SDL_RENDERER_PRESENTVSYNC*/ );
	if( gRenderer == NULL )
	{
		printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	
	//Initialize renderer color
	SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
	//Initialize PNG loading
	int imgFlags = IMG_INIT_PNG;
	if( !( IMG_Init( imgFlags ) & imgFlags ) )
	{
		printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
		success = false;
	}	

	

	return success;
}

bool loadMedia()
{
	//Loading success flag
	bool success = true;

	start_texture = loadTexture( "./img/background.bmp" );		//在這裡來載入圖片 
	explanation_texture = loadTexture("./img/explanation.bmp");
	burning_texture = loadTexture("./img/fire.png");
	stunning_texture = loadTexture("./img/stun.png");
	
	if( start_texture == NULL ){
		printf( "Failed to load ./img/background.bmp !\n" );	success = false;
	}
	if( explanation_texture == NULL ){
		printf( "Failed to load ./img/explanation.bmp!\n" );	success = false;
	}
	if( burning_texture = NULL ){
		printf( "Failed to load ./img/fire.png!\n" );			success = false;
	}
	if( stunning_texture = NULL ){
		printf( "Failed to load ./img/stun.png!\n" );			success = false;
	}

	return success;
}

void close()
{
	//Free loaded image
	SDL_DestroyTexture( start_texture );		start_texture = NULL;
	SDL_DestroyTexture( explanation_texture );	explanation_texture = NULL;
	SDL_DestroyTexture( burning_texture );		burning_texture = NULL;
	SDL_DestroyTexture( stunning_texture );		stunning_texture = NULL;
	
	
	//Destroy window	
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

SDL_Texture* loadTexture( std::string path )
{
	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
	if( loadedSurface == NULL )
	{
		printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
	}
	else
	{
		//Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
		if( newTexture == NULL )
		{
			printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
		}

		//Get rid of old loaded surface
		SDL_FreeSurface( loadedSurface );
	}

	return newTexture;
}

int main( int argc, char* args[] )
{
	//Start up SDL and create window
	if( !init() )
	{
		printf( "Failed to initialize!\n" );
	}
	else
	{
		//Load media
		if( !loadMedia() )
		{
			printf( "Failed to load media!\n" );
		}
		else
		{	
			bool quit = false;	//Main loop flag

			SDL_Event e;		//Event handler

			//While application is running
			while( !quit )
			{
				
				//Handle events on queue
				while( SDL_PollEvent( &e ) != 0 )
				{
					//User requests quit
					if( e.type == SDL_QUIT )
					{
						quit = true;
					}
					else if(state == start){
						SDL_RenderClear( gRenderer );//Clear screen
						SDL_RenderCopy( gRenderer, start_texture, NULL, NULL );//Render texture to screen
						SDL_RenderPresent( gRenderer );//Update screen
						if(e.type == SDL_KEYDOWN){
							
							switch( e.key.keysym.sym )
                	        {
                	    	    case SDLK_SPACE:
                	        	    state = explanation;
                	        		SDL_RenderClear (gRenderer);//Clear screen
									SDL_RenderCopy( gRenderer, explanation_texture , NULL , NULL );//Render texture to screen
									SDL_RenderPresent( gRenderer );//Update screen
                	        	    SDL_Delay(500);
									break;
                   	     	}
						}
					}
					else if (state == explanation){
						
						
						if(e.type == SDL_KEYDOWN){
							switch( e.key.keysym.sym )
                   	    	{
                   	 	    case SDLK_SPACE:
                   	     	    state = enter_stage;
                   	     	    break;
                   	    	}
						
						}
					}
					else if (state == enter_stage){
						
					}
					
				}
				
				
			}
		}
	}

	//Free resources and close SDL
	close();

	return 0;
}
