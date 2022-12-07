// AnOrcAndAKnight.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//

#include "Fighter.h"
#include "Battle.h"
#include "Skill.h"
#include "UITools.h"

#include <conio.h>
#include <iostream>

using namespace std;

int main()
{
    // init random number generation seed with current time, so the fights won't end the same every time
    srand((unsigned int)time(0));

    // create a knight
    Fighter knight("Lord Belmesh", Weapon("long sword", 5), Stats(20, 50));
    Chaaaaaarge knightCharge;
    knight.AddSkill(&knightCharge);

    // create an orc to fight against
    Fighter orc("Gromek", Weapon("axe", 8), Stats(60, 0));
    Stun orcStun;
    orcStun._accuracy = 100;
    orc.AddSkill(&orcStun);

    Battle battle(knight, orc);

    // list of all modifier currently applying on both fighters
    int turn = 1;
    do {
        int a = _getch();
        system("cls");   // Clear output for next display
        string summary = battle.PlayTurn();       
        UITools::DrawStats(knight, orc);
        cout << summary;
        turn++;
    } while (battle.IsOver() == false);

    int a = _getch();
    system("cls");   // Clear output for next display
    battle.DisplayScore();
}

// Exécuter le programme : Ctrl+F5 ou menu Déboguer > Exécuter sans débogage
// Déboguer le programme : F5 ou menu Déboguer > Démarrer le débogage

// Astuces pour bien démarrer : 
//   1. Utilisez la fenêtre Explorateur de solutions pour ajouter des fichiers et les gérer.
//   2. Utilisez la fenêtre Team Explorer pour vous connecter au contrôle de code source.
//   3. Utilisez la fenêtre Sortie pour voir la sortie de la génération et d'autres messages.
//   4. Utilisez la fenêtre Liste d'erreurs pour voir les erreurs.
//   5. Accédez à Projet > Ajouter un nouvel élément pour créer des fichiers de code, ou à Projet > Ajouter un élément existant pour ajouter des fichiers de code existants au projet.
//   6. Pour rouvrir ce projet plus tard, accédez à Fichier > Ouvrir > Projet et sélectionnez le fichier .sln.
