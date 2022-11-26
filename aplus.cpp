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

SDL_Surface* gScreenSurface = NULL;

SDL_Texture* start_texture = NULL;  		//texture of start scene
SDL_Texture* explanation_texture = NULL;	//texture of explanation scene
SDL_Texture* burning_texture = NULL;		//texture of burning small icon
SDL_Texture* stunning_texture = NULL; 		//texture of stunning small icon
SDL_Texture* claw_texture = NULL;			//texture of claw(attack of professor)
SDL_Texture* get_f_texture = NULL;			//texture of get f

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
	gScreenSurface = SDL_GetWindowSurface( gWindow );
	if( gScreenSurface == NULL )
	{
		printf( "gScreenSurface could not be created! SDL Error: %s\n", SDL_GetError() );
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
	burning_texture = loadTexture("./img/fire.bmp");
	stunning_texture = loadTexture("./img/stun.bmp");
	claw_texture = loadTexture("./img/claw.bmp");
	get_f_texture = loadTexture("./img/get_f.bmp");
	
	if( start_texture == NULL ){
		printf( "Failed to load ./img/background.bmp !\n" );	success = false;
	}
	if( explanation_texture == NULL ){
		printf( "Failed to load ./img/explanation.bmp!\n" );	success = false;
	}
	if( burning_texture = NULL ){
		printf( "Failed to load ./img/fire.bmp!\n" );			success = false;
	}
	if( stunning_texture = NULL ){
		printf( "Failed to load ./img/stun.bmp!\n" );			success = false;
	}
	if( claw_texture = NULL ){
		printf( "Failed to load ./img/claw.bmp!\n" );			success = false;
	}
	if( get_f_texture = NULL ){
		printf( "Failed to load ./img/get_f.bmp!\n" );			success = false;
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
	SDL_DestroyTexture( claw_texture );			claw_texture = NULL;
	SDL_DestroyTexture( get_f_texture );		get_f_texture = NULL;
	
	
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
		
		SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, 0, 0xFF, 0xFF ) );
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
			
			student_class student;
			
			professor_class* professor;
			professor = new professor_class [5];
			for(int i=0;i<5;i++){
				professor[i] = professor_class(i);
				
			}
			
			
			SDL_Rect student_fire_rect = {90,270,90,90};
			
			
			SDL_Rect student_stun_rect = {180,270,90,90};
			
			
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
						SDL_RenderClear (gRenderer);//Clear screen
						SDL_RenderCopy( gRenderer, start_texture , NULL , NULL );//Render texture to screen
						SDL_RenderPresent( gRenderer );//Update screen
						
						if(e.type == SDL_KEYDOWN){
							
							switch( e.key.keysym.sym )
                	        {
                	    	    case SDLK_SPACE:
                	        	    state = explanation;
                	        		SDL_RenderClear (gRenderer);//Clear screen
									SDL_RenderCopy( gRenderer, explanation_texture , NULL , NULL );//Render texture to screen
									SDL_RenderPresent( gRenderer );//Update screen
                	        	    
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
					else if (state == student_attacking){
						
					}
					else if (state == professor_attacking){
						
						student.hurt( professor[ stage ].attack );

						switch ( professor[ stage ].special ){
							case health_to_attack:
								if (student.burning == true)	student.burning = false;
								if (student.stunning == true)	student.stunning = false;
								break;
							case swifty:
								if (student.burning == true)	student.burning = false;
								if (student.stunning == true)	student.stunning = false;
								break;
							case armored:
								if (student.burning == true)	student.burning = false;
								if (student.stunning == true)	student.stunning = false;
								break;
							case stun:
								if(student.stunning == false && professor[stage].stun_counter >= 6){
									student.stunning == true;
									professor[stage].stun_counter = 0;
								}
								else{
									if(professor[stage].stun_counter >= 2)
										professor[stage].stun_counter = 0;
									else
										professor[stage].stun_counter +=1;
									if (student.stunning == true){ student.stunning = false;}
								}
								
								if (student.burning == true){ student.burning = false;}
								break;
								
							case firing:
								if(student.burning == false && professor[stage].ignite_counter >= 5){
									student.burning == true;
									professor[stage].ignite_counter = 0;
								}
								else{
									if(professor[stage].ignite_counter >= 5)
										professor[stage].ignite_counter = 0;
									else
										professor[stage].ignite_counter +=1;
								}
								if(student.stunning == true){ student.stunning = false;}
								break;
							}
						if( student.burning == true){ student.hurt(3); }
						if( student.alive() == false ){	state = no_school; }
					}
					else if(state == gatcha){
						
					}
					else if(state == no_school){
						
					}
					else if(state == get_f){
					
					}
					else if(state == get_aplus){
						
					}
					
				}
				
				
			}
		}
	}

	//Free resources and close SDL
	close();
	
	return 0;
}
