#include "logic.h"

const logic::action logic::RANDOM_MOVES[3][4] = {
	//standard
	{
		{1, 56, 2},
		{1, 57, 2},
		{1, 59, 1},
		{1, 60, 1}
	},
	//german
	{
		{1, 9, 5},
		{1, 17, 5},
		{1, 51, 2},
		{1, 43, 2}
	},
	//belgian
	{
		{1, 3, 5},
		{1, 4, 5},
		{1, 56, 2},
		{1, 57, 2}
	}
};

std::multiset<logic::weightedActionState, std::greater<logic::weightedActionState>> logic::getAllValidMoveOrdered(bitState& state, bool isBlackTurn) {
	
	std::multiset<logic::weightedActionState, std::greater<logic::weightedActionState>> actionStates;
	//count 1-3, index 0-60, direction 0-5
	for (auto bitindex = (int)isBlackTurn; bitindex < 122; bitindex += 2) {
		if ((bitindex & 64 ? (bool)(state._2 & 1ull << (bitindex & 63)) : (bool)(state._1 & 1ull << bitindex))) {
			for (int direction = 0; direction < 6; ++direction) {
				for (int count = 1; count < 4; ++count) {
					weightedAction act{ {count, bitindex >> 1, direction}, 0, 0, -1 };
					if (isValidMove_ai(state, act, isBlackTurn)) {
						actionStates.emplace(act, move_ai(state, act, isBlackTurn));
					}
				}
			}
		}
	}
	return actionStates;
}

bool logic::isValidSideMove(bitState& state, weightedAction & act, bool isBlackTurn)
{
	act.teamCount = act.act.count;
	//check 1: path is blocked
	int next = MOVE_TABLE[act.act.index][act.act.direction];
	if (next == -1) {
		return false;
	}

	if (next & 32) {
		if (state._2 & (3ull << ((next & 31) << 1)))
			return false;
	}
	else {
		if (state._1 & (3ull << (next << 1)))
			return false;
	}

	//check 2: aligning direction
	//assume its aligning on counter clockwise -1 direction
	int alignDrr = act.act.direction - 1;
	if (alignDrr < 0) 
		alignDrr = 5;
	int alignNext = MOVE_TABLE[act.act.index][alignDrr];

	//if check 2 is outbound or blocked, other direction is blocked anyway
	/*if (alignNext == -1 || state[(alignNext << 1) + (int)!isBlackTurn]) {
		return false;
	}*/
	if (alignNext == -1) {
		return false;
	}

	if (alignNext & 32) {
		if (state._2 & 1ull << ((alignNext & 31) << 1) + (int)!isBlackTurn)
			return false;
	}
	else {
		if (state._1 & 1ull << (alignNext << 1) + (int)!isBlackTurn)
			return false;
	}

	//found same color 2nd marble, confirm aligning direction
	//if (state[(alignNext << 1) + (int)isBlackTurn]) {
	if ((alignNext & 32 ? (bool)(state._2 & 1ull << ((alignNext & 31) << 1) + (int)isBlackTurn)
		: (bool)(state._1 & 1ull << (alignNext << 1) + (int)isBlackTurn))) {
		next = MOVE_TABLE[alignNext][act.act.direction];
		//2nd marble's path is blocked or outbound
		/*if (next == -1 || state[next << 1] || state[(next << 1) + 1]) {
			return false;
		}*/
		if (next == -1) {
			return false;
		}
		if (next & 32) {
			if (state._2 & 3ull << ((next & 31) << 1))
				return false;
		}
		else {
			if (state._1 & 3ull << (next << 1))
				return false;
		}

		//addtion check for count 3
		if (act.act.count == 3) {
			alignNext = MOVE_TABLE[alignNext][alignDrr];
			//expect 3rd, but not found
			/*if (alignNext == -1 || !state[(alignNext << 1) + (int)isBlackTurn]) {
				return false;
			}*/
			if (alignNext == -1) {
				return false;
			}
			if (alignNext & 32) {
				if (!(state._2 & 1ull << ((alignNext & 31) << 1) + (int)isBlackTurn))
					return false;
			}
			else {
				if (!(state._1 & 1ull << (alignNext << 1) + (int)isBlackTurn))
					return false;
			}

			next = MOVE_TABLE[alignNext][act.act.direction];
			//3rd marble's path is blocked or outbound
			/*if (next == -1 || state[next << 1] || state[(next << 1) + 1]) {
				return false;
			}*/
			if (next == -1) {
				return false;
			}
			if (next & 32) {
				if (state._2 & (3ull << ((next & 31) << 1)))
					return false;
			}
			else {
				if (state._1 & (3ull << (next << 1)))
					return false;
			}
		}
	}
	//drr 2
	else {
		//assume its aligning on counter clockwise -2 direction
		alignDrr = alignDrr - 1;
		if (alignDrr < 0) 
			alignDrr = 5;

		//2nd marble's path is already clear, so check 2nd marble itself.
		alignNext = MOVE_TABLE[alignNext][COUNTER_DIRECTION[act.act.direction]];
		//2nd marble is outbound or is not same color
		/*if (alignNext == -1 || !state[(alignNext << 1) + (int)isBlackTurn]) {
			return false;
		}*/
		if (alignNext == -1) {
			return false;
		}
		if (alignNext & 32) {
			if (!(state._2 & 1ull << ((alignNext & 31) << 1) + (int)isBlackTurn))
				return false;
		}
		else {
			if (!(state._1 & 1ull << (alignNext << 1) + (int)isBlackTurn))
				return false;
		}

		//addtion check for count 3
		if (act.act.count == 3) {
			alignNext = MOVE_TABLE[alignNext][alignDrr];
			//expect 3rd, but not found
			/*if (alignNext == -1 || !state[(alignNext << 1) + (int)isBlackTurn]) {
				return false;
			}*/
			if (alignNext == -1) {
				return false;
			}
			if (alignNext & 32) {
				if (!(state._2 & 1ull << ((alignNext & 31) << 1) + (int)isBlackTurn))
					return false;
			}
			else {
				if (!(state._1 & 1ull << (alignNext << 1) + (int)isBlackTurn))
					return false;
			}
			next = MOVE_TABLE[alignNext][act.act.direction];
			//3rd marble's path is blocked or outbound
			/*if (next == -1 || state[next << 1] || state[(next << 1) + 1]) {
				return false;
			}*/
			if (next == -1) {
				return false;
			}
			if (next & 32) {
				if (state._2 & (3ull << ((next & 31) << 1)))
					return false;
			}
			else {
				if (state._1 & (3ull << (next << 1)))
					return false;
			}

		}

	}
	act.alignDrr = alignDrr;
	//all check passed;
	return true;
}

