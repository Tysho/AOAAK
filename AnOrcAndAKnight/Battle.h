#pragma once

#include "Hero.h"
#include "Skill.h"
#include "TempModifier.h"

#include <iostream>

class Hero;
class UIManager;

class Battle
{
public:
	Battle(Hero& knight, Hero& orc); 
	~Battle() {};

private:
	bool _gameOver = false;
	Hero& _hero1;
	Hero& _hero2;
	vector<TempModifier*> _listModifiers;

public:
	void PlayTurn();
	bool DisplayScore();
	bool IsOver();

private:
	void InitBattle();
	TempModifier* TriggerEffect(Skill* pSkill, Hero& target);
};

