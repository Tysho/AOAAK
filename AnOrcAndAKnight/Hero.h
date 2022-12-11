#pragma once

#include <vector>
#include <string>

using namespace std;

class Skill;

struct Stats {
	Stats() {};
	Stats(int hp, int shield) {
		_maxHP = _currentHP = hp;
		_maxShield = _currentShield = shield;
	}

	int _maxHP = 100;
	int _currentHP = 100;
	int _maxShield = 100;
	int _currentShield = 100;
};

struct Weapon {
public:
	Weapon() {};

	Weapon(string name, int damages) {
		_name = name;
		_damages = damages;
	};

	int _damages = 5;
	string _name = "UNNAMED_WEAPON";
};


class Hero
{
	// cons/destructor
public:
	Hero();
	Hero(const string& className, const string& name, const Weapon& weapon, const Stats& stats);
	~Hero();
	
	Hero& operator=(const Hero&);

	// attributes
public:
	Weapon _weapon;
	string _class;
	string _name;
	string _gameOver;
	Stats _stats;				// hp&shield, current and max values
	int _stun = 0;				// stunned turns left

private:
	vector<Skill*> _listSkills;	// skills

	friend class TempModifier;

	// methods
public:
	void EndTurn();
	void Stun(int duration);
	void AddSkill(Skill*);
	bool IsDead();
	unsigned int GetDamages();
	string RecieveDamages(int);
	std::vector<Skill*> GetAvailableSkillsThisTurn();
	const char* GetName() { return _name.c_str(); };
	const char* GetWeaponName() { return _weapon._name.c_str(); };
};

// ordre du tour : 
//		1- on demande pour chaque Hero s'il a des skills dispo.
//		2- on utilise les compétences de chacun
//		3- on 
