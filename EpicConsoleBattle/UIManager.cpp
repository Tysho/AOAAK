// this is the class of the HMI : all display will be done there, as well as functions to get user keyboards entries.
// it also contains an history off all consecutive sequences, and the methodes to navigate between.

#include "UIManager.h"
#include "Settings.h"

#include "Hero.h"
#include "ResourcesManager.h"
#include "Effect.h"

enum class SeparatorType {
	NONE,
	BEGINNING,
	MIDDLE,
	END
};
enum class InstructionType {
	NAVIGATE,
	FORM_SELECT
};

#pragma region Inline fct tools

// clear console
inline void CleanScreen() { system("cls"); };


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


// static methode return true if the string contains only numeric characters ("-" and "./," not allowed)
bool UIManager::IsNumber(const string& s)
{
	std::string::const_iterator it = s.begin();
	while (it != s.end() && std::isdigit(*it)) ++it;
	return !s.empty() && it == s.end();
}


#pragma region static local functions (tools)

// count number of line in a string (=number of "\t"
int GetNbLines(const string& s)
{
	int nbLines = std::count(s.begin(), s.end(), '\n') + 1;
	return nbLines;
}


// create a string with empty line until the bottom of the screen, considering the string starts at the very first line)
string AddEmptyLinesUntilBottomScreen(const string& s, int stopBefore = 1)
{
	string temp = s;
	int nbLines = GetNbLines(s);
	while (nbLines < NB_LINES_DISPLAYABLE_IN_CONSOLE) {
		nbLines++;
		temp += "\n";
	}
	return temp;
}


// display selector with current selection highlighted
string DisplayHeroSelector(int& curSelection, const string& firstHeroName = "", const string& firstHeroClass = "")
{
	string screen;

	int curPosition = 0;
	for (Hero* p : ResourcesManager::_listHeroes)
	{
		if (p->_name == firstHeroName && p->GetClass() == firstHeroClass)
		{
			if (curPosition == curSelection)
				curSelection++;
			curPosition++;
			continue;
		}

		if (curSelection == curPosition)
			screen += "\t > [ " + p->_name + " (" + p->GetClass() + ") ]\n\n";
		else
			screen += "\t" + p->_name + " (" + p->GetClass() + ")\n\n";
		curPosition++;
	}

	if (curSelection == curPosition)
		screen += "\t > [ " + GetT("CUSTOM_HERO") + " ]\n\n";
	else
		screen += "\t" + GetT("CUSTOM_HERO") + "\n\n";

	return screen;
}


// display screen while user don't tape text then enter
string ReadText(const string& screen) {
	string s;
	do {
		CleanScreen();
		cout << screen;
		getline(cin, s);
	} while (s == "");
	return s;
};


// display screen while user don't enter a integer value
int ReadNumber(const string& screen, bool canBeZero = false) 
{
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
string AddEmtpyLine(SeparatorType separator = SeparatorType::NONE)
{
	string display;

	if (separator == SeparatorType::MIDDLE || separator == SeparatorType::END)
		display = AddEmtpyLine();
	else
		display = "\t";

	string c = separator != SeparatorType::NONE ? "*" : " ";
	for (int i = 0; i < SCREEN_WIDTH - 1; i++) {
		if (i == (SCREEN_WIDTH - 5) / 2) { // -5 > -4 for left margin and -1 for separator
			display += "|";
			i++;
		}
		display += c;
	}

	if (separator == SeparatorType::MIDDLE || separator == SeparatorType::BEGINNING)
		display += "\n" + AddEmtpyLine();
	else
		display += "\n\t";

	return display;
}


// Draw life or shield bar for both Heroes stats
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


// Get a line which display curValue / maxValue for both Heroes stats
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
	for (int i = (int)leftHpCounter.size(); i < SCREEN_WIDTH - (int)rightHpCounter.size() - 4; i++) {
		if (i == (SCREEN_WIDTH - 5) / 2) {	// -5 = -1 for separator and -4 for left margin
			result += "|";
			i++;
		}
		result += " ";
	}
	result += rightHpCounter + "\n";

	return result;
}


