#include "Hero.h"

#include "Skill.h"
#include "UITools.h"
#include "ResourcesManager.h"
#include <string>
#include <iostream>

#define LOG(str) cout << "\n\t"; cout << str;

using namespace std;

#define _SHIELD _stats._currentShield
#define _HP _stats._currentHP

unsigned int Hero::GetDamages()
{
    if (_stun > 0) {
        return 0;
    }
    return _weapon._damages;
}

Stats Hero::RecieveDamages(int damages)
{
    // no damages ? then nothing
    if (damages == 0)
        return _stats;
    
    // shield ? -> damages to shield
    if (_SHIELD > 0) {
        _SHIELD -= damages;
        string text = Format(GetT("SHIELD_BLOCK"), _name.c_str());

        // shield still ok
        if (_SHIELD > 0) {
            UITools::LogSummary(text);
            return _stats;
        }

        // shield destroyed, remainent damages will be inflicted to HP
        damages = -_SHIELD;
        _SHIELD = 0;
        text += "\n\t\t\t" + Format(GetT("SHIELD_DESTROYED"), _name.c_str());
        UITools::LogSummary(text);
    }

    // no(more) shield -> damage to HP
    _HP -= damages;
    _HP = max(0, _HP);
  
    string format = GetT("TAKES_DAMAGES");
    string log = Format(format, _name.c_str(), damages);
    UITools::LogSummary(log);

    return _stats;
}

vector<Skill*> Hero::GetAvailableSkillsThisTurn()
{
    vector<Skill*> ret;

    for (int i = 0; i < _listSkills.size(); i++) {
        Skill* pSkill = _listSkills[i];

        if (pSkill->_timer == 0)
            ret.push_back(_listSkills[i]);
    }

    return ret;
}

void Hero::EndTurn()
{
    _stun--;
    for (int i = 0; i < _listSkills.size(); i++) {
        Skill* pSkill = _listSkills[i];
        pSkill->EndTurn();
    }
}

void Hero::Stun(int duration)
{
    if (_stun < duration)
        _stun = duration;
}

void Hero::AddSkill(Skill* pSkill)
{
    if (pSkill != nullptr)
        _listSkills.push_back(pSkill);
}

bool Hero::IsDead()
{
    if (_stats._currentHP <= 0)
        return true;
    return false;
}

bool Hero::IsStunned(string& summary) 
{
    if (_stun > 0) {
        summary += "\n\t" + Format(GetT("STUNNED_THIS_TURN"), _name, _stun);
        return true;
    }
    return false;
}