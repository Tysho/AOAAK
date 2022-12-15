#include "Skill.h"

#include "Fighter.h"
#include "TempModifier.h"

#include <string>
#include <iostream>

Stun::Stun() : Skill()
{
    _target = TypeTarget::opponent;
    _timer = 0;
    _cooldown = 5;
    _accuracy = 20;
    _name = "Stun";
};

TempModifier* Stun::Trigger(Fighter& target)
{
    Skill::Trigger(target);

    // miss ?
    int random = rand() % 100;
    if (_accuracy <= random) {
        string s = "\t" + target._name + " avoid the blow of his opponent weapon !\n";
        cout << s;
        return nullptr;
    }

    string s = target._name + " got stunned !\n";
    cout << s;
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

TempModifier* Chaaaaaarge::Trigger(Fighter& target) {
    Skill::Trigger(target);

    // miss ?
    int random = rand() % 100;
    if (_accuracy <= random) {
        string s = "\t" + target._name + " miserably miss his charge...\n";
        cout << s;
        return nullptr;
    }

    // damages doubled for 1 turn !
    string s = target._name + " chaaaaaarges !!!!\n";
    cout << s;
    DamageModifier* pDM = new DamageModifier(target, 1, target._weapon._damages);
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

TempModifier* Skill::Trigger(Fighter& target)
{
    _timer = _cooldown;
    return nullptr;
}

void Skill::EndTurn()
{
    _timer--;
}