// Get a line which display effects affecting the heroes (one call per effect for both)
string DisplayEffectOrSkill(const string& effectLeft, const string& effectRight)
{
	string result = effectLeft;
	for (int i = (int)effectLeft.size(); i < SCREEN_WIDTH - (int)effectRight.size() - 4; i++) {
		if (i == (SCREEN_WIDTH - 5) / 2) {	// -5 = -1 for separator and -4 for left margin
			result += "|";
			i++;
		}
		result += " ";
	}
	result += effectRight + "\n";
	return result;
}


// display action the user can do in the current state
string DisplayInstructions(InstructionType type) 
{
	string right, left, up, down;
	right += char(16);
	left += char(17);
	up += char(30);
	down += char(31);

	switch (type) {
	case InstructionType::FORM_SELECT:
		return "\t\t" + up + " | " + down + GetT("INSTRUCTIONS_MOVE") + "\t\t" + GetT("INSTRUCTIONS_SELECT");
	case InstructionType::NAVIGATE:
		return "\t\t" + left + " | " + up + GetT("INSTRUCTIONS_DISPLAY_PREVIOUS") + "\t\t" + right + " | " + down + GetT("INSTRUCTIONS_DISPLAY_NEXT");
	default:
		return "";
	}
}


// prepare the name to be written on a tombstone (fit and center it in a 39-characters string)
string CarveNameToTombstone(string name)
{
	// name too long ? we cut
	if (name.size() >= 39)
		name = name.substr(0, 35) + "...";
	else {
		// we center the name in the 39-character-wide space
		int nbEmptySpace = 39 - (int)name.size();
		for (int i = 0; i < nbEmptySpace / 2; i++)
			name = " " + name;
		while (name.size() < 39)
			name += " ";
	}
	return name;
}


// cut a string between separator substring
vector<string> SplitString(string source, const string& separator)
{
	vector<string> result;

	size_t pos = 0;
	string token;
	while ((pos = source.find(separator)) != string::npos) {
		token = source.substr(0, pos);
		result.push_back(token);
		source.erase(0, pos + separator.length());
	}
	result.push_back(source);
	return result;
}


// from two multiline string (ascii art), we made one with the two image side to side
string MergeTombstone(const string& ts1, const string& ts2)
{
	vector<string>&& listLines1 = SplitString(ts1, "\n");
	vector<string>&& listLines2 = SplitString(ts2, "\n");

	string twoTombstone = "\n";
	for (int i = 0; i < (int)listLines1.size(); i++) {
		// draw line of left tombstone
		twoTombstone += listLines1[i];

		// go to the position of the second ascii image (70 char from left border)
		for (int j = listLines1[i].size(); j < 60; j++)
			twoTombstone += " ";

		// draw line of right tombstone
		twoTombstone += listLines2[i] + "\n";
	}
	return twoTombstone;
}

#pragma endregion


