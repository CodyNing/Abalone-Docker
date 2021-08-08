#include "util.h"

std::string util::printState(std::bitset<128U> state) {
	std::string res;
	auto slotIndex = 0u;
	for (unsigned int slotNumber : logic::ROW_NUMBER_MAP) {
        auto space = 9u - slotNumber;
		for (auto ss = 0u; ss < space; ++ss) {
			res += ' ';
		}
		for (size_t j = 0; j < slotNumber; ++j) {
			auto slotState = state[slotIndex] << 1 | state[slotIndex + 1] << 0;

			if (slotState == 0) {
				res += '+';
			}
			else if (slotState == 1) {
				res += '@';
			}
			else {
				res += 'O';
			}
			slotIndex += 2;
			res += ' ';
		}
		res += '\n';
	}
	return res;
}

logic::bitState util::stringState2BitState(std::string const & stateStr){
    logic::bitState b {};
    b._2 = std::stoull(stateStr.substr(0, 64), nullptr, 2);
    b._1 = std::stoull(stateStr.substr(64, 64), nullptr, 2);
    return b;
}

std::string util::getAllValidMovesStr(const std::string& stateStr, bool isBlackTurn) {
    auto bitState = stringState2BitState(stateStr);
    auto generated = logic::getAllValidMoveOrdered(bitState, isBlackTurn);

    std::string res;
    for (const auto & iter : generated) {
        auto wact = iter.act;
        auto act = wact.act;
        auto state = iter.state;
        res += std::to_string(act.count) + "," + std::to_string(act.index) + "," + std::to_string(act.direction)
                + "," + std::to_string(wact.alignDrr) + "," + std::to_string(wact.teamCount) + "," + std::to_string(wact.oppCount);
        res += ":";
        res += logic::b2b(state).to_string();
        res += "\n";
    }
    return res.substr(0, res.size() - 1);
}

std::string util::getAINextMove(automata & ai, const std::string& stateStr, bool isBlackTurn, unsigned int moveLeft, int timeLeft) {
    auto bitState = stringState2BitState(stateStr);

    auto wam = ai.getBestMove(bitState, isBlackTurn, moveLeft, timeLeft);
//    std::cout << printState(logic::b2b(wam.state)) << std::endl;
    return logic::b2b(wam.state).to_string();
}

std::vector<std::string> util::split(std::string const & str, char del){
    using namespace std;
    size_t last = 0;
    size_t next = 0;
    auto splited = vector<string>{};

    while ((next = str.find(del, last)) != string::npos)
    {
        splited.push_back(str.substr(last, next - last));
        last = next + 1;
    }
    splited.push_back(str.substr(last));
    return std::move(splited);
}

std::string util::route(const std::string& requestStr, automata & ai){
    using namespace std;
    auto token = split(requestStr, '\n');
    auto funcName = token[0];
    auto argStr = token[1];
    auto args = split(argStr, ',');
    if(funcName == "GetValidMoves"){
        if(args.size() != 2){
            return "Failed";
        } else{
            return getAllValidMovesStr(args[0], args[1] == "True");
        }

    } else if(funcName == "GetAINextMove"){
        if(args.size() != 4){
            return "Failed";
        } else{
            return getAINextMove(ai, args[0], args[1] == "True", std::stoi(args[2]), std::stoi(args[3]));
        }
    }
    return "Invalid path";
}

