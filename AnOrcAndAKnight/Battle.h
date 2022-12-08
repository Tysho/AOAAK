#pragma once
#include "Hero.h"
#include "Skill.h"
#include "TempModifier.h"

#include <iostream>

class Hero;

class Battle
{
public:
	Battle(Hero& knight, Hero& orc); 
	~Battle() {};

private:
	int _turn = 0;
	Hero& _knight;
	Hero& _orc;

	vector<TempModifier*> _listModifiers;
public:
	void DisplayScore();
	bool IsOver();
	void PlayTurn();
	TempModifier* TriggerEffect(Skill* pSkill, Hero& target);

private:
	void InitBattle();
	void LogTurnCount();
};

