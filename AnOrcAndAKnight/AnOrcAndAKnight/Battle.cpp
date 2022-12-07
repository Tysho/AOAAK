#include "Battle.h"

#include "Fighter.h"
#include "Skill.h"
#include "TempModifier.h"

#include <iostream>

#define LOG(str) cout << "\n\t"; cout << str;

Battle::Battle(Fighter& knight, Fighter& orc) : _knight(knight), _orc(orc)
{
    InitBattle();
}


TempModifier* Battle::TriggerEffect(Skill* pSkill, Fighter& target)
{
    TempModifier* pTM = pSkill->Cast(target);

    if (pTM == nullptr)
        return pTM;

    _listModifiers.push_back(pTM);
    pTM->Affect();
    return pTM;
}

void Battle::InitBattle()
{
    // Fight !!!
    LOG("FIIIGHT !!!\n");
    _turn = 1;
}

void Battle::DisplayConsoleTurn()
{
    string strTurn = to_string(_turn);
    if (_turn < 10)
        strTurn = "0" + strTurn;

    LOG("\t------------------------\n\t\t------- TURN " + strTurn + " --------\n\t\t------------------------\n");
}

void Battle::PlayTurn()
{ 
    DisplayConsoleTurn();

    // get lists of available skills
    vector<Skill*>&& listOrcSkills = _orc.GetAvailableSkillsThisTurn();
    vector<Skill*>&& list_knightSkills = _knight.GetAvailableSkillsThisTurn();

    if (listOrcSkills.size() + list_knightSkills.size() > 0)
    {
        // don't remove brackets because two lines in "LOG"
        LOG(">-  SKILL PHASE\n");
    }

    // orc skills
    for (int i = 0; i < listOrcSkills.size(); i++) {
        Skill* pSkill = listOrcSkills[i];

        LOG(_orc._name + " uses his skill \"" + pSkill->_name + "\"");

        if (pSkill->_target == TypeTarget::both || pSkill->_target == TypeTarget::opponent)
            TriggerEffect(pSkill, _knight);

        if (pSkill->_target == TypeTarget::both || pSkill->_target == TypeTarget::self)
            TriggerEffect(pSkill,_orc);

        cout << "\n";
    }

    // knight skills
    for (int i = 0; i < list_knightSkills.size(); i++) {
        Skill* pSkill = list_knightSkills[i];

        LOG(_knight._name + " use his skill \"" + pSkill->_name + "\"");

        if (pSkill->_target == TypeTarget::both || pSkill->_target == TypeTarget::opponent)
            TriggerEffect(pSkill, _orc);

        if (pSkill->_target == TypeTarget::both || pSkill->_target == TypeTarget::self)
            TriggerEffect(pSkill, _knight);

        cout << "\n";
    }

    LOG(">-  BATTLE PHASE\n");

    // then, exchange blows
    if (_knight.IsStunned() == false) {
        LOG(_orc._name + " attacks \"" + _knight._name + "\" with his " + _orc._weapon._name + "!");
        _knight.RecieveDamages(_orc.GetDamages());
    }

    cout << "\n";

    if (_orc.IsStunned() == false) {
        LOG(_orc._name + " attacks \"" + _knight._name + "\" with his " + _knight._weapon._name + "!");
        _orc.RecieveDamages(_knight.GetDamages());
    }

    // then, effects update
    for (int i = 0; i < _listModifiers.size(); i++) {
        TempModifier* pTM = _listModifiers[i];

        // effect still apply
        if (pTM->Update() > 0)
            continue;

        // expired, we remove it from the list
        _listModifiers.erase(_listModifiers.begin() + i);
        delete pTM;
        pTM = nullptr;
        i--;
    }

    _knight.EndTurn();
    _orc.EndTurn();

   _turn++;
}

bool Battle::IsOver()
{
    if (_knight._stats._currentHP <= 0)
        return true;
    if(_orc._stats._currentHP <= 0)
        return true;
    return false;
}

