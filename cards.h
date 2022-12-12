#ifndef _CARDS_H
#define _CARDS_H

#include "character.h"
#include <ctime>
#include <cstdlib>

class cards
{
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
    friend void card_effect(cards card, professor_class prof, student_class stud)
    {
        stud.health += card.self_heal;
        stud.shield += card.self_shield;
        stud.defence += card.self_defense;
        stud.attack_rate *= card.self_attack_rate;
        stud.hit_rate *= card.self_hit_rate;
        stud.avoid_rate += card.self_avoid_rate;

        prof.attack *= card.enemy_attack_rate;
        prof.defence -= card.enemy_defense_loss;
        prof.hit_rate *= card.enemy_hit_rate;
        prof.avoid_rate -= card.enemy_avoid_rate;

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
            card.attack = (temp_health - stud.health) * (stud.attack_rate);
        }

        if(card.id == 8)
        {
            prof.burning = false;
        }

        if(card.id == 10)
        {
            prof.stunning = true;
        }

        if(card.id == 14)
        {
            stud.stunning = true;
        }

        if(card.id == 19)
        {
            stud.burning = false;
        }

        card.id = -1;
    }
    public:
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
            enemy_hit_rate = 0.00;
            enemy_avoid_rate = 0.00;
        }
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
        double enemy_hit_rate = 0.00;
        double enemy_avoid_rate = 0.00;
};

cards** deck_initialize(cards all[])
{
    int i, j, random_id;
    cards **deck;
    deck = new cards* [2];

    for(i = 0 ; i < 2 ; i++)
    {
        deck[i] = new cards [3];
    }

    srand(time(0));

    for(i = 0 ; i < 2 ; i++)
    {
        for(j = 0 ; j < 3 ; j++)
        {
            random_id = rand() % 21;
            deck[i][j] = all[random_id];
        }
    }

    return deck;
}



void card_draw(cards *deck[], cards all[])
{
    int i, j;
    srand(time(0));
    
    for(i = 0 ; i < 2 ; i++)
    {
        for(j = 0 ; j < 3 ; j++)
        {
            if(deck[i][j].id == -1)
            {
                deck[i][j] = (all[rand() % 21]);
            }
        }
    }
}

#endif