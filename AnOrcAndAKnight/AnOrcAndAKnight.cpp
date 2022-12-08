// AnOrcAndAKnight.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//

#include "Hero.h"
#include "Battle.h"
#include "Skill.h"
#include "UITools.h"
#include "ResourcesManager.h"

#include <conio.h>
#include <iostream>

using namespace std;

int main()
{
    // init random number generation seed with current time, so the fights won't end the same every time
    srand((unsigned int)time(0));

    // get languages resources
    vector<string>&& listLang = ResourcesManager::GetAvailableLanguages();
    ResourcesManager::LoadLanguage(listLang[0]);

    // create a knight
    Hero knight("Lord Belmesh", Weapon("long sword", 5), Stats(20, 50));
    Chaaaaaarge knightCharge;
    knight.AddSkill(&knightCharge);

    // create an orc to fight against
    Hero orc("Gromek", Weapon("axe", 8), Stats(60, 0));
    Stun orcStun;
    orc.AddSkill(&orcStun);

    // Allow user to edit his heroes
    int key = -1;
    cout << GetT("CUSTOMIZE_HEROES_YN");
    do {
        key = _getch();
        if (key == 224)
            key += _getch();

        // yes => display form to edit Heroes
        if (key == 121)
            UITools::LaunchEditForm(knight, orc);

    } while (key != 121 && key != 110);


    system("cls");

    // begin battle
    Battle battle(knight, orc);

    // list of all modifier currently applying on both Heroes
    while (1) {
        int key = _getch();

        // clicking on an arrow trigger _getch twice : first 224 (not interesting), we will look for the second one
        if (key == 224)
            key += _getch();

        // left-arrow (296) OR up-arrow (299) -> we get previous display from history
        if (key == 296 || key == 299) {
            UITools::DisplayPreviousTurn();
            continue;
        }

        // only allow 301 (rightArrow), 304 (downArrow), 32 (spacebar) and enter (13)
        if (key != 301 && key != 304 && key != 32 && key != 13)
            continue;

        // if next turn already computed (from history), we just display it
        if (UITools::DisplayNextTurn())
            continue;

        // we are on the end screen
        if (battle.IsOver())
        {
            system("cls");

            battle.DisplayScore();
            
            key = _getch();
            if (key == 224)
                key += _getch();

            // we can still visit history
            if (key == 296 || key == 298) {
                UITools::DisplayPreviousTurn(true);
                continue;
            }

            // if we continue now we are on the last screen, it's over
            if (key == 301 || key == 304 || key == 32 || key == 13)
                break;
        }

        battle.PlayTurn();

        // display result screen
        UITools::DrawTurn(knight, orc);

    }

    int a=_getch();
    system("cls");
    return 1;
}