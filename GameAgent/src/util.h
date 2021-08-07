#pragma once

#include <vector>
#include <set>
#include <fstream>
#include <sstream>
#include <bitset>
#include <algorithm>
#include "automata.h"
#include "logic.h"

class util
{
public:
	static constexpr unsigned int TESTCASE_NUMBER = 1000;

	~util() = default;

	util(const util &) = delete;
	void operator=(const util &) = delete;

	static std::string printState(std::bitset<128U> state);

    static std::string getAllValidMovesStr(const std::string& stateStr, bool isBlackTurn);
    static std::string getAINextMove(automata & ai, const std::string& stateStr, bool isBlackTurn, unsigned int moveLeft, int timeLeft);
    static std::vector<std::string> split(const std::string &str, char del);
    static std::string route(const std::string& requestStr, automata & ai);
	
private:
	util() = default;

    static logic::bitState stringState2BitState(std::string const & stateStr);

};

