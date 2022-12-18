// this is the class of the HMI : all display will be done there, as well as functions to get user keyboards entries.
// it also contains an history off all consecutive sequences, and the methodes to navigate between.

#pragma once

#include <vector>
#include <string>

using namespace std;

class Hero;

enum class NextDisplay {
    FORWARD,
    BACKWARD
};

enum class VerticalSelection {
    UP,
    DOWN,
    SELECT
};

class UIManager
{
    // singleton patern
private:
    static UIManager* _instance;

public:
    static UIManager& GetInstance()
    {
        static UIManager instance;
        return instance;
    };

    // private constructor (singleton)
private:
    UIManager() {};
    ~UIManager() {};

    // Attributes
private:
    int _currentTurn = -1;
    string _header = "";
    string _summary = "";
    vector<string> _history;

    // Methodes
public:
    static bool IsNumber(const string& s);

    void SelectLanguage();
    void SetupConsoleSize();
    void DisplayBattleEnd();
    void DisplayBattleStart(Hero&, Hero&);
    void PrepareGameOverScreen(Hero&, Hero&);
    void DisplayPreviousTurn(bool forceLastTurn = false);
    void DrawNewTurn(Hero&, Hero&);
    void LaunchEditForm(Hero&, int numHero);
    void LogSummary(const string&);
    void LogTurnCount(int);
    void SelectHero(Hero&, int numHero, const string& firstHeroClass = "", const string& firstHeroName = "");
    bool DisplayNextTurn();
    bool AskUserYesNoQuestion(const string& questionKey);
    string DisplayClassSelector(Hero&, int numHero);
    NextDisplay GetInputKeyForwardBackward();
    VerticalSelection GetInputKeyVerticalSelection();
private:
    string DrawNiceLine();
    string DrawStats(Hero& left, Hero& right);
};

inline static UIManager& UI() { return UIManager::GetInstance(); };