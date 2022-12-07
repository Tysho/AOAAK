#pragma once

#include <string>
#include <vector>

using namespace std;

class Fighter;

class UITools
{
public:
	static int _currentTurn;
	static string _header;
	static string _summary;
	static vector<string> _history;

public:
	static void DrawTurn(const Fighter& left, const Fighter& right);
	static void LogSummary(const string& text);
	static void LogHeader(const string& text);
	static bool DisplayNextTurn();
	static void DisplayPreviousTurn(bool forceLastTurn = false);
private:
	static string DrawStats(const Fighter& left, const Fighter& right);
};

