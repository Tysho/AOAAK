#include "UITools.h"
#include "Fighter.h"

#include <iostream>

#define SCREEN_WIDTH 111

int UITools::_currentTurn = -1;
string UITools::_summary;
string UITools::_header;
vector<string> UITools::_history;

// add an empty line with separator on the middle
static string AddEmtpyLine() {
	string display = "\t";
	for (int i = 0; i < SCREEN_WIDTH - 1; i++) {
		if (i == (SCREEN_WIDTH - 5) / 2) { // -5 > -4 for left margin and -1 for separator
			display = "|";
			i++;
		}
		display = " ";
	}
	display = "\n\t";
	
	return display;
}

/// <summary>
/// Draw life or shield bar for both Fighters, return string which will be displayed
/// </summary>
/// <param name="isHp">true > HP / false > shield </param>
/// <param name="left"></param>
/// <param name="right"></param>
/// <returns></returns>
string DrawBar(bool isHp, const Stats& left, const Stats& right) 
{
	string result;

	int barreWidth = (SCREEN_WIDTH - 24) / 2;	// 24 = 10 for middle space each side of the separator "|" + 4 space for left margin

	int leftCur = isHp ? left._currentHP : left._currentShield;
	int leftMax = isHp ? left._maxHP : left._maxShield;
	int rightCur = isHp ? right._currentHP : right._currentShield;
	int rightMax = isHp ? right._maxHP : right._maxShield;

	// no shield for both figthers ?
	if (isHp == false && leftMax + rightMax == 0)
		return "";

	// left bar (if shield bar for no shield fighter, don't display)
	if (isHp || leftMax > 0) {
		int hpPlain = leftCur * barreWidth / leftMax;
		for (int i = 0; i < hpPlain; i++)
			result += char(219);
		for (int i = hpPlain; i < barreWidth; i++)
			result += char(176);
	}
	else {
		for (int i = 0; i < barreWidth; i++)
			result += "";
	}

	// separator
	result += "          |          ";

	// right bar (if shield bar for no shield fighter, don't display)

	if (isHp || rightMax > 0) {
		int hpPlain = rightCur * barreWidth / rightMax;
		for (int i = 0; i < hpPlain; i++)
			result += char(219);
		for (int i = hpPlain; i < barreWidth; i++)
			result += char(176);
	}
	result += "\n\t";

	return result;
}


string DisplayValue(bool isHp, const Stats& left, const Stats& right)
{
	string dataType = isHp ? "HP : " : "Shield : ";
	int leftCur = isHp ? left._currentHP : left._currentShield;
	int leftMax = isHp ? left._maxHP : left._maxShield;
	int rightCur = isHp ? right._currentHP : right._currentShield;
	int rightMax = isHp ? right._maxHP : right._maxShield;

	string leftHpCounter = dataType + to_string(leftCur) + " / " + to_string(leftMax);

	string rightHpCounter = dataType + to_string(rightCur) + " / " + to_string(rightMax);

	// if max == 0, then there is nothing to display
	if (leftMax <= 0)
		leftHpCounter = "";
	if (rightMax <= 0)
		rightHpCounter = "";

	string result = leftHpCounter;
	for (int i = (int)leftHpCounter.size(); i < SCREEN_WIDTH - rightHpCounter.size() - 4; i++) {
		if (i == (SCREEN_WIDTH - 5) / 2) {	// -5 > -1 for separator and -4 for left margin
			result += "|";
			i++;
		}
		result += " ";
	}
	result += rightHpCounter + "\n";
	
	return result;
}

void UITools::LogSummary(const string& text)
{
	_summary += "\n\t" + text;
}

void UITools::LogHeader(const string& text)
{
	_header += "\n\t" + text;
}

