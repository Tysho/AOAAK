#include "Settings.h"
#include "ResourcesManager.h"

#include "Hero.h"
#include "UIManager.h"

map<string, map<string, string>> ResourcesManager::_catLanguageResources;
vector<Hero*> ResourcesManager::_listHeroes;
string ResourcesManager::_currentLanguage;


string ToAscii(const string& text)
{
    const char* s = text.c_str();
    char buffer[10000];
    CharToOemA(s, buffer);
    string str(buffer);
    return str;
}

vector<string> ResourcesManager::GetListAvailableLanguages()
{
    ifstream languageFile("Settings/Language.csv");
    if (languageFile.is_open() == false) {
        std::cerr << "Unable to open file !\n";
        exit(1);
        return vector<string>();
    }

    // read first line to get all availables language
    string line;
    getline(languageFile, line);

    // tokenize the line to get all languages
    stringstream checkLanguage(line);
    string intermediate;
    vector<string> tokens;
    while (getline(checkLanguage, intermediate, ';'))
        tokens.push_back(ToAscii(intermediate));

    // remove first value ("LANGUAGE" key)
    tokens.erase(tokens.begin());

    ResourcesManager::_currentLanguage = tokens[0];
    
    languageFile.close();
    return tokens;
}

bool ResourcesManager::LoadLanguages()
{
    vector<string>&& listLanguages = GetListAvailableLanguages();

    if (listLanguages.size() == 0) {
        std::cerr << "Unable to open file !";
        return false;
    }

    ifstream languageFile("Settings/Language.csv");
    if (languageFile.is_open() == false) {
        std::cerr << "Unable to open file !";
        return false;
    }

    // read first line to get position of current language
    string line;
    getline(languageFile, line);

    // get all texts in current langue with key
    vector<string> tokens;
    while (getline(languageFile, line)) {
        // empty line ?
        if (line == "")
            continue;

        tokens.clear();
        stringstream texts(line);
        string s;
        while (getline(texts, s, ';')) {
            // replace "\n" by real line breaks
            size_t pos;
            while ((pos = s.find("\\n")) != string::npos)
                s.replace(pos, 2, "\n");

            // replace "\t" by real tabs
            pos = -1;
            while ((pos = s.find("\\t")) != string::npos)
                s.replace(pos, 2, "\t");
            s = ToAscii(s);
            tokens.push_back(s);
        }

        // stock in catalog
        string key = tokens[0];
        _catLanguageResources.insert({ key, map<string, string>() });
        for (int i = 0; i < (int)listLanguages.size(); i++) {
            _catLanguageResources[key].insert({ listLanguages[i], tokens[i + 1] });
        }
    }
    languageFile.close();
    
    return true;
}

bool ResourcesManager::LoadHeroes()
{
    ifstream heroesFile("Settings/Heroes.csv");
    if (heroesFile.is_open() == false) {
        std::cerr << "Unable to open file !";
        return false;
    }

    // ignore first line (information to complete it with Excel
    string line;
    getline(heroesFile, line);

    string parseError;
    while (getline(heroesFile, line)) {
        // tokenize the line to get all distinct values
        stringstream stream(line);
        string val;
        vector<string> tokens;
        while (getline(stream, val, ';'))
            tokens.push_back(val);

        // HERO CLASS
        bool classFound = false;
        HeroClass heroClass = HeroClass::_listClasses[0];
        for (auto& hc : HeroClass::_listClasses) {
            string className = GetT(tokens[0]);
            if (hc._name != className)
                continue;

            classFound = true;
            heroClass = hc;
            break;
        }
        if(classFound == false)
            parseError += "Invalid Class Name for Hero : \"" + tokens[0] + "\" could not be found. \"" + heroClass._name + "\" used by default\n";

        // HERO NAME
        string name = ToAscii(tokens[1]);
        
        // HERO HP
        int hp = 50;
        if (UI().IsNumber(tokens[2]) == false || atoi(tokens[2].c_str()) == 0)
            parseError += "Invalid HP value for Hero [" + to_string(_listHeroes.size() + 1) + "] : must be number, is \"" + tokens[2] + "\" ; 50 used by default\n";
        else
            hp = atoi(tokens[2].c_str());

        // HERO SHIELD
        int shield = 0;
        if (tokens[3] != "" && UI().IsNumber(tokens[3]) == false)
            parseError += "Invalid Shield value for Hero [" + to_string(_listHeroes.size() + 1) + "] : must be number, is \"" + tokens[3] + "\" ; 0 used by default\n";
        else
            shield = atoi(tokens[3].c_str());

        // HERO WEAPON'S NAME
        Weapon weapon;
        weapon._name = GetT(tokens[4]);

        // HERO WEAPON'S DAMAGE
        weapon._damages = 5;
        if (tokens[5] != "" && UI().IsNumber(tokens[5]) == false)
            parseError += "Invalid damage value for Hero [" + to_string(_listHeroes.size() + 1) + "] : must be number, is \"" + tokens[5] + "\" ; 5 used by default\n";
        else
            weapon._damages = atoi(tokens[5].c_str());

        Hero* pHero = new Hero(heroClass, name, Stats(hp, shield));
        pHero->EquipWeapon(weapon);
        _listHeroes.push_back(pHero);
    }

    if (parseError == "") 
        return true;
    
    // parsing error
    std::cerr << parseError;
    return false;
}

string ResourcesManager::GetText(const string& key, const string& language)
{
    if (_catLanguageResources.find(key) == _catLanguageResources.end())
        return key;
    
    string text = _catLanguageResources[key][language];
    return text != "" ? text : key;
}