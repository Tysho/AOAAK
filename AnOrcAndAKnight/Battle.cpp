#include "Battle.h"

#include "Hero.h"
#include "Skill.h"
#include "TempModifier.h"
#include "UIManager.h"
#include "ResourcesManager.h"

Battle::Battle(Hero& hero1, Hero& hero2) : _hero1(hero1), _hero2(hero2)
{
    InitBattle(); 
}

// trigger an effect of a skill to a target, log in summary what is happening and return the temporary modifier if any
TempModifier* Battle::TriggerEffect(Skill* pSkill, Hero& target)
{
    TempModifier* pTM = pSkill->Cast(target);

    if (pTM == nullptr)
        return pTM;

    _listModifiers.push_back(pTM);
    string log = pTM->Affect();
    UIManager::GetInstance().LogSummary(log);
    return pTM;
}

/// Fight !!!
void Battle::InitBattle()
{
    UIManager::GetInstance().DisplayBattleStart(_hero1, _hero2);
}


// Play a turn and prepare the summary as string
void Battle::PlayTurn()
{ 
    // get lists of available skills
    vector<Skill*>&& listSkills1 = _hero1.GetAvailableSkillsThisTurn();
    vector<Skill*>&& listSkills2 = _hero2.GetAvailableSkillsThisTurn();

    if (listSkills1.size() + listSkills2.size() > 0)
    {
        // don't remove brackets because two lines in "LOG"
        string log = GetT("SKILL_PHASE");
        UIManager::GetInstance().LogSummary(log);
    }

    // orc skills
    for (Skill* pSkill : listSkills1) {

        string log = Format(GetT("USES_SKILL"), _hero1.GetName(), pSkill->GetName());
        UIManager::GetInstance().LogSummary(log);

        if (pSkill->_target == TypeTarget::both || pSkill->_target == TypeTarget::opponent)
            TriggerEffect(pSkill, _hero2);

        if (pSkill->_target == TypeTarget::both || pSkill->_target == TypeTarget::self)
            TriggerEffect(pSkill, _hero1);
    }

    // knight skills
    for (Skill* pSkill : listSkills2) {

        string log = Format(GetT("USES_SKILL"), _hero2.GetName(), pSkill->GetName());
        UIManager::GetInstance().LogSummary(log);

        if (pSkill->_target == TypeTarget::both || pSkill->_target == TypeTarget::opponent)
            TriggerEffect(pSkill, _hero1);

        if (pSkill->_target == TypeTarget::both || pSkill->_target == TypeTarget::self)
            TriggerEffect(pSkill, _hero2);
    }

    string log = GetT("BATTLE_PHASE");
    UIManager::GetInstance().LogSummary(log);

    // hero1 attacks
    if (_hero1._stun > 0) {
        log = Format(GetT("STUNNED_THIS_TURN"), _hero1.GetName(), _hero1._stun);
        UIManager::GetInstance().LogSummary(log);
    }
    else {
        // write attack line in summary
        log = Format(GetT("ATTACK"), _hero1.GetName(), _hero2.GetName(), _hero1.GetWeaponName());
        UIManager::GetInstance().LogSummary(log);

        log = _hero2.RecieveDamages(_hero1.GetDamages());
        UIManager::GetInstance().LogSummary(log + "\n");
    }

    // orc attacks
    if (_hero2._stun > 0) {
        log = Format(GetT("STUNNED_THIS_TURN"), _hero2.GetName(), _hero2._stun);
        UIManager::GetInstance().LogSummary(log + "\n");
    }
    else {
        string log = Format(GetT("ATTACK"), _hero2.GetName(), _hero1.GetName(), _hero2.GetWeaponName());
        UIManager::GetInstance().LogSummary(log);
        log = _hero1.RecieveDamages(_hero2.GetDamages());
        UIManager::GetInstance().LogSummary(log + "\n");
    }

    // then, effects update
    for (int i = 0; i < _listModifiers.size(); i++) {
        TempModifier* pTM = _listModifiers[i];

        // effect still apply
        if (pTM->Update() > 0)
            continue;

        // expired, we remove it from the list
        _listModifiers.erase(_listModifiers.begin() + i);
        delete pTM;
        pTM = nullptr;
        i--;
    }

    _hero1.EndTurn();
    _hero2.EndTurn();

   UIManager::GetInstance().LogSummary("\n");
}

bool Battle::IsOver()
{
    if (_hero1._stats._currentHP <= 0 || _hero2._stats._currentHP <= 0)
        return true;
    return false;
}

bool Battle::DisplayScore() 
{
    if (_gameOver)
        return false;
    _gameOver = true;

    if (_hero1._stats._currentHP == 0) 
        UIManager::GetInstance().LogSummary(_hero1._gameOver);

    if (_hero2._stats._currentHP == 0)
        UIManager::GetInstance().LogSummary(_hero2._gameOver);

    UIManager::GetInstance().LogSummary("\n\n\t\t" + GetT("END"));
    UIManager::GetInstance().DisplayBattleEnd();
    return true;
}