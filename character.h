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

enum effect{
	health_to_attack,	// attack increases while losing health
	swifty,  			// high avoid rate
	armored,			// high defence 
	stun,				// tend to stun student	  (stun every 3 rounds)
	firing,				// tend to ignite student (ignite every 6 rounds)
	
};


class student_class{						// class of the main character (student) 
	private:
		int health_limit;
		bool living;
	public:
		int health;
		int shield;	
		int defence;
		double attack_rate;
		double hit_rate;
		double avoid_rate;
		bool burning;
		bool stunning;
		
		student_class();
		void init();
		
		int get_health_limit();				//取得血量上限 
		void raise_health_limit(double);	//提高血量上限 
		
		bool alive();						//回傳是生(true)是死(false) 
		
		void hurt(double);					//受傷的函數，輸入受到的傷害，經過防禦計算後修改血量 
};

class professor_class{					//class of enemy
	
	private:
		int health_limit;
		int difficulty;  			//討伐難度 
		attribute element;			//屬性 
		bool living;
	public:
		int health;
		int shield;
		int attack;	
		int defence;
		double hit_rate;
		double avoid_rate;
		bool burning;
		bool stunning;
		effect special;				//特殊技能 
		int ignite_counter;
		int stun_counter;
		
		professor_class(int a = 0);
		attribute get_attribute() { return element; }	//回傳屬性 
		bool alive() { return living; }					//回傳是否生存 
		
		//void hurt(double);							//因為還不確定屬性相剋要怎麼寫，我就先不寫 
};



#endif
