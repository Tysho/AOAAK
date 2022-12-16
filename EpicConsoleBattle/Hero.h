// Class HeroClass (determine if the hero has a shield and Hero, describing hero objects

#include "settings.h"

#pragma once

using namespace std;

class Effect;
class Skill;

struct Stats {
	Stats() {};
	Stats(int damages, int hp, int shield){
		_damages = damages;
		_maxHP = _currentHP = hp;
		_maxShield = _currentShield = shield;
	}

	int _damages = 10;
	int _maxHP = 100;
	int _currentHP = 100;
	int _maxShield = 100;
	int _currentShield = 100;
};

class HeroClass {
public:
	HeroClass() {};
	HeroClass(const string& name, const string& weapon, vector<int> listSkillsId, const string& description);
	~HeroClass() {};

	HeroClass& operator=(const HeroClass& other);

public:
	string _description = "MISSING_DESCRIPTION";
	string _name = "MISSING_CLASS_NAME";
	string _weapon = "WEAPON";
	vector<int> _listSkillsId;				// name of the skills available for this class

	static vector<HeroClass> _listClasses;

public:
	static void InitClasses();
};

class Hero
{
	// cons/destructor
public:
	Hero() {};
	Hero(const HeroClass& heroClass, const string& name, const Stats& stats);
	~Hero();
	Hero& operator=(Hero&);

	// attributes
public:
	bool _isLeft = false;
	HeroClass _class = HeroClass();		// class of the hero (determine if he has shield, his weapon name and damages and what his skill is)
	string _name = "MISSING_NAME";		// name of the hero
	string _gameOver = "RIP";			// message displayed at the end when hero die
	Stats _stats = Stats();				// damages, hp and shield of the hero, current and max values for each
	int _stun = 0;						// stunned turns left, 0 = not stunned

private:
	vector<Effect*> _listEffects;		// list of effects affecting the hero (damage's modifier, stun)
	vector<Skill*> _listSkills;			// skills instances, with their Cooldown

	// methods
public:
	void EndTurn();
	void SetClass(const HeroClass& heroClass);
	void Stun(int duration);
	int GetDamages();
	int GetNbEffects() { return (int)_listEffects.size(); };
	int GetNbSkills() { return (int)_listSkills.size(); };
	string AddEffect(Effect* pEffect);
	string RecieveDamages(int);
	string GetEffectDisplayText(int i);
	string GetSkillDisplayText(int i);
	const char* GetName() { return _name.c_str(); };
	const char* GetWeaponName() { return _class._weapon.c_str(); };
	vector<Skill*> GetAvailableSkillsThisTurn();
};
