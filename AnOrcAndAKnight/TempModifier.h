#pragma once

#include <string>
class Hero;

class TempModifier {
public:
	TempModifier(Hero& target, short duration) : _target(target), _duration(duration) {};
	~TempModifier() {};

protected:
	short _duration;
	Hero& _target;

public:
	virtual std::string Affect() = 0;
	virtual void Expire() = 0;
	short Update();
};


class DamageModifier : public TempModifier {
public:
	DamageModifier(Hero& target, short duration, int value) : TempModifier(target, duration), _value(value) {};
	virtual ~DamageModifier() {};

private:
	int _value = 1;

public:
	virtual std::string Affect() final;
	virtual void Expire() final;
};