// display selector to chose the language
void UIManager::SelectLanguage()
{
	// get available languages
	vector<string>&& listLang = ResourcesManager::GetListAvailableLanguages();

	// stocking current selected language to display instructions properly
	string curLanguage = "";

	// display selector
	int curSelection = 0;

	string sep = " | ";
	string arrowUp, arrowDown;
	arrowUp += char(30);
	arrowDown += char(31);

	while (1) {
		CleanScreen();
		string screen = DrawNiceLine() + "\n\n";

		string title = "\n \
                    /$$$$$$$$ /$$$$$$$  /$$$$$$  /$$$$$$\n\
                   | $$_____/| $$__  $$|_  $$_/ /$$__  $$\n\
                   | $$      | $$  \\ $$  | $$  | $$  \\__/\n\
                   | $$$$$   | $$$$$$$/  | $$  | $$\n\
                   | $$__/   | $$____/   | $$  | $$\n\
                   | $$      | $$        | $$  | $$    $$\n\
                   | $$$$$$$$| $$       /$$$$$$|  $$$$$$/\n\
                   |________/|__/      |______/ \\______/\n\
\n\
\n\
\n\
	  /$$$$$$   /$$$$$$  /$$   /$$  /$$$$$$   /$$$$$$  /$$       /$$$$$$$$\n\
	 /$$__  $$ /$$__  $$| $$$ | $$ /$$__  $$ /$$__  $$| $$      | $$_____/\n\
	| $$  \\__/| $$  \\ $$| $$$$| $$| $$  \\__/| $$  \\ $$| $$      | $$\n\
	| $$      | $$  | $$| $$ $$ $$|  $$$$$$ | $$  | $$| $$      | $$$$$\n\
	| $$      | $$  | $$| $$  $$$$ \\____  $$| $$  | $$| $$      | $$__/\n\
	| $$    $$| $$  | $$| $$\\  $$$ /$$  \\ $$| $$  | $$| $$      | $$\n\
	|  $$$$$$/|  $$$$$$/| $$ \\  $$|  $$$$$$/|  $$$$$$/| $$$$$$$$| $$$$$$$$\n\
	 \\______/  \\______/ |__/  \\__/ \\______/  \\______/ |________/|________/\n\
\n\
\n\
\n\
             /$$$$$$$   /$$$$$$  /$$$$$$$$ /$$$$$$$$ /$$       /$$$$$$$$\n\
            | $$__  $$ /$$__  $$|__  $$__/|__  $$__/| $$      | $$_____/\n\
            | $$  \\ $$| $$  \\ $$   | $$      | $$   | $$      | $$\n\
            | $$$$$$$ | $$$$$$$$   | $$      | $$   | $$      | $$$$$\n\
            | $$__  $$| $$__  $$   | $$      | $$   | $$      | $$__/\n\
            | $$  \\ $$| $$  | $$   | $$      | $$   | $$      | $$\n\
            | $$$$$$$/| $$  | $$   | $$      | $$   | $$$$$$$$| $$$$$$$$\n\
            |_______/ |__/  |__/   |__/      |__/   |________/|________/ \n\n\n";

		screen += title + "\n\n";
		int curPosition = 0;
		string curLanguage = ""; // stocking current selected language to display instructions properly
		for (string lang : listLang)
		{
			if (curSelection == curPosition) {
				curLanguage = lang;
				screen += "\t > [ " + lang + " ]\n\n";
			}
			else
				screen += "\t" + lang + "\n\n";
			curPosition++;
		}

		// reaching last displayed line...
		screen = AddEmptyLinesUntilBottomScreen(screen);

		// ... to display instructions (can't use DisplayInstructions() because we need to specify a language
		screen += "\t\t" + arrowUp;
		screen += " | " + arrowDown + GetT("INSTRUCTIONS_MOVE", curLanguage);
		screen += "\t\t" + GetT("INSTRUCTIONS_SELECT", curLanguage);
		screen += "\n" + DrawNiceLine();
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
			ResourcesManager::_currentLanguage = curLanguage;	// language id start at 1
			return;
		}
	}
}


// move and resize Console for the content to fit perfectly
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
	_summary = DrawNiceLine() + "\n\n" + _summary;
	_summary = AddEmptyLinesUntilBottomScreen(_summary) + "\n" + DrawNiceLine();
	cout << _summary;
	_history.push_back(_summary);
	_currentTurn = int(_history.size() - 1);
	_summary = "";
}


// First display of he battle
void UIManager::DisplayBattleStart(Hero& left, Hero& right)
{
	LogSummary("\n\t\t" + GetT("BATTLE_BEGIN"));
	DrawNewTurn(left, right);
}


// Generate the game over in _summary, ready to be display
void UIManager::PrepareGameOverScreen(Hero& hero1, Hero& hero2)
{
	string tombstone1, tombstone2;

	if (hero1._stats._currentHP == 0) {
		string name = CarveNameToTombstone(hero1._name);
		tombstone1 = Format(GetT("GAME_OVER"), name.c_str());
	}

	if (hero2._stats._currentHP == 0) {
		string name = CarveNameToTombstone(hero2._name);
		tombstone2 = Format(GetT("GAME_OVER"), name.c_str());
	}

	if (tombstone1.size() > 0 && tombstone2.size() > 0) {
		string twoTombstones = MergeTombstone(tombstone1, tombstone2);
		UI().LogSummary(twoTombstones);
	}
	else
		UI().LogSummary(tombstone1 + tombstone2);

	UI().LogSummary("\n\n\t\t" + GetT("END"));
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
	_header += "\n" + log;
}


