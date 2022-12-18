#include "Settings.h"
#include "Skill.h"

#include "Effect.h"
#include "Hero.h"
#include "ResourcesManager.h"
#include "UIManager.h"

#define LOG(str) cout << "\n\t"; cout << str;

const char* Skill::GetName()
{ 
    return _name.c_str();
}

string Skill::GetResume()
{
    string resume = _name + " : ";
    string empty = " ", plain = " ";
    empty += char(176);
    plain += char(219);
    for (int k = 0; k < _cooldown - _timer; k++)
        resume += plain;
    for (int k = _cooldown - _timer; k < _cooldown; k++)
        resume += empty;

    return resume;
}

// static function declaration
Skill* Skill::CreateSkillInstanceById(int idSkill) {
    switch (idSkill) {
    case STUN:
        return new Stun;
    case CHARGE:
        return new Charge;
    case ARROW_KNEE:
        return new ArrowInTheKnee;
    }
    return nullptr;
}

#pragma region STUN

Stun::Stun() : Skill()
{
    _target = TypeTarget::opponent;
    _timer = 0;
    _cooldown = STUN_COOLDOWN;
    _accuracy = STUN_ACCURACY;
    _name = GetT("STUN");
};

bool Stun::Cast(Hero& target)
{
    Skill::Cast(target);

    // miss ?
    int random = rand() % 100;
    if (_accuracy <= random) {
        string log = "\t" + Format(GetT("MISS_STUN"), target._name.c_str());
        UI().LogSummary(log);
        return false;
    }

    // hit !
    string log = "\t" + Format(GetT("STUNNED"), target._name.c_str());
    UI().LogSummary(log);
    
    // stun is a specific effect directly handled by class Hero, no effect obj associated
    target.Stun(STUN_DURATION);
    return true;
}

#pragma endregion

#pragma region CHARGE

Charge::Charge() : Skill()
{
    _target = TypeTarget::self;
    _timer = 0;
    _cooldown = CHARGE_COOLDOWN;
    _accuracy = CHARGE_ACCURACY;
    _name = GetT("CHARGE");
};

bool Charge::Cast(Hero& target) 
{
    Skill::Cast(target);

    // miss ?
    int random = rand() % 100;
    if (_accuracy <= random) {
        string log = "\t" + Format(GetT("MISS_CHARGE"), target._name.c_str());
        UI().LogSummary(log);
        return false;
    }

    // damages doubled for 1 turn !
    string effectName = GetT("DAMAGES") + " x 2";
    DamageModifier* pChargeEffect = new DamageModifier(effectName, target, CHARGE_DURATION, CHARGE_DAMAGE_MULTIPLIER);
    string log = target.AddEffect(pChargeEffect);
    UI().LogSummary(log);

    return true;
}

#pragma endregion

#pragma region ARROW IN THE KNEE

ArrowInTheKnee::ArrowInTheKnee() : Skill()
{
    _target = TypeTarget::opponent;
    _timer = 0;
    _cooldown = ARROW_KNEE_COOLDOWN;
    _accuracy = ARROW_KNEE_ACCURACY;
    _name = GetT("ARROW_IN_THE_KNEE");
};

bool ArrowInTheKnee::Cast(Hero& target) 
{
    Skill::Cast(target);

    // miss ?
    int random = rand() % 100;
    if (_accuracy <= random) {
        string log = "\t" + Format(GetT("MISS_ARROW_IN_THE_KNEE"), target._name.c_str());
        UI().LogSummary(log);
        return false;
    }

    // damages / 2 for 3 turn !
    string name = GetT("DAMAGES") + " / 2";
    DamageModifier* pNoMoreAnAdventurer = new DamageModifier(name, target, ARROW_KNEE_DURATION, ARROW_KNEE_DAMAGE_MUTLIPLIER);
    string log = target.AddEffect(pNoMoreAnAdventurer);
    UI().LogSummary(log);
    return true;
}

#pragma endregion
