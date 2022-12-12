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
#define PI 3.14159265358979323846
using namespace std;
const int SCREEN_WIDTH = 1440;  //Screen dimension constants
const int SCREEN_HEIGHT = 810;
int block_x = SCREEN_WIDTH/16;
int block_y = SCREEN_HEIGHT/9;
bool gatcha_played_animation[6] = {false};
bool noschool_played = false;
bool get_aplus_played = false;
bool get_f_played = false;
bool drawn_paper[6] = {false};
int the_paper;//the paper drawn in every stage

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
bool paper[3] = {};
int paper_num = 0;
int yes;
int no;
	
bool init();				  //Starts up SDL and creates window

bool loadMedia();     		  //Loads media

void close();				  //Frees media and shuts down SDL

SDL_Window* gWindow = NULL;	  //The window we'll be rendering to

SDL_Renderer* gRenderer = NULL;//The window renderer

SDL_Surface* gScreenSurface = NULL;

TTF_Font *conti_font = NULL;

TTF_Font *gFont = NULL;

TTF_Font *chinesefont = NULL;

cards all_card[21], **battle_deck;


int probability(double hit_rate, double avoid_rate);

void papertable_render();

void battlescene_render();

void continue_button_render();

void quitgame_button_render();

void background_texture_render();

void gatcha_animation(int);	//input the number of drawn testpaper(0, 1, 2)

void get_f_script();

void get_aplus_script();

void prof_attack_animation();

void noschool_script();

void game_init();

int draw_paper();

SDL_Rect student_burn_rect 		= { block_x*5 + 625	, 40 + block_y*4	 	, 40		 , 40		 }; //student burning icon position
SDL_Rect student_stun_rect 		= { block_x*5 + 665	, 40 + block_y*4 		, 40		 , 40		 };//student stunning icon position
SDL_Rect professor_burn_rect 	= { block_x*8 + 332 , block_y*0	 			, 40		 , 40		 }; //professor burning icon position
SDL_Rect professor_stun_rect 	= { block_x*8 + 372 , block_y*0				, 40		 , 40	 	 }; //professor stunning icon position
SDL_Rect professor_pos_rect 	= { block_x*3  		, block_y*0 + 40		, block_x*10 , block_y*4 };//professor on stage position
SDL_Rect paper_table_rect		= {	0				, 0						, 270		 , 90		 };
SDL_Rect paper_1_rect			= {	35				, 15					, 60		 , 60		 };
SDL_Rect paper_2_rect			= {	105				, 15					, 60		 , 60		 };
SDL_Rect paper_3_rect			= {	175				, 15					, 60		 , 60		 };
SDL_Rect deck_rect[6] = {{280, 450, 280, 140}, {580, 450, 280, 140}, {880, 450, 280, 140}, {280, 600, 280, 140}, {580, 600, 280, 140}, {880, 600, 280, 140}};


SDL_Color continue_button_color = {0xFF,0xFF,0xFF};
SDL_Color get_f_text_color = {0xFF,0xFF,0xFF};
SDL_Color quitgame_button_color = {0x00,0x00,0x00};
SDL_Color godsound_color = {255,239,69};

LTexture start_texture ;  				//texture of start scene
LTexture explanation_texture ;			//texture of explanation scene
LTexture burning_texture ;				//texture of burning small icon
LTexture stunning_texture ; 			//texture of stunning small icon
LTexture claw_texture ;					//texture of claw(attack of professor)
LTexture get_f_texture ;				//texture of getting f end
LTexture professor_texture[6] ;			//texture of professor
LTexture stage_background_texture[6];	//texture of battle backgrounds
LTexture healthbar_texture ;			//texture of healthbar
LTexture card_texture[21];              //texture of the cards
LTexture paper_status_table_texture ;
LTexture paper_texture[3] ;
LTexture continue_button ;
LTexture quitgame_button ;
LTexture chinese_test_texture;
LTexture get_f_subtitle[5];
LTexture testpaper_postman_walk;
LTexture testpaper_postman_jump;
LTexture block_nothit;
LTexture block_hit;
LTexture get_aplus_texture;
LTexture get_aplus_subtitle[2];
LTexture magicball;
LTexture noschool_subtitle[3];
LTexture wasted; 