bool logic::isValidInlineMove(bitState& state, weightedAction & act, bool isBlackTurn)
{
	act.alignDrr = act.act.direction;
	bool countingTeam = true;
	int teamCount = 1;
	int oppCount = 0;
	for (int next = MOVE_TABLE[act.act.index][act.act.direction]; true; next = MOVE_TABLE[next][act.act.direction]) {
		//outbound
		if (next == -1) {
			act.teamCount = teamCount;
			act.oppCount = oppCount;
			return !countingTeam;
		}

		//next is team
		//if (state[(next << 1) + (int)isBlackTurn]) {
		if((next & 32 ? (bool)(state._2 & 1ull << ((next & 31) << 1) + (int)isBlackTurn)
			: (bool)(state._1 & 1ull << (next << 1) + (int)isBlackTurn))){
			//blocked
			if (!countingTeam || ++teamCount > 3) {
				return false;
			}
		}
		//next is not team
		else {
			//next is opponent
			//if (state[(next << 1) + (int)!isBlackTurn]) {
			if ((next & 32 ? (bool)(state._2 & 1ull << ((next & 31) << 1) + (int)!isBlackTurn)
				: (bool)(state._1 & 1ull << (next << 1) + (int)!isBlackTurn))) {
				if (countingTeam) {
					countingTeam = !countingTeam;
				}
				//opponent is more than team
				if (++oppCount >= teamCount) {
					return false;
				}
			}
			//next is empty
			else {
				act.teamCount = teamCount;
				act.oppCount = oppCount;
				return true;
			}
		}
	}
	return false;
}

