// structs and Classes about the Heroes and their attributes : weapon, stats, HeroClass, Hero
#pragma once

#include <string>
#include <vector>

using namespace std;

class Effect;
class Skill;

// { name, damages }
struct Weapon {
	string _name = "MISSING_WEAPON";
	int _damages = 10;
};


// { HP, shield } (current and max)
struct Stats {
	Stats() {};
	Stats(int hp, int shield){
		_maxHP = _currentHP = hp;
		_maxShield = _currentShield = shield;
	}

	int _maxHP = 100;
	int _currentHP = 100;
	int _maxShield = 100;
	int _currentShield = 100;
};


// "type/race/class" of the hero (orc, elf, human); The hero class determine the skills of the hero
class HeroClass {
public:
	// cons/destructor/operators
	HeroClass() {};
	HeroClass(const string& name, vector<int> listSkillsId, const string& description);
	~HeroClass() {};

	HeroClass& operator=(const HeroClass& other);

	// list of all existing classes
public:
	static vector<HeroClass> _listClasses;

	// attributes
public:
	string _description = "MISSING_DESCRIPTION";
	string _name = "MISSING_CLASS_NAME";
	vector<int> _listSkillsId;				// name of the skills available for this class

	// methods
public:
	static void InitClasses();
};


class Hero
{
	// cons/destructor/operators
public:
	Hero() {};
	Hero(const HeroClass& heroClass, const string& name, const Stats& stats);
	~Hero();
	Hero& operator=(Hero&);

	// attributes
public:
	bool _isLeft = false;
	string _name = "MISSING_NAME";		// name of the hero
	Stats _stats = Stats();				// damages, hp and shield of the hero, current and max values for each
	int _stun = 0;						// stunned turns left, 0 = not stunned

private:
	HeroClass _class = HeroClass();		// class of the hero (determine if he has shield, his weapon name and damages and what his skill is)
	vector<Effect*> _listEffects;		// list of effects affecting the hero (damage's modifier, stun)
	vector<Skill*> _listSkills;			// skills instances, with their Cooldown
	Weapon _weapon;						// weapon of the hero (got Name and Damages)

	// methods
public:
	void EndTurn();
	void UpdateEffects();
	void EquipWeapon(const Weapon weapon);
	void SetClass(const HeroClass& heroClass);
	void Stun(int duration);
	int GetDamages();
	int GetNbEffects();
	int GetNbSkills() { return (int)_listSkills.size(); };
	string AddEffect(Effect* pEffect);
	string GetClass() { return _class._name; };
	string GetEffectResume(int i);
	string GetSkillResume(int i);
	string RecieveDamages(int);
	const char* GetName() { return _name.c_str(); };
	const char* GetWeaponName() { return _weapon._name.c_str(); };
	vector<Skill*> GetAvailableSkillsThisTurn();
};
