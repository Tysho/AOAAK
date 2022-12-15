#include "TempModifier.h"
#include "Fighter.h"

#include <iostream>

short TempModifier::Update()
{
	_duration--;
	if (_duration <= 0)
		Expire();

	return _duration;
}

/// <summary>
/// Apply effect to target and return summary
/// </summary>
/// <returns></returns>
string DamageModifier::Affect()
{
	_target._weapon._damages += _value;
	string s = "\t" + _target._name + " recieved " + to_string(_value) + " bonus damages for " + to_string(_duration) + " turn(s) !!\n";
	return s;
}

void DamageModifier::Expire()
{
	_target._weapon._damages -= _value;
}