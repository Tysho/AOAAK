#include "UIManager.h"
#include "Settings.h"

#include "Hero.h"
#include "ResourcesManager.h"
#include "Effect.h"

#define SCREEN_WIDTH 111

#pragma region Inline fct tools

// clear console
void CleanScreen() { system("cls"); };


// display selector with current selection highlighted
string DisplayHeroSelector(int& curSelection, const string& firstHeroName = "", const string& firstHeroClass = "")
{
	string screen;

	int curPosition = 0;
	for (Hero* p : ResourcesManager::_listHeroes)
	{
		if (p->_name == firstHeroName && p->_class._name == firstHeroClass)
		{
			curPosition++;
			continue;
		}

		if (curSelection == curPosition)
			screen += "\t > [ " + p->_name + " (" + p->_class._name + ") ]\n\n";
		else
			screen += "\t" + p->_name + " (" + p->_class._name + ")\n\n";
		curPosition++;
	}

	if (curSelection == curPosition)
		screen += "\t > [ " + GetT("CUSTOM_HERO") + " ]\n\n";
	else
		screen += "\t" + GetT("CUSTOM_HERO") + "\n\n";
	
	return screen;
}


// display screen while user don't tape text then enter
inline string ReadText(const string& screen) {
	string s; 
	do {
		CleanScreen();
		cout << screen;
		getline(cin, s);
	} while (s == ""); 
	return s; 
};


// display screen while user don't enter a integer value
inline int ReadNumber(const string& screen, bool canBeZero = false) { 
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
		if (val == 0 && canBeZero)
			return val;
	}
	return 1;
};


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


// Draw life or shield bar for both Heroes stats
inline string DrawBar(bool isHp, const Stats& left, const Stats& right)
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


// Get a line which display curValue / maxValue for both Heroes stats
inline string DisplayValue(bool isHp, const Stats& left, const Stats& right)
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
	for (int i = (int)leftHpCounter.size(); i < SCREEN_WIDTH - (int)rightHpCounter.size() - 4; i++) {
		if (i == (SCREEN_WIDTH - 5) / 2) {	// -5 > -1 for separator and -4 for left margin
			result += "|";
			i++;
		}
		result += " ";
	}
	result += rightHpCounter + "\n";
	
	return result;
}


// Get a line which display effects affecting the heroes (one call per effect for both)
inline string DisplayEffect(const string& effectLeft, const string& effectRight)
{
	string result = effectLeft;
	for (int i = (int)effectLeft.size(); i < SCREEN_WIDTH - (int)effectRight.size(); i++) {
		if (i == (SCREEN_WIDTH - 5) / 2) {	// -5 > -1 for separator and -4 for left margin
			result += "|";
			i++;
		}
		result += " ";
	}
	result += effectRight + "\n";
	return result;
}


// resize screen buffer to match console window size to avoid scrollbar
inline void RemoveScrollbars() 
{
	HANDLE handleConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	// retrieve screen buffer info
	CONSOLE_SCREEN_BUFFER_INFO scrBufferInfo;
	GetConsoleScreenBufferInfo(handleConsole, &scrBufferInfo);

	// current window size
	short winWidth = scrBufferInfo.srWindow.Right - scrBufferInfo.srWindow.Left + 1;
	short winHeight = scrBufferInfo.srWindow.Bottom - scrBufferInfo.srWindow.Top + 1;

	// current screen buffer size
	short scrBufferWidth = scrBufferInfo.dwSize.X;
	short scrBufferHeight = scrBufferInfo.dwSize.Y;

	// to remove the scrollbar the window height must match the screen buffer height
	COORD newSize{ scrBufferWidth, winHeight };

	// set new screen buffer dimensions
	SetConsoleScreenBufferSize(handleConsole, newSize);
}

#pragma endregion