student_class student;
professor_class professor[6];

healthbar_class student_healthbar( block_x * 5, 45 + block_y * 4, student );
healthbar_class professor_healthbar[6] ;

Uint16 chinese_test[] = {0x65e9,0x5b89,0x4f60,0x597d};	
Uint16 get_f_text_0[] = {0x3002,0x3002,0x3002};
Uint16 get_f_text_1[] = {0x6211,0x76e1,0x529b,0x4e86,0xff0c,0x6211,0x76e1,0x5168,0x529b,0x4e86,0x3002};
Uint16 get_f_text_2[] = {0x5982,0x679c,0x628a,0x6642,0x9593,0x62ff,0x53bb,0x8907,0x7fd2,0x7684,0x8a71,0xff0c,0x8aaa,0x4e0d,0x5b9a,0x9084,0x6709,0x6a5f,0x6703,0x901a,0x904e,0x8003,0x8a66,0x3002};
Uint16 get_f_text_3[] = {0x65e9,0x77e5,0x9053,0x5c31,0x4e0d,0x8a72,0x6d6a,0x8cbb,0x6642,0x9593,0x5728,0x9019,0x5b87,0x5b99,0x7cde,0x904a,0x4e0a,0x4e86,0x3002};
Uint16 get_f_text_4[] = {0x4f86,0x751f,0x9084,0x60f3,0x62ff,0x66f8,0x5377,0x734e,0x554a,0x3002};
Uint16 get_aplus_text_0[] = {0x7d93,0x6b77,0x4e86,0x5343,0x8f9b,0x842c,0x82e6,0xff0c,0x6211,0x7d42,0x65bc,0x7372,0x5f97,0x4e86,0x81f3,0x9ad8,0x5c0a,0x5bf6,0x300e,0x66f8,0x5377,0x734e,0x300f,0x3002,0x6211,0x8214,0x3002,0x0000,0x0000};
Uint16 get_aplus_text_1[] = {0x6211,0x8214};
Uint16 noschool_text_0[] = {0x53ef,0x60e1,0xff0c,0x4f86,0x751f,0x9084,0x60f3,0x8b80,0x53f0,0x5927,0x554a,0xff01};
Uint16 noschool_text_1[] = {0x8b0e,0x4e4b,0x8072,0xff1a,0x52c7,0x8005,0xff0c,0x6b64,0x5730,0x975e,0x6c5d,0x4e4b,0x7d42,0x7109,0xff0c,0x7576,0x596e,0x8d77,0x518d,0x6230,0x3002};
Uint16 noschool_text_2[] = {0x554a,0xff1f,0x54a6,0xff1f,0x86e4,0xff1f,0x55ef,0xff1f,0x0000,0x0000};

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
	if( !get_aplus_texture.loadFromFile( "img/get_aplus.bmp" ) ){
		printf( "Failed to load get_aplus texture!\n" );		success = false;	}
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
	if( !card_texture[0].loadFromFile("img/card_06.bmp")){
		printf("Failed to load card_00 texture!\n");	success = false;}
	if( !card_texture[1].loadFromFile("img/card_06.bmp")){
		printf("Failed to load card_01 texture!\n");	success = false;}
	if( !card_texture[2].loadFromFile("img/card_06.bmp")){
		printf("Failed to load card_02 texture!\n");	success = false;}
	if( !card_texture[3].loadFromFile("img/card_06.bmp")){
		printf("Failed to load card_03 texture!\n");	success = false;}
	if( !card_texture[4].loadFromFile("img/card_06.bmp")){
		printf("Failed to load card_04 texture!\n");	success = false;}
	if( !card_texture[5].loadFromFile("img/card_06.bmp")){
		printf("Failed to load card_05 texture!\n");	success = false;}
	if( !card_texture[6].loadFromFile("img/card_06.bmp")){
		printf("Failed to load card_06 texture!\n");	success = false;}
	if( !card_texture[7].loadFromFile("img/card_12.bmp")){
		printf("Failed to load card_07 texture!\n");	success = false;}
	if( !card_texture[8].loadFromFile("img/card_12.bmp")){
		printf("Failed to load card_08 texture!\n");	success = false;}
	if( !card_texture[9].loadFromFile("img/card_12.bmp")){
		printf("Failed to load card_09 texture!\n");	success = false;}
	if( !card_texture[10].loadFromFile("img/card_12.bmp")){
		printf("Failed to load card_10 texture!\n");	success = false;}
	if( !card_texture[11].loadFromFile("img/card_12.bmp")){
		printf("Failed to load card_11 texture!\n");	success = false;}
	if( !card_texture[12].loadFromFile("img/card_12.bmp")){
		printf("Failed to load card_12 texture!\n");	success = false;}
	if( !card_texture[13].loadFromFile("img/card_12.bmp")){
		printf("Failed to load card_13 texture!\n");	success = false;}
	if( !card_texture[14].loadFromFile("img/card_17.bmp")){
		printf("Failed to load card_14 texture!\n");	success = false;}
	if( !card_texture[15].loadFromFile("img/card_17.bmp")){
		printf("Failed to load card_15 texture!\n");	success = false;}
	if( !card_texture[16].loadFromFile("img/card_17.bmp")){
		printf("Failed to load card_16 texture!\n");	success = false;}
	if( !card_texture[17].loadFromFile("img/card_17.bmp")){
		printf("Failed to load card_17 texture!\n");	success = false;}
	if( !card_texture[18].loadFromFile("img/card_17.bmp")){
		printf("Failed to load card_18 texture!\n");	success = false;}
	if( !card_texture[19].loadFromFile("img/card_17.bmp")){
		printf("Failed to load card_19 texture!\n");	success = false;}
	if( !card_texture[20].loadFromFile("img/card_17.bmp")){
		printf("Failed to load card_20 texture!\n");	success = false;}
	if( !healthbar_texture.loadFromFile( "img/healthbar.bmp" ) ){
		printf( "Failed to load healthbar texture!\n" );		success = false;	}
	if( !paper_status_table_texture.loadFromFile( "img/testpaper_background.bmp" ) ){
		printf( "Failed to load paper table texture!\n" );		success = false;	}
	if( !paper_texture[0].loadFromFile( "img/testpaper_1.bmp" ) ){
		printf( "Failed to load paper0 texture!\n" );			success = false;	}
	if( !paper_texture[1].loadFromFile( "img/testpaper_2.bmp" ) ){
		printf( "Failed to load paper1 texture!\n" );			success = false;	}
	if( !paper_texture[2].loadFromFile( "img/testpaper_3.bmp" ) ){
		printf( "Failed to load paper2 texture!\n" );			success = false;	}
	if( !continue_button.loadFromRenderedText_goldenage( "--- Press Space To Continue ---" ,continue_button_color ) ){
		printf( "Failed to load continue button texture!\n" );		success = false;	}
	if( !quitgame_button.loadFromRenderedText_goldenage( "--- Press Space To Quit Playing ---" ,quitgame_button_color ) ){
		printf( "Failed to load quitgame button texture!\n" );		success = false;	}
	if( !chinese_test_texture.loadFromRenderedText_chinese( chinese_test ,continue_button_color ) ){
		printf( "Failed to load good_morning_chinese texture!\n" );		success = false;	}
    if( !testpaper_postman_walk.loadFromFile( "img/testpaper_postman_walk.bmp"  ) ){
		printf( "Failed to load testpaper_postman_walk texture!\n" );	success = false;	}
	if( !testpaper_postman_jump.loadFromFile( "img/testpaper_postman_jump.bmp"  ) ){
		printf( "Failed to load testpaper_postman_jump texture!\n" );	success = false;	}
	if( !block_nothit.loadFromFile( "img/block_nothit.bmp"  ) ){
		printf( "Failed to load block_nothit texture!\n" );		success = false;	}
	if( !block_hit.loadFromFile( "img/block_hit.bmp"  ) ){
		printf( "Failed to load block_hit texture!\n" );		success = false;	}
	if( !get_f_subtitle[0].loadFromRenderedText_chinese( get_f_text_0 ,get_f_text_color ) ){
		printf( "Failed to load get_f_text_0 texture!\n" );		success = false;	}
	if( !get_f_subtitle[1].loadFromRenderedText_chinese( get_f_text_1 ,get_f_text_color ) ){
		printf( "Failed to load get_f_text_1 texture!\n" );		success = false;	}
	if( !get_f_subtitle[2].loadFromRenderedText_chinese( get_f_text_2 ,get_f_text_color ) ){
		printf( "Failed to load get_f_text_2 texture!\n" );		success = false;	}
	if( !get_f_subtitle[3].loadFromRenderedText_chinese( get_f_text_3 ,get_f_text_color ) ){
		printf( "Failed to load get_f_text_3 texture!\n" );		success = false;	}
	if( !get_f_subtitle[4].loadFromRenderedText_chinese( get_f_text_4 ,get_f_text_color ) ){
		printf( "Failed to load get_f_text_4 texture!\n" );		success = false;	}
	if( !get_aplus_subtitle[0].loadFromRenderedText_chinese( get_aplus_text_0 ,get_f_text_color ) ){
		printf( "Failed to load get_aplus_text_0 texture!\n" );	success = false;	}
	if( !get_aplus_subtitle[1].loadFromRenderedText_chinese( get_aplus_text_1 ,get_f_text_color ) ){
		printf( "Failed to load get_aplus_text_1 texture!\n" );	success = false;	}
	if( !magicball.loadFromFile( "img/magicball.bmp"  ) ){
		printf( "Failed to load magicball texture!\n" );		success = false;	}	
	if( !noschool_subtitle[0].loadFromRenderedText_chinese( noschool_text_0 ,get_f_text_color ) ){
		printf( "Failed to load noschool text0 texture!\n" );	success = false;	}
	if( !noschool_subtitle[1].loadFromRenderedText_chinese( noschool_text_1 ,godsound_color ) ){
		printf( "Failed to load noschool text1 texture!\n" );	success = false;	}
	if( !noschool_subtitle[2].loadFromRenderedText_chinese( noschool_text_2 ,get_f_text_color ) ){
		printf( "Failed to load noschool text2 texture!\n" );	success = false;	}
	if( !wasted.loadFromFile( "img/wasted.bmp"  ) ){
		printf( "Failed to load wasted texture!\n" );		success = false;	}	
	
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
	continue_button.free();
	quitgame_button.free();
	testpaper_postman_walk.free();
	testpaper_postman_jump.free();
	block_nothit.free();
	block_hit.free();
	for(int i = 0 ; i < 21 ; i++)	card_texture[i].free();
	for(int i=0;i<5;i++)	get_f_subtitle[i].free();
	for(int i=0;i<2;i++)	get_aplus_subtitle[i].free();
	get_aplus_texture.free();
	magicball.free();
	for(int i=0;i<3;i++)	noschool_subtitle[i].free();
	wasted.free();
	
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
	cards_initialize(all_card);
	battle_deck = deck_initialize(all_card);

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
							student.burning = false;			student.stunning = false;
							professor[stage].burning = false;	professor[stage].stunning = false;
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
								prof_attack_animation();
								student.hurt( probability( professor[ stage ].hit_rate, student.avoid_rate ) * professor[ stage ].attack );
								professor[stage].do_effect(student);
							}
							SDL_Delay(300);
							student_healthbar.update(student);
							SDL_Delay(300);
							if( student.burning == true){ student.hurt(3); }{
								student_healthbar.update(student);
								SDL_Delay(300);
							}
							if( student.alive() == false ){	
								state = no_school; 
							}
							else{
								state = student_attacking;
							}
						}
					
						
					}
					else if(state == gatcha){
						
						if(e.type == SDL_KEYDOWN){
							switch( e.key.keysym.sym ){
                   	 	    case SDLK_SPACE:
                   	     	    if(stage < 5){
                                    
		                   	     	stage += 1;
									state = enter_stage;
								}
								else{
									if( paper[0] && paper[1] && paper[2] ){
										state = get_aplus;
									}
									else{
										state = get_f;
									}
								}
                   	     	    break;
                   	    	}
						} 	    
                   	}
					else if(state == no_school){
						//wasted animation
					}
					else if(state == get_f){
						switch( e.key.keysym.sym ){
                   	 	    case SDLK_SPACE:
                   	     	    quit = true;
                   	     	    break;
                   	    }
					}
					else if(state == get_aplus){
						switch( e.key.keysym.sym ){
                   	 	    case SDLK_SPACE:
                   	     	    quit = true;
                   	     	    break;
                   	    }
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
	if(paper[0])	paper_texture[0].render(35,15,&paper_1_rect);
	if(paper[1])	paper_texture[1].render(105,15,&paper_2_rect);
	if(paper[2])	paper_texture[2].render(175,15,&paper_3_rect);
}

