#pragma once

#include "Hero.h"

class Hero;
class Effect;
class Hero;
class UIManager;
class Skill;

class Battle
{
public:
	Battle(Hero& knight, Hero& orc); 
	~Battle() {};

private:
	bool _gameOver = false;
	Hero& _hero1;
	Hero& _hero2;
	vector<Effect*> _listModifiers;

public:
	void PlayTurn();
	void EndTurn();
	bool DisplayScore();
	bool IsOver();

private:
	void InitBattle();
	bool UseSkill(Hero& user, Skill* pSkill, Hero& target);
};

