#include "TempModifier.h"
#include "Hero.h"
#include "ResourcesManager.h"

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
	string s = "\t" + Format(GetT("BONUS_DAMAGES"), _target._name.c_str(), _value, _duration);
	return s;
}

void DamageModifier::Expire()
{
	_target._weapon._damages -= _value;
}