void card_graph_render()
{
	int i, j;
	for(i = 0 ; i < 2 ; i++)
	{
		for(j = 0 ; j < 3 ; j++)
		{
			card_texture[battle_deck[i][j].id].render(deck_rect[3 * i + j].x, deck_rect[3 * i + j].y, &deck_rect[3 * i + j]);
		}
	}
}

void battlescene_render(){
	stage_background_texture[stage].render(0,0);//Render texture to screen
	if (student.burning){ burning_texture.render( student_burn_rect.x , student_burn_rect.y, &student_burn_rect ); }
	if (student.stunning){ stunning_texture.render( student_stun_rect.x , student_stun_rect.y, &student_stun_rect );	}
	if (professor[stage].burning){ burning_texture.render( professor_burn_rect.x , professor_burn_rect.y, &professor_burn_rect) ; }
	if (professor[stage].stunning){ stunning_texture.render( professor_stun_rect.x , professor_stun_rect.y, &professor_stun_rect ); }
	
	professor_texture[stage].render( professor_pos_rect.x , professor_pos_rect.y , &professor_pos_rect );

	card_graph_render();

	
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

void quitgame_button_render(){
	static double quitgame_button_iterator = 0;
	double intransparancy = 128 + 127 * sin(quitgame_button_iterator);
	quitgame_button.setBlendMode(SDL_BLENDMODE_BLEND);
	quitgame_button.setAlpha( intransparancy );
	SDL_Rect pos = {SCREEN_WIDTH/2 - quitgame_button.getWidth() / 2 , 710,quitgame_button.getWidth(),quitgame_button.getHeight()};
	quitgame_button.render(pos.x, pos.y,&pos);
	quitgame_button_iterator += 0.02;
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
		
		if(!gatcha_played_animation[stage]){
			the_paper = draw_paper();
			gatcha_animation(the_paper);
			gatcha_played_animation[stage] = true;
		}
		SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0xFF );
		SDL_RenderClear( gRenderer );
		SDL_Rect paperRect = { 420, 105, 600, 600};
		paper_texture[the_paper].render(paperRect.x,paperRect.y,&paperRect);
		continue_button_render();
		SDL_RenderPresent( gRenderer );
	}
	else if(state == no_school){
		noschool_script();
		//wasted animation
	}
	else if(state == get_f){
		get_f_script();
		get_f_texture.render(0,0);
		quitgame_button_render();
	}
	else if(state == get_aplus){
		get_aplus_script();
		get_aplus_texture.render(0,0);
		quitgame_button_render();
	}
}

