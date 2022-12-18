#ifndef _CARDS_H
#define _CARDS_H

#include "character.h"
#include <bits/stdc++.h>
#ifndef _ATTRIBUTE
#define _ATTRIBUTE
enum attribute{
	fire,
	water,
	grass
};
#endif
extern int stage;
extern cards **battle_deck;
extern cards all_card[21];
extern void card_draw(cards *deck[], cards all[]);

class cards
{
    public:
	
	friend void cards_initialize(cards deck[])
    {
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
        deck[3].self_attack_rate = 1.30;

        deck[4].nature = fire;

        deck[5].nature = fire;
        deck[5].enemy_defense_loss = 50;

        deck[6].nature = fire;

        // --------------------

        // Water Cards
        deck[7].nature = water;
        deck[7].attack = 20;
        deck[7].self_hit_rate = -0.05;

        deck[8].nature = water;
        deck[8].attack = 15;
        deck[8].enemy_attack_rate = 0.95;

        deck[9].nature = water;
        deck[9].attack = 10;
        deck[9].enemy_attack_rate = 0.85;

        deck[10].nature = water;
        deck[10].attack = 10;

        deck[11].nature = water;
        deck[11].self_defense = 50;

        deck[12].nature = water;
        deck[12].self_shield = 40;
        
        deck[13].nature = water;
        deck[13].enemy_avoid_rate = 0.03;

        // --------------------

        // Grass Cards
        deck[14].nature = grass;
        deck[14].attack = 20;

        deck[15].nature = grass;
        deck[15].attack = 10;
        deck[15].self_hit_rate = 0.05;

        deck[16].nature = grass;
        deck[16].attack = 10;
        deck[16].self_heal = 40;
        deck[16].self_health_limit = 40;

        deck[17].nature = grass;
        deck[17].attack = 5;
        deck[17].self_heal = 70;

        deck[18].nature = grass;
        deck[18].enemy_hit_rate = 0.90;

        deck[19].nature = grass;

        deck[20].nature = grass;
        deck[20].self_attack = 20;

        // ---------------------

    }
    friend void card_effect(cards &card, professor_class &prof, student_class &stud)
    {
        stud.raise_health_limit(card.self_health_limit);
		stud.health += card.self_heal;
        stud.shield += card.self_shield;
        stud.defence += card.self_defense;
        stud.attack_rate *= card.self_attack_rate;
        stud.hit_rate += card.self_hit_rate;
        stud.avoid_rate += card.self_avoid_rate;
        stud.attack += card.self_attack;
        if(stud.health > stud.get_health_limit()){
        	stud.health = stud.get_health_limit();
		}
        if(stud.hit_rate >= 1){
        	stud.hit_rate = 1;
		}
		else if(stud.hit_rate <= 0){
			stud.hit_rate = 0;
		}
        if(stud.avoid_rate >= 1){
        	stud.avoid_rate = 1;
		}
		

        prof.attack *= card.enemy_attack_rate;
        prof.defence -= card.enemy_defense_loss;
        prof.hit_rate *= card.enemy_hit_rate;
        prof.avoid_rate -= card.enemy_avoid_rate;
        if(prof.defence <= 0){
        	prof.defence = 0;
		}
        if(prof.avoid_rate <= 0){
        	prof.avoid_rate = 0;
		}
		if(card.id == 0){
			for(int i=0;i<6;i++){
				battle_deck[i/3][i%3].id = -1;
				card_draw(battle_deck,all_card);
			}
		}
        if(card.id == 2) // burning effect
        {
            prof.burning = true;
        }

        if(card.id == 4)
        {
            stud.avoid_rate += (1.00 - stud.avoid_rate) / 7.00;
        }

        if(card.id == 6)
        {
            int temp_health;
            temp_health = stud.health;
            stud.health = 1;
            card.attack = ((temp_health - stud.health) * (stud.attack_rate) * stage ) ;
        }

        if(card.id == 8)
        {
            prof.burning = false;
        }

        if(card.id == 10)
        {
            int prob = rand()% 5;
            if(prob > 1){
            	prof.stunning = true;
			}
        }

        if(card.id == 14)
        {
            stud.stunning = true;
        }

        if(card.id == 19)
        {
            stud.burning = false;
        }

        //card.id = -1;
    }
        int id;
        cards()
        {
            id = 0;
            nature = fire;
            self_attack = 0;
            self_heal = 0;
            self_health_limit = 0;
            self_shield = 0;
            self_defense = 0;
            self_attack_rate = 1.00;
            self_hit_rate = 0.00;
            self_avoid_rate = 0.00;
            enemy_attack_rate = 1.00;
            enemy_defense_loss = 0;
            enemy_hit_rate = 1.00;
            enemy_avoid_rate = 0.00;
        }
        cards(cards& card){
        	nature = card.nature;
	        attack = card.attack;
	        self_attack = card.self_attack;
	        self_heal = card.self_heal;
	        self_health_limit = card.self_health_limit;
	        self_shield = card.self_shield;
	        self_defense = card.self_defense;
	        self_attack_rate = card.self_attack_rate;
	        self_hit_rate = card.self_hit_rate;
	        self_avoid_rate = card.self_avoid_rate;
	        enemy_attack_rate = card.enemy_attack_rate;
	        enemy_defense_loss = card.enemy_defense_loss;
	        enemy_hit_rate = card.enemy_hit_rate;
	        enemy_avoid_rate = card.enemy_avoid_rate;
		}
        attribute get_attribute() {return nature;}
        int get_attack() { return attack;	}
        void set_attack(int value) { attack = value;}
    private:
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
        double enemy_hit_rate = 1.00;
        double enemy_avoid_rate = 0.00;
};


#endif
