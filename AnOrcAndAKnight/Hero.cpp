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

vector<HeroClass> HeroClass::_listClasses = {
    HeroClass("Knight", "Sword", true, {CHARGE}),
    HeroClass("Orc", "Axe", false, {STUN}),
    HeroClass("Elf", "Longbow", false, {ARROW_KNEE})
};

HeroClass::HeroClass(const string& name, const string& weapon, bool shield, vector<int> listSkills) :
    _name(name),
    _weapon(weapon),
    _hasShield(shield)
{
    for (int skillId : listSkills)
        _listSkillsId.push_back(skillId);
};


HeroClass& HeroClass::operator=(const HeroClass& other)
{
    _name = other._name;
    _weapon = other._weapon;
    _hasShield = other._hasShield;
    _listSkillsId = other._listSkillsId;

    return *this;
}


unsigned int Hero::GetDamages()
{
    return _stats._damages;
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

        // shield destroyed
        damages = -_SHIELD;
        _SHIELD = 0;
        summary += "\n\t\t\t" + Format(GetT("SHIELD_DESTROYED"), _name.c_str());
        return summary.c_str();
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

Hero::Hero(const HeroClass& heroClass, const string& name, const Stats& stats) :
    _name(name),
    _stats(stats)
{
    _class = heroClass;
    for (int skillId : _class._listSkillsId) {
        Skill* pSkill = Skill::CreateSkillInstanceById(skillId);
        _listSkills.push_back(pSkill);
    }
}

Hero::~Hero()
{
    for (Skill* pSkill : _listSkills) {
        if (pSkill)
            delete pSkill;
    }
}

Hero& Hero::operator=(const Hero& other)
{
    _class = other._class;
    _name = other._name;
    _gameOver = other._gameOver;
    _stats = other._stats;				// hp&shield, current and max values
    _stun = other._stun;
    _listSkills = other._listSkills;
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

bool Hero::IsDead()
{
    if (_stats._currentHP <= 0)
        return true;
    return false;
}