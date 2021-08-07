#pragma once

#include "logic.h"
#include <bitset>
#include <functional>
#include <queue>
#include <chrono>
#include <climits>
#include "ThreadPool.h"

class automata
{
public:
	typedef std::function<int(logic::bitState&, bool, int&, int&)> heuristic;

	struct maxTopReturn {
		logic::weightedActionState bestAs;
		int bestV;
		bool completed;
	};

	static const std::bitset<128U> MASKS_BLACK;
	static const std::bitset<128U> MASKS_WHITE;

	static constexpr int MIDDLE_H[61] = {
		   1, 1, 1, 1, 1,
		   1, 2, 2, 2, 2, 1,
		   1, 2, 3, 3, 3, 2, 1,
		   1, 2, 3, 4, 4, 3, 2, 1,
		   1, 2, 3, 4, 5, 4, 3, 2, 1,
		   1, 2, 3, 4, 4, 3, 2, 1,
		   1, 2, 3, 3, 3, 2, 1,
		   1, 2, 2, 2, 2, 1,
		   1, 1, 1, 1, 1,
	};

	automata();

	automata(heuristic h);

	~automata();


	logic::weightedActionState getBestMove(logic::bitState& state, bool isBlack, unsigned int moveLeft, int timeLeft);

	static int h1(logic::bitState& state, bool isBlack, int& blackLost, int& whiteLost);

	static int h2(logic::bitState&, bool, int& blackLost, int& whiteLost);

private:
	const static int threadNumber = 4;
	ThreadPool threadPool;
	std::condition_variable cv;
	std::mutex blocker;
	std::mutex mtQ;
	std::mutex mtVal;
	bool returned = false;
	int counter = 0;
	std::chrono::high_resolution_clock::time_point startTime;
	// clock_t clock;
	heuristic hn;


	logic::weightedActionState alphaBeta(logic::bitState& state, bool isBlack, unsigned int& moveLeft, int& timeLeft);
	maxTopReturn maxTop(std::multiset<logic::weightedActionState, std::greater<logic::weightedActionState>>& actionStates,
		logic::bitState& state, bool isBlack, unsigned int depth, unsigned int moveLeft, int & timeLeft, int alpha, int beta);
	int maxValue(logic::bitState& state, bool isBlack, unsigned int depth, unsigned int moveLeft, int alpha, int beta);
	int minValue(logic::bitState& state, bool isBlack, unsigned int depth, unsigned int moveLeft, int alpha, int beta);
};
