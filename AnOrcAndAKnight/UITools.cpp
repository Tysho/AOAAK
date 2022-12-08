#include "UITools.h"
#include "Hero.h"
#include "ResourcesManager.h"
#include <iostream>

#define SCREEN_WIDTH 111

#pragma region Inline fct tools

inline void CleanScreen() { system("cls"); };

inline bool IsNumber(const string& s)
{
	std::string::const_iterator it = s.begin();
	while (it != s.end() && std::isdigit(*it)) ++it;
	return !s.empty() && it == s.end();
};

inline string ReadText() {
	string s; 
	do { 
		getline(cin, s); 
	} while (s == ""); 
	return s; 
};

inline int ReadNumber() { 
	string s; 
	do { 
		getline(cin, s); 
	} while (s == "" || !IsNumber(s));
	return atoi(s.c_str()); 
};

#pragma endregion

int UITools::_currentTurn = -1;
string UITools::_summary;
string UITools::_header;
vector<string> UITools::_history;

// add an empty line with separator on the middle
static string AddEmtpyLine() {
	string display = "\t";
	for (int i = 0; i < SCREEN_WIDTH - 1; i++) {
		if (i == (SCREEN_WIDTH - 5) / 2) { // -5 > -4 for left margin and -1 for separator
			display += "|";
			i++;
		}
		display += " ";
	}
	display += "\n\t";
	
	return display;
}

/// <summary>
/// Draw life or shield bar for both Heroes stats
/// </summary>
/// <param name="isHp"> : true=HP, false=shield </param>
/// <returns>string containing the draw to display in cout</returns>
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

	// left bar (if shield bar for no shield Hero, don't display)
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

	// right bar (if shield bar for no shield Hero, don't display)

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


/// <summary>
/// Get a line which display curValue / maxValue for both Heroes stats
/// </summary>
/// <param name="isHp"> : true=HP, false=shield </param>
/// <returns>string containing the values to display in cout</returns>
string DisplayValue(bool isHp, const Stats& left, const Stats& right)
{
	string dataType = isHp ? GetT("LIFE_POINT") : GetT("SHIELD");
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

string UITools::DrawStats(const Hero& left, const Hero& right)
{
	string display = "\n\t";

	// names of the Heroes
	display += left._name;

	int begin = (int)left._name.size(); 
	int end = SCREEN_WIDTH - (int)right._name.size() - 4; // 4 because of first tab
	for (int i = begin; i < end; i++) {
		if (i == (SCREEN_WIDTH - 5) /2) {
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

void UITools::DrawTurn(const Hero& left, const Hero& right) 
{
	CleanScreen();

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

	_currentTurn = int(_history.size() - 1);
}

// display next turn ONLY if it was already generated
bool UITools::DisplayNextTurn() 
{
	if (_currentTurn >= _history.size() - 1)
		return false;

	CleanScreen();

	_currentTurn++;
	cout << _history[_currentTurn];
	return true;
}

// display previous turn if not already on the first one
void UITools::DisplayPreviousTurn(bool forceLastTurn) 
{
	if (_history.size() == 0)
		return;
	if (_currentTurn == 0)
		return;

	CleanScreen();

	// came from scoreboard ? display last
	if (forceLastTurn)
		_currentTurn = int(_history.size() - 1);
	
	// display previous
	else 
		_currentTurn--;

	cout << _history[_currentTurn];
}

// Generate a form in the cout to collect caracteristics for both Heroes from user
void UITools::LaunchEditForm(Hero& knight, Hero& orc)
{
	CleanScreen();
	//cout << "Name of the Knight : ";
	string custom;
	getline(cin, custom);
	knight._name = ReadText();

	cout << "\"" + knight._name + "\" HP (enter a number) : ";
	knight._stats._currentHP = knight._stats._maxHP = ReadNumber();

	cout << "\"" + knight._name + "\" Shield value (enter a number) : ";	
	knight._stats._currentShield = knight._stats._maxShield = ReadNumber();

	cout << "\"" + knight._name + "\" weapon's name : ";
	knight._weapon._name = ReadText();

	cout << "damage inflicted by \"" + knight._weapon._name + "\" : (enter a number) : ";
	knight._weapon._damages = ReadNumber();

	cout << "Name of the Orc : ";
	orc._name = ReadText();

	cout << "\"" + orc._name + "\" HP (enter a number) : ";	
	orc._stats._currentHP = orc._stats._maxHP = ReadNumber();

	cout << "\"" + orc._name + "\" Shield value (enter a number) : ";
	orc._stats._currentShield = orc._stats._maxShield = ReadNumber();

	cout << "\"" + orc._name + "\" weapon's name : ";
	orc._weapon._name = ReadText();

	cout << "damage inflicted by \"" + orc._weapon._name + "\" : (enter a number) : ";
	orc._weapon._damages = ReadNumber();
}