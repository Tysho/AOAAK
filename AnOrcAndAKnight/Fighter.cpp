#include "Fighter.h"

#include "Skill.h"
#include "UITools.h"

#include <string>
#include <iostream>

#define LOG(str) cout << "\n\t"; cout << str;

using namespace std;

#define _SHIELD _stats._currentShield
#define _HP _stats._currentHP

unsigned int Fighter::GetDamages()
{
    if (_stun > 0) {
        return 0;
    }
    return _weapon._damages;
}

Stats Fighter::RecieveDamages(int damages)
{
    // no damages ? then nothing
    if (damages == 0)
        return _stats;
    
    // shield ? -> damages to shield
    if (_SHIELD > 0) {
        _SHIELD -= damages;
        string text = "\t" + _name + " blocked the hit with his shield !";

        // shield still ok
        if (_SHIELD > 0) {
            text += "\t(shield left : " + to_string(_SHIELD) + "/" + to_string(_stats._maxShield) + ")";
            UITools::LogSummary(text);
            return _stats;
        }

        // shield destroyed, remainent damages will be inflicted to HP
        damages = -_SHIELD;
        _SHIELD = 0;
        text += "\n\t\tThe shield of " + _name + " just explode ! No more shield :(";
        UITools::LogSummary(text);
    }

    // no(more) shield -> damage to HP
    _HP -= damages;
    _HP = max(0, _HP);
  
    string log = "\t" + _name + " recieved " + to_string(damages) + " damages !!!\t(" + to_string(_HP) + "/" + to_string(_stats._maxHP) + ")";
    UITools::LogSummary(log);

    return _stats;
}

vector<Skill*> Fighter::GetAvailableSkillsThisTurn()
{
    vector<Skill*> ret;

    for (int i = 0; i < _listSkills.size(); i++) {
        Skill* pSkill = _listSkills[i];

        if (pSkill->_timer == 0)
            ret.push_back(_listSkills[i]);
    }

    return ret;
}

void Fighter::EndTurn()
{
    _stun--;
    for (int i = 0; i < _listSkills.size(); i++) {
        Skill* pSkill = _listSkills[i];
        pSkill->EndTurn();
    }
}

void Fighter::Stun(int duration)
{
    if (_stun < duration)
        _stun = duration;
}

void Fighter::AddSkill(Skill* pSkill)
{
    if (pSkill != nullptr)
        _listSkills.push_back(pSkill);
}

bool Fighter::IsDead()
{
    if (_stats._currentHP <= 0)
        return true;
    return false;
}

bool Fighter::IsStunned(string& summary) 
{
    if (_stun > 0) {
        summary += "\n\t" + _name + " is stunned this turn !(" + to_string(_stun) + " turn(s) left)";
        return true;
    }
    return false;
}