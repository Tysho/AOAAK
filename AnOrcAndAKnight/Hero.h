// contain 
#pragma once

#include "Skill.h"

#include <vector>
#include <string>

using namespace std;

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
	HeroClass(const string& name, const string& weapon, bool shield, vector<int> listSkillsId);
	~HeroClass() {};

	HeroClass& operator=(const HeroClass& other);

public:
	string _name = "MISSING_CLASS_NAME";
	string _weapon = "WEAPON";
	bool _hasShield = false;
	vector<int> _listSkillsId;				// name of the skills available for this class

	static vector<HeroClass> _listClasses;
};

class Hero
{
	// cons/destructor
public:
	Hero() {};
	Hero(const HeroClass& heroClass, const string& name, const Stats& stats);
	~Hero();
	
	Hero& operator=(const Hero&);

	static vector<HeroClass*> _listeClass;

	// attributes
public:
	HeroClass _class = HeroClass();		// class of the hero (determine if he has shield, his weapon name and damages and what his skill is)
	string _name = "MISSING_NAME";		// name of the hero
	string _gameOver = "RIP";			// message displayed at the end when hero die
	Stats _stats = Stats();				// damages, hp and shield of the hero, current and max values for each
	int _stun = 0;						// stunned turns left, 0 = not stunned

private:
	vector<Skill*> _listSkills;		// skills instances, with their Cooldown
	friend class TempModifier;

	// methods
public:
	void EndTurn();
	void Stun(int duration);
	bool IsDead();
	unsigned int GetDamages();
	string RecieveDamages(int);
	std::vector<Skill*> GetAvailableSkillsThisTurn();
	const char* GetName() { return _name.c_str(); };
	const char* GetWeaponName() { return _class._weapon.c_str(); };
};
