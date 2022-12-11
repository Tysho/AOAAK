#include "Hero.h"

#include "Skill.h"
#include "UIManager.h"
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

string Hero::RecieveDamages(int damages)
{
    // no damages ? then nothing
    if (damages == 0)
        return "";

    string summary = "";

    // shield ? -> damages to shield
    if (_SHIELD > 0) {
        _SHIELD -= damages;
        summary = Format(GetT("SHIELD_BLOCK"), _name.c_str());

        // shield still ok
        if (_SHIELD > 0)
            return summary.c_str();

        // shield destroyed, remainent damages will be inflicted to HP
        damages = -_SHIELD;
        _SHIELD = 0;
        summary += "\n\t\t\t" + Format(GetT("SHIELD_DESTROYED"), _name.c_str());
    }

    // no(more) shield -> damage to HP
    _HP -= damages;
    _HP = max(0, _HP);
  
    string format = GetT("TAKES_DAMAGES");
    summary += "\n\t" + Format(format, _name.c_str(), damages);

    return summary.c_str();
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

Hero::Hero()
{
    _class = "Hero";
    _name = "The One";
    _weapon = Weapon();
    _stats = Stats();
    _gameOver = "R.I.P The One...";
}

Hero::Hero(const string& className, const string& name, const Weapon& weapon, const Stats& stats) :
    _class(className),
    _name(name),
    _weapon(weapon),
    _stats(stats) 
{ }

Hero::~Hero()
{
    for (Skill* pSkill : _listSkills)
        delete pSkill;
}

Hero& Hero::operator=(const Hero& other)
{
    _class = other._class;
    _weapon = other._weapon;
    _name = other._name;
    _gameOver = other._gameOver;
    _stats = other._stats;				// hp&shield, current and max values
    _stun = other._stun;

    return *this;
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
