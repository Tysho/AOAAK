// this is the class of the HMI : all display will be done there, as well as functions to get user keyboards entries.
// it also contains an history off all consecutive sequences, and the methodes to navigate between.

#pragma once

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
    int _currentTurn = 0;
    string _header = "";
    string _summary = "";
    vector<string> _history;

    // Methodes
public:
    static bool IsNumber(const string& s);

    void DisplayPreviousTurn(bool forceLastTurn = false);
    void DrawNewTurn(const Hero&, const Hero&);
    void LaunchEditForm(Hero&, int numHero);
    void DisplayBattleStart(const Hero&, const Hero&);
    void LogSummary(const string&);
    void LogTurnCount(int);
    void SelectHero(Hero&, int numHero);
    bool DisplayNextTurn();
    bool AskUserYesNoQuestion(const string& questionKey);
    NextDisplay GetInputKeyForwardBackward();
    VerticalSelection GetInputKeyVerticalSelection();
private:
    string DrawStats(const Hero& left, const Hero& right);
};
