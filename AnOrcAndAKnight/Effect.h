#pragma once

#include "Settings.h"

using namespace std;

class Hero;

class Effect {
public:
	Effect(string name, Hero& target, short duration) : _name(name), _target(target), _duration(duration) {};
	~Effect() {};

public:

protected:
	short _duration;
	string _name;
	Hero& _target;

	// methodes
public:
	// pur virtual
	virtual void Expire() = 0;
	virtual string Affect() = 0;

	// overridable
	virtual float GetDamageMultiplier() { return 1; };
	
	short Update();
	int GetDurationLeft() { return _duration; };
	const char* GetName() { return _name.c_str(); };
};


class DamageModifier : public Effect {
public:
	DamageModifier(string name, Hero& target, short duration, float value) : Effect(name, target, duration), _damageMultiplier(value) {};
	virtual ~DamageModifier() {};

private:
	float _damageMultiplier = 1;

public:
	virtual float GetDamageMultiplier() { return _damageMultiplier; };
	virtual std::string Affect() final;
	virtual void Expire() final {};
};


