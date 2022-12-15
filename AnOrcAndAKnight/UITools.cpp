#include "UITools.h"

#include "Fighter.h"
#include <iostream>

#define SCREEN_WIDTH 111

// add an empty line with separator on the middle
static void AddEmtpyLine() {
	cout << "\t";
	for (int i = 0; i < SCREEN_WIDTH - 1; i++) {
		if (i == (SCREEN_WIDTH - 5) / 2) { // -5 > -4 for left margin and -1 for separator
			cout << "|";
			i++;
		}
		cout << " ";
	}
	cout << "\n\t";
}

/// <summary>
/// Draw life or shield bar for both Fighters, return true if something was actually drawn
/// </summary>
/// <param name="isHp">true > HP / false > shield </param>
/// <param name="left"></param>
/// <param name="right"></param>
/// <returns></returns>
bool DrawBar(bool isHp, const Stats& left, const Stats& right) 
{
	int barreWidth = (SCREEN_WIDTH - 24) / 2;	// 24 = 10 for middle space each side of the separator "|" + 4 space for left margin

	int leftCur = isHp ? left._currentHP : left._currentShield;
	int leftMax = isHp ? left._maxHP : left._maxShield;
	int rightCur = isHp ? right._currentHP : right._currentShield;
	int rightMax = isHp ? right._maxHP : right._maxShield;

	// no shield for both figthers ?
	if (isHp == false && leftMax + rightMax == 0)
		return false;

	// left bar (if shield bar for no shield fighter, don't display)
	if (isHp || leftMax > 0) {
		int hpPlain = leftCur * barreWidth / leftMax;
		for (int i = 0; i < hpPlain; i++)
			cout << char(219);
		for (int i = hpPlain; i < barreWidth; i++)
			cout << char(176);
	}
	else {
		for (int i = 0; i < barreWidth; i++)
			cout << "";
	}

	// separator
	cout << "          |          ";

	// right bar (if shield bar for no shield fighter, don't display)

	if (isHp || rightMax > 0) {
		int hpPlain = rightCur * barreWidth / rightMax;
		for (int i = 0; i < hpPlain; i++)
			cout << char(219);
		for (int i = hpPlain; i < barreWidth; i++)
			cout << char(176);
	}
	cout << "\n\t";
	return true;
}


void DisplayValue(bool isHp, const Stats& left, const Stats& right)
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

	cout << leftHpCounter;
	for (int i = leftHpCounter.size(); i < SCREEN_WIDTH - rightHpCounter.size() - 4; i++) {
		if (i == (SCREEN_WIDTH - 5) / 2) {	// -5 > -1 for separator and -4 for left margin
			cout << "|";
			i++;
		}
		cout << " ";
	}
	cout << rightHpCounter + "\n";
}

void UITools::DrawStats(const Fighter& left, const Fighter& right)
{
	cout << "\n\t";
	string test;

	// names of the fighters
	cout << left._name;
	int separatorLength = SCREEN_WIDTH - left._name.size() - right._name.size() - 4; // - 4 for left margin
	for (int i = 0; i < separatorLength; i++)
	{
		if(i)
		cout << " ";
	}
	cout << right._name;
	cout << "\n\t";

	// LIFE BARS
	DrawBar(true, left._stats, right._stats);

	// HP VALUES
	DisplayValue(true, left._stats, right._stats);

	AddEmtpyLine();

	// SHIELD BARS
	bool shieldsDrawn = DrawBar(false, left._stats, right._stats);
	
	// SHIELDS VALUES
	if (shieldsDrawn) {
		DisplayValue(false, left._stats, right._stats);
		AddEmtpyLine();
	}
}