void gatcha_animation(int num){
	
	SDL_Rect postmanR = {-100, 700, 100, 100};
	SDL_Rect blockR = { 670, 410, 100, 100};
	SDL_Rect paperR = { 670, 410, 100, 100};
	SDL_Rect paperRf = { 720, 405, 0, 0};
	bool jumped = false;
	bool finished = false;
	while(postmanR.x < 1440){
		SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0xFF );
        SDL_RenderClear( gRenderer );
        if(jumped){
			if(paperR.y > 310){
				paper_texture[num].render(paperR.x,paperR.y,&paperR);
				paperR.y -= 5;
			}
			else{
				paper_texture[num].render(paperR.x,paperR.y,&paperR);
			}
			
		}
		
		if(!jumped){
        	block_nothit.render(blockR.x,blockR.y,&blockR);
		}
        else{
        	block_hit.render(blockR.x,blockR.y,&blockR);
		}
		
		if(postmanR.x < 670){
			testpaper_postman_walk.render(postmanR.x,postmanR.y,&postmanR);
			postmanR.x += 5;
		}
		else if(postmanR.x == 670){
			if(!jumped){
				testpaper_postman_jump.render(postmanR.x,postmanR.y ,&postmanR);
				if(postmanR.y > 510){
					postmanR.y -= 10;
				}
				else{
					jumped = true;
				}
			}
			else{
				testpaper_postman_jump.render(postmanR.x,postmanR.y ,&postmanR);
				if(postmanR.y < 710){
					postmanR.y += 10;
				}
				else {
					postmanR.x += 5;
				}
			}
		}
		else if(postmanR.x > 670 && postmanR.x <= 1430){
			testpaper_postman_walk.render(postmanR.x,postmanR.y,&postmanR);
			postmanR.x += 5;
		}
		else if(postmanR.x > 1430){
			finished = true;
		}
		
		if(finished){
			
			if(paperRf.w < 600){
				paperRf.x -= 3;
				paperRf.y -= 3;
				paperRf.w += 6;
				paperRf.h += 6;
			}
			else return;
			paper_texture[num].render(paperRf.x,paperRf.y,&paperRf);
		}
		SDL_Delay(10);
		SDL_RenderPresent( gRenderer );
	}
}

