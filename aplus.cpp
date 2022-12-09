//Using SDL, SDL_image, standard IO, and strings
#include <SDL2/SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <cstdio>
#include <string>
#include <iostream>
#include <ctime>
#include <cmath>
#include "character.h"
#include "LTexture.h"
#include "healthbar.h"
#include "cards.h" 
using namespace std;
const int SCREEN_WIDTH = 1440;  //Screen dimension constants
const int SCREEN_HEIGHT = 810;

int block_x = SCREEN_WIDTH/16;
int block_y = SCREEN_HEIGHT/9;
bool getpaper[3] = {false};

enum game_state {				//game states
	start,						//just entered game
	explanation,				//game background detail explanation
	enter_stage,				//enter battling stage
	student_attacking,  		//student draw card and attack
	professor_attacking,		//professor attack student
	gatcha,						//draw a test paper
	no_school,					//student be defeated by professor
	get_f,						//student defeated boss but did not get all paper
	get_aplus					//student defeated boss and got all paper
};
game_state state = start;	  //define state as the variable indicates current game state
int stage = 1;				  //stage indicate which stage now is in
	
bool init();				  //Starts up SDL and creates window

bool loadMedia();     		  //Loads media

void close();				  //Frees media and shuts down SDL

SDL_Window* gWindow = NULL;	  //The window we'll be rendering to

SDL_Renderer* gRenderer = NULL;//The window renderer

SDL_Surface* gScreenSurface = NULL;

TTF_Font *conti_font = NULL;

TTF_Font *gFont = NULL;

TTF_Font *chinesefont = NULL;

int probability(double hit_rate, double avoid_rate);

void papertable_render();

void battlescene_render();

void continue_button_render();

void background_texture_render();

SDL_Rect student_burn_rect 		= { block_x*5 + 625	, 40 + block_y*4	 	, 40		 , 40		 }; //student burning icon position
SDL_Rect student_stun_rect 		= { block_x*5 + 665	, 40 + block_y*4 		, 40		 , 40		 };//student stunning icon position
SDL_Rect professor_burn_rect 	= { block_x*8 + 332 , block_y*0	 			, 40		 , 40		 }; //professor burning icon position
SDL_Rect professor_stun_rect 	= { block_x*8 + 372 , block_y*0				, 40		 , 40	 	 }; //professor stunning icon position
SDL_Rect professor_pos_rect 	= { block_x*3  		, block_y*0 + 40		, block_x*10 , block_y*4 };//professor on stage position
SDL_Rect paper_table_rect		= {	0				, 0						, 270		 , 90		 };
SDL_Rect paper_1_rect			= {	35				, 15					, 60		 , 60		 };
SDL_Rect paper_2_rect			= {	105				, 15					, 60		 , 60		 };
SDL_Rect paper_3_rect			= {	175				, 15					, 60		 , 60		 };

SDL_Color continue_button_color = {0xFF,0xFF,0xFF};

LTexture start_texture ;  				//texture of start scene
LTexture explanation_texture ;			//texture of explanation scene
LTexture burning_texture ;				//texture of burning small icon
LTexture stunning_texture ; 			//texture of stunning small icon
LTexture claw_texture ;					//texture of claw(attack of professor)
LTexture get_f_texture ;				//texture of getting f end
LTexture professor_texture[6] ;			//texture of professor
LTexture stage_background_texture[6];	//texture of battle backgrounds
LTexture healthbar_texture ;			//texture of healthbar
LTexture paper_status_table_texture ;
LTexture paper_texture[3] ;
LTexture continue_button ;
LTexture chinese_test_texture;

student_class student;
professor_class professor[6];

healthbar_class student_healthbar( block_x * 5, 45 + block_y * 4, student );
healthbar_class professor_healthbar[6] ;

Uint16 chinese_test[] = {0x65e9,0x5b89,0x4f60,0x597d};	

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
	gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
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
	if( TTF_Init() == -1 )
	{
	    printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
	    success = false;
	}
	else{
		gFont = TTF_OpenFont( "img/lazy.ttf", 28 );
	    if( gFont == NULL )
		{
			printf( "Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError() );
			success = false;
		}
		conti_font = TTF_OpenFont( "img/Golden_Age_Shad.ttf", 48 );
	    if( conti_font == NULL )
		{
			printf( "Failed to load continue font! SDL_ttf Error: %s\n", TTF_GetError() );
			success = false;
		}
		chinesefont = TTF_OpenFont( "img/Pixel.ttf", 48 );
		if( chinesefont == NULL )
		{
			printf( "Failed to load chinese font! SDL_ttf Error: %s\n", TTF_GetError() );
			success = false;
		}
	}
	

	return success;
}

