#pragma once

using namespace std;

#define NEW_HERO_CLASS(classname, code)		\
class classname : public Skill {			\
public:										\
	classname();							\
	virtual ~classname() {};				\
public:										\
	virtual bool Cast(Hero& target) final;	\
	int GetCode() { return code; };			\
};

enum class TypeTarget {
	self,
	opponent,
	both
};

class Hero;
class Effect;

// skill the hero can use
class Skill {
public:
	Skill() {};
	virtual ~Skill() {};

	// Attributes
public:
	string _name = "UNDEF_SKILL";
	TypeTarget _target = TypeTarget::opponent;
	short _timer = 0;							// timer left before next cast
	int _accuracy = 50;							// percentage (0 = never hit to 100 = always hit)
	short _cooldown = 5;						// delay between 2 casts

	// Methods
public:
	virtual bool Cast(Hero& target) { _timer = _cooldown; return false; };

	void EndTurn() { _timer--; };
	int GetCode() { return -1; };
	const char* GetName();
	string GetResume();
	static Skill* CreateSkillInstanceById(int idSkill);
};

#define STUN 1
NEW_HERO_CLASS(Stun, STUN);

#define CHARGE 2
NEW_HERO_CLASS(Charge, CHARGE);

#define ARROW_KNEE 3
NEW_HERO_CLASS(ArrowInTheKnee, ARROW_KNEE);