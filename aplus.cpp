//Using SDL, SDL_image, standard IO, and strings
#include <SDL2/SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <cstdio>
#include <string>
#include "character.h"
#include <iostream>
#include <ctime>
#include <cmath>

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

SDL_Window* gWindow = NULL;	  //The window we'll be rendering to

SDL_Renderer* gRenderer = NULL;//The window renderer

SDL_Surface* gScreenSurface = NULL;

TTF_Font *gFont = NULL;

int probability(double hit_rate, double avoid_rate){		//function of hitting of not
	srand(time(NULL));
	double rate = pow(hit_rate * (1-avoid_rate),0.5);
	rate *= 100;
	if( (rand()%100+1) < rate )	return 1 ;
	else						return 0 ;
}

class LTexture
{
	public:
		//Initializes variables
		LTexture();

		//Deallocates memory
		~LTexture();

		//Loads image at specified path
		bool loadFromFile( std::string path );
		
		#if defined(SDL_TTF_MAJOR_VERSION)
		//Creates image from font string
		bool loadFromRenderedText( std::string textureText, SDL_Color textColor );
		#endif
		
		//Deallocates texture
		void free();

		//Set color modulation
		void setColor( Uint8 red, Uint8 green, Uint8 blue );

		//Set blending
		void setBlendMode( SDL_BlendMode blending );

		//Set alpha modulation
		void setAlpha( Uint8 alpha );
		
		//Renders texture at given point
		void render( int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE );

		//Gets image dimensions
		int getWidth();
		int getHeight();

	private:
		//The actual hardware texture
		SDL_Texture* mTexture;

		//Image dimensions
		int mWidth;
		int mHeight;
};

//Mouse button sprites


//Buttons objects

//Texture wrapper class


//The mouse button
LTexture::LTexture()
{
	//Initialize
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;
}

LTexture::~LTexture()
{
	//Deallocate
	free();
}

bool LTexture::loadFromFile( std::string path )
{
	//Get rid of preexisting texture
	free();

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
		//Color key image
		SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, 0, 0xFF, 0xFF ) );

		//Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
		if( newTexture == NULL )
		{
			printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
		}
		else
		{
			//Get image dimensions
			mWidth = loadedSurface->w;
			mHeight = loadedSurface->h;
		}

		//Get rid of old loaded surface
		SDL_FreeSurface( loadedSurface );
	}

	//Return success
	mTexture = newTexture;
	return mTexture != NULL;
}

