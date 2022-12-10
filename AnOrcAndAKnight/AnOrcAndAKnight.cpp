// AnOrcAndAKnight.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//

#include "Hero.h"
#include "Battle.h"
#include "Skill.h"
#include "UIManager.h"
#include "ResourcesManager.h"

#include <conio.h>
#include <iostream>

using namespace std;

void Init() {
    // init random number generation seed with current time, so the fights won't end the same every time
    srand((unsigned int)time(0));

    // get heroes saved
    ResourcesManager::LoadHeroes();

    // get languages resources
    vector<string>&& listLang = ResourcesManager::GetAvailableLanguages();
    ResourcesManager::LoadLanguage(listLang[0]);
}


// Edit/select hero
void SelectHeroes(Hero& hero1, Hero& hero2) {

    UIManager::GetInstance().SelectHero(hero1, 1);    
    Chaaaaaarge* pCharge = new Chaaaaaarge;
    hero1.AddSkill(pCharge);

    UIManager::GetInstance().SelectHero(hero2, 2);
    Stun* pStun = new Stun;
    hero2.AddSkill(pStun);
}

int main()
{
    Init();

    Hero hero1, hero2;

    SelectHeroes(hero1, hero2);

    // begin battle
    Battle battle(hero1, hero2);

    // based on user input, play another turn of move in battle history
    while (1) {
        NextDisplay input = UIManager::GetInstance().GetInputKeyForwardBackward();

        if (input == NextDisplay::BACKWARD) {
            // display previous turn
            UIManager::GetInstance().DisplayPreviousTurn();
            continue;
        }
        else {
            // if alredy player, just display
            if (UIManager::GetInstance().DisplayNextTurn())
                continue;
        }

        // already on end screen ? display winner
        if (battle.IsOver())
        {
            battle.DisplayScore();

            if (UIManager::GetInstance().GetInputKeyForwardBackward() == NextDisplay::FORWARD) {
                UIManager::GetInstance().DisplayPreviousTurn(true);
                continue;
            }
        }

        // compute another turn for the fight
        battle.PlayTurn();

        // display result screen
        UIManager::GetInstance().DrawNewTurn(hero1, hero2);
    }

    int a=_getch();
    return 1;
}