// display form with selector to chose the language
void UIManager::SelectLanguage()
{
	// get available languages
	vector<string>&& listLang = ResourcesManager::GetAvailableLanguages();

	// display selector
	int curSelection = 0;
	while (1) {
		CleanScreen();
		string screen = "\n\n";
		int curPosition = 0;
		for (string lang : listLang)
		{
			if (curSelection == curPosition)
				screen += "\t > [ " + lang + " ]\n\n";
			else
				screen += "\t" + lang + "\n\n";
			curPosition++;
		}
		cout << screen;

		// get user input
		switch (GetInputKeyVerticalSelection()) {
		case VerticalSelection::DOWN:
			curSelection++;
			curSelection = min(curSelection, (int)listLang.size() - 1);
			break;
		case VerticalSelection::UP:
			curSelection--;
			curSelection = max(0, curSelection);
			break;
		case VerticalSelection::SELECT:
			ResourcesManager::LoadLanguage(curSelection + 1);	// language id start at 1
			return;
		}
	}
}

void UIManager::SetupConsoleSize()
{
	// set windows pos and size
	HWND console = GetConsoleWindow();
	MoveWindow(console, WINDOW_X, WINDOW_Y, WINDOW_WIDTH, WINDOW_HEIGHT, TRUE);
	RemoveScrollbars();
}


// Display the end screen for the first time
void UIManager::DisplayBattleEnd()
{
	CleanScreen();
	cout << _summary;
	_history.push_back(_summary);
	_currentTurn = int(_history.size() - 1);
	_summary = "";
}


// First display of he battle
void UIManager::DisplayBattleStart(Hero& left, Hero& right) {
	LogSummary("\n\t\t" + GetT("BATTLE_BEGIN"));
	DrawNewTurn(left, right);
}


// ad a line to the next battle resume
void UIManager::LogSummary(const string& text)
{
	_summary += "\n\t" + text;
}


// display the turn count in the header
void UIManager::LogTurnCount(int turn)
{
	string strTurn = to_string(turn);
	if (turn < 10)
		strTurn = "0" + strTurn;

	string log = Format(GetT("TURN_COUNTER"), strTurn.c_str());
	_header += "\n\t" + log;
}


// display a form to select a hero, the first hero picked is hidden if it's the second pick
void UIManager::SelectHero(Hero& hero, int numHero, const string& firstHeroName, const string& firstHeroClass)
{
	int curSelection = 0;
	
	// title
	string header;

	// announce first champ selected
	if (numHero > 1)
		header += "\n\t" + Format(GetT("FIRST_HERO_PICKED"), firstHeroName.c_str(), firstHeroClass.c_str()) + "\n\n";
	header += "\n" + Format(GetT("HERO_SELECT"), numHero) + "\n\n";
	
	// waiting for user choice
	while (1) {
		string screen = header + DisplayHeroSelector(curSelection, firstHeroName, firstHeroClass);
		CleanScreen();
		cout << screen;

		switch (GetInputKeyVerticalSelection()) {
		case VerticalSelection::DOWN:
		{
			curSelection++;
			curSelection = min(curSelection, (int)ResourcesManager::_listHeroes.size());
			
			// already at the end
			if (curSelection == (int)ResourcesManager::_listHeroes.size())
				break;

			// if we fall on the already picked one, then we jump to the next one
			Hero* pHero = ResourcesManager::_listHeroes[curSelection];
			if (pHero->_name == firstHeroName && pHero->_class._name == firstHeroClass) {
				curSelection++;

				// no more next one ? we stay at the end
				if (curSelection == (int)ResourcesManager::_listHeroes.size() + 1) // +1 because there is still custom hero creation button
					curSelection -= 2;
			}
			curSelection = min(curSelection, (int)ResourcesManager::_listHeroes.size());
		}
		break;
		case VerticalSelection::UP:
		{
			curSelection--;
			curSelection = max(0, curSelection);

			// if we fall on the already picked one, then we jump to the next one
			Hero* pHero = ResourcesManager::_listHeroes[curSelection];
			if (pHero->_name == firstHeroName && pHero->_class._name == firstHeroClass) {
				curSelection--;

				// no more next one ? we stay at the beginning
				if (curSelection < 0)
					curSelection = 1;
			}
		}
		break;
		case VerticalSelection::SELECT:
			if (curSelection == ResourcesManager::_listHeroes.size()) {
				LaunchEditForm(hero, numHero);
				CleanScreen();
				return;
			}
			hero = *ResourcesManager::_listHeroes[curSelection];
			return;
		}
	}
}


