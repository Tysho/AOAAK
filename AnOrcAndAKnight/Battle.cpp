#include "Battle.h"

#include "Fighter.h"
#include "Skill.h"
#include "TempModifier.h"
#include "UITools.h" 
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
    string log = pTM->Affect();
    UITools::LogSummary(log);
    return pTM;
}

/// Fight !!!
void Battle::InitBattle()
{
    LOG("FIIIGHT !!!\n");
    _turn = 1;
}

void Battle::LogTurnCount()
{
    string strTurn = to_string(_turn);
    if (_turn < 10)
        strTurn = "0" + strTurn;

    strTurn = "\t------------------------\n\t\t------- TURN " + strTurn + " --------\n\t\t------------------------\n";
    UITools::LogHeader(strTurn);
}

/// <summary>
/// Play a turn and return the summary as string
/// </summary>
/// <returns></returns>
void Battle::PlayTurn()
{ 
    LogTurnCount();

    string summary;

    // get lists of available skills
    vector<Skill*>&& listOrcSkills = _orc.GetAvailableSkillsThisTurn();
    vector<Skill*>&& list_knightSkills = _knight.GetAvailableSkillsThisTurn();

    if (listOrcSkills.size() + list_knightSkills.size() > 0)
    {
        // don't remove brackets because two lines in "LOG"
        string log = ">-  SKILL PHASE\n";
        UITools::LogSummary(log);
    }

    // orc skills
    for (int i = 0; i < listOrcSkills.size(); i++) {
        Skill* pSkill = listOrcSkills[i];
        string log = _orc._name + " uses his skill \"" + pSkill->_name + "\"";
        UITools::LogSummary(log);

        if (pSkill->_target == TypeTarget::both || pSkill->_target == TypeTarget::opponent)
            TriggerEffect(pSkill, _knight);

        if (pSkill->_target == TypeTarget::both || pSkill->_target == TypeTarget::self)
            TriggerEffect(pSkill, _orc);

        summary += "\n";
    }

    // knight skills
    for (int i = 0; i < list_knightSkills.size(); i++) {
        Skill* pSkill = list_knightSkills[i];

        string log = _knight._name + " use his skill \"" + pSkill->_name + "\"";
        UITools::LogSummary(log);

        if (pSkill->_target == TypeTarget::both || pSkill->_target == TypeTarget::opponent)
            TriggerEffect(pSkill, _orc);

        if (pSkill->_target == TypeTarget::both || pSkill->_target == TypeTarget::self)
            TriggerEffect(pSkill, _knight);

        summary += "\n";
    }

    string log = ">-  BATTLE PHASE\n";
    UITools::LogSummary(log);

    // knight attacks
    if (_knight.IsStunned(summary) == false) {
        string log = _knight._name + " attacks \"" + _orc._name + "\" with his " + _knight._weapon._name + "!";
        UITools::LogSummary(log);
        _orc.RecieveDamages(_knight.GetDamages());
    }

    cout << "\n";

    // orc attacks
    if (_orc.IsStunned(summary) == false) {
        string log = _orc._name + " attacks \"" + _knight._name + "\" with his " + _orc._weapon._name + "!";
        UITools::LogSummary(log);
        _knight.RecieveDamages(_orc.GetDamages());
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
   UITools::LogSummary("\n");
}

bool Battle::IsOver()
{
    if (_knight._stats._currentHP <= 0)
        return true;
    if(_orc._stats._currentHP <= 0)
        return true;
    return false;
}

void Battle::DisplayScore() {

    if (_knight._stats._currentHP == 0) {
        LOG(_knight._name + ", sadly, didn't survive another day...\n");
    }

    if (_orc._stats._currentHP == 0) {
        LOG(_orc._name + ", won't kiss his familly tonight...\n");
    }

    LOG("THE END\n\n");
}