void get_f_script(){
	
	if(!get_f_played){
		SDL_Rect r[5] ;
		for(int i=0;i<5;i++){
			r[i] = {50,630,get_f_subtitle[i].getWidth(),get_f_subtitle[i].getHeight()};
		}
		for(int i=0;i<5;i++){
			SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0xFF );
        	SDL_RenderClear( gRenderer );
			get_f_subtitle[i].render(r[i].x,r[i].y,&r[i]);
			SDL_RenderPresent( gRenderer );
			SDL_Delay(5000); 
		}
		get_f_played = true;
	}
	
}

void get_aplus_script(){
	
	if(!get_aplus_played){
		SDL_Rect r[2] ;
		r[0] = {50,630,get_aplus_subtitle[0].getWidth(),get_aplus_subtitle[0].getHeight()};
		r[1] = {0,0,get_aplus_subtitle[1].getWidth(),get_aplus_subtitle[1].getHeight()};
		
		SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0xFF );
    	SDL_RenderClear( gRenderer );
		get_aplus_subtitle[0].render(r[0].x,r[0].y,&r[0]);
		SDL_RenderPresent( gRenderer );
		SDL_Delay(5000);
		
		SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0xFF );
    	SDL_RenderClear( gRenderer );
		for(int i=0;i<=SCREEN_WIDTH/get_aplus_subtitle[1].getWidth();i++){
			for(int j=0;j<=SCREEN_HEIGHT/get_aplus_subtitle[1].getHeight();j++){
				r[1].x = i * get_aplus_subtitle[1].getWidth();
				r[1].y = j * get_aplus_subtitle[1].getHeight();
				get_aplus_subtitle[1].render(r[1].x,r[1].y,&r[1]);
				
			}
		}
		SDL_RenderPresent( gRenderer );
		SDL_Delay(5000); 
		
		get_aplus_played = true;
	}
}

