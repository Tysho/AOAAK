#pragma once
#include "Fighter.h"
#include "Skill.h"
#include "TempModifier.h"

#include <iostream>

class Fighter;

class Battle
{
public:
	Battle(Fighter& knight, Fighter& orc); 
	~Battle() {};

private:
	int _turn = 0;
	Fighter& _knight;
	Fighter& _orc;

	vector<TempModifier*> _listModifiers;
public:
	void DisplayScore();
	bool IsOver();
	string PlayTurn();
	TempModifier* TriggerEffect(Skill* pSkill, Fighter& target, string& summary);

private:
	void InitBattle();
	void LogTurnCount();
};

