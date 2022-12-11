#include "Skill.h"

#include "Hero.h"
#include "TempModifier.h"
#include "UIManager.h"
#include "ResourcesManager.h"

#define LOG(str) cout << "\n\t"; cout << str;

#pragma region STUN

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
        UIManager::GetInstance().LogSummary(log);
        return nullptr;
    }

    // hit !
    string log = "\t" + Format(GetT("STUNNED"), target._name.c_str());
    UIManager::GetInstance().LogSummary(log);
    target.Stun(1);
    return nullptr;
}

#pragma endregion

#pragma region CHARGE

Charge::Charge() : Skill()
{
    _target = TypeTarget::self;
    _timer = 0;
    _cooldown = 3;
    _accuracy = 50;
    _name = "Charge !!!";
};

TempModifier* Charge::Cast(Hero& target) 
{
    Skill::Cast(target);

    // miss ?
    int random = rand() % 100;
    if (_accuracy <= random) {
        string log = "\t" + Format(GetT("MISS_CHARGE"), target._name.c_str());
        UIManager::GetInstance().LogSummary(log);
        return nullptr;
    }

    // damages doubled for 1 turn !
    int bonusDamages = target.GetDamages();

    DamageModifier* pDM = new DamageModifier(_name, target, 1, bonusDamages);
    return pDM;
}

#pragma endregion

#pragma region ARROW IN THE KNEE

ArrowInTheKnee::ArrowInTheKnee() : Skill()
{
    _target = TypeTarget::opponent;
    _timer = 0;
    _cooldown = 5;
    _accuracy = 35;
    _name = "Arrow in the knee !";
};

TempModifier* ArrowInTheKnee::Cast(Hero& target) 
{
    Skill::Cast(target);

    // miss ?
    int random = rand() % 100;
    if (_accuracy <= random) {
        string log = "\t" + Format(GetT("MISS_ARROW_IN_THE_KNEE"), target._name.c_str());
        UIManager::GetInstance().LogSummary(log);
        return nullptr;
    }

    // damages / 2 for 3 turn !
    int bonusDamages = int(-0.5 * target.GetDamages());

    DamageModifier* pDM = new DamageModifier(_name, target, 3, bonusDamages);
    return pDM;
}

#pragma endregion

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