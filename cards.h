#ifndef _CARDS_H
#define _CARDS_H

#include "character.h"
#include <ctime>
#include <cstdlib>

class cards
{
    private:
        int id;
        attribute nature;
        int attack = 0;
        int self_attack = 0;
        int self_heal = 0;
        int self_health_limit = 0;
        int self_shield = 0;
        int self_defense = 0;
        double self_attack_rate = 1.00;
        double self_hit_rate = 0.00;
        double self_avoid_rate = 0.00;
        double enemy_attack_rate = 1.00;
        int enemy_defense_loss = 0;
        double enemy_hit_rate = 0.00;
        double enemy_avoid_rate = 0.00;
        
    friend void card_effect(cards*, professor_class, student_class);    
    friend void cards_initialize(cards deck[]){
	    for(int i = 0 ; i < 21 ; i++)
	    {
	        deck[i].id = i;
	    }
	
	    // Fire cards 
	    deck[0].nature = fire;
	    deck[0].attack = 20;
	
	    deck[1].nature = fire;
	    deck[1].attack = 10;
	    deck[1].enemy_attack_rate = 0.85;
	
	    deck[2].nature = fire;
	    deck[2].attack = 10;
	
	    deck[3].nature = fire;
	    deck[3].attack = 5;
	    deck[3].self_attack_rate = 1.20;
	
	    deck[4].nature = fire;
	
	    deck[5].nature = fire;
	    deck[5].enemy_defense_loss = 50;
	
	    deck[6].nature = fire;
	
	    // --------------------
	
	    // Water Cards
	    deck[7].nature = water;
	    deck[7].attack = 20;
	
	    deck[8].nature = water;
	    deck[8].attack = 15;
	
	    deck[9].nature = water;
	    deck[9].attack = 10;
	    deck[9].enemy_attack_rate = 0.85;
	
	    deck[10].nature = water;
	    deck[10].attack = 10;
	
	    deck[11].nature = water;
	    deck[11].attack = 5;
	    deck[11].self_defense = 50;
	
	    deck[12].nature = water;
	    deck[12].self_shield = 30;
	    
	    deck[13].nature = water;
	    deck[13].enemy_avoid_rate = 0.05;
	
	    // --------------------
	
	    // Grass Cards
	    deck[14].nature = grass;
	    deck[14].attack = 20;
	
	    deck[15].nature = grass;
	    deck[15].attack = 10;
	    deck[15].self_hit_rate = 0.30;
	
	    deck[16].nature = grass;
	    deck[16].attack = 10;
	    deck[16].self_heal = 40;
	    deck[16].self_health_limit = 40;
	
	    deck[17].nature = grass;
	    deck[17].attack = 5;
	    deck[17].self_heal = 60;
	
	    deck[18].nature = grass;
	    deck[18].enemy_hit_rate = 0.85;
	
	    deck[19].nature = grass;
	
	    deck[20].nature = grass;
	    deck[20].self_attack = 30;
	
	    // ---------------------
	}
};



void deck_initialize(cards* *deck[]);

void card_effect(cards *card, professor_class prof, student_class stud);

void card_draw(cards* *deck[]);

#endif
