#pragma once

#include <string>
#include <stdexcept>
#include <map>
#include <vector>

using namespace std;

class Hero;

class ResourcesManager
{
public:
	static vector<Hero*> _listHeroes;				// catalog of all available Heroes 

private:
	static map<string, string> _catLanguageResources;	// catalog of all language resources (= texts displayed in the app)

public:
	static void LoadLanguage(const string& Langue);
	static void LoadHeroes();
	static void SaveHeroes(const Hero& Hero);
	static string GetText(const char* key);
	static vector<string> GetAvailableLanguages();
};

template<typename ... Args>
inline static string Format(const string& fmt, Args... args)
{
	size_t bufferSize = 1000;
	char* buffer = new char[bufferSize];
	int n = snprintf(buffer, bufferSize, fmt.c_str(), args...);
	_ASSERT(n >= 0 and n < (int)bufferSize - 1 && "check fmt_str output");

	string str(buffer);
	delete buffer;
	return str;
}

inline static string GetT(const string& key) { return ResourcesManager::GetText(key.c_str()); }; // shorter