string UITools::DrawStats(const Fighter& left, const Fighter& right)
{
	string display = "\n\t";

	// names of the fighters
	display += left._name;

	int separatorLength = int(SCREEN_WIDTH - left._name.size() - right._name.size() - 4); // - 4 for left margin
	for (int i = 0; i < separatorLength; i++) {
		if (i == (SCREEN_WIDTH - 5) / 2) {	// -5 > -1 for separator and -4 for left margin
			display += "|";
			i++;
		}
		display += " ";
	}

	display += right._name + "\n\t";

	// LIFE BARS
	display += DrawBar(true, left._stats, right._stats);

	// HP VALUES
	display += DisplayValue(true, left._stats, right._stats);

	display += AddEmtpyLine();

	// SHIELD BARS
	string shieldsDraw = DrawBar(false, left._stats, right._stats);
	
	// SHIELDS VALUES
	if (shieldsDraw != "") {
		display += shieldsDraw;
		display += DisplayValue(false, left._stats, right._stats);
		display += AddEmtpyLine();
	}

	return display;
}

void UITools::DrawTurn(const Fighter& left, const Fighter& right) 
{
	system("cls");   // Clear output for next display

	// save header
	string screen = _header;
	_header = "";
	
	// get stats display (bars etc.)
	screen += DrawStats(left, right);

	// finally summary
	screen += _summary;
	_summary = "";

	// save history
	_history.push_back(screen);
	
	// display everything on console
	cout << screen;

	_currentTurn = _history.size() - 1;
}

bool UITools::DisplayNextTurn() 
{
	if (_currentTurn >= _history.size() - 1)
		return false;

	system("cls");   // Clear output for next display

	_currentTurn++;
	cout << _history[_currentTurn];
	return true;
}

void UITools::DisplayPreviousTurn(bool forceLastTurn) 
{
	if (_history.size() == 0)
		return;

	if (_currentTurn == 0)
		return;

	system("cls");   // Clear output for next display

	if (forceLastTurn)
		_currentTurn = _history.size() - 1;
	else 
		_currentTurn--;
	cout << _history[_currentTurn];
}

void UITools::LaunchEditForm(Fighter& knight, Fighter& orc)
{
	cout << "Name of the Knight : ";
	string custom;
	getline(cin, custom);
	knight._name = custom;

	cout << "\"" + knight._name + "\" HP (enter a number) : ";
	do {
		getline(cin, custom);
	} while (false); // is not a number
	knight._stats._currentHP = knight._stats._maxHP = atoi(custom.c_str());

	cout << "\"" + knight._name + "\" Shield value (enter a number) : ";
	do {
		getline(cin, custom);
	} while (false); // is not a number
	knight._stats._currentShield = knight._stats._maxShield = atoi(custom.c_str());

	cout << "\"" + knight._name + "\" weapon's name : ";
	getline(cin, custom);
	knight._weapon._name = custom;

	cout << "damage inflicted by \"" + knight._weapon._name + "\" : (enter a number) : ";
	do {
		getline(cin, custom);
	} while (false); // is not a number
	knight._weapon._damages = atoi(custom.c_str());

	cout << "Name of the Orc : ";
	getline(cin, custom);
	orc._name = custom;

	cout << "\"" + orc._name + "\" HP (enter a number) : ";
	do {
		getline(cin, custom);
	} while (false); // is not a number
	orc._stats._currentHP = orc._stats._maxHP = atoi(custom.c_str());

	cout << "\"" + orc._name + "\" Shield value (enter a number) : ";
	do {
		getline(cin, custom);
	} while (false); // is not a number
	orc._stats._maxShield = atoi(custom.c_str());
	orc._stats._currentShield = orc._stats._maxShield;

	cout << "\"" + orc._name + "\" weapon's name : ";
	getline(cin, custom);
	orc._weapon._name = custom;

	cout << "damage inflicted by \"" + orc._weapon._name + "\" : (enter a number) : ";
	do {
		getline(cin, custom);
	} while (false); // is not a number
	orc._weapon._damages = atoi(custom.c_str());
}