bool loadMedia()
{
	//Loading success flag
	bool success = true;
	
	if( !start_texture.loadFromFile( "img/background.bmp" ) ){
		printf( "Failed to load start texture!\n" );			success = false;	}
	if( !explanation_texture.loadFromFile( "img/explanation.bmp" ) ){
		printf( "Failed to load explanation texture!\n" );		success = false;	}
	if( !burning_texture.loadFromFile( "img/fire.bmp" ) ){
		printf( "Failed to load burning texture!\n" );			success = false;	}
	if( !stunning_texture.loadFromFile( "img/stun.bmp" ) ){
		printf( "Failed to load stunning texture!\n" );			success = false;	}
	if( !claw_texture.loadFromFile( "img/claw.bmp" ) ){
		printf( "Failed to load claw texture!\n" );				success = false;	}
	if( !get_f_texture.loadFromFile( "img/get_f.bmp" ) ){
		printf( "Failed to load get_f texture!\n" );			success = false;	}
	if( !professor_texture[1].loadFromFile( "img/monster_1.bmp" ) ){
		printf( "Failed to load monster 1 texture!\n" );		success = false;	}
	if( !professor_texture[2].loadFromFile( "img/monster_2.bmp" ) ){
		printf( "Failed to load monster 2 texture!\n" );		success = false;	}
	if( !professor_texture[3].loadFromFile( "img/monster_3.bmp" ) ){
		printf( "Failed to load monster 3 texture!\n" );		success = false;	}
	if( !professor_texture[4].loadFromFile( "img/monster_4.bmp" ) ){
		printf( "Failed to load monster 4 texture!\n" );		success = false;	}
	if( !professor_texture[5].loadFromFile( "img/monster_5.bmp" ) ){
		printf( "Failed to load monster 5 texture!\n" );		success = false;	}
	if( !stage_background_texture[1].loadFromFile( "img/stage_background_1.bmp" ) ){
		printf( "Failed to load stage 1 bg texture!\n" );		success = false;	}
	if( !stage_background_texture[2].loadFromFile( "img/stage_background_2.bmp" ) ){
		printf( "Failed to load stage 2 bg texture!\n" );		success = false;	}
	if( !stage_background_texture[3].loadFromFile( "img/stage_background_3.bmp" ) ){
		printf( "Failed to load stage 3 bg texture!\n" );		success = false;	}
	if( !stage_background_texture[4].loadFromFile( "img/stage_background_4.bmp" ) ){
		printf( "Failed to load stage 4 bg texture!\n" );		success = false;	}
	if( !stage_background_texture[5].loadFromFile( "img/stage_background_5.bmp" ) ){
		printf( "Failed to load stage 5 bg texture!\n" );		success = false;	}
	if( !healthbar_texture.loadFromFile( "img/healthbar.bmp" ) ){
		printf( "Failed to load healthbar texture!\n" );		success = false;	}
	if( !paper_status_table_texture.loadFromFile( "img/testpaper_background.bmp" ) ){
		printf( "Failed to load paper table texture!\n" );		success = false;	}
	if( !paper_texture[0].loadFromFile( "img/testpaper_1.bmp" ) ){
		printf( "Failed to load paper0 texture!\n" );		success = false;	}
	if( !paper_texture[1].loadFromFile( "img/testpaper_1.bmp" ) ){
		printf( "Failed to load paper1 texture!\n" );		success = false;	}
	if( !paper_texture[2].loadFromFile( "img/testpaper_1.bmp" ) ){
		printf( "Failed to load paper2 texture!\n" );		success = false;	}
	if( !continue_button.loadFromRenderedText_goldenage( "--- Press Space To Continue ---" ,continue_button_color ) ){
		printf( "Failed to load continue button texture!\n" );		success = false;	}
	if( !chinese_test_texture.loadFromRenderedText_chinese( chinese_test ,continue_button_color ) ){
		printf( "Failed to load good_morning_chinese texture!\n" );		success = false;	}
    
	return success;
}

