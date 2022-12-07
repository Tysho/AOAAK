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


class Fighter
{
	// cons/destructor
public:
	Fighter(const string& name, const Weapon& weapon, const Stats& stats) {
		_name = name;
		_weapon = weapon;
		_stats = stats;
	};
	~Fighter() {};
	
	// attributes
public:
	Weapon _weapon;
	string _name;
	Stats _stats;				// hp&shield, current and max

private:
	int _stun = 0;				// stunned turns left
	vector<Skill*> _listSkills;	// skills

	friend class TempModifier;

	// methods
public:
	void EndTurn();
	void Stun(int duration);
	void AddSkill(Skill* pSkill);
	bool IsDead();
	bool IsStunned(string& summary);
	unsigned int GetDamages();
	Stats RecieveDamages(int damages, string& summary);
	std::vector<Skill*> GetAvailableSkillsThisTurn();
};

// ordre du tour : 
//		1- on demande pour chaque fighter s'il a des skills dispo.
//		2- on utilise les compétences de chacun
//		3- on 
