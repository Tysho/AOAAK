#include "Settings.h"
#include "Effect.h"

#include "Hero.h"
#include "ResourcesManager.h"

/// <summary>
/// Decrease duration 
/// Could be overriden to eventually apply effect every turn for exemple (regen, increasing/decreasing effect, etc.)
/// </summary>
/// <returns>duration left</returns>
short Effect::Update()
{
	_duration = max(_duration - 1, 0);
	return _duration;
}

/// <summary>
/// return string explaining the effect
/// </summary>
/// <returns></returns>
string DamageModifier::Affect()
{
	// error...
	if (_damageMultiplier == 1)
		return "";

	string format = _damageMultiplier > 1 ? GetT("BONUS_DAMAGES") : GetT("MALUS_DAMAGES");
	int value;
	if (_damageMultiplier > 1)
		value = int(_damageMultiplier * 100 - 100);	// percent bonus
	else
		value = int(100 - _damageMultiplier * 100); // percent malus

	string s = "\t" + Format(format, _target._name.c_str(), value, _duration);
	return s;
}