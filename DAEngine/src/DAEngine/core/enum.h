#pragma once
#include <unordered_map>
#include <iostream>

#define CREATE_ENUM(name) name,
#define CREATE_ENUM_DEFINED(name, x) name = x,

#define CREATE_STRINGS(name) #name,
#define CREATE_STRINGS_DEFINED(name, value) {value, #name},

#define ENUM_CREATE(name, func) enum class name { func(CREATE_ENUM) }; \
static const char* const* get##name##Names() { static const char* str[] = {func(CREATE_STRINGS)}; return str; }


#define ENUM_CREATE_DEFINED(name, func) enum class name { func(CREATE_ENUM) }; \
static std::unordered_map<int, std::string> get##name##Names() {static std::unordered_map<int, std::string> strs = {func(CREATE_STRINGS_DEFINED)}; return strs; };


/*
// Examples
#define FRUITS(fruit) \
  fruit(Apple)        \
  fruit(Orange)       \
  fruit(Banana)

ENUM_CREATE(EFruit, FRUITS);


// Define the enum using CREATE_ENUM
enum class Fruit {
	FRUITS(CREATE_ENUM)
};

// Create an array of string representations
const char* fruitStrings[] = {
	FRUITS(CREATE_STRINGS)
};

#define CARS(car) \
	car(Honda, 2) \
	car(Toyota, 3) \
	car(Mazda, 5) 

ENUM_CREATE_DEFINED(ECars, CARS);

std::unordered_map<int, std::string> ECarNames = {
	CARS(CREATE_STRINGS_DEFINED)
};

*/