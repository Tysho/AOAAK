#pragma once

#include <algorithm>
#include <conio.h>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>
#include <windows.h> // for CharToOemA

using namespace std;
#define WINDOW_X 100
#define WINDOW_Y 180
#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 700

// default Hero Classes are defined in Hero.cpp

// default Heroes proposed are defined in file "Heroes.csv"

// cooldown and duration are in turns
// accuracy are in %

#define STUN_DURATION 1
#define STUN_ACCURACY 20
#define STUN_COOLDOWN 5

#define CHARGE_DAMAGE_MULTIPLIER 2
#define CHARGE_DURATION 1
#define CHARGE_ACCURACY 50
#define CHARGE_COOLDOWN 3

#define ARROW_KNEE_DAMAGE_MUTLIPLIER 0.5
#define ARROW_KNEE_DURATION 3
#define ARROW_KNEE_ACCURACY 33
#define ARROW_KNEE_COOLDOWN 4