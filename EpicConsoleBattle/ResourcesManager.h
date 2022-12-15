// reads stocks and gives access to resources from csv files in "Settings" folder
#pragma once

using namespace std;

class Hero;

class ResourcesManager
{
public:
	static vector<Hero*> _listHeroes;	// catalog of all available Heroes (hero picker at the beginning of the battle)
	static string _currentLanguage;
private:
	static map<string, map<string, string>> _catLanguageResources;	// string[KEY][LANG] : catalog of all language resources (meaning all texts displayed in the app)

public:
	static bool LoadHeroes();
	static bool LoadLanguages();
	static string GetText(const string& key, const string& lang);
	static vector<string> GetListAvailableLanguages();
};

// Allow to format a string and return it in a single line fonction
template<typename ... Args>
inline static string Format(const string& fmt, Args... args)
{
	size_t bufferSize = 1000;
	char* buffer = new char[bufferSize];
	int n = snprintf(buffer, bufferSize, fmt.c_str(), args...);
	_ASSERT(n >= 0 and n < (int)bufferSize - 1 && "check fmt_str output");

	string str(buffer);
	delete[] buffer;
	return str;
}

// shorter call to ResourceManager::GetText
inline static string GetT(const string& key, const string& lang = ResourcesManager::_currentLanguage) { return ResourcesManager::GetText(key, lang); };