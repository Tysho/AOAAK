// reads stocks and gives access to resources from csv files in "Settings" folder

#pragma once

using namespace std;

class Hero;

class ResourcesManager
{
public:
	static vector<Hero*> _listHeroes;	// catalog of all available Heroes (hero picker at the beginning of the battle)

private:
	static map<string, string> _catLanguageResources;	// catalog of all language resources (meaning all texts displayed in the app)

public:
	static void LoadLanguage(int langueId);
	static void LoadHeroes();
	static string GetText(const char* key);
	static vector<string> GetAvailableLanguages();
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
inline static string GetT(const string& key) { return ResourcesManager::GetText(key.c_str()); };