void close()
{
	//Free loaded image
	start_texture.free();		
	explanation_texture.free();	
	burning_texture.free();		
	stunning_texture.free();		
	claw_texture.free();			
	get_f_texture.free();		
	for(int i=0;i<6;i++){
		stage_background_texture[i].free();
		professor_texture[i].free();
	}
	paper_status_table_texture.free();
	for(int i=0;i<3;i++)	paper_texture[i].free();
	
	TTF_CloseFont( gFont );
	gFont = NULL;
	TTF_CloseFont( conti_font );
	conti_font = NULL;
	TTF_CloseFont( chinesefont );
	chinesefont = NULL;
	
	//Destroy window	
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;
	
	
	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
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
			
			for(int i=0;i<5;i++)	{ professor[i] = professor_class(i); }
			professor[5] = professor_class(0);
			
			for(int i=0;i<5;i++)	{ professor_healthbar[i] = healthbar_class( block_x * 8 - 332+38 , 5 , professor[i] ); }
			
			
			//While application is running
			while( !quit )
			{
				background_texture_render();	//render image in this function
				SDL_RenderPresent( gRenderer );//update screen
				//Handle events on queue
				while( SDL_PollEvent( &e ) != 0 )
				{
					//User requests quit
					if( e.type == SDL_QUIT )
					{
						quit = true;
					}
					else if(state == start){
						if(e.type == SDL_KEYDOWN){
							switch( e.key.keysym.sym ){
                	    	    case SDLK_SPACE:
                	        	    state = explanation;
                	        		break;
                   	     	}
						}
					}
					else if (state == explanation){
						if(e.type == SDL_KEYDOWN){
							switch( e.key.keysym.sym ){
                   	 	    case SDLK_SPACE:
                   	     	    state = enter_stage;
                   	     	    break;
                   	    	}
						}
					}
					else if (state == enter_stage || state == student_attacking || state == professor_attacking){
						
						if ( state == enter_stage ){
							//tell player what ability that enemy posesses
							
							
						}
						else if( state == student_attacking ){
							//if mouse is on card: show detail
							
							//professor get hurt 
							//professor died or not
							
							//deal with card effect
							
							
						}
						else if( state == professor_attacking ){
							
							if(!professor[stage].stunning){
								student.hurt( probability( professor[ stage ].hit_rate, student.avoid_rate ) * professor[ stage ].attack );
								professor[stage].do_effect(student);
							}
							student_healthbar.update(student);
							if( student.burning == true){ student.hurt(3); }
							student_healthbar.update(student);
							if( student.alive() == false ){	state = no_school; }
						}
					
						
					}
					else if(state == gatcha){
						//
					}
					else if(state == no_school){
						//wasted animation
					}
					else if(state == get_f){
						//wasted animation
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

int probability(double hit_rate, double avoid_rate){		//function of hitting of not
	srand(time(NULL));
	double rate = pow(hit_rate * (1-avoid_rate),0.5);
	rate *= 100;
	if( (rand()%100+1) < rate )	return 1 ;
	else						return 0 ;
}

void papertable_render(){
	paper_status_table_texture.render(0,0,&paper_table_rect);
	if(getpaper[0])	paper_texture[0].render(35,15,&paper_1_rect);
	if(getpaper[1])	paper_texture[1].render(105,15,&paper_2_rect);
	if(getpaper[2])	paper_texture[2].render(175,15,&paper_3_rect);
}

void battlescene_render(){
	stage_background_texture[stage].render(0,0);//Render texture to screen
	if (student.burning){ burning_texture.render( student_burn_rect.x , student_burn_rect.y, &student_burn_rect ); }
	if (student.stunning){ stunning_texture.render( student_stun_rect.x , student_stun_rect.y, &student_stun_rect );	}
	if (professor[stage].burning){ burning_texture.render( professor_burn_rect.x , professor_burn_rect.y, &professor_burn_rect) ; }
	if (professor[stage].stunning){ stunning_texture.render( professor_stun_rect.x , professor_stun_rect.y, &professor_stun_rect ); }
	
	professor_texture[stage].render( professor_pos_rect.x , professor_pos_rect.y , &professor_pos_rect );
	
	papertable_render();
	professor_healthbar[stage].render();	//render healthbar
	student_healthbar.render();				//render healthbar
}

void continue_button_render(){
	static double continue_button_iterator = 0;
	double intransparancy = 128 + 127 * sin(continue_button_iterator);
	continue_button.setBlendMode(SDL_BLENDMODE_BLEND);
	continue_button.setAlpha( intransparancy );
	SDL_Rect pos = {SCREEN_WIDTH/2 - continue_button.getWidth() / 2 , 710,continue_button.getWidth(),continue_button.getHeight()};
	continue_button.render(pos.x, pos.y,&pos);
	continue_button_iterator += 0.02;
}

void background_texture_render(){
	if( state == start ){
		start_texture.render(0,0);//Render texture to screen
		continue_button_render();
		
	}
	else if( state == explanation ){
		explanation_texture.render(0,0);//Render texture to screen
		
		//here to render the explanation words 
		//or put the words in the explanation_texture
		continue_button_render();
		
	}
	else if (state == enter_stage || state == student_attacking || state == professor_attacking){
		battlescene_render();
		
	}
	else if(state == gatcha){
		//
	}
	else if(state == no_school){
		//wasted animation
	}
	else if(state == get_f){
		get_f_texture.render(0,0);
	}
	else if(state == get_aplus){
		
	}
}
