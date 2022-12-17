#ifndef _HEALTHBAR_H
#define _HEALTHBAR_H

#include <SDL2/SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <cstdio>
#include <string>
#include <iostream>
#include <ctime>
#include <cmath>
#include "LTexture.h"
#include "character.h"

extern LTexture healthbar_texture;

class healthbar_class{
	private:
		int x;
		int y;
		int max_value;			//max healthbar value
		int previous_value;		//previous health value
		int current_value;		//current health value
	public:
		healthbar_class(int,int,student_class);
		healthbar_class(int,int,const professor_class);
		healthbar_class(){for(int i=0;i<100;++i){ hp[i] = true; }}
		healthbar_class( const healthbar_class & );
		bool hp[100];
		
		void init(student_class);
		void init(professor_class);
		void update(student_class);
		void update(professor_class);
		void render();
		void render(student_class);
};
#endif