void prof_attack_animation(){
	
	double iter = PI/2;
	SDL_Rect ballR = {700,160,40,40};//y=160~y=400
	for(int i=0;i<60;i++){
		background_texture_render();
		ballR.x = 700 + 60*cos(iter);
		ballR.y = 280 - 120*sin(iter);
		magicball.render(ballR.x,ballR.y,&ballR);
		student_healthbar.render();
		SDL_RenderPresent( gRenderer );
		iter -= (PI)/60;
	}
}

void noschool_script(){
	if(!noschool_played){
		SDL_Rect r[3] ;
		SDL_Rect wr = {0,100,wasted.getWidth(),wasted.getHeight()};
		for(int i=0;i<3;i++){
			r[i] = {50,630,noschool_subtitle[i].getWidth(),noschool_subtitle[i].getHeight()};
		}
		for(int i=0;i<3;i++){
			SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0xFF );
	    	SDL_RenderClear( gRenderer );
	    	if(i <= 1){ wasted.render(wr.x,wr.y,&wr);}
			noschool_subtitle[i].render(r[i].x, r[i].y, &r[i]);
			SDL_RenderPresent( gRenderer );
			SDL_Delay(5000);
		}
		noschool_played = true;
		game_init();
		
	}
}

void game_init(){
	state = start;
	stage = 1;
	student.init();
	for(int i=0;i<5;i++)	{ professor[i] = professor_class(i); }
	professor[5] = professor_class(0);
	for(int i=0;i<3;i++) 	paper[i] = false;
	for(int i=0;i<6;i++)	{gatcha_played_animation[i] = false; drawn_paper[i] = false;}
	get_aplus_played = false;
	get_f_played = false;
	noschool_played = false;
	paper_num = 0;
}

