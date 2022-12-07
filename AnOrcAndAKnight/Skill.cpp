#include "Skill.h"

#include "Fighter.h"
#include "TempModifier.h"
#include "UITools.h"

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

TempModifier* Stun::Cast(Fighter& target)
{
    Skill::Cast(target);

    // miss ?
    int random = rand() % 100;
    if (_accuracy <= random) {
        string log = "\n\t\t" + target._name + " avoid the blow of his opponent weapon !";
        UITools::LogSummary(log);
        return nullptr;
    }

    // hit !
    string log = "\n\t\t" + target._name + " got stunned !";
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

TempModifier* Chaaaaaarge::Cast(Fighter& target) {
    Skill::Cast(target);

    // miss ?
    int random = rand() % 100;
    if (_accuracy <= random) {
        string log = "\n\t\t" + target._name + " miserably miss his charge...";
        UITools::LogSummary(log);
        return nullptr;
    }

    // damages doubled for 1 turn !
    string log = "\n\t\t" + target._name + " chaaaaaarges !!!!";
    UITools::LogSummary(log);

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

TempModifier* Skill::Cast(Fighter& target)
{
    _timer = _cooldown;
    return nullptr;
}

void Skill::EndTurn()
{
    _timer--;
}