logic::bitState& logic::inlineMove(bitState& state, weightedAction & act, bool isBlackTurn) {
	int index = act.act.index;
	int i = 0;
	//state.set(index << 1, 0);
	//state.set((index << 1) + 1, 0);
	if (index & 32) {
		state._2 &= ~(3ull << ((index & 31) << 1));
	}
	else {
		state._1 &= ~(3ull << (index << 1));
	}
	
	for (; i < act.teamCount; ++i, index = MOVE_TABLE[index][act.act.direction]);
	//state.set(index << 1, !isBlackTurn);
	//state.set((index << 1) + 1, isBlackTurn);
	if (index & 32) {
		state._2 &= ~(3ull << ((index & 31) << 1));
		state._2 |= (unsigned long long)(isBlackTurn << 1 | !isBlackTurn) << ((index & 31) << 1);
	}
	else {
		state._1 &= ~(3ull << (index << 1));
		state._1 |= (unsigned long long)(isBlackTurn << 1 | !isBlackTurn) << (index << 1);
	}
	if (act.oppCount == 0)
		return state;
	for (; i < act.teamCount + act.oppCount; ++i, index = MOVE_TABLE[index][act.act.direction]);
	if (index == -1) {
		auto scoreI = (isBlackTurn) * 3 + 58;
		//auto max = scoreI + 3;
		//bool val = state[scoreI];
		//bool carry = val & 1;
		//state.set(scoreI, val ^ 1);
		//while (carry && scoreI < max) {
		//	//val = state[++scoreI];
		//	//state.set(scoreI, val ^ carry);
		//	carry &= val;
		//}
		auto score = (state._2 >> scoreI & 7);
		state._2 &= ~(7ull << scoreI);
		state._2 |= ++score << scoreI;
		return state;
	}
	//state.set(index << 1, isBlackTurn);
	//state.set((index << 1) + 1, !isBlackTurn);
	if (index & 32) {
		state._2 |= (unsigned long long)(!isBlackTurn << 1 | isBlackTurn) << ((index & 31) << 1);
	}
	else {
		state._1 |= (unsigned long long)(!isBlackTurn << 1 | isBlackTurn) << (index << 1);
	}
	return state;
}

logic::bitState& logic::sideMove(bitState& state, weightedAction & act, bool isBlackTurn) {
	int index = act.act.index;
	int next = MOVE_TABLE[index][act.act.direction];
	for (int i = 0; i < act.teamCount; ++i) {
		//unsigned long long eraseMask = ~(3 << (index << 1 & 63));
		//unsigned long long setMask = (isBlackTurn << 1 | !isBlackTurn) << (next << 1 & 63);
		if (index & 32) {
			state._2 &= ~(3ull << ((index & 31) << 1));
		}
		else {
			state._1 &= ~(3ull << (index << 1));
		}
		if (next & 32) {
			state._2 |= (unsigned long long)(isBlackTurn << 1 | !isBlackTurn) << ((next & 31) << 1);
		}
		else {
			state._1 |= (unsigned long long)(isBlackTurn << 1 | !isBlackTurn) << (next << 1);
		}
		/*state.set(index << 1, 0);
		state.set((index << 1) + 1, 0);
		state.set(next << 1, !isBlackTurn);
		state.set((next << 1) + 1, isBlackTurn);*/
		index = MOVE_TABLE[index][act.alignDrr];
		next = MOVE_TABLE[index][act.act.direction];
	}
	return state;
}

logic::Vector2i logic::getScoreFromState(std::bitset<128U>& state) {
	auto whiteLost = 0, blackLost = 0;
	for (auto i = 124u, j = 127u; i > 121; --i, --j) {
		whiteLost <<= 1;
		blackLost <<= 1;
		whiteLost |= state[j] << 0;
		blackLost |= state[i] << 0;
	}
	return { blackLost, whiteLost };
}

logic::bitState logic::b2b(std::bitset<128U> const & state) {
	bitState b;
	auto str = state.to_string();
	b._2 = std::stoull(str.substr(0, 64), nullptr, 2);
	b._1 = std::stoull(str.substr(64, 64), nullptr, 2);
	return b;
}

std::bitset<128U> logic::b2b(logic::bitState const & state) {
	return std::bitset<128U>(std::bitset<64>(state._2).to_string() + std::bitset<64>(state._1).to_string());
}