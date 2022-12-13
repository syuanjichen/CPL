#include "character.h"
#include <iostream>


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
int character_class::get_health_limit(){
	return health_limit;
}
void character_class::raise_health_limit(double a){
	health_limit += (int)a;
}
void student_class::hurt(double a){
	
	a = a * ( 100 / (100 + defence) );
	int damage = (int)a;
	std::cout<<"damage = "<<damage<<std::endl;
		
	if(damage > ( health + shield ) ){
		shield = 0;
		health = 0;
		living = false;
	}
	else if(damage > shield){
		damage -= shield;
		shield = 0;
		std::cout<<"damage = "<<damage<<std::endl;
		health -= damage;
		std::cout<<"health = "<<health<<std::endl;
		
	}
	else if(damage < shield){
		shield -= damage;
	}
}

professor_class::professor_class(int a){
	switch(a){
		case 1:
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
		case 2:
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
			
		case 3:
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
			
			
		case 4:
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
		case 0:
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
void professor_class::do_effect(student_class student){
	switch ( special ){
		case health_to_attack:
		
		break;

		case swifty:
		
		break;
		
		case armored:
		
		break;
		
		
		case stun:
		if(student.stunning == false && this->stun_counter >= 2){
			student.stunning = true;
			this->stun_counter = 0;
		}
		else{
			if(this->stun_counter >= 2)
				this->stun_counter = 0;
			else{
				this->stun_counter +=1;
				if (student.stunning == true){ student.stunning = false;}
			}
		}
		
		
		break;
		
		case firing:
		if(student.burning == false && this->ignite_counter >= 5){
			student.burning = true;
			this->ignite_counter = 0;
		}
		else{
			if(this->ignite_counter >= 5)
				this->ignite_counter = 0;
			else
				this->ignite_counter +=1;
		}
		
		break;
		
	}
}
