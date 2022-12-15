#include "TempModifier.h"
#include "Fighter.h"

#include <string>
#include <iostream>

short TempModifier::Update()
{
	_duration--;
	if (_duration <= 0)
		Expire();

	return _duration;
}

void DamageModifier::Affect()
{
	string s = "\t" + _target._name + " recieved " + to_string(_value) + " bonus damages for " + to_string(_duration) + " turn(s) !!\n";
	cout << s;
	_target._weapon._damages += _value;
}

void DamageModifier::Expire()
{
	_target._weapon._damages -= _value;
}