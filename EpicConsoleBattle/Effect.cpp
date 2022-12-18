#include "Settings.h"
#include "Effect.h"

#include "Hero.h"
#include "ResourcesManager.h"

// return the string resuming the duration for the effet, ready to dispay
string Effect::GetResume()
{
	string result = _name + " : ";
	string turn = " ";
	turn += char(219);
	for (int i = 0; i < GetDurationLeft(); i++)
		result += turn;

	return result;
}

/// <summary>
/// Decrease duration.
/// Could be overriden to eventually apply effect every turn for exemple (regen, increasing/decreasing effect, etc.)
/// </summary>
/// <returns>duration left</returns>
short Effect::Update()
{
	_duration = max(_duration - 1, 0);
	return _duration;
}


// return string explaining the effect
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