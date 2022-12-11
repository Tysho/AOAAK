#include "Hero.h"
#include "ResourcesManager.h"
#include "UIManager.h"

#include <string>
#include <vector>
#include <conio.h>
#include <iostream>

#define SCREEN_WIDTH 111

#pragma region Inline fct tools

void CleanScreen() { system("cls"); };


inline string ReadText(const string& screen) {
	string s; 
	do {
		CleanScreen();
		cout << screen;
		getline(cin, s);
	} while (s == ""); 
	return s; 
};

inline int ReadNumber(const string& screen) { 
	string s; 
	while (1) {
		CleanScreen();
		cout << screen;
		getline(cin, s);

		if (s == "")
			continue;

		if (UIManager::IsNumber(s) == false)
			continue;

		int val = atoi(s.c_str());
		if (val > 0)
			return val;
	}
	return 1;
};

#pragma endregion

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

	// left bar (if hero doesn't have shield don't display)
	if (isHp || leftMax > 0) {
		int hpPlain = leftCur * barreWidth / leftMax;
		for (int i = 0; i < hpPlain; i++)
			result += char(219);
		for (int i = hpPlain; i < barreWidth; i++)
			result += char(176);
	}
	else {
		for (int i = 0; i < barreWidth; i++)
			result += " ";
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


void UIManager::DisplayBattleEnd()
{
	CleanScreen();
	cout << _summary;
	_history.push_back(_summary);
	_currentTurn = int(_history.size() - 1);
	_summary = "";
}

void UIManager::DisplayBattleStart(const Hero& left, const Hero& right) {
	LogSummary("\n\t\t" + GetT("BATTLE_BEGIN"));
	DrawNewTurn(left, right);
}


void UIManager::LogSummary(const string& text)
{
	_summary += "\n\t" + text;
}


void UIManager::LogTurnCount(int turn)
{
	string strTurn = to_string(turn);
	if (turn < 10)
		strTurn = "0" + strTurn;

	string log = Format(GetT("TURN_COUNTER"), strTurn.c_str());
	_header += "\n\t" + log;
}

void UIManager::SelectHero(Hero& hero, int numHero)
{
	int curSelection = 0;
	while (1) {
		CleanScreen();
		string screen = "\n" + Format(GetT("HERO_SELECT"), numHero) + "\n\n";
		int curPosition = 0;
		for (Hero* p : ResourcesManager::_listHeroes)
		{
			if (curSelection == curPosition)
				screen += "\t > [ " + p->_name + " (" + p->_class._name + ") ]" + "\n\n";
			else
				screen += "\t" + p->_name + " (" + p->_class._name + ")\n\n";
			curPosition++;
		}

		if (curSelection == curPosition)
			screen += "\t > [ " + GetT("CUSTOM_HERO") + " ]" + "\n\n";
		else 
			screen += "\t" + GetT("CUSTOM_HERO") + "\n\n";

		cout << screen;

		switch (GetInputKeyVerticalSelection()) {
		case VerticalSelection::DOWN:
			curSelection++;
			curSelection = min(curSelection, (int)ResourcesManager::_listHeroes.size());
			break;
		case VerticalSelection::UP:
			curSelection--;
			curSelection = max(0, curSelection);
			break;
		case VerticalSelection::SELECT:
			if (curPosition == curSelection) {
				LaunchEditForm(hero, numHero);
				CleanScreen();
				return;
			}
			hero = *ResourcesManager::_listHeroes[curSelection];
			return;
		}
	}
}


string UIManager::DrawStats(const Hero& left, const Hero& right)
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


void UIManager::DrawNewTurn(const Hero& left, const Hero& right)
{
	_currentTurn++;
	LogTurnCount(_currentTurn);

	// display everything on console
	CleanScreen();
	string screen = _header + DrawStats(left, right) + _summary;
	cout << screen;

	// save history
	_history.push_back(screen);

	_currentTurn = int(_history.size() - 1);

	// clean buffer
	_header = "";
	_summary = "";
}


// display next turn ONLY if it was already generated, return false otherwise
bool UIManager::DisplayNextTurn()
{
	// already on the last calculated battle scene
	if (_currentTurn >= _history.size() - 1)
		return false;

	CleanScreen();
	_currentTurn++;
	cout << _history[_currentTurn];
	return true;
}

bool UIManager::IsNumber(const string& s)
{
	std::string::const_iterator it = s.begin();
	while (it != s.end() && std::isdigit(*it)) ++it;
	return !s.empty() && it == s.end();
}


// display previous turn if not already on the first one
void UIManager::DisplayPreviousTurn(bool forceLastTurn)
{
	if (_history.size() == 0)
		return;
	if (_currentTurn == 0)
		return;

	CleanScreen();
	
	// display previous
	_currentTurn--;
	cout << _history[_currentTurn];
}


// Generate a form in the cout to collect caracteristics for both Heroes from user
void UIManager::LaunchEditForm(Hero& hero, int heroNumber)
{
	CleanScreen();
	string screen = "\n" + Format(GetT("HERO_EDIT"), heroNumber) +"\n\n";

	screen += Format(GetT("HERO_NAME"), hero.GetName());
	hero._name = ReadText(screen);
	screen += " " + hero._name;

	screen += "\n\n" + Format(GetT("HERO_HP"), hero.GetName());
	hero._stats._currentHP = hero._stats._maxHP = ReadNumber(screen);
	screen += " " + to_string(hero._stats._currentHP);

	screen += "\n\n" + Format(GetT("HERO_SHIELD"), hero.GetName());
	hero._stats._currentShield = hero._stats._maxShield = ReadNumber(screen);
	screen += " " + to_string(hero._stats._currentShield);

	screen += "\n\n" + Format(GetT("HERO_WEAPON_NAME"), hero.GetName());
	hero._class._name = ReadText(screen);
	screen += " " + hero._class._name;

	screen += "\n\n" + Format(GetT("HERO_WEAPON_DAMAGES"), hero.GetWeaponName());
	hero._stats._damages = ReadNumber(screen);

	CleanScreen();
}

// Get question linked to questionKey, display it to user and return answer
bool UIManager::AskUserYesNoQuestion(const string& questionKey) {

	string log = "\n\n\t" + GetT(questionKey.c_str());
	cout << log;
	while (1) {
		int ynKey = _getch();
		if (ynKey == 224)
			ynKey += _getch();

		// yes
		if (ynKey == 121)
			return true;

		// no
		if (ynKey == 110)
			return false;
	}
}


NextDisplay UIManager::GetInputKeyForwardBackward()
{
	while (1) {
		int key = _getch();
		if (key == 224)
			key += _getch();

		switch (key) {
		case 296:   // up-arrow
		case 299:   // left-arrow
			return NextDisplay::BACKWARD;
		case 13:    // enter
		case 32:    // spacebar
		case 301:   // right-arrow
		case 304:   // down-arrow
			return NextDisplay::FORWARD;
		default:
			continue;
		}
	}
}

VerticalSelection UIManager::GetInputKeyVerticalSelection() {
	while (1) {
		int key = _getch();
		if (key == 224)
			key += _getch();

		switch (key) {
		case 296:   // up-arrow
			return VerticalSelection::UP;
		case 304:   // down-arrow
			return VerticalSelection::DOWN;
		case 13:    // enter
		case 32:    // spacebar
			return VerticalSelection::SELECT;
		default:
			continue;
		}
	}
}