#include "Battle.h"
#include "Settings.h"

#include "Hero.h"
#include "Skill.h"
#include "Effect.h"
#include "UIManager.h"
#include "ResourcesManager.h"

Battle::Battle(Hero& hero1, Hero& hero2) : _hero1(hero1), _hero2(hero2)
{
    InitBattle(); 
}

// trigger an effect of a skill to a target, log in summary what is happening and return the temporary modifier if any
bool Battle::UseSkill(Hero& user, Skill* pSkill, Hero& target)
{
    string log;
    // skill on hiself)
    if (user._isLeft == target._isLeft)
        log = Format(GetT("USES_SKILL"), user.GetName(), pSkill->GetName());
    else 
        log = Format(GetT("USES_SKILL_ON"), user.GetName(), pSkill->GetName(), target.GetName());
    UI().LogSummary(log);
    return pSkill->Cast(target);
}

/// Fight !!!
void Battle::InitBattle()
{
    _hero1._isLeft = true;
    UI().DisplayBattleStart(_hero1, _hero2);
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
        UI().LogSummary(log);
    }

    // orc skills
    for (Skill* pSkill : listSkills1) {
        if (pSkill->_target == TypeTarget::both || pSkill->_target == TypeTarget::opponent)
            UseSkill(_hero1, pSkill, _hero2);

        if (pSkill->_target == TypeTarget::both || pSkill->_target == TypeTarget::self)
            UseSkill(_hero1, pSkill, _hero1);
    }
    if (listSkills1.size() > 0)
        UI().LogSummary(""); // add Empty line

    // knight skills
    for (Skill* pSkill : listSkills2) {
        if (pSkill->_target == TypeTarget::both || pSkill->_target == TypeTarget::opponent)
            UseSkill(_hero2, pSkill, _hero1);

        if (pSkill->_target == TypeTarget::both || pSkill->_target == TypeTarget::self)
            UseSkill(_hero2, pSkill, _hero2);
    }

    string log = GetT("BATTLE_PHASE");
    UI().LogSummary(log);

    // hero1 attacks
    if (_hero1._stun > 0) {
        log = Format(GetT("STUNNED_THIS_TURN"), _hero1.GetName());
        UI().LogSummary(log);
    }
    else {
        // write attack line in summary
        log = Format(GetT("ATTACK"), _hero1.GetName(), _hero2.GetName(), _hero1.GetWeaponName());
        UI().LogSummary(log);

        log = _hero2.RecieveDamages(_hero1.GetDamages());
        UI().LogSummary(log + "\n");
    }

    // orc attacks
    if (_hero2._stun > 0) {
        log = Format(GetT("STUNNED_THIS_TURN"), _hero2.GetName());
        UI().LogSummary(log);
    }
    else {
        string log = Format(GetT("ATTACK"), _hero2.GetName(), _hero1.GetName(), _hero2.GetWeaponName());
        UI().LogSummary(log);
        log = _hero1.RecieveDamages(_hero2.GetDamages());
        UI().LogSummary(log + "\n");
    }

    // update effect (could apply effect)
    _hero1.UpdateEffects();
    _hero2.UpdateEffects();

    UI().LogSummary("\n");
}

void Battle::EndTurn() 
{
    _hero1.EndTurn();
    _hero2.EndTurn();
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
    UI().PrepareGameOverScreen(_hero1, _hero2);
    UI().DisplayBattleEnd();
    return true;
}