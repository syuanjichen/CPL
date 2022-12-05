#include "cards.h"

cards all_card[21];
cards* battle_deck[2][3];

void deck_initialize(cards* *deck[])
{
    int i, j;
    srand(time(0));

    for(i = 0 ; i < 2 ; i++)
    {
        for(j = 0 ; j < 3 ; j++)
        {
            deck[i][j] = &(all_card[rand() % 21]);
        }
    }
}

void card_effect(cards *card, professor_class prof, student_class stud)
{
    stud.health += card->self_heal;
    stud.shield += card->self_shield;
    stud.defence += card->self_defense;
    stud.attack_rate *= card->self_attack_rate;
    stud.hit_rate *= card->self_hit_rate;
    stud.avoid_rate += card->self_avoid_rate;

    prof.attack *= card->enemy_attack_rate;
    prof.defence -= card->enemy_defense_loss;
    prof.hit_rate *= card->enemy_hit_rate;
    prof.avoid_rate -= card->enemy_avoid_rate;

    if(card->id == 2) // burning effect
    {
        prof.burning = true;
    }

    if(card->id == 4)
    {
        stud.avoid_rate += (1.00 - stud.avoid_rate) / 7.00;
    }

    if(card->id == 6)
    {
        int temp_health;
        temp_health = stud.health;
        stud.health = 1;
        card->attack = (temp_health - stud.health) * (stud.attack_rate);
    }

    if(card->id == 8)
    {
        prof.burning = false;
    }

    if(card->id == 10)
    {
        prof.stunning = true;
    }

    if(card->id == 14)
    {
        stud.stunning = true;
    }

    if(card->id == 19)
    {
        stud.burning = false;
    }

    card = NULL;
}

void card_draw(cards* *deck[])
{
    int i, j;
    srand(time(0));
    
    for(i = 0 ; i < 2 ; i++)
    {
        for(j = 0 ; j < 3 ; j++)
        {
            if(deck[i][j] == NULL)
            {
                deck[i][j] = &(all_card[rand() % 21]);
            }
        }
    }
}