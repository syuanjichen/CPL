#include "character.h"
#include "cards.h"
#include <bits/stdc++.h>
extern student_class student;

student_class::student_class(){
	health_limit = 100;
	living = true;
	attack = 0;
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
	attack = 0;
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
	
	a = a * ( 100.0 / (100.0 + (double)defence) );
	int damage = (int)a;
		
	if(damage >= ( health + shield ) ){
		shield = 0;
		health = 0;
		living = false;
	}
	else if(damage >= shield){
		damage -= shield;
		shield = 0;
		health -= damage;
		
	}
	else if(damage < shield){
		shield -= damage;
	}
}

void student_class::direct_hurt(double a){
	a = a * ( 100.0 / (100.0 + defence) );
	int damage = (int)a;
		
	if(damage >= health ){
		health = 0;
		living = false;
	}
	else {
		health -= damage;
	}
}

int professor_class::hurt(cards card){
	double damage;
	if(element == fire){
		if(card.get_attribute() == fire ){
			damage = 1.0 *card.get_attack()*1.0*100.0 / (100.0+this->defence); 
		}
		else if(card.get_attribute() == water){
			damage = 2.0 *card.get_attack()*1.0*100.0 / (100.0+this->defence); 
		}
		else{
			damage = 0.5 *card.get_attack()*1.0*100.0 / (100.0+this->defence); 
		}
	}
	else if(element == water){
		if(card.get_attribute() == fire ){
			damage = 0.5 *card.get_attack()*1.0*100.0 / (100.0+this->defence); 
		}
		else if(card.get_attribute() == water){
			damage = 1.0 *card.get_attack()*1.0*100.0 / (100.0+this->defence); 
		}
		else{
			damage = 2.0 *card.get_attack()*1.0*100.0 / (100.0+this->defence); 
		}
	}
	else if(element == grass){
		if(card.get_attribute() == fire ){
			damage = 2.0 *card.get_attack()*1.0*100.0 / (100.0+this->defence); 
		}
		else if(card.get_attribute() == water){
			damage = 0.5 *card.get_attack()*1.0*100.0 / (100.0+this->defence); 
		}
		else{
			damage = 1.0 *card.get_attack()*1.0*100.0 / (100.0+this->defence); 
		}
	}
	damage += student.attack;
	damage *= student.attack_rate;
	if(damage >= health){
		health = 0;
		living = false;
	}
	else{
		health -= int(damage);
	}
	
	return (int)damage;
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
			defence = 600;
			hit_rate = 1.00;
			avoid_rate = 0.3;
			burning = false;
			stunning = false;
			special = armored;
			ignite_counter = 0;
			stun_counter = 0;
			break;	
		case 2:
			health_limit = 400;
			difficulty = 2;
			element = grass;
			living = true;
			health = health_limit;
			shield = 0;
			attack = 15;
			defence = 150;
			hit_rate = 1.00;
			avoid_rate = 1.00;
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
			hit_rate = 0.96;
			avoid_rate = 0.2;
			burning = false;
			stunning = false;
			special = firing;
			ignite_counter = 10;
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
			defence = 250;
			hit_rate = 0.90;
			avoid_rate = 0.3;
			burning = false;
			stunning = false;
			special = stun;
			ignite_counter = 0;
			stun_counter = 5;
			break;
		case 0:
			health_limit = 2000;
			difficulty = 100;
			element = water;
			living = true;
			health = health_limit;
			shield = 0;
			attack = 20;
			defence = 350;
			hit_rate = 1.00;
			avoid_rate = 0.40;
			burning = false;
			stunning = false;
			special = health_to_attack;
			ignite_counter = 0;
			stun_counter = 0;
			break;
	}
	
}
void professor_class::do_effect(student_class &student){
	switch ( special ){
		case health_to_attack:
			this->attack = 20 + (this->get_health_limit() - this->health);
		break;

		case swifty:
		break;
		
		case armored:
		
		break;
		
		case stun:
		if(this->stun_counter >= 2){
			student.stunning = true;
			this->stun_counter = 0;
		}
		else{
			this->stun_counter +=1;
		}
		break;
		
		case firing:
		if(this->ignite_counter >= 5){
			student.burning = true;
			this->ignite_counter = 0;
		}
		else{
			this->ignite_counter +=1;
		}
		break;
		
	}
}
