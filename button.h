#ifndef _BUTTON_H
#define _BUTTON_H

#include <SDL2/SDL.h>
#include <SDL_image.h>
#include <bits/stdc++.h>
#include "cards.h"
#include "LTexture.h"

const int BUTTON_WIDTH = 350;
const int BUTTON_HEIGHT = 160;

enum LButtonSprite
{
	BUTTON_SPRITE_MOUSE_OUT = 0,
	BUTTON_SPRITE_MOUSE_OVER_MOTION = 1,
	BUTTON_SPRITE_MOUSE_DOWN = 2,
	BUTTON_SPRITE_MOUSE_UP = 3,
	BUTTON_SPRITE_FREEZE = 4,
	BUTTON_SPRITE_TOTAL = 5
};

//The mouse button
class LButton
{
	public:
		//Initializes internal variables
		LButton();

		//Sets top left position
		void setPosition( int x, int y );

		//Handles mouse event
		void handleEvent( SDL_Event* e );
		
		void handleEvent_hint( SDL_Event* );

		//Shows button sprite
		void render(LTexture card_texture[], cards **battle_deck, int i);
		
		void render_hint(LTexture& texture);

		int GetPosition_x()
		{
			return mPosition.x;
		}

		int GetPosition_y()
		{
			return mPosition.y;
		}

		int get_LButtonSprite(){return mCurrentSprite;}
		void Freese_LButtonSprite(){mCurrentSprite = BUTTON_SPRITE_FREEZE;}
		void SetDefaultSprite(){mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT;}

		bool mouse_on;

	private:
		//Top left position
		SDL_Point mPosition;

		//Currently used global sprite
		LButtonSprite mCurrentSprite;
};

//The window renderer
extern SDL_Renderer* gRenderer;

//Mouse button sprites
SDL_Rect gSpriteClips[ BUTTON_SPRITE_TOTAL ];
LTexture gButtonSpriteSheetTexture;
SDL_Rect ButtonSpriteClips[ BUTTON_SPRITE_TOTAL ];

//Buttons objects

LButton::LButton()
{
	mPosition.x = 0;
	mPosition.y = 0;

	mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT;
}

void LButton::setPosition( int x, int y )
{
	mPosition.x = x;
	mPosition.y = y;
}

void LButton::handleEvent( SDL_Event* e )
{
	//If mouse event happened
	if( e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP )
	{
		//Get mouse position
		int x, y;
		SDL_GetMouseState( &x, &y );

		//Check if mouse is in button
		bool inside = true;

		//Mouse is left of the button
		if( x < mPosition.x )
		{
			inside = false;
		}
		//Mouse is right of the button
		else if( x > mPosition.x + BUTTON_WIDTH )
		{
			inside = false;
		}
		//Mouse above the button
		else if( y < mPosition.y )
		{
			inside = false;
		}
		//Mouse below the button
		else if( y > mPosition.y + BUTTON_HEIGHT )
		{
			inside = false;
		}
		mouse_on = inside;
		//Mouse is outside button
		if( !inside )
		{
			mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT;
		}
		//Mouse is inside button
		else
		{
			//Set mouse over sprite
			switch( e->type )
			{
				case SDL_MOUSEMOTION:
				mCurrentSprite = BUTTON_SPRITE_MOUSE_OVER_MOTION;
				break;

				case SDL_MOUSEBUTTONDOWN:
				mCurrentSprite = BUTTON_SPRITE_MOUSE_DOWN;
				break;

				case SDL_MOUSEBUTTONUP:
				mCurrentSprite = BUTTON_SPRITE_MOUSE_UP;
				break;
			}
		}
	}
}

void LButton::handleEvent_hint( SDL_Event* e )
{
	//If mouse event happened
	if( e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP )
	{
		//Get mouse position
		int x, y;
		SDL_GetMouseState( &x, &y );

		//Check if mouse is in button
		bool inside = true;

		//Mouse is left of the button
		if( x < mPosition.x )
		{
			inside = false;
		}
		//Mouse is right of the button
		else if( x > mPosition.x + 40 )
		{
			inside = false;
		}
		//Mouse above the button
		else if( y < mPosition.y )
		{
			inside = false;
		}
		//Mouse below the button
		else if( y > mPosition.y + 40 )
		{
			inside = false;
		}
		mouse_on = inside;
		//Mouse is outside button
		if( !inside )
		{
			mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT;
		}
		//Mouse is inside button
		else
		{
			//Set mouse over sprite
			switch( e->type )
			{
				case SDL_MOUSEMOTION:
				mCurrentSprite = BUTTON_SPRITE_MOUSE_OVER_MOTION;
				break;

				case SDL_MOUSEBUTTONDOWN:
				mCurrentSprite = BUTTON_SPRITE_MOUSE_DOWN;
				break;

				case SDL_MOUSEBUTTONUP:
				mCurrentSprite = BUTTON_SPRITE_MOUSE_UP;
				break;
			}
		}
	}
}

void LButton::render(LTexture card_texture[], cards** battle_deck, int i)
{
	//Show current button sprite
	//card_texture[id].render_card( mPosition.x, mPosition.y, &gSpriteClips[ mCurrentSprite ], &gSpriteClips[ mCurrentSprite ]);
	int pos = battle_deck[i / 3][i % 3].id;
	card_texture[pos].render_card( mPosition.x, mPosition.y, &gSpriteClips[ mCurrentSprite ]);
}

void LButton::render_hint(LTexture &texture)
{
	texture.render_card( mPosition.x, mPosition.y, &ButtonSpriteClips[ mCurrentSprite ]);
}

#endif