// display a form to select a hero, the first hero picked is hidden if it's the second pick
void UIManager::SelectHero(Hero& hero, int numHero, const string& firstHeroName, const string& firstHeroClass)
{
	int curSelection = 0;

	// title
	string header = DrawNiceLine() + "\n";

	// announce first champ selected
	if (numHero > 1)
		header += "\n\t" + Format(GetT("FIRST_HERO_PICKED"), firstHeroName.c_str(), firstHeroClass.c_str()) + "\n\n";
	header += "\n" + Format(GetT("HERO_SELECT"), numHero) + "\n\n";

	// waiting for user choice
	while (1) {
		string screen = header + DisplayHeroSelector(curSelection, firstHeroName, firstHeroClass);

		// reaching last displayed line to display instructions
		screen = AddEmptyLinesUntilBottomScreen(screen);
		screen += DisplayInstructions(InstructionType::FORM_SELECT);
		screen += "\n" + DrawNiceLine();

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
			if (pHero->_name == firstHeroName && pHero->GetClass() == firstHeroClass) {
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
			if (pHero->_name == firstHeroName && pHero->GetClass() == firstHeroClass) {
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


// ask for user if he want to start again (return true) or close the app
bool UIManager::SelectRestartOrQuit()
{
	string title = DrawNiceLine() + "\n\n\t" + GetT("RESTART_OR_QUIT") + "\n\n\t";

	int curSelection = 0;
	string arrowUp, arrowDown, sep = " | ";
	arrowUp += char(30);
	arrowDown += char(31);

	while (1) {
		CleanScreen();
		string screen = title;

		if (curSelection == 0)
			screen += "> [ " + GetT("RESTART") + " ]\n\n\t";
		else
			screen += GetT("RESTART") + "\n\n\t";

		if (curSelection == 1)
			screen += "> [ " + GetT("QUIT") + " ]\n\n";
		else
			screen += GetT("QUIT") + "\n\n";

		// reaching last displayed line...
		screen = AddEmptyLinesUntilBottomScreen(screen);

		// ... to display instructions (can't use DisplayInstructions() because we need to specify a language
		screen += "\t\t" + arrowUp;
		screen += " | " + arrowDown + GetT("INSTRUCTIONS_MOVE");
		screen += "\t\t" + GetT("INSTRUCTIONS_SELECT");
		screen += "\n" + DrawNiceLine();
		cout << screen;

		// get user input
		switch (GetInputKeyVerticalSelection()) {
		case VerticalSelection::DOWN:
			curSelection++;
			curSelection = min(curSelection, 1);
			break;
		case VerticalSelection::UP:
			curSelection--;
			curSelection = max(0, curSelection);
			break;
		case VerticalSelection::SELECT:
			if (curSelection == 0)
				return true;
			return false;
		}
	}
}


// draw a beautiful line in console (for first and last line of screen display)
string UIManager::DrawNiceLine()
{
	string ret, a, b, c, d;
	a += char(16);
	b += char(17);
	c += char(30);
	d += char(31);
	for (int i = 0; i < 40; i++)
		ret += b + c + d + a;
	return ret;
}


// display hp/shield, skills cooldown and effect of both champs
string UIManager::DrawStats(Hero& left, Hero& right)
{
	string display = AddEmtpyLine(SeparatorType::BEGINNING);

	// names of the Heroes
	display += left._name;

	int begin = (int)left._name.size();
	int end = SCREEN_WIDTH - (int)right._name.size() - 4; // 4 because of first tab
	for (int i = begin; i < end; i++) {
		if (i == (SCREEN_WIDTH - 5) / 2) {
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
		display += AddEmtpyLine(SeparatorType::MIDDLE);
	}
	else
		display += AddEmtpyLine(SeparatorType::MIDDLE);

	// SKILLS COOLDOWN
	int maxSkillsNb = max(left.GetNbSkills(), right.GetNbSkills());
	int maxEffectsNb = max(left.GetNbEffects(), right.GetNbEffects());
	for (int i = 0; i < maxSkillsNb; i++) {
		string skillLeft = left.GetSkillResume(i);
		string skillRight = right.GetSkillResume(i);
		display += DisplayEffectOrSkill(skillLeft, skillRight);

		// add tab for next skill line, or empty line if over to separate
		if (i + 1 == maxSkillsNb && maxEffectsNb > 0)
			display += AddEmtpyLine(SeparatorType::MIDDLE);	// last skill, add separator if there is effects after
		else if (maxEffectsNb == 0)
			display += AddEmtpyLine(SeparatorType::END);	// last skill, no effect to display
		else
			display += AddEmtpyLine();						// empty line before next skill
	}

	// DISPLAY EFFECTS
	for (int i = 0; i < maxEffectsNb; i++) {
		string effectLeft = left.GetEffectResume(i);
		string effectRight = right.GetEffectResume(i);
		display += DisplayEffectOrSkill(effectLeft, effectRight);

		// add tab for next effect line, or empty line if over to separate
		if (i + 1 == maxEffectsNb)
			display += AddEmtpyLine(SeparatorType::END);
		else
			display += AddEmtpyLine();
	}

	return display;
}


// dipslay a calculated turn (using summary for battle details
void UIManager::DrawNewTurn(Hero& left, Hero& right)
{
	_currentTurn++;
	LogTurnCount(_currentTurn);

	// _summary is completed with "playTurn" function in Battle Class
	string screen = _header + DrawStats(left, right) + _summary;

	// reach last line of console...
	screen = AddEmptyLinesUntilBottomScreen(screen);

	// ... to display instructions
	screen += DisplayInstructions(InstructionType::NAVIGATE);
	screen += "\n" + DrawNiceLine();

	// display everything on console
	CleanScreen();
	screen = DrawNiceLine() + screen + DrawNiceLine();
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
	string screen = DrawNiceLine() + "\n" + DisplayClassSelector(hero, heroNumber);

	CleanScreen();
	cout << screen;

	// hero name
	screen += "\n\n\t" + Format(GetT("HERO_NAME"), hero.GetName());
	hero._name = ReadText(screen);
	screen += " " + hero._name;

	// hero health points
	screen += "\n\n\t" + Format(GetT("HERO_HP"), hero.GetName());
	hero._stats._currentHP = hero._stats._maxHP = ReadNumber(screen);
	screen += " " + to_string(hero._stats._currentHP);

	// hero shield value
	screen += "\n\n\t" + Format(GetT("HERO_SHIELD"), hero.GetName());
	hero._stats._currentShield = hero._stats._maxShield = ReadNumber(screen, true); // can be zero
	screen += " " + to_string(hero._stats._currentShield);

	// hero weapon name
	Weapon weapon;
	screen += "\n\n\t" + Format(GetT("HERO_WEAPON_NAME"), hero.GetName());
	weapon._name = ReadText(screen);
	screen += " " + weapon._name;

	// hero weapon damage
	screen += "\n\n\t" + Format(GetT("HERO_WEAPON_DAMAGES"), hero.GetWeaponName());
	weapon._damages = ReadNumber(screen);

	hero.EquipWeapon(weapon);

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

		// display a resume of the class under the selector
		screen += "\n\t" + classDescription;

		// reaching last displayed line...
		screen = AddEmptyLinesUntilBottomScreen(screen);

		// ... to display instructions
		screen += DisplayInstructions(InstructionType::FORM_SELECT);
		screen += "\n" + DrawNiceLine();

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

			header += "\t" + GetT("HERO_CLASS") + hero.GetClass();
			return header;
		}
	}
}


// Get question linked to questionKey, display it to user and return answer
bool UIManager::AskUserYesNoQuestion(const string& questionKey) 
{
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
VerticalSelection UIManager::GetInputKeyVerticalSelection() 
{
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