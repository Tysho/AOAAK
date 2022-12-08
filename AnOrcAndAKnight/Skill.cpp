#include "Skill.h"

#include "Hero.h"
#include "TempModifier.h"
#include "UITools.h"
#include "ResourcesManager.h"

#include <string>
#include <iostream>

#define LOG(str) cout << "\n\t"; cout << str;

Stun::Stun() : Skill()
{
    _target = TypeTarget::opponent;
    _timer = 0;
    _cooldown = 5;
    _accuracy = 20;
    _name = "Stun";
};

TempModifier* Stun::Cast(Hero& target)
{
    Skill::Cast(target);

    // miss ?
    int random = rand() % 100;
    if (_accuracy <= random) {
        string log = "\t" + Format(GetT("MISS_STUN"), target._name.c_str());
        UITools::LogSummary(log);
        return nullptr;
    }

    // hit !
    string log = "\t" + Format(GetT("STUNNED"), target._name.c_str());
    UITools::LogSummary(log);
    target.Stun(1);
    return nullptr;
}

Chaaaaaarge::Chaaaaaarge() : Skill() {
    _target = TypeTarget::self;
    _timer = 0;
    _cooldown = 3;
    _accuracy = 50;
    _name = "Chaaaaaarge !!!";
};

TempModifier* Chaaaaaarge::Cast(Hero& target) {
    Skill::Cast(target);

    // miss ?
    int random = rand() % 100;
    if (_accuracy <= random) {
        string log = "\t" + Format(GetT("MISS_CHARGE"), target._name.c_str());
        UITools::LogSummary(log);
        return nullptr;
    }

    // damages doubled for 1 turn !
    int bonusDamages = target._weapon._damages;

    DamageModifier* pDM = new DamageModifier(target, 1, bonusDamages);
    return pDM;
}

Skill::Skill() 
{
    _name = "UNNAMED_SKILL";
    _target = TypeTarget::opponent;
    _cooldown = 5;
    _timer = 0;
    _accuracy = 50;
};

TempModifier* Skill::Cast(Hero& target)
{
    _timer = _cooldown;
    return nullptr;
}

void Skill::EndTurn()
{
    _timer--;
}
