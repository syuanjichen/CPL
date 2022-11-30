#include "character.h"

student_class::student_class(){
	health_limit = 100;
	living = true;
	
	health = 100;
	shield = 0;
	defence = 0;
	attack_rate = 1;
	hit_rate = 0.9;
	avoid_rate = 0.1;
	burning = false;
	stunning = false;
	
}
void student_class::init(){
	health_limit = 100;
	living = true;
	
	health = 100;
	shield = 0;
	defence = 0;
	attack_rate = 1;
	hit_rate = 0.9;
	avoid_rate = 0.1;
	burning = false;
	stunning = false;
}
int student_class::get_health_limit(){
	return health_limit;
}
void student_class::raise_health_limit(double a){
	health_limit += (int)a;
}
void student_class::hurt(double a){
	
	a = a * ( 100 / (100 + defence) );
	int damage = (int)a;
		
	if(damage > ( health + shield ) ){
		shield = 0;
		health = 0;
		living = false;
	}
	else if(damage > shield){
		shield = 0;
		health -= (damage - shield);
		
	}
	else if(damage < shield){
		shield -= damage;
	}
}
bool student_class::alive(){
	return living;
}



professor_class::professor_class(int a = 0){
	switch(a){
		case 0:
			health_limit = 200;
			difficulty = 1;
			element = water;
			living = true;
			health = health_limit;
			shield = 0;
			attack = 5;
			defence = 400;
			hit_rate = 0.95;
			avoid_rate = 0;
			burning = false;
			stunning = false;
			special = armored;
			ignite_counter = 0;
			stun_counter = 0;
			break;	
		case 1:
			health_limit = 300;
			difficulty = 2;
			element = grass;
			living = true;
			health = health_limit;
			shield = 0;
			attack = 15;
			defence = 50;
			hit_rate = 0.9;
			avoid_rate = 0.95;
			burning = false;
			stunning = false;
			special = swifty;
			ignite_counter = 0;
			stun_counter = 0;
			break;
			
		case 2:
			health_limit = 600;
			difficulty = 3;
			element = fire;
			living = true;
			health = health_limit;
			shield = 0;
			attack = 10;
			defence = 150;
			hit_rate = 0.9;
			avoid_rate = 0.1;
			burning = false;
			stunning = false;
			special = firing;
			ignite_counter = 6;
			stun_counter = 0;
			break;
			
			
		case 3:
			health_limit = 1000;
			difficulty = 4;
			element = grass;
			living = true;
			health = health_limit;
			shield = 0;
			attack = 20;
			defence = 50;
			hit_rate = 0.9;
			avoid_rate = 0.1;
			burning = false;
			stunning = false;
			special = stun;
			ignite_counter = 0;
			stun_counter = 3;
			break;
		case 4:
			health_limit = 500;
			difficulty = 10;
			element = grass;
			living = true;
			health = health_limit;
			shield = 0;
			attack = 20;
			defence = 50;
			hit_rate = 0.9;
			avoid_rate = 0.1;
			burning = false;
			stunning = false;
			special = health_to_attack;
			ignite_counter = 0;
			stun_counter = 0;
			break;
	}
	
}
