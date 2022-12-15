// AnOrcAndAKnight.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//
#include "Fighter.h"
#include "Skill.h"
#include "TempModifier.h"

#include <iostream>

vector<TempModifier*> listModifiers;

TempModifier* TriggerEffect(Skill* pSkill, Fighter& target) 
{
    TempModifier* pTM = pSkill->Trigger(target);

    if (pTM == nullptr)
        return pTM;

    listModifiers.push_back(pTM);
    pTM->Affect();
    return pTM;
};


int main()
{
    cout << "Hello World!\n";

    // create a knight
    Fighter knight("Lord Belmesh", Weapon("long sword", 5), Stats(20, 50));
    Chaaaaaarge knightCharge;
    knight.AddSkill(&knightCharge);

    // create an orc to fight against
    Fighter orc("Gromek", Weapon("axe", 8), Stats(60, 0));
    Stun orcStun;
    orc.AddSkill(&orcStun);

    // Fight !!!
    cout << "\n\n\tFIIIGHT\n";

    // list of all modifier currently applying on both fighters
    int turnCnt = 1;
    do {
        cout << "\t--------------\n";
        cout << "\t--- TURN " + to_string(turnCnt) + " ---\n";
        cout << "\t--------------\n\n";

        // first, apply skill if possible
        vector<Skill*>&& listOrcSkills = orc.GetAvailableSkillsThisTurn();
        vector<Skill*>&& listKnightSkills = knight.GetAvailableSkillsThisTurn();

        if (listOrcSkills.size() + listKnightSkills.size() > 0) 
            cout << "\n----------  SKILL PHASE  ----------\n\n";

        // orc skills
        for (int i = 0; i < listOrcSkills.size(); i++) {
            Skill* pSkill = listOrcSkills[i];

            cout << orc._name + " use his skill \"" + pSkill->_name +"\"\n";

            if (pSkill->_target == TypeTarget::both || pSkill->_target == TypeTarget::opponent)
                TriggerEffect(pSkill, knight);

            if (pSkill->_target == TypeTarget::both || pSkill->_target == TypeTarget::self)
                TriggerEffect(pSkill, orc);

            cout << "\n\n";
        }

        // knight skills
        for (int i = 0; i < listKnightSkills.size(); i++) {
            Skill* pSkill = listKnightSkills[i];

            cout << orc._name + " use his skill \"" + pSkill->_name + "\"\n";

            if (pSkill->_target == TypeTarget::both || pSkill->_target == TypeTarget::opponent)
                TriggerEffect(pSkill, orc);

            if (pSkill->_target == TypeTarget::both || pSkill->_target == TypeTarget::self)
                TriggerEffect(pSkill, knight);

            cout << "\n";
        }

        knight.EndTurn();
        orc.EndTurn();

        cout << "\n----------  BATTLE PHASE  ---------\n\n";

        // then, effects update
        for (int i = 0; i < listModifiers.size(); i++) {
            TempModifier* pTM = listModifiers[i];

            // effect still apply
            if (pTM->Update() > 0)
                continue;

            // expired, we remove it from the list
            listModifiers.erase(listModifiers.begin() + i);
            delete pTM;
            pTM = nullptr;
            i--;
        }

        // then, exchange blows
        knight.InflictDamages(orc.GetDamages());
        orc.InflictDamages(knight.GetDamages());
        
        turnCnt++;
        cout << "\n";
    } while (knight._stats._currentHP > 0 && orc._stats._currentHP > 0);
    
    if (knight._stats._currentHP == 0) 
        cout << knight._name + ", sadly, didn't survive another day...\n";
    
    if (orc._stats._currentHP == 0) 
        cout << orc._name + ", won't kiss his familly tonight...\n";
    
    cout << "\n\nTHE END";
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