int draw_paper(){
	srand(time(NULL));
	int the_drawn_paper = 0;
    int a = rand();
    if(paper_num == 0)
    {
        paper[a%3] = true;
        paper_num++;
        yes = a%3;
        the_drawn_paper = a%3;
    }
    else if(paper_num == 1)
    {
        if(a%6)
        {
            if((a%6)%2)
            {
                paper[(yes+1)%3] = true;
                paper_num++;
                no = (yes+2)%3;
                the_drawn_paper = (yes+1)%3;
            }
            else
            {
                paper[(yes+2)%3] = true;
                paper_num++;
                no = (yes+1)%3;
                the_drawn_paper = (yes+2)%3;
            }
        }
        else
        {
            paper[yes] = true; // got same paper
            the_drawn_paper = yes;
        }
    }
    else if(paper_num == 2)
    {
        if(a%3)
        {
            paper[no] = true;
            paper_num++;
            the_drawn_paper = no;
        }
        else
        {
            if(a%2)
            {
                paper[(no+1)%3] = true; // got same paper
                the_drawn_paper = (no+1)%3;
            }
            else
            {
                paper[(no+2)%3] = true; // got same paper
                the_drawn_paper = (no+1)%3;
            }
        }
    }
    else if(paper_num == 3)
    {
        paper[a%3] = true; // got same paper
        the_drawn_paper = a%3;
        // scene of getting the a%3 th paper
    }
    drawn_paper[stage] = true;
    return the_drawn_paper;
}

