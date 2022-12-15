#include "Fighter.h"

#include "Skill.h"

#include <string>
#include <iostream>

using namespace std;

#define _SHIELD _stats._currentShield
#define _HP _stats._currentHP

unsigned int Fighter::GetDamages()
{
    if (_stun > 0) {
        string s = _name + " is stunned this turn !(" + to_string(_stun) + " turn(s) left)\n";
        cout << s;
        return 0;
    }
    return _weapon._damages;
}

Stats Fighter::InflictDamages(int damages)
{
    // no damages ? then nothing
    if (damages == 0)
        return _stats;

    string s;

    // shield ? -> damages to shield
    if (_SHIELD > 0) {
        _SHIELD -= damages;
        s = _name + " blocked the hit with his shield !";
        cout << s;

        // shield still ok
        if (_SHIELD > 0) {
            s = "\n\t(shield left : " + to_string(_SHIELD) + "/" + to_string(_stats._maxShield) + ")\n";
            cout << s;
            return _stats;
        }

        // shield destroyed, remainent damages will be inflicted to HP
        else {
            damages = -_SHIELD;
            _SHIELD = 0;
            s = "\n\tthe shield of " + _name + " just explode ! No more shield :(\n";
            cout << s;
        }
    }

    // no(more) shield -> damage to HP
    _HP -= damages;
    _HP = max(0, _HP);

    s = _name + " recieved " + to_string(damages) + " damages !!! (" + to_string(_HP) + "/" + to_string(_stats._maxHP) + ")\n";
    cout << s;

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
