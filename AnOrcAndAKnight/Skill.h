#pragma once

#include <string>

using namespace std;

enum class TypeTarget {
	self,
	opponent,
	both
};

class Fighter;
class TempModifier;

// skill the figter can use
class Skill {
public:
	Skill();
	virtual ~Skill() {};

public:
	std::string _name;
	TypeTarget _target;
	short _timer;		// timer left before next cast
	int _accuracy;		// percentage (0 = never hit to 100 = always hit)

protected:
	short _cooldown;	// delay between 2 casts

public:
	virtual TempModifier* Cast(Fighter& target);
	void EndTurn();
};

class Stun : public Skill {
public:
	Stun();
	virtual ~Stun() {};

public:
	virtual TempModifier* Cast(Fighter& target) final;
};

class Chaaaaaarge : public Skill {
public:
	Chaaaaaarge();
	virtual ~Chaaaaaarge() {};

public:
	virtual TempModifier* Cast(Fighter& target) final;
};
