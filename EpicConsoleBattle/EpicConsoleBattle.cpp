// EpicConsoleBattle.cpp : (no .h) Contain 'main' function, where the program begin and close
//

#include "Settings.h"
#include "Hero.h"

#include "Battle.h"
#include "Skill.h"
#include "UIManager.h"
#include "ResourcesManager.h"


using namespace std;

// Get resources from csv files in Setting folder
void Init() {
    // init random number generation seed with current time, so the fights won't end the same every time
    srand((unsigned int)time(0));
    ResourcesManager::LoadLanguages();

    // load languages resources from Language.csv file
    if (ResourcesManager::LoadLanguages() == false) {
        system("pause");
        exit(1);
    }

    HeroClass::InitClasses();

    // get heroes saved
    if(ResourcesManager::LoadHeroes() == false) {
        system("pause");
        exit(1);
    }

    UI().SetupConsoleSize();
    UI().SelectLanguage();
}


// Edit/select hero
void SelectHeroes(Hero& hero1, Hero& hero2) 
{
    UI().SelectHero(hero1, 1);

    UI().SelectHero(hero2, 2, hero1._name, hero1.GetClass());
}


// a battle, from hero selection to the game over screen
void PlayBattle()
{
    // creation/selection of the 2 challengers
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
                return;

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


// main function of program
int main()
{
    Init();

    while (1) {
        PlayBattle();

        if (UI().SelectRestartOrQuit() == false)
            exit(1);

        // restart random seed
        srand((unsigned int)time(0));
    }
}