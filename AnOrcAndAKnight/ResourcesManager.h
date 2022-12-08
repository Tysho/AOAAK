#pragma once

#include <memory>
#include <string>
#include <stdexcept>
#include <map>

#include "Hero.h"

using namespace std;

class Hero;

class ResourcesManager
{
public:
	static void LoadLanguage(const string& Langue);
	static void LoadHeroes();
	static void SaveHeroes(const Hero& Hero);
	static string GetText(const char* key);
	static vector<string> GetAvailableLanguages();


private:
	static map<string, string> _catLanguageResources;	// catalog of all language resources (= texts displayed in the app)
	static map<string, Hero> _catHeroes;			// catalog of all available Heroes 
};

template<typename ... Args>
inline static string Format(const string& fmt, Args... args)
{
	size_t bufferSize = 1000;
	char* buffer = new char[bufferSize];
	int n = snprintf(buffer, bufferSize, fmt.c_str(), args...);
	_ASSERT(n >= 0 and n < (int)bufferSize - 1 && "check fmt_str output");

	std::string str(buffer);
	delete buffer;
	return str;
}

inline static string GetT(const char* key) { return ResourcesManager::GetText(key); }; // shorter