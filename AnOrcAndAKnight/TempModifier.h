#pragma once

#include <string>

using namespace std;

class Hero;

class TempModifier {
public:
	TempModifier(const string& name, Hero& target, short duration) : _name(name), _target(target), _duration(duration) {};
	~TempModifier() {};

protected:
	short _duration;
	string _name;
	Hero& _target;

public:
	virtual std::string Affect() = 0;
	virtual void Expire() = 0;
	short Update();
	int GetDurationLeft() { return _duration; };
};


class DamageModifier : public TempModifier {
public:
	DamageModifier(const string& name, Hero& target, short duration, int value) : TempModifier(name, target, duration), _value(value) {};
	virtual ~DamageModifier() {};

private:
	int _value = 1;

public:
	virtual std::string Affect() final;
	virtual void Expire() final;
};