#if defined(SDL_TTF_MAJOR_VERSION)
bool LTexture::loadFromRenderedText( std::string textureText, SDL_Color textColor )
{
	//Get rid of preexisting texture
	free();

	//Render text surface
	SDL_Surface* textSurface = TTF_RenderText_Solid( gFont, textureText.c_str(), textColor );
	if( textSurface == NULL )
	{
		printf( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
	}
	else
	{
		//Create texture from surface pixels
        mTexture = SDL_CreateTextureFromSurface( gRenderer, textSurface );
		if( mTexture == NULL )
		{
			printf( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
		}
		else
		{
			//Get image dimensions
			mWidth = textSurface->w;
			mHeight = textSurface->h;
		}

		//Get rid of old surface
		SDL_FreeSurface( textSurface );
	}
	
	//Return success
	return mTexture != NULL;
}
#endif

void LTexture::free()
{
	//Free texture if it exists
	if( mTexture != NULL )
	{
		SDL_DestroyTexture( mTexture );
		mTexture = NULL;
		mWidth = 0;
		mHeight = 0;
	}
}

void LTexture::setColor( Uint8 red, Uint8 green, Uint8 blue )
{
	//Modulate texture rgb
	SDL_SetTextureColorMod( mTexture, red, green, blue );
}

void LTexture::setBlendMode( SDL_BlendMode blending )
{
	//Set blending function
	SDL_SetTextureBlendMode( mTexture, blending );
}
		
void LTexture::setAlpha( Uint8 alpha )
{
	//Modulate texture alpha
	SDL_SetTextureAlphaMod( mTexture, alpha );
}

void LTexture::render( int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip )
{
	//Set rendering space and render to screen
//	SDL_Rect renderQuad = { x, y, mWidth, mHeight };

	//Set clip rendering dimensions
//	if( clip != NULL )
//	{
//		renderQuad.w = clip->w;
//		renderQuad.h = clip->h;
//	}

	//Render to screen
	SDL_RenderCopyEx( gRenderer, mTexture, NULL, clip, angle, center, flip );
}

int LTexture::getWidth()
{
	return mWidth;
}

int LTexture::getHeight()
{
	return mHeight;
}


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


LTexture start_texture ;  		//texture of start scene
LTexture explanation_texture ;	//texture of explanation scene
LTexture burning_texture ;		//texture of burning small icon
LTexture stunning_texture ; 	//texture of stunning small icon
LTexture claw_texture ;			//texture of claw(attack of professor)
LTexture get_f_texture ;		//texture of getting f end
LTexture professor_texture[6] ;	//texture of professor
LTexture stage_background_texture[6];	//texture of battle backgrounds

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
		printf( "Failed to load monster 1 texture!\n" );		success = false;	}
	if( !stage_background_texture[2].loadFromFile( "img/stage_background_2.bmp" ) ){
		printf( "Failed to load monster 2 texture!\n" );		success = false;	}
	if( !stage_background_texture[3].loadFromFile( "img/stage_background_3.bmp" ) ){
		printf( "Failed to load monster 3 texture!\n" );		success = false;	}
	if( !stage_background_texture[4].loadFromFile( "img/stage_background_4.bmp" ) ){
		printf( "Failed to load monster 4 texture!\n" );		success = false;	}
	if( !stage_background_texture[5].loadFromFile( "img/stage_background_5.bmp" ) ){
		printf( "Failed to load monster 5 texture!\n" );		success = false;	}

//    gFont = TTF_OpenFont( "img/lazy.ttf", 28 );
//    if( gFont == NULL )
//	{
//		printf( "Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError() );
//		success = false;
//	}
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
	
	
	TTF_CloseFont( gFont );
	gFont = NULL;
	
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
			
			student_class student;
			
			professor_class* professor;
			professor = new professor_class [6];
			for(int i=0;i<5;i++){
				professor[i] = professor_class(i);
				
			}
			professor[5] = professor_class(0);
			
			SDL_Rect student_burn_rect = {90*2,90*5,90,90}; //burning icon position
			SDL_Rect professor_burn_rect = {90*13,90,90,90}; //burning icon position
			SDL_Rect student_stun_rect = {90*2,90*6,90,90};
			SDL_Rect professor_stun_rect = {90*13,90*2,90,90}; //burning icon position
			
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
						start_texture.render(0,0);//Render texture to screen
						SDL_RenderPresent( gRenderer );//update screen
						
						if(e.type == SDL_KEYDOWN){
							
							switch( e.key.keysym.sym )
                	        {
                	    	    case SDLK_SPACE:
                	        	    state = explanation;
                	        		break;
                   	     	}
						}
					}
					else if (state == explanation){
						explanation_texture.render(0,0);//Render texture to screen
						//這裡要render說明的文字
						//或是把文字打在圖片裡 
						SDL_RenderPresent( gRenderer );//Update screen
						if(e.type == SDL_KEYDOWN){
							switch( e.key.keysym.sym )
                   	    	{
                   	 	    case SDLK_SPACE:
                   	     	    state = enter_stage;
                   	     	    break;
                   	    	}
						}
					}
					else if (state == enter_stage || state == student_attacking || state == professor_attacking){
						explanation_texture.render(0,0);//Render texture to screen
						if (student.burning){ burning_texture.render(student_burn_rect.x, student_burn_rect.y,&student_burn_rect); }
						if (student.stunning){ stunning_texture.render(student_stun_rect.x, student_stun_rect.y, &student_stun_rect);	}
						if (professor[stage].burning){ burning_texture.render(professor_burn_rect.x, professor_burn_rect.y, &professor_burn_rect); }
						
						
						
						
						SDL_RenderPresent( gRenderer );//Update screen
						if ( state == enter_stage ){
							
						}
						else if( state == student_attacking ){
							
						}
						else if( state == professor_attacking ){
							
							if(!professor[stage].stunning){
								student.hurt( probability( professor[ stage ].hit_rate, student.avoid_rate ) * professor[ stage ].attack );

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
								
							}
							
							if( student.burning == true){ student.hurt(3); }
							if( student.alive() == false ){	state = no_school; }
						}
					
						
					}
					else if(state == gatcha){
						
					}
					else if(state == no_school){
						//wasted動畫 
					}
					else if(state == get_f){
						//wasted動畫 
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
