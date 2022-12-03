#include "healthbar.h"

healthbar_class::healthbar_class(int x0, int y0, student_class student){
	x = x0;
	y = y0;
	max_value = student.get_health_limit();
	previous_value = max_value;
	current_value = max_value;
	for(int i=0;i<100;++i){ hp[i] = true; }
	
}
healthbar_class::healthbar_class(int x0, int y0, const professor_class professor){
	x = x0;
	y = y0;
	max_value = professor.health;
	previous_value = max_value;
	current_value = max_value;
	for(int i=0;i<100;++i){ hp[i] = true; }
	
}
healthbar_class::healthbar_class( const healthbar_class &s ){
	x = s.x;
	y = s.y;
	max_value = s.max_value;
	previous_value = s.previous_value;
	current_value = s.current_value;
	for(int i=0;i<100;++i){ hp[i] = s.hp[i] ;}
	
}
void healthbar_class::init(student_class student){
//	x = x0;
//	y = y0;
	max_value = student.get_health_limit();
	previous_value = max_value;
	current_value = max_value;
	for(int i=0;i<100;++i){ hp[i] = true; }
	
}
void healthbar_class::init( const professor_class professor){
//	x = x0;
//	y = y0;
	max_value = professor.health;
	previous_value = max_value;
	current_value = max_value;
	for(int i=0;i<100;++i){ hp[i] = true; }
	
}
void healthbar_class::render(){
	SDL_Rect bgRect = { x-38 , y-5 , 663 , 40 };
	healthbar_texture.render( bgRect.x , bgRect.y , &bgRect );
	for(int i=0;i<100;++i){
		SDL_Rect rect = {x + 6 * i, y, 6, 30};
		if( hp[i] == true ){
			SDL_SetRenderDrawColor( gRenderer,76,205,47,0xFF );
			SDL_RenderFillRect( gRenderer, &rect );
		}
		else{
			SDL_SetRenderDrawColor( gRenderer,0,0,0,0xFF );
			SDL_RenderFillRect( gRenderer, &rect );
		}
	}
}
void healthbar_class::update(student_class student){
	current_value = student.health;
	int end = ( current_value * 100 ) / max_value;
	int start = ( previous_value * 100 ) / max_value;
	if(end < start){
		
		while(start != end){
			for(int i=0;i<start;i++){
				hp[i] = true;
			}
			for(int i=start;i<100;i++){
				hp[i] = false;
			} 
			render();
			SDL_RenderPresent( gRenderer );//update screen
			start--;
			SDL_Delay(20);
		}
	}
	else if(end > start){
		while(start != end){
			for(int i=0;i<start;i++){
				hp[i] = true;
			}
			for(int i=start;i<100;i++){
				hp[i] = false;
			} 
			render();
			SDL_RenderPresent( gRenderer );//update screen
			start++;
			SDL_Delay(20);
		}
	}
	render();
}
