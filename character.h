#ifndef _CHARACTER_H
#define _CHARACTER_H

#ifndef _ATTRIBUTE
#define _ATTRIBUTE
enum attribute{
	fire,
	water,
	grass
};
#endif
class cards;

enum effect{
	health_to_attack,	// attack increases while losing health
	swifty,  			// high avoid rate
	armored,			// high defence 
	stun,				// tend to stun student	  (stun every 3 rounds)
	firing,				// tend to ignite student (ignite every 6 rounds)
	
};
class character_class{
	protected:
		int health_limit;
		bool living;
	public:
		int health;
		int shield;
		int defence;
		double hit_rate;
		double avoid_rate;
		bool burning;
		bool stunning;
		bool alive() { return living; }		//return alive or not
		int get_health_limit();				//
		void raise_health_limit(double);	//
};

class student_class : public character_class{						// class of the main character (student) 
	public:
		int attack;
		double attack_rate;
		
		student_class();
		void init();
		void hurt(double);					// input the damage and adjust the health of student by calculating defence factors
};

class professor_class : public character_class{						//class of enemy
	
	protected:
		int difficulty;  			
		attribute element;			
	public:
		int attack;	
		effect special;						//spacial ability
		int ignite_counter;
		int stun_counter;
		
		professor_class(int a = 0);
		attribute get_attribute() { return element; }	//return attribute 
		void do_effect( student_class student);
		int hurt(cards);
		//void hurt(double);							//this is lee's job
};



#endif