// display hp/shield and effect of both champs
string UIManager::DrawStats(Hero& left, Hero& right)
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

	// DISPLAY EFFECTS
	int maxEffectsNb = max(left.GetNbEffects(), right.GetNbEffects());
	for (int i = 0; i < maxEffectsNb; i++) {
		string effectLeft = left.GetEffectDisplayText(i);
		string effectRight = right.GetEffectDisplayText(i);
		display += DisplayEffect(effectLeft, effectRight);

		if (i + 1 < maxEffectsNb)
			display += +"\t";
	}

	// SKILLS COOLDOWN

	return display;
}


// dipslay a calculated turn (using summary for battle details
void UIManager::DrawNewTurn(Hero& left, Hero& right)
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
	// already on the last history frame, need to play another turn
	if (_currentTurn >= (int)_history.size() - 1)
		return false;

	CleanScreen();
	_currentTurn++;
	cout << _history[_currentTurn];
	return true;
}


// static methode return true if the string contains only numeric characters ("-" and "./," not allowed)
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
	// user select the class of the hero
	string screen = DisplayClassSelector(hero, heroNumber);

	CleanScreen();
	cout << screen;

	screen += "\n\n\t"+ Format(GetT("HERO_NAME"), hero.GetName());
	hero._name = ReadText(screen);
	screen += " " + hero._name;

	screen += "\n\n\t" + Format(GetT("HERO_HP"), hero.GetName());
	hero._stats._currentHP = hero._stats._maxHP = ReadNumber(screen);
	screen += " " + to_string(hero._stats._currentHP);

	screen += "\n\n\t" + Format(GetT("HERO_SHIELD"), hero.GetName());
	hero._stats._currentShield = hero._stats._maxShield = ReadNumber(screen, true); // can be zero
	screen += " " + to_string(hero._stats._currentShield);

	screen += "\n\n\t" + Format(GetT("HERO_WEAPON_NAME"), hero.GetName());
	hero._class._weapon = ReadText(screen);
	screen += " " + hero._class._weapon;

	screen += "\n\n\t" + Format(GetT("HERO_WEAPON_DAMAGES"), hero.GetWeaponName());
	hero._stats._damages = ReadNumber(screen);

	CleanScreen();
}


// allow the user to pick a class for his custom hero, return the beginning of the form for next steps
string UIManager::DisplayClassSelector(Hero& hero, int numHero)
{
	CleanScreen();
	string header = "\n" + Format(GetT("HERO_EDIT"), numHero) + "\n\n";

	int curSelection = 0;

	// display selector
	while (1) {
		string screen = header;
		int curPosition = 0;
		string classDescription;

		// display each class, highlighting the current selection
		for (HeroClass& hc : HeroClass::_listClasses)
		{
			if (curSelection == curPosition) {
				classDescription = hc._description;
				screen += "\t > [ " + hc._name + " ]\n\n";
			}
			else
				screen += "\t" + hc._name + "\n\n";
			curPosition++;
		}
		screen += "\n\t" + classDescription;

		// display a resume of the class under the selector

		CleanScreen();
		cout << screen;

		// get user input
		switch (GetInputKeyVerticalSelection()) {
		case VerticalSelection::DOWN:
			curSelection++;
			curSelection = min(curSelection, (int)HeroClass::_listClasses.size() - 1);
			break;
		case VerticalSelection::UP:
			curSelection--;
			curSelection = max(0, curSelection);
			break;
		case VerticalSelection::SELECT:
			hero.SetClass(HeroClass::_listClasses[curSelection]);

			header += "\t" + GetT("HERO_CLASS") + hero._class._name;
			return header;
		}
	}
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


// wait for a top/left or bottom/right key entry by the user and return the direction for next display
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


// wait for an entry from user : left, right, or enter only; used for selectors (languages, heroes, class for custom hero)
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