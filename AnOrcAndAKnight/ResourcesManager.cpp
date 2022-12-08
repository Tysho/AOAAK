#include "ResourcesManager.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

map<string, string> ResourcesManager::_catLanguageResources;
map<string, Hero> ResourcesManager::_catHeroes;


vector<string> ResourcesManager::GetAvailableLanguages()
{
    ifstream languageFile("Settings/Language.csv");
    if (languageFile.is_open() == false) {
        cout << "Unable to open file !";
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
        tokens.push_back(intermediate);

    // remove first value ("LANGUAGE" key)
    tokens.erase(tokens.begin());

    languageFile.close();
    return tokens;
}

void ResourcesManager::LoadLanguage(const string& curLangue)
{
    ifstream languageFile("Settings/Language.csv");
    if (languageFile.is_open() == false) {
        cout << "Unable to open file !";
        return;
    }

    // read first line to get position of current language
    string line;
    getline(languageFile, line);

    // tokenize the line to get all languages
    stringstream checkLanguage(line);
    vector<string> tokens = GetAvailableLanguages();
    int languePos = 1;
    
    // try to find current langue in all defined languages
    for (int i = 0; i < tokens.size(); i++) {
        if (tokens[i] != curLangue)
            continue;
         
        languePos = i + 1;
        break;
    }

    // get all texts in current langue with key
    while (getline(languageFile, line)) {
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

            tokens.push_back(s);
        }

        _catLanguageResources.insert(make_pair(tokens[0], tokens[languePos]));
    }
    languageFile.close();
}

void ResourcesManager::LoadHeroes()
{
}

void ResourcesManager::SaveHeroes(const Hero& Hero)
{
}

string ResourcesManager::GetText(const char* key)
{
    if (_catLanguageResources.find(key) != _catLanguageResources.end()){}
        return _catLanguageResources[key];
    return key;
}