#include "automata.h"

#include <utility>


automata::automata() : threadPool{ threadNumber }, hn{automata::h2}{}

automata::automata(heuristic h) : hn{std::move( h )}, threadPool{ threadNumber }{}

automata::~automata() = default;

int getElapsedMilliseconds(std::chrono::high_resolution_clock::time_point & t1){
	using namespace std::chrono;
	auto t2 = high_resolution_clock::now();
	auto duration = duration_cast<milliseconds>(t2 - t1);
	return duration.count();
}

logic::weightedActionState automata::getBestMove(logic::bitState & state, bool isBlack, unsigned int moveLeft, int timeLeft)
{
	threadPool.wait();
	// clock.restart();
	startTime = std::chrono::high_resolution_clock::now();
	counter = 0;
	if (moveLeft == 0) moveLeft = 4;
	if (timeLeft == 0 || timeLeft > 100) timeLeft = 5;
	timeLeft *= 1000;
	return alphaBeta(state, isBlack, moveLeft, timeLeft);
}

logic::weightedActionState automata::alphaBeta(logic::bitState & state, bool isBlack, unsigned int & moveLeft, int & timeLeft)
{
	auto depth = 3u;
	// int miliSec = clock.getElapsedTime().asMilliseconds();
	int miliSec = getElapsedMilliseconds(startTime);
	int lastLayerUsed = 0;
	logic::weightedActionState best{};
	int bestV = INT_MIN;
	auto actionStates = logic::getAllValidMoveOrdered(state, isBlack);
	do {
		returned = false;
		auto r = maxTop(actionStates, state, isBlack, depth, moveLeft, timeLeft, INT_MIN, INT_MAX);
		if (r.completed) {
			best = r.bestAs;
			bestV = r.bestV;
		}
		// lastLayerUsed = clock.getElapsedTime().asMilliseconds() - miliSec;
		lastLayerUsed = getElapsedMilliseconds(startTime) - miliSec;
		miliSec += lastLayerUsed;

//		std::cout << "Depth: " << depth << std::endl;
//		std::cout << "Time used: " << lastLayerUsed << std::endl;
//		std::cout << "Searched state: " << counter << std::endl;
//		std::cout << "bestV: " << bestV << std::endl;
//		std::cout << "Best Action: " << best.act.act.count << " " << best.act.act.direction << " " << best.act.act.index << std::endl;

		++depth;
	} while (depth < moveLeft && timeLeft - miliSec >(lastLayerUsed << 3));
	return best;
}

automata::maxTopReturn automata::maxTop(std::multiset<logic::weightedActionState, std::greater<logic::weightedActionState>>& actionStates, logic::bitState & state, bool isBlack, unsigned int depth, unsigned int moveLeft, int & timeLeft, int alpha, int beta)
{
	int bestV = INT_MIN;
	logic::weightedActionState bestAs;
	auto iter = actionStates.begin();
	int threadRemain = threadNumber;
	int completedBranch = 0;
	for (auto i = 0u; iter != actionStates.end(); ++i, ++iter) {
		//take a thread
		--threadRemain;
		threadPool.schedule([this, iter, &bestV, &bestAs, &completedBranch, &threadRemain, isBlack, depth, moveLeft, &alpha, beta]() {
			logic::bitState oneState;
			{
				std::unique_lock<std::mutex> lck{ mtQ };
				if (returned) {
					return;
				}
				oneState = iter->state;
			}
			int curV = minValue(oneState, !isBlack, depth - 1, moveLeft - 1, alpha, beta);
			std::unique_lock<std::mutex> lck{ mtQ };
			if (returned)
				return;

			if (curV > bestV) {
				bestV = curV;
				bestAs = *iter;
			}
			alpha = std::max(alpha, bestV);


			//return a thread
			++threadRemain;
			//increase completed branch
			++completedBranch;
			//notify cv thread made change.
			cv.notify_one();
		});
		//wait for there is remaining thread, if timeout then set cutoff variable then return.
		std::unique_lock<std::mutex> blockLck{ blocker };
		// if (!cv.wait_for(blockLck, std::chrono::milliseconds{ timeLeft - clock.getElapsedTime().asMilliseconds() }, [&threadRemain] {return threadRemain > 0; })) {
		if (!cv.wait_for(blockLck, std::chrono::milliseconds{timeLeft - getElapsedMilliseconds(startTime) }, [&threadRemain] {return threadRemain > 0; })) {
			std::unique_lock<std::mutex> lck{ mtQ };
//			std::cout << "<timeout>" << std::endl;
			returned = true;
			return { bestAs, bestV, false };
		}
	}
	//wait for all branch are completed, if timeout then set cutoff variable then return.
	std::unique_lock<std::mutex> blockLck{ blocker };
	// if (!cv.wait_for(blockLck, std::chrono::milliseconds{ timeLeft - clock.getElapsedTime().asMilliseconds() },
	if (!cv.wait_for(blockLck, std::chrono::milliseconds{timeLeft - getElapsedMilliseconds(startTime) },
		[&completedBranch, &actionStates] {return completedBranch == actionStates.size(); })) {
		std::unique_lock<std::mutex> lck{ mtQ };
//		std::cout << "<timeout>" << std::endl;
		returned = true;
		return { bestAs, bestV, false };
	}


	std::unique_lock<std::mutex> lck{ mtQ };
//	std::cout << "<completed>" << std::endl;
	returned = true;

	return { bestAs, bestV, true };
}

int automata::maxValue(logic::bitState & state, bool isBlack, unsigned int depth, unsigned int moveLeft, int alpha, int beta)
{
	if (returned)
		return INT_MAX;
	++counter;
	auto blackLoss = (int)(state._2 >> 58 & 7);
	auto whiteLoss = (int)(state._2 >> 61 & 7);
	if (depth < 1 || moveLeft < 1 || whiteLoss > 5 || blackLoss > 5) {
		return hn(state, isBlack, blackLoss, whiteLoss);
	}
	auto actionStates = logic::getAllValidMoveOrdered(state, isBlack);
	int bestV = INT_MIN;
	for (auto as : actionStates) {
		bestV = std::max(bestV, minValue(as.state, !isBlack, depth - 1, moveLeft - 1, alpha, beta));
		if (bestV >= beta)
			return bestV;
		alpha = std::max(alpha, bestV);
	}
	return bestV;
}

int automata::minValue(logic::bitState & state, bool isBlack, unsigned int depth, unsigned int moveLeft, int alpha, int beta)
{
	if (returned)
		return INT_MIN;
	++counter;
	auto blackLoss = (int)(state._2 >> 58 & 7);
	auto whiteLoss = (int)(state._2 >> 61 & 7);
	if (depth < 1 || moveLeft < 1 || whiteLoss > 5 || blackLoss > 5) {
		return hn(state, !isBlack, blackLoss, whiteLoss);
	}
	auto actionStates = logic::getAllValidMoveOrdered(state, isBlack);
	int bestV = INT_MAX;
	for (auto as : actionStates) {
		bestV = std::min(bestV, maxValue(as.state, !isBlack, depth - 1, moveLeft - 1, alpha, beta));
		if (bestV <= alpha)
			return bestV;
		beta = std::min(beta, bestV);
	}

	return bestV;
}

int automata::h1(logic::bitState & state, bool isBlack, int& blackLost, int& whiteLost)
{
	int scoreMean = 0;
	if (isBlack) {
		if (blackLost == 6) {
			return INT_MIN;
		}
		if (whiteLost == 6) {
			return INT_MAX;
		}
		scoreMean = whiteLost * 100 - blackLost * 150;
	}
	else {
		if (whiteLost == 6) {
			return INT_MAX;
		}
		if (blackLost == 6) {
			return INT_MIN;
		}
		scoreMean = blackLost * 100 - whiteLost * 150;
	}
	int thisMid = 0;
	int thatMid = 0;
	int spyMean = 0;
	int hexMean = 0;
	int adjacency = 0;
	for (auto i = isBlack << 0, j = isBlack ^ 1; i < 122; i += 2, j += 2) {
		if ((i & 64 ? (bool)(state._2 & 1ull << (i & 63)) : (bool)(state._1 & 1ull << i))) {
			thisMid += MIDDLE_H[i >> 1];
			int diff = 0;
			int same = 0;
			for (auto k = 0u; k < 6; ++k) {
				auto adj = logic::MOVE_TABLE[i >> 1][k];
				if (adj == -1) {
					continue;
				}
				if ((adj & 32 ? (bool)(state._2 & 1ull << ((adj & 31) << 1) + isBlack) : (bool)(state._1 & 1ull << (adj << 1) + isBlack))) {
					++same;
					++adjacency;
				}
				if ((adj & 32 ? (bool)(state._2 & 1ull << ((adj & 31) << 1) + (isBlack ^ 1)) : (bool)(state._1 & 1ull << (adj << 1) + (isBlack ^ 1)))) {
					++diff;
				}
			}
			if (diff > 5) {
				++spyMean;
			}
			if (same == 6) {
				++hexMean;
			}

		}
		if ((j & 64 ? (bool)(state._2 & 1ull << (j & 63)) : (bool)(state._1 & 1ull << j))) {
			thatMid += MIDDLE_H[j >> 1];
		}
	}
	return (int)(thisMid * 1.1) - thatMid + (adjacency >> 2) + spyMean + hexMean + scoreMean;
}

int automata::h2(logic::bitState & state, bool isBlack, int& blackLost, int& whiteLost)
{
	//follow your heart
	int scoreMean = 0;
	if (isBlack) {
		if (blackLost == 6) {
			return INT_MIN;
		}
		if (whiteLost == 6) {
			return INT_MAX;
		}
		scoreMean = whiteLost * 100 - blackLost * 150;
	}
	else {
		if (whiteLost == 6) {
			return INT_MAX;
		}
		if (blackLost == 6) {
			return INT_MIN;
		}
		scoreMean = blackLost * 100 - whiteLost * 150;
	}
	int thisMid = 0;
	int thatMid = 0;
	int adjacency = 0;
	int hex = 0;
	int cutMean = 0;
	int danger = 0;
	int trap = 0;
	for (auto i = 0; i < 122; i += 2) {
		int slot = i & 64 ? state._2 >> (i & 63) & 3 : state._1 >> i & 3;
		if (!slot) {
			continue;
		}
		//10 black 01 + 01 = 10, 01 white, 00 + 01 = 01
		if (slot == ((int)isBlack + 1)) {
			int midScore = MIDDLE_H[i >> 1];
			int same = 0;
			int diff = 0;
			auto k = 0u;
			for (; k < 3; ++k) {
				auto adj = logic::MOVE_TABLE[i >> 1][k];
				if (adj == -1) {
					continue;
				}
				int adjslot = adj & 32 ? state._2 >> ((adj & 31) << 1) & 3: state._1 >> (adj << 1) & 3;
				if (!adjslot) {
					continue;
				}
				if (adjslot == ((int)isBlack + 1)) {
					++same;
					++adjacency;
				}
				else {
					++diff;
					auto adjOpp = logic::COUNTER_DIRECTION[k];
					if ((adjOpp & 32 ? (bool)(state._2 & 1ull << ((adjOpp & 31) << 1) + (isBlack ^ 1)) : (bool)(state._1 & 1ull << (adjOpp << 1) + (isBlack ^ 1)))) {
						++cutMean;
					}
				}
			}
			for (; k < 6; ++k) {
				auto adj = logic::MOVE_TABLE[i >> 1][k];
				if (adj == -1) {
					continue;
				}
				int adjslot = adj & 32 ? state._2 >> ((adj & 31) << 1) & 3 : state._1 >> (adj << 1) & 3;
				if (!adjslot) {
					continue;
				}
				if (adjslot == ((int)isBlack + 1)) {
					++same;
					++adjacency;
				}
				else {
					++diff;
				}
			}
			if (midScore < 3 && same < diff) {
				danger += (4 - midScore);
			}
			if (same == 6) {
				++hex;
			}
			thisMid += midScore;
		} else {
			int midScore = MIDDLE_H[i >> 1];
			int same = 0;
			int diff = 0;
			for (auto k = 0u; k < 6; ++k) {
				auto adj = logic::MOVE_TABLE[i >> 1][k];
				if (adj == -1) {
					continue;
				}
				int adjslot = adj & 32 ? state._2 >> ((adj & 31) << 1) & 3 : state._1 >> (adj << 1) & 3;
				if (!adjslot) {
					continue;
				}
				if (adjslot == ((int)isBlack + 1)) {
					++diff;
				}
				else {
					++same;
				}
			}
			if (midScore < 4 && same < diff) {
				trap += (4 - midScore);
			}
			thatMid += midScore;
		}
	}
	//std::cout << "mid: " << thisMid << std::endl;
	//std::cout << "that mid: " << thatMid << std::endl;
	//std::cout << "adjacency: " << adjacency	<< std::endl;
	//std::cout << "cut: " << cutMean << std::endl;
	//std::cout << "hex: " << hex << std::endl;
	//std::cout << "score: " << scoreMean << std::endl;
	//confidence
	return (int)(thisMid * 1.1) - thatMid + (adjacency >> 1) + hex + cutMean + trap - danger + scoreMean;
}
