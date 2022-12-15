// AnOrcAndAKnight.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//

#include "Settings.h"
#include "Hero.h"

#include "Battle.h"
#include "Skill.h"
#include "UIManager.h"
#include "ResourcesManager.h"


using namespace std;

void Init() {
    // init random number generation seed with current time, so the fights won't end the same every time
    srand((unsigned int)time(0));

    // get heroes saved
    ResourcesManager::LoadHeroes();
}

// Edit/select hero
void SelectHeroes(Hero& hero1, Hero& hero2) {

    UI().SelectHero(hero1, 1);

    UI().SelectHero(hero2, 2, hero1._name, hero1._class._name);
}

int main()
{
    UI().SelectLanguage();

    Init();

    Hero hero1, hero2;
    SelectHeroes(hero1, hero2);

    // begin battle
    Battle battle(hero1, hero2);

    // based on user input, play another turn of move in battle history
    while (1) {
        NextDisplay input = UI().GetInputKeyForwardBackward();

        if (input == NextDisplay::BACKWARD) {
            // display previous turn
            UI().DisplayPreviousTurn();
            continue;
        }
        else {
            // if alredy player, just display
            if (UI().DisplayNextTurn())
                continue;
        }

        // Battle is over ? display winner
        if (battle.IsOver())
        {
            // first time on score board : display winner, otherwise, leave
            if (battle.DisplayScore() == false)
                return 1;

            // the end if we continue from there
            if (UI().GetInputKeyForwardBackward() == NextDisplay::FORWARD) 
                break;

            UI().DisplayPreviousTurn(true);
            continue;
        }

        // compute another turn for the fight
        battle.PlayTurn();

        // display result screen
        UI().DrawNewTurn(hero1, hero2);

        battle.EndTurn();
    }
}