//Using SDL, SDL_image, standard IO, and strings
#include <SDL2/SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <bits/stdc++.h>
#include "character.h"
#include "LTexture.h"
#include "healthbar.h"
#include "cards.h" 
#include "button.h" 
#define PI 3.14159265358979323846
using namespace std;
const int SCREEN_WIDTH = 1440;  //Screen dimension constants
const int SCREEN_HEIGHT = 810;
int block_x = SCREEN_WIDTH/16;
int block_y = SCREEN_HEIGHT/9;
int VOLUME = 36; //music volume
bool battling = true;
bool gatcha_played_animation[6] = {false};
bool noschool_played = false;
bool get_aplus_played = false;
bool get_f_played = false, round_attacked = false;
bool shown_explanation[6] = {false};
bool drawn_paper[6] = {false};
bool start_attacking = false;

int the_paper;//the paper drawn in every stage
Mix_Music *OPmusic = NULL;
Mix_Music *Battlemusic = NULL;
Mix_Chunk *Attackmusic1 = NULL;
Mix_Chunk *Attackmusic2 = NULL;
Mix_Chunk *Attackmusic3 = NULL;
Mix_Chunk *Attackmusic4 = NULL;
Mix_Chunk *Hitmusic1 = NULL;
Mix_Chunk *Hitmusic2 = NULL;
Mix_Chunk *Hitmusic3 = NULL;
Mix_Chunk *Hitmusic4 = NULL;
Mix_Chunk *Hitmusic5 = NULL;
Mix_Chunk *Jumpmusic = NULL;
Mix_Chunk *Getpapermusic = NULL;
Mix_Chunk *Diemusic = NULL;
Mix_Chunk *Clearmusic = NULL;
Mix_Chunk *Gururinpo = NULL;
Mix_Chunk *Stunmusic = NULL;

LButton gButtons[6];
const int TOTAL_BUTTONS = 5;

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
stringstream student_health_text;
stringstream damage_text;
	
bool init();				  //Starts up SDL and creates window

bool loadMedia();     		  //Loads media

void close();				  //Frees media and shuts down SDL

SDL_Window* gWindow = NULL;	  //The window we'll be rendering to

SDL_Renderer* gRenderer = NULL;//The window renderer

SDL_Surface* gScreenSurface = NULL;

TTF_Font *conti_font = NULL;

TTF_Font *gFont = NULL;

TTF_Font *chinesefont = NULL;

TTF_Font *namefont = NULL;

TTF_Font *damagefont = NULL;

cards all_card[21], **battle_deck;

int probability(double hit_rate, double avoid_rate);
void card_sprite_preset();
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
void card_graph_render();
void professor_name_render();
void stud_attack_animation( cards* );
cards** deck_initialize(cards all[]);
void card_draw(cards *deck[], cards all[]);
void stud_health_render();
void damage_render(int x, int y,int damage);
void miss_render(int x,int y);
void professor_function();
void render_damage_text(int damage, bool hit = true);
void stun_animation(bool);

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
SDL_Rect shieldRect = {block_x * 5-50, 45 + block_y * 4-15,60,60};
SDL_Rect diabgRect;

SDL_Color continue_button_color = {0xFF,0xFF,0xFF};
SDL_Color get_f_text_color = {0xFF,0xFF,0xFF};
SDL_Color quitgame_button_color = {0x00,0x00,0x00};
SDL_Color godsound_color = {255,239,69};
SDL_Color prof_name_color = {255,0,0};
SDL_Color colll = {180,50,200}; // color of damage text

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
LTexture shield_texture; 
LTexture professor_name[6];
LTexture magicball_center;
LTexture magicball_ring_1;
LTexture magicball_ring_2;
LTexture student_health_bg; 
LTexture student_health_text_texture;
LTexture damage_text_texture;
LTexture dialogue[11];
LTexture opening_introduction;
LTexture dialogue_background;

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
Uint16 professor_name_text_1[] = {0x7ba1,0x9662,0x7334,0x0000,0x0000};
Uint16 professor_name_text_2[] = {0x7ba1,0x9662,0x7334,0xff0c,0x4f46,0x662f,0x5f88,0x5feb,0x0000,0x0000};
Uint16 professor_name_text_3[] = {0x706b,0x7206,0x6559,0x6388,0x0000,0x0000};
Uint16 professor_name_text_4[] = {0x8001,0x5a46,0x0000,0x0000};
Uint16 professor_name_text_5[] = {0x6bc0,0x6ec5,0x4e4b,0x795e,0xff0e,0x68c4,0x5929,0x5e1d,0x0000,0x0000};

Uint16 dialogue_text_1[] = {0x300c,0x9019,0x662f,0x7ba1,0x9662,0x7334,0xff0c,0x7ba1,0x9662,0x7334,0x5011,0x5e73,0x5e38,0x6700,0x611b,0x5728,0x7e3d,0x5716,0x524d,0x559d,0x9152,0xff0c,0x4ed6,0x770b,0x8d77,0x4f86,0x9084,0x633a,0x80d6,0x7684,0xff0c,0x9632,0x79a6,0x80af,0x5b9a,0x5f88,0x9ad8,0xff0c,0x653b,0x64ca,0x529b,0x22ef,0x770b,0x8d77,0x4f86,0x4e0d,0x600e,0x9ebc,0x6a23,0xff0c,0x54c8,0x54c8,0xff01,0x52a0,0x6cb9,0x5427,0x3002,0x300d};
Uint16 dialogue_text_2[] = {0x300c,0x9019,0x4e5f,0x662f,0x7ba1,0x9662,0x7334,0xff0c,0x4ed6,0x53ef,0x662f,0x653b,0x7565,0x7d44,0x7d44,0x9577,0xff0c,0x64c5,0x9577,0x661f,0x7206,0x6c23,0x6d41,0x65ac,0xff0c,0x4e0d,0x64c5,0x9577,0x661f,0x5149,0x9023,0x7559,0x7d1a,0x3002,0x7528,0x4e00,0x53e5,0x8a71,0x5f62,0x5bb9,0x4ed6,0x7684,0x8a71,0x5c31,0x662f,0x300e,0x5f88,0x5feb,0xff0c,0x7e3d,0x4e4b,0x5f88,0x5feb,0x3002,0x300f,0x8981,0x6253,0x4e2d,0x4ed6,0x53ef,0x4e0d,0x5bb9,0x6613,0x3002,0x300d};
Uint16 dialogue_text_3[] = {0x300c,0x4ed6,0x662f,0x77f3,0x6559,0x6388,0xff0c,0x5c0d,0x6559,0x5b78,0x5f88,0x6709,0x71b1,0x60c5,0xff08,0x7269,0x7406,0xff09,0xff0c,0x5e0c,0x671b,0x6253,0x8d0f,0x4ed6,0x80fd,0x62ff,0x5230,0x8003,0x5377,0x800c,0x4e0d,0x662f,0x7070,0x71fc,0x22ef,0x300d};
Uint16 dialogue_text_4[] = {0x300c,0x5979,0x662f,0x7cfb,0x4e0a,0x6700,0x6b63,0x7684,0x6559,0x6388,0x8b1d,0x66f8,0x6021,0xff0c,0x807d,0x8aaa,0x53ea,0x6709,0x4e00,0x500b,0x53eb,0x70ba,0x5ef7,0x7684,0x5b78,0x9577,0x6253,0x8d0f,0x904e,0x5979,0x22ef,0x5c0f,0x5fc3,0x5225,0x592a,0x6688,0x5979,0xff01,0x9019,0x6b21,0x6211,0x53ef,0x4e0d,0x7ad9,0x5728,0x4f60,0x9019,0x908a,0x4e86,0xff0c,0x6559,0x6388,0x52a0,0x6cb9,0xff01,0x300d};
Uint16 dialogue_text_5[] = {0x68c4,0x5929,0x5e1d,0xff1a,0x300c,0x4eba,0x985e,0xff0c,0x4f60,0x5df2,0x7d93,0x76e1,0x529b,0x4e86,0x3002,0x300d};
Uint16 dialogue_text_6[] = {0x300c,0x4ed6,0x662f,0x68c4,0x5929,0x5e1d,0xff0c,0x60f3,0x8981,0x5f97,0x5230,0xff21,0xff0b,0x5f97,0x901a,0x904e,0x4ed6,0x9019,0x95dc,0xff0c,0x7576,0x7136,0x4ed6,0x53ef,0x4e0d,0x597d,0x5c0d,0x4ed8,0x3002,0x6211,0x807d,0x904e,0x88ab,0x4ed6,0x7576,0x6389,0x7684,0x5b78,0x9577,0x8aaa,0xff0c,0x7576,0x4ed6,0x5931,0x53bb,0x751f,0x547d,0x6642,0xff0c,0x653b,0x64ca,0x529b,0x5c31,0x6703,0x63d0,0x9ad8,0xff0c,0x591a,0x6ce8,0x610f,0x4e00,0x9ede,0xff0c,0x52a0,0x6cb9,0xff01,0x300d};
Uint16 opening_introduction_text[] = {0x56e0,0x70ba,0x5b78,0x5206,0x5f88,0x96e3,0x62ff,0xff0c,0x6240,0x4ee5,0x70ba,0x4e86,0x7576,0x500b,0x4e0d,0x8a8d,0x771f,0x7684,0x5b78,0x5206,0x5c0f,0x5077,0xff0c,0x6211,0x5011,0x53ea,0x597d,0x771f,0x7684,0x628a,0x81ea,0x5df1,0x5316,0x70ba,0x5c0f,0x5077,0x3002,0x6211,0x5011,0x73fe,0x5728,0x8eab,0x8655,0x528d,0x8207,0x9b54,0x6cd5,0x7684,0x53f0,0x5927,0xff0c,0x5316,0x8eab,0x70ba,0x300c,0x985e,0x6b66,0x85e4,0x904a,0x6232,0x300d,0x7684,0x4eba,0x7269,0xff0c,0x4f7f,0x7528,0x5361,0x7247,0xff0c,0x4e26,0x4e00,0x908a,0x5927,0x558a,0x300c,0x6296,0x56c9,0x300d,0x4e00,0x908a,0x6253,0x5012,0x6301,0x6709,0x671f,0x672b,0x8003,0x8003,0x5377,0x89e3,0x7b54,0x7684,0x90aa,0x60e1,0x7684,0x5316,0x8eab,0x5011,0x3002,0x6211,0x5011,0x53ea,0x8981,0x6536,0x96c6,0xff13,0x4efd,0x671f,0x672b,0x8003,0x89e3,0x7b54,0x5c31,0x53ef,0x4ee5,0x5728,0x6b63,0x5f0f,0x8003,0x8a66,0x5927,0x986f,0x795e,0x5a01,0xff0c,0x6241,0x721b,0x5176,0x4ed6,0x62c9,0x57fa,0x540c,0x5b78,0x5011,0xff0c,0x7576,0x4e0a,0x5377,0x54e5,0x3002,0x6240,0x4ee5,0xff0c,0x70ba,0x4e86,0x81ea,0x5df1,0x7684,0x5149,0x660e,0x672a,0x4f86,0x8457,0x60f3,0xff0c,0x73fe,0x5728,0x5c31,0x5149,0x660e,0x6b63,0x5927,0x5730,0x6536,0x96c6,0x7b54,0x6848,0x5427,0xff01};

bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO ) < 0 )
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
	if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )							//Initialize SDL_mixer
    {
        printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
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
		gFont = TTF_OpenFont( "img/Pixel.ttf", 28 );
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
		namefont = TTF_OpenFont( "img/Pixel.ttf", 30 );
		if( namefont == NULL )
		{
			printf( "Failed to load name font! SDL_ttf Error: %s\n", TTF_GetError() );
			success = false;
		}
		damagefont = TTF_OpenFont( "img/Golden_Age.ttf", 60 );
		if( namefont == NULL )
		{
			printf( "Failed to load damage font! SDL_ttf Error: %s\n", TTF_GetError() );
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
	if( !card_texture[0].loadFromFile("img/card_00.bmp")){
		printf("Failed to load card_00 texture!\n");	success = false;}
	else{card_sprite_preset();}
	if( !card_texture[1].loadFromFile("img/card_01.bmp")){
		printf("Failed to load card_01 texture!\n");	success = false;}
	else{card_sprite_preset();}
	if( !card_texture[2].loadFromFile("img/card_02.bmp")){
		printf("Failed to load card_02 texture!\n");	success = false;}
	else{card_sprite_preset();}
	if( !card_texture[3].loadFromFile("img/card_03.bmp")){
		printf("Failed to load card_03 texture!\n");	success = false;}
	else{card_sprite_preset();}
	if( !card_texture[4].loadFromFile("img/card_04.bmp")){
		printf("Failed to load card_04 texture!\n");	success = false;}
	else{card_sprite_preset();}
	if( !card_texture[5].loadFromFile("img/card_05.bmp")){
		printf("Failed to load card_05 texture!\n");	success = false;}
	else{card_sprite_preset();}
	if( !card_texture[6].loadFromFile("img/card_06.bmp")){
		printf("Failed to load card_06 texture!\n");	success = false;}
	else{card_sprite_preset();}
	if( !card_texture[7].loadFromFile("img/card_07.bmp")){
		printf("Failed to load card_07 texture!\n");	success = false;}
	else{card_sprite_preset();}
	if( !card_texture[8].loadFromFile("img/card_08.bmp")){
		printf("Failed to load card_08 texture!\n");	success = false;}
	else{card_sprite_preset();}
	if( !card_texture[9].loadFromFile("img/card_09.bmp")){
		printf("Failed to load card_09 texture!\n");	success = false;}
	else{card_sprite_preset();}
	if( !card_texture[10].loadFromFile("img/card_10.bmp")){
		printf("Failed to load card_10 texture!\n");	success = false;}
	else{card_sprite_preset();}
	if( !card_texture[11].loadFromFile("img/card_11.bmp")){
		printf("Failed to load card_11 texture!\n");	success = false;}
	else{card_sprite_preset();}
	if( !card_texture[12].loadFromFile("img/card_12.bmp")){
		printf("Failed to load card_12 texture!\n");	success = false;}
	else{card_sprite_preset();}
	if( !card_texture[13].loadFromFile("img/card_13.bmp")){
		printf("Failed to load card_13 texture!\n");	success = false;}
	else{card_sprite_preset();}
	if( !card_texture[14].loadFromFile("img/card_14.bmp")){
		printf("Failed to load card_14 texture!\n");	success = false;}
	else{card_sprite_preset();}
	if( !card_texture[15].loadFromFile("img/card_15.bmp")){
		printf("Failed to load card_15 texture!\n");	success = false;}
	else{card_sprite_preset();}
	if( !card_texture[16].loadFromFile("img/card_16.bmp")){
		printf("Failed to load card_16 texture!\n");	success = false;}
	else{card_sprite_preset();}
	if( !card_texture[17].loadFromFile("img/card_17.bmp")){
		printf("Failed to load card_17 texture!\n");	success = false;}
	else{card_sprite_preset();}
	if( !card_texture[18].loadFromFile("img/card_18.bmp")){
		printf("Failed to load card_18 texture!\n");	success = false;}
	else{card_sprite_preset();}
	if( !card_texture[19].loadFromFile("img/card_19.bmp")){
		printf("Failed to load card_19 texture!\n");	success = false;}
	else{card_sprite_preset();}
	if( !card_texture[20].loadFromFile("img/card_20.bmp")){
		printf("Failed to load card_20 texture!\n");	success = false;}
	else{card_sprite_preset();}
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
	if( !shield_texture.loadFromFile( "img/shield.bmp"  ) ){
		printf( "Failed to load shield texture!\n" );		success = false;	}		
	shield_texture.setBlendMode(SDL_BLENDMODE_BLEND);		shield_texture.setAlpha(230);
	if( !professor_name[1].loadFromRenderedText_name( professor_name_text_1 , prof_name_color ) ){
		printf( "Failed to load professor_name1!\n" );	success = false;	}
	if( !professor_name[2].loadFromRenderedText_name( professor_name_text_2 , prof_name_color ) ){
		printf( "Failed to load professor_name2 texture!\n" );	success = false;	}
	if( !professor_name[3].loadFromRenderedText_name( professor_name_text_3 , prof_name_color ) ){
		printf( "Failed to load professor_name3 texture!\n" );	success = false;	}
	if( !professor_name[4].loadFromRenderedText_name( professor_name_text_4 , prof_name_color ) ){
		printf( "Failed to load professor_name4 texture!\n" );	success = false;	}
	if( !professor_name[5].loadFromRenderedText_name( professor_name_text_5 , prof_name_color ) ){
		printf( "Failed to load professor_name5 texture!\n" );	success = false;	}
	if( !magicball_center.loadFromFile( "img/magic_sphere.bmp"  ) ){
		printf( "Failed to load magicball_center texture!\n" );		success = false;	}
	if( !magicball_ring_1.loadFromFile( "img/magic_ring_1.bmp"  ) ){
		printf( "Failed to load magic_ring_1 texture!\n" );		success = false;	}
	if( !magicball_ring_2.loadFromFile( "img/magic_ring_2.bmp"  ) ){
		printf( "Failed to load magic_ring_2 texture!\n" );		success = false;	}	
	if( !student_health_bg.loadFromFile( "img/stud_health_bg.bmp"  ) ){
		printf( "Failed to load student_health_bg texture!\n" );		success = false;	}
	OPmusic = Mix_LoadMUS( "img/OP.wav" );
    if( OPmusic == NULL ){
        printf( "Failed to load OPmusic! SDL_mixer Error: %s\n", Mix_GetError() );	success = false; }
    Battlemusic = Mix_LoadMUS( "img/battle_bgm.wav" );
    if( Battlemusic == NULL ){
        printf( "Failed to load Battle music! SDL_mixer Error: %s\n", Mix_GetError() );	success = false; }
	Attackmusic1 = Mix_LoadWAV( "img/attack_sound_1.wav" );
    if( Attackmusic1 == NULL ){
        printf( "Failed to load Attackmusic1! SDL_mixer Error: %s\n", Mix_GetError() );	success = false; }
	Attackmusic2 = Mix_LoadWAV( "img/attack_sound_2.wav" );
    if( Attackmusic2 == NULL ){
        printf( "Failed to load Attackmusic2! SDL_mixer Error: %s\n", Mix_GetError() );	success = false; }
	Attackmusic3 = Mix_LoadWAV( "img/attack_sound_3.wav" );
    if( Attackmusic3 == NULL ){
        printf( "Failed to load Attackmusic3! SDL_mixer Error: %s\n", Mix_GetError() );	success = false; }
	Attackmusic4 = Mix_LoadWAV( "img/attack_sound_4.wav" );
    if( Attackmusic4 == NULL ){
        printf( "Failed to load Attackmusic4! SDL_mixer Error: %s\n", Mix_GetError() );	success = false; }
	Hitmusic1 = Mix_LoadWAV( "img/hit_sound_1.wav" );
    if( Hitmusic1 == NULL ){
        printf( "Failed to load Hitmusic1! SDL_mixer Error: %s\n", Mix_GetError() );	success = false; }
    Hitmusic2 = Mix_LoadWAV( "img/hit_sound_2.wav" );
    if( Hitmusic2 == NULL ){
        printf( "Failed to load Hitmusic2! SDL_mixer Error: %s\n", Mix_GetError() );	success = false; }
    Hitmusic3 = Mix_LoadWAV( "img/hit_sound_3.wav" );
    if( Hitmusic3 == NULL ){
        printf( "Failed to load Hitmusic3! SDL_mixer Error: %s\n", Mix_GetError() );	success = false; }
    Hitmusic4 = Mix_LoadWAV( "img/hit_sound_4.wav" );
    if( Hitmusic4 == NULL ){
        printf( "Failed to load Hitmusic4! SDL_mixer Error: %s\n", Mix_GetError() );	success = false; }
    Hitmusic5 = Mix_LoadWAV( "img/hit_sound_5.wav" );
    if( Hitmusic5 == NULL ){
        printf( "Failed to load Hitmusic5! SDL_mixer Error: %s\n", Mix_GetError() );	success = false; }
    Jumpmusic = Mix_LoadWAV( "img/jump.wav" );
    if( Jumpmusic == NULL ){
        printf( "Failed to load Jumpmusic! SDL_mixer Error: %s\n", Mix_GetError() );	success = false; }
    Getpapermusic = Mix_LoadWAV( "img/getpaper.wav" );
    if( Getpapermusic == NULL ){
        printf( "Failed to load Getpapermusic! SDL_mixer Error: %s\n", Mix_GetError() );	success = false; }
    Diemusic = Mix_LoadWAV( "img/died.wav" );
    if( Diemusic == NULL ){
        printf( "Failed to load Diemusic! SDL_mixer Error: %s\n", Mix_GetError() );	success = false; }
	Clearmusic = Mix_LoadWAV( "img/stageclear.wav" );
    if( Clearmusic == NULL ){
        printf( "Failed to load Clearmusic! SDL_mixer Error: %s\n", Mix_GetError() );	success = false; }
    Gururinpo = Mix_LoadWAV( "img/gururinpo.wav" );
    if( Gururinpo == NULL ){
        printf( "Failed to load Clearmusic! SDL_mixer Error: %s\n", Mix_GetError() );	success = false; }
    Stunmusic = Mix_LoadWAV( "img/sleep.wav" );
    if( Stunmusic == NULL ){
        printf( "Failed to load Stunmusic! SDL_mixer Error: %s\n", Mix_GetError() );	success = false; }
	if( !dialogue[0].loadFromRenderedText_chinese( dialogue_text_1 ,get_f_text_color ) ){
        printf( "Failed to load dialogue text1 texture!\n" );	success = false;	}
    if( !dialogue[1].loadFromRenderedText_chinese( dialogue_text_2 ,get_f_text_color ) ){
        printf( "Failed to load dialogue text2 texture!\n" );	success = false;	}
    if( !dialogue[2].loadFromRenderedText_chinese( dialogue_text_3 ,get_f_text_color ) ){
        printf( "Failed to load dialogue text3 texture!\n" );	success = false;	}
    if( !dialogue[3].loadFromRenderedText_chinese( dialogue_text_4 ,get_f_text_color ) ){
        printf( "Failed to load dialogue text4 texture!\n" );	success = false;	}
    if( !dialogue[4].loadFromRenderedText_chinese( dialogue_text_5 ,godsound_color ) ){
        printf( "Failed to load dialogue text5 texture!\n" );	success = false;	}
    if( !dialogue[5].loadFromRenderedText_chinese( dialogue_text_6 ,get_f_text_color ) ){
        printf( "Failed to load dialogue text6 texture!\n" );	success = false;	}
    if( !opening_introduction.loadFromRenderedText_chinese( opening_introduction_text ,get_f_text_color ) ){
        printf( "Failed to load opening introduction text texture!\n" );	success = false;	}
    if( !dialogue_background.loadFromFile( "img/dialogue_background.bmp" ) ){
        printf( "Failed to load opening dialogue_background texture!\n" );	success = false;	}
    else{diabgRect = {60,360,dialogue_background.getWidth(),dialogue_background.getHeight()};}
    
    
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
	shield_texture.free();
	for(int i=0;i<6;i++)	professor_name[i].free();
	magicball_center.free();
	magicball_ring_1.free();
	magicball_ring_2.free(); 
	student_health_bg.free();
	dialogue_background.free();
	
	TTF_CloseFont( gFont );
	gFont = NULL;
	TTF_CloseFont( conti_font );
	conti_font = NULL;
	TTF_CloseFont( chinesefont );
	chinesefont = NULL;
	TTF_CloseFont( namefont );
	namefont = NULL;
	TTF_CloseFont( damagefont );
	damagefont = NULL;
	
	//Destroy window	
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;
	
	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
	Mix_Quit();
}

cards** deck_initialize(cards all[])
{
    int i, j, random_id;
    cards **deck;
    deck = new cards* [2];

    for(i = 0 ; i < 2 ; i++)
    {
        deck[i] = new cards [3];
    }

    srand(time(0));

    for(i = 0 ; i < 2 ; i++)
    {
        for(j = 0 ; j < 3 ; j++)
        {
            random_id = rand() % 21;
            deck[i][j] = all[random_id];
        }
    }

    return deck;
}

void card_draw(cards *deck[], cards all[])
{
    int i, j;
    srand(time(0));
    
    for(i = 0 ; i < 2 ; i++)
    {
        for(j = 0 ; j < 3 ; j++)
        {
            if(deck[i][j].id == -1)
            {
                deck[i][j] = (all[rand() % 21]);
            }
        }
    }
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
			
			for(int i=0;i<6;i++)	{ professor_healthbar[i] = healthbar_class( block_x * 8 - 332+38 , 5 , professor[i] ); }
			
			//While application is running
			while( !quit )
			{
				background_texture_render();		//render image in this function
				SDL_RenderPresent( gRenderer );		//update screen
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
                   	 	    	if(battling == false){
                   	 	    		Mix_FadeOutMusic(1000);
                   	     	    	Mix_PlayMusic(Battlemusic,-1);
                   	     	    	battling = true;
								}
                   	     	    state = enter_stage;
                   	     	    break;
                   	    	}
						}
					}
					else if (state == enter_stage || state == student_attacking || state == professor_attacking){
						
						if ( state == enter_stage ){
							
							student.burning = false;			student.stunning = false;
							professor[stage].burning = false;	professor[stage].stunning = false;
							student.health = student.get_health_limit();
							student_healthbar.init(student);
							//tell player what ability that enemy posesses
							if(e.type == SDL_KEYDOWN){
								switch( e.key.keysym.sym ){
	                   	 	    case SDLK_SPACE:
	                   	 	    	if(battling == false){
	                   	 	    		Mix_FadeOutMusic(1000);
	                   	     	    	Mix_PlayMusic(Battlemusic,-1);
	                   	     	    	battling = true;
									}
	                   	     	    state = student_attacking;
	                   	     	    break;
	                   	    	}
							}
						}
						else if( state == student_attacking ){
							cards selected_card;
							int row=-1,col=-1;
							for(int i = 0 ; i < 6 ; i++){
								gButtons[i].handleEvent(&e);
							}
							
							if(e.type == SDL_MOUSEBUTTONUP ){
								for(int i=0;i<6;i++){
									if(gButtons[i].mouse_on == true){
										selected_card = battle_deck[i/3][i%3];
										start_attacking = true;
										row = i/3;
										col = i%3;
										cout <<"row = "<< row <<" col = " << col <<endl;
										break;
									}
								} 
							}
							if( start_attacking ){
								if(!student.stunning){
									int professor_hurt_damage = -1;			//the damage professor take
									
									card_effect(selected_card, professor[stage], student);
									student_healthbar.update(student);
									
									stud_attack_animation( &selected_card ); //attack
									
									if( probability(student.hit_rate,professor[stage].avoid_rate) == 1){
										professor_hurt_damage = professor[stage].hurt(selected_card);	//professor get hurt 
										render_damage_text(professor_hurt_damage);
										professor_healthbar[stage].update(professor[stage]);
									}
									else{
										render_damage_text(professor_hurt_damage,false);
									}
									SDL_Delay(300);	
									cout<<selected_card.get_attack()<<endl;	//for testing
									
									battle_deck[row][col].id = -1;
									card_draw(battle_deck,  all_card);
									
									if( professor[stage].burning == true){ professor[stage].health -= 3; }{
										professor_healthbar[stage].update(professor[stage]);
										SDL_Delay(300);
									}
									
									//deal with card effect
								}
								else{	//student is stunning
									Mix_PlayChannel( -1, Stunmusic, 0 );
									stun_animation(true);	//true means student is stunning
									student.stunning = false; 
								}
								
								if(professor[stage].alive() == false){
									Mix_PlayChannel( -1, Clearmusic, 0 );
									state = gatcha;
									start_attacking = false;
								}
								else{
									state = professor_attacking;
									start_attacking = false;
								}
							}
							
							
							
						}
						else if( state == professor_attacking ){
							
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

void card_sprite_preset(){
	for( int i = 0; i < BUTTON_SPRITE_TOTAL; ++i )
	{
		gSpriteClips[ i ].x = 0;
		gSpriteClips[ i ].y = i * 160;
		gSpriteClips[ i ].w = BUTTON_WIDTH;
		gSpriteClips[ i ].h = BUTTON_HEIGHT;
	}
	//Set buttons in corners
	gButtons[0].setPosition( 200, 430 );
	gButtons[1].setPosition( 560, 430 );
	gButtons[2].setPosition( 920, 430 );
	gButtons[3].setPosition( 200, 600 );
	gButtons[4].setPosition( 560, 600 );
	gButtons[5].setPosition( 920, 600 );	
}

void papertable_render(){
	paper_status_table_texture.render(0,0,&paper_table_rect);
	if(paper[0])	paper_texture[0].render(35,15,&paper_1_rect);
	if(paper[1])	paper_texture[1].render(105,15,&paper_2_rect);
	if(paper[2])	paper_texture[2].render(175,15,&paper_3_rect);
}

void battlescene_render(){
	stage_background_texture[stage].render(0,0);//Render texture to screen
	if (student.burning){ burning_texture.render( student_burn_rect.x , student_burn_rect.y, &student_burn_rect ); }
	if (student.stunning){ stunning_texture.render( student_stun_rect.x , student_stun_rect.y, &student_stun_rect );	}
	if (professor[stage].burning){ burning_texture.render( professor_burn_rect.x , professor_burn_rect.y, &professor_burn_rect) ; }
	if (professor[stage].stunning){ stunning_texture.render( professor_stun_rect.x , professor_stun_rect.y, &professor_stun_rect ); }
	
	professor_texture[stage].render( professor_pos_rect.x , professor_pos_rect.y , &professor_pos_rect );

	if(start_attacking == true || state != student_attacking){
		for(int i = 0 ; i < 6 ; i++){
			gButtons[i].Freese_LButtonSprite();
		}	
	}
	for(int i = 0 ; i < 6 ; i++){
		gButtons[i].render(card_texture, battle_deck, i);
	}

	
	papertable_render();
	professor_healthbar[stage].render();	//render healthbar
	student_healthbar.render(student);				//render healthbar
	stud_health_render();
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
		Mix_VolumeMusic(VOLUME);
		start_texture.render(0,0);//Render texture to screen
		continue_button_render();
		if(battling == true){
    		Mix_FadeOutMusic(1000);
 	    	Mix_PlayMusic(OPmusic,-1);
 	    	battling = false;
		}
		
	}
	else if( state == explanation ){
		explanation_texture.render(0,0);//Render texture to screen
        if(shown_explanation[0] == false){
        	shown_explanation[0] = true;
			for(int i=0;i<50;i++){
				diabgRect.y = 360 - dialogue_background.getHeight()/50 * i ;
				diabgRect.h = dialogue_background.getHeight()/50 * i * 2 ;
				dialogue_background.render(diabgRect.x,diabgRect.y,&diabgRect);
				SDL_RenderPresent( gRenderer );
				SDL_Delay(20);
			}
		}
		dialogue_background.render(diabgRect.x,diabgRect.y,&diabgRect);
		SDL_Rect opr = {80,100,opening_introduction.getWidth(),opening_introduction.getHeight()};
		opening_introduction.render(opr.x,opr.y,&opr);
		
		
		//here to render the explanation words 
		//or put the words in the explanation_texture
		continue_button_render();
		
	}
	else if (state == enter_stage || state == student_attacking || state == professor_attacking){
		battlescene_render();
		if(state == student_attacking && round_attacked == true){
			round_attacked = false;
		}
		if (state == professor_attacking && round_attacked == false){
			round_attacked = true;
			professor_function();
		}
		if(state == enter_stage){
            if(stage == 1){
                SDL_Rect diaR = {80,500,dialogue[0].getWidth(),dialogue[0].getHeight()};
                if(shown_explanation[1] == false){
		        	shown_explanation[1] = true;
					for(int i=0;i<50;i++){
						diabgRect.y = 630 - dialogue_background.getHeight()/100 * i ;
						diabgRect.h = dialogue_background.getHeight()/50 * i ;
						dialogue_background.render(diabgRect.x,diabgRect.y,&diabgRect);
						SDL_RenderPresent( gRenderer );
						SDL_Delay(20);
					}
				}
                dialogue_background.render(diabgRect.x,diabgRect.y,&diabgRect);
				dialogue[0].render(diaR.x,diaR.y,&diaR);
				continue_button_render();
            }
            else if(stage == 2){
                SDL_Rect diaR = {80,500,dialogue[1].getWidth(),dialogue[1].getHeight()};
                if(shown_explanation[2] == false){
		        	shown_explanation[2] = true;
					for(int i=0;i<50;i++){
						diabgRect.y = 630 - dialogue_background.getHeight()/100 * i ;
						diabgRect.h = dialogue_background.getHeight()/50 * i ;
						dialogue_background.render(diabgRect.x,diabgRect.y,&diabgRect);
						SDL_RenderPresent( gRenderer );
						SDL_Delay(20);
					}
				}
                dialogue_background.render(diabgRect.x,diabgRect.y,&diabgRect);
                dialogue[1].render(diaR.x,diaR.y,&diaR);
                continue_button_render();
            }
            else if(stage == 3){
                SDL_Rect diaR = {80,500,dialogue[2].getWidth(),dialogue[2].getHeight()};
                if(shown_explanation[3] == false){
		        	shown_explanation[3] = true;
					for(int i=0;i<50;i++){
						diabgRect.y = 630 - dialogue_background.getHeight()/100 * i ;
						diabgRect.h = dialogue_background.getHeight()/50 * i ;
						dialogue_background.render(diabgRect.x,diabgRect.y,&diabgRect);
						SDL_RenderPresent( gRenderer );
						SDL_Delay(20);
					}
				}
                dialogue_background.render(diabgRect.x,diabgRect.y,&diabgRect);
                dialogue[2].render(diaR.x,diaR.y,&diaR);
                continue_button_render();
            }
            else if(stage == 4){
                SDL_Rect diaR = {80,500,dialogue[3].getWidth(),dialogue[3].getHeight()};
                if(shown_explanation[4] == false){
		        	shown_explanation[4] = true;
					for(int i=0;i<50;i++){
						diabgRect.y = 630 - dialogue_background.getHeight()/100 * i ;
						diabgRect.h = dialogue_background.getHeight()/50 * i ;
						dialogue_background.render(diabgRect.x,diabgRect.y,&diabgRect);
						SDL_RenderPresent( gRenderer );
						SDL_Delay(20);
					}
				}
                dialogue_background.render(diabgRect.x,diabgRect.y,&diabgRect);
                dialogue[3].render(diaR.x,diaR.y,&diaR);
                continue_button_render();
            }
            else if(stage == 5){
                SDL_Rect diaR_1 = {80,500,dialogue[4].getWidth(),dialogue[4].getHeight()};                //SDL_Rect diaR = {100,400,dialogue[4].getWidth(),dialogue[4].getHeight()};
                SDL_Rect diaR_2 = {80,600,dialogue[5].getWidth(),dialogue[5].getHeight()};
                if(shown_explanation[5] == false){
		        	shown_explanation[5] = true;
					for(int i=0;i<50;i++){
						diabgRect.y = 630 - dialogue_background.getHeight()/100 * i ;
						diabgRect.h = dialogue_background.getHeight()/50 * i ;
						dialogue_background.render(diabgRect.x,diabgRect.y,&diabgRect);
						SDL_RenderPresent( gRenderer );
						SDL_Delay(20);
					}
				}
                dialogue_background.render(diabgRect.x,diabgRect.y,&diabgRect);
                dialogue[4].render(diaR_1.x,diaR_1.y,&diaR_1);
                dialogue[5].render(diaR_2.x,diaR_2.y,&diaR_2);
                continue_button_render();
            }
        }
		 
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
		if(battling == true){
    		Mix_FadeOutMusic(1000);
 	    	Mix_PlayMusic(OPmusic,-1);
 	    	battling = false;
		}
		noschool_script();
	}
	else if(state == get_f){
		if(battling == true){
    		Mix_FadeOutMusic(1000);
 	    	Mix_PlayMusic(OPmusic,-1);
 	    	battling = false;
		}
		get_f_script();
		get_f_texture.render(0,0);
		quitgame_button_render();
		
	}
	else if(state == get_aplus){
		if(battling == true){
    		Mix_FadeOutMusic(1000);
 	    	Mix_PlayMusic(OPmusic,-1);
 	    	battling = false;
		}
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
	bool jumped = false, playedgetpaper = false;
	bool finished = false, playedmusic = false;
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
				if(!playedmusic){
					playedmusic = true;
					Mix_PlayChannel(-1,Jumpmusic,0);
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
			if(!playedgetpaper){
				playedgetpaper = true;
				Mix_PlayChannel(-1,Getpapermusic,0);
			}
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
		ballR.x = 700 + 60*cos(iter) - i * 5;
		ballR.y = 280 - 120*sin(iter);
		student_healthbar.render(student);
		magicball.render(ballR.x,ballR.y,&ballR);
		SDL_RenderPresent( gRenderer );
		iter -= (PI)/60;
		SDL_Delay(10);
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
	for(int i=0;i<6;i++)	{shown_explanation[i] = false;}
	student.init();
	for(int i=0;i<5;i++)	{ professor[i] = professor_class(i); }
	professor[5] = professor_class(0);
	for(int i=0;i<3;i++) 	paper[i] = false;
	for(int i=0;i<6;i++)	{gatcha_played_animation[i] = false; drawn_paper[i] = false;}
	get_aplus_played = false;
	get_f_played = false;
	noschool_played = false;
	paper_num = 0;
	for(int i=0;i<6;i++){professor_healthbar[stage].init(professor[stage]);}
	student_healthbar.init(student);
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

void professor_name_render(){
	SDL_Rect Rect = {(1440-professor_name[stage].getWidth())/2, 5 , professor_name[stage].getWidth() , professor_name[stage].getHeight() };
//	SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0xFF );
//	SDL_RenderFillRect( gRenderer , &Rect );
	professor_name[stage].render( Rect.x, Rect.y, &Rect);
}

void stud_attack_animation( cards* card ){
	double deg = 0;
	int num = 0;
	num = rand()%4;
	if ( card->id == 13){
		Mix_PlayChannel(-1,Gururinpo,0);
	}
	else{
		if(num == 0){	Mix_PlayChannel(-1,Attackmusic1,0);}
		else if(num == 1){	Mix_PlayChannel(-1,Attackmusic2,0);}
		else if(num == 2){	Mix_PlayChannel(-1,Attackmusic3,0);}
		else if(num == 3){	Mix_PlayChannel(-1,Attackmusic4,0);}
	}
	
	
	SDL_Point center = {block_x * 5+19, 45 + block_y * 4+20};
	SDL_Rect ballR = { center.x - 30 , center.y - 30 , 60 , 60 };
	SDL_Rect ringR = { center.x - 60 , center.y - 60 , 120, 120};
	for(int i=0;i<240;i++){
		background_texture_render();
		center.x +=  1;
		center.y -=  1;
		ballR = { center.x - 30 , center.y - 30 , 60 , 60 };
		ringR = { center.x - 60 , center.y - 60 , 120, 120};
		professor_healthbar[stage].render();
		magicball_center.render(ballR.x,ballR.y,&ballR);
		magicball_ring_1.render(ringR.x,ringR.y,&ringR  ,  5*i);
		magicball_ring_2.render(ringR.x,ringR.y,&ringR  , -5*i);
		deg = 5*i;
		SDL_RenderPresent( gRenderer );
		SDL_Delay(10);
	}
}

void render_damage_text(int damage,bool hit){
	int xpos = rand()%300 + 721 ;
	int ypos = rand()%200 + 91  ;
	int left = rand() % 2;
	SDL_Rect ballR = { block_x * 5+19+240 - 30 , 45 + block_y * 4+20-240 - 30 , 60 , 60 };
	SDL_Rect ringR = { block_x * 5+19+240 - 60 , block_y * 4+20-240 -20 , 120, 120};
	for(int i=0;i<30;i++){
		background_texture_render();
		professor_healthbar[stage].render();
		magicball_center.render(ballR.x,ballR.y,&ballR);
		magicball_ring_1.render(ringR.x,ringR.y,&ringR  ,  1200);
		magicball_ring_2.render(ringR.x,ringR.y,&ringR  , -1200);
		if(hit){
			if(left)	damage_render(xpos-2*i,ypos-30*i+2*i*i,damage);
			else		damage_render(xpos+2*i,ypos-30*i+2*i*i,damage);
		}
		else{
			if(left)	miss_render(xpos-2*i,ypos-30*i+2*i*i);
			else		miss_render(xpos+2*i,ypos-30*i+2*i*i);
		}
		SDL_RenderPresent( gRenderer );
		SDL_Delay(30);
	}
}

void stud_health_render(){
	student_health_text.str("");
    student_health_text << "HP: " << student.health;
	if( !student_health_text_texture.loadFromRenderedText( student_health_text.str().c_str(), continue_button_color ) )
    {  printf( "Unable to render student_health_text_texture!\n" );}
	SDL_Rect HPbg = {block_x * 5-200, 40 + block_y * 4,160,40};
	student_health_bg.render(HPbg.x,HPbg.y,&HPbg);
	SDL_Rect healthtextrect = {HPbg.x+32,HPbg.y+4,student_health_text_texture.getWidth(),student_health_text_texture.getHeight()};
	student_health_text_texture.render(healthtextrect.x,healthtextrect.y,&healthtextrect);
}

void damage_render(int x,int y,int damage){	
	
	damage_text.str("");
	damage_text << "-" << damage;
	
	if (!damage_text_texture.loadFromRenderedText_damage( damage_text.str().c_str(), colll ) ){
		printf("Unable to render damage_text_texture!\n");	}
	
	SDL_Rect damageR = { x ,  y , damage_text_texture.getWidth(), damage_text_texture.getHeight()};
	damage_text_texture.render(damageR.x,damageR.y,&damageR);
}

void miss_render(int x,int y){
	damage_text.str("");
	damage_text << "MISS";
	if (!damage_text_texture.loadFromRenderedText_damage( damage_text.str().c_str(), {255,0,0} ) ){
		printf("Unable to render damage_text_texture!\n");	}
	SDL_Rect damageR = { x ,  y , damage_text_texture.getWidth(), damage_text_texture.getHeight()};
	damage_text_texture.render(damageR.x,damageR.y,&damageR);
}

void professor_function(){
	if(!professor[stage].stunning ){
		prof_attack_animation();
		int hitted = probability( professor[ stage ].hit_rate, student.avoid_rate );
		if(hitted != 0){
			int num = 0;
			num = rand()%4;
			if(num == 0){	Mix_PlayChannel(-1,Hitmusic1,0);}
			else if(num == 1){	Mix_PlayChannel(-1,Hitmusic2,0);}
			else if(num == 2){	Mix_PlayChannel(-1,Hitmusic3,0);}
			else if(num == 3){	Mix_PlayChannel(-1,Hitmusic4,0);}
			else if(num == 4){	Mix_PlayChannel(-1,Hitmusic5,0);}
			
			student.hurt( professor[ stage ].attack );
			professor[stage].do_effect( student );
			SDL_Delay(300);
			student_healthbar.update( student );
		}
		else{
			bool left = rand()%2;
			
			for(int i=0;i<30;i++){
				background_texture_render();
				if(left)	miss_render(680-2*i,student_stun_rect.y-3-30*i+2*i*i);
				else		miss_render(680+2*i,student_stun_rect.y-3-30*i+2*i*i);	
				
				SDL_RenderPresent( gRenderer );
				SDL_Delay(30);
				damage_render(1440,720,true);
				SDL_RenderPresent(gRenderer);
			}
		}
		
	}
	else{
		SDL_Delay(300);
		student_healthbar.update(student);
		professor[stage].stunning = false;
	}
	
	SDL_Delay(300);
	if( student.burning == true){ 
		student.hurt(3);
		student_healthbar.update(student);
		SDL_Delay(300);
	}
	if( student.alive() == false ){
		Mix_PlayChannel( -1, Diemusic, 0 );
		SDL_Delay(2000);
		state = no_school; 
	}
	else{
		state = student_attacking;
		for(int i = 0 ; i < 6 ; i++){
			gButtons[i].SetDefaultSprite();
		}
	}
	return;
}

void stun_animation(bool student){
	int posx = 0 , posy = 0;
	if(student){
		posy = student_stun_rect.y - student_stun_rect.h/2 ; 
	}
	else{
		posy = professor_stun_rect.y;
	}
	
	SDL_Rect dizzyR = {posx,posy,stunning_texture.getWidth()*2,stunning_texture.getHeight()*2};
	for(int i=0;i<144;i++){
		battlescene_render();
		dizzyR.x += 10;
		stunning_texture.render(dizzyR.x, dizzyR.y, &dizzyR);
		SDL_RenderPresent( gRenderer );
		SDL_Delay(10);
	}
}

