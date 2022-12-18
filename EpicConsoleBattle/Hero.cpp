#include "Hero.h"
#include "Settings.h"

#include "Effect.h"
#include "Skill.h"
#include "UIManager.h"
#include "ResourcesManager.h"

#define LOG(str) cout << "\n\t"; cout << str;

using namespace std;

#define _SHIELD _stats._currentShield
#define _HP _stats._currentHP

#pragma region CLASS HEROCLASS

vector<HeroClass> HeroClass::_listClasses = vector<HeroClass>();

HeroClass::HeroClass(const string& name, vector<int> listSkills, const string& description) :
    _name(name),
    _description(description)
{
    for (int skillId : listSkills)
        _listSkillsId.push_back(skillId);
};

void HeroClass::InitClasses()
{
    _listClasses = {
        HeroClass(GetT("KNIGHT"), { CHARGE }, GetT("KNIGHT_DESCRIPTION")),
        HeroClass(GetT("ORC"),{ STUN }, GetT("ORC_DESCRIPTION")),
        HeroClass(GetT("ELF"),{ ARROW_KNEE }, GetT("ELF_DESCRIPTION"))
    };
}

HeroClass& HeroClass::operator=(const HeroClass& other)
{
    _name = other._name;
    _listSkillsId = other._listSkillsId;

    return *this;
}

#pragma endregion 


#pragma region CLASS HERO

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

Hero& Hero::operator=(Hero& other)
{
    _class = other._class;
    _name = other._name;
    _weapon = other._weapon;
    _stats = other._stats;				// hp&shield, current and max values
    _stun = other._stun;
    _listSkills = other._listSkills;
    _isLeft = other._isLeft;
    return *this;
}

// update shield and hp according to damage recieved, return summary
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

    if (summary != "")
        summary += "\n\t";

    string format = GetT("TAKES_DAMAGES");
    summary += Format(format, _name.c_str(), damages);

    return summary.c_str();
}


// return all skills for which the timer is to 0 (ready to be cast)
vector<Skill*> Hero::GetAvailableSkillsThisTurn()
{
    vector<Skill*> ret;

    for (Skill* skill : _listSkills) {
        if (skill->_timer == 0)
            ret.push_back(skill);
    }

    return ret;
}


// End turn of the Hero : update skills cooldown and update effects, removing expired ones 
void Hero::EndTurn()
{
    _stun = max(_stun - 1, 0);
    for (Skill* pSkill : _listSkills)
        pSkill->EndTurn();

    vector<Effect*> listExpired;
    for (Effect* pEffect : _listEffects) {
        if (pEffect->Update() > 0)
            continue;

        // expired
        pEffect->Expire();
    }

    // remove expired effects
    auto expired = [](Effect* pE) {return pE->GetDurationLeft() == 0; };
    _listEffects.erase(std::remove_if(_listEffects.begin(), _listEffects.end(), expired), _listEffects.end());
}

void Hero::EquipWeapon(const Weapon weapon)
{
    _weapon._name = weapon._name;
    _weapon._damages = weapon._damages;
}


// stun the hero if not already stunned for a greater duration
void Hero::Stun(int duration)
{
    if (_stun < duration)
        _stun = duration;
}


// get damages dealt by the hero after all damages modifers are applied
int Hero::GetDamages() {
    // base damages 
    int damages = _weapon._damages;

    // all multipliers currently affecting the hero
    for (Effect* pEffect : _listEffects)
        damages = (int)ceil(pEffect->GetDamageMultiplier() * damages);

    return damages;
}

// Return number of effect affecting the hero (stun + effects)
int Hero::GetNbEffects()
{ 
    int nbEffects = _stun > 0 ? 1 : 0;
    nbEffects += (int)_listEffects.size();
    return nbEffects;
}


// add and effect to the hero, and trigger it 
string Hero::AddEffect(Effect* pEffect)
{
    _listEffects.push_back(pEffect);
    return pEffect->Affect();
}


// change the class of the hero, update all impacted values
void Hero::SetClass(const HeroClass& heroClass)
{
    _class = heroClass;

    // remove old skills
    for (Skill* pSkill : _listSkills) {
        if(pSkill)
            delete pSkill;
    }
    _listSkills.clear();

    // add new skills
    for (int skillId : _class._listSkillsId) {
        Skill* pSkill = Skill::CreateSkillInstanceById(skillId);
        _listSkills.push_back(pSkill);
    }
}


// return the text of an effect applied to the Hero with the remaining duration
string Hero::GetEffectResume(int i) 
{
    // Stun effect
    if (_stun > 0 && i == 0) {
        string result = GetT("STUN") + " : ";
        string turn = " ";
        turn += char(219);
        for (int i = 0; i < _stun; i++)
            result += turn;
        return result;
    }

    // taking account that stun was displayed with i=0
    if (_stun > 0)
        i--;

    if (i >= (int)_listEffects.size())
        return "";

    string result = _listEffects[i]->GetResume();
    return result;
}

string Hero::GetSkillResume(int i)
{
    if (i >= (int)_listSkills.size())
        return "";

    string result = _listSkills[i]->GetResume();
    return result;
}

#pragma endregion 