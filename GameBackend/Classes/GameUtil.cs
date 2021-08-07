using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace GameBackend
{
    public static class GameUtil
    {
        static readonly int[] ROW_NUMBER_MAP = { 5, 6, 7, 8, 9, 8, 7, 6, 5 };

		static readonly int[, ] MOVE_TABLE =
		{
			//0 1ST ROW
			{-1, -1, -1, 1, 6, 5},
			//1
			{0, -1, -1, 2, 7, 6},
			//2
			{1, -1, -1, 3, 8, 7},
			//3
			{2, -1, -1, 4, 9, 8},
			//4
			{3, -1, -1, -1, 10, 9},
			//5 2ND ROW
			{-1, -1, 0, 6, 12, 11},
			//6
			{5, 0, 1, 7, 13, 12},
			//7
			{6, 1, 2, 8, 14, 13},
			//8
			{7, 2, 3, 9, 15, 14},
			//9
			{8, 3, 4, 10, 16, 15},
			//10
			{9, 4, -1, -1, 17, 16},
			//11 3RD ROW
			{-1, -1, 5, 12, 19, 18},
			//12
			{11, 5, 6, 13, 20, 19},
			//13
			{12, 6, 7, 14, 21, 20},
			//14
			{13, 7, 8, 15, 22, 21},
			//15
			{14, 8, 9, 16, 23, 22},
			//16
			{15, 9, 10, 17, 24, 23},
			//17
			{16, 10, -1, -1, 25, 24},
			//18 4TH ROW
			{-1, -1, 11, 19, 27, 26},
			//19
			{18, 11, 12, 20, 28, 27},
			//20
			{19, 12, 13, 21, 29, 28},
			//21
			{20, 13, 14, 22, 30, 29},
			//22
			{21, 14, 15, 23, 31, 30},
			//23
			{22, 15, 16, 24, 32, 31},
			//24
			{23, 16, 17, 25, 33, 32},
			//25
			{24, 17, -1, -1, 34, 33},
			//26 5TH ROW
			{-1, -1, 18, 27, 35, -1},
			//27
			{26, 18, 19, 28, 36, 35},
			//28
			{27, 19, 20, 29, 37, 36},
			//29
			{28, 20, 21, 30, 38, 37},
			//30
			{29, 21, 22, 31, 39, 38},
			//31
			{30, 22, 23, 32, 40, 39},
			//32
			{31, 23, 24, 33, 41, 40},
			//33
			{32, 24, 25, 34, 42, 41},
			//34
			{33, 25, -1, -1 , -1, 42},
			//35 6TH ROW
			{-1, 26, 27, 36, 43, -1},
			//36
			{35, 27, 28, 37, 44, 43},
			//37
			{36, 28, 29, 38, 45, 44},
			//38
			{37, 29, 30, 39, 46, 45},
			//39
			{38, 30, 31, 40, 47, 46},
			//40 
			{39, 31, 32, 41, 48, 47},
			//41
			{40, 32, 33, 42, 49, 48},
			//42
			{41, 33, 34, -1, -1, 49},
			//43 7TH ROW
			{-1, 35, 36, 44, 50, -1},
			//44
			{43, 36, 37, 45, 51, 50},
			//45
			{44, 37, 38, 46, 52, 51},
			//46
			{45, 38, 39, 47, 53, 52},
			//47
			{46, 39, 40, 48, 54, 53},
			//48
			{47, 40, 41, 49, 55, 54},
			//49
			{48, 41, 42, -1, -1, 55},
			//50 8TH ROW
			{-1, 43, 44, 51, 56, -1},
			//51
			{50, 44, 45, 52, 57, 56},
			//52
			{51, 45, 46, 53, 58, 57},
			//53
			{52, 46, 47, 54, 59, 58},
			//54
			{53, 47, 48, 55, 60, 59 },
			//55
			{54, 48, 49, -1, -1, 60},
			//56 9TH ROW
			{-1, 50, 51, 57, -1, -1},
			//57
			{56, 51, 52, 58, -1, -1},
			//58
			{57, 52, 53, 59, -1, -1 },
			//59
			{58, 53, 54, 60, -1, -1},
			//60
			{59, 54, 55, -1, -1, -1}
		};


        public static void TranslateAction(AgentAction act, AgentState state, ReadableState origin, out ReadableAction readableAction, out ReadableState readableState)
        {
            readableState = TranslateState(state);
			readableAction = new ReadableAction
			{
				Direction = act.Direction,
				Movements = new ReadableAction.Movement[act.TeamCount + act.OppCount]
			};
            int slotIndex = act.Index;

			for(int i = 0; slotIndex != -1 && i < act.TeamCount + act.OppCount; slotIndex = MOVE_TABLE[slotIndex, act.AlignDirection])
            {
				readableAction.Movements[i++] = new ReadableAction.Movement(slotIndex, MOVE_TABLE[slotIndex, act.Direction]);
            }
        }

		public static AgentState TranslateState(ReadableState state)
		{
			var stateStr = "";

			int slotIndex = 0;
			for (; slotIndex < 61; ++slotIndex)
			{
				string slotstate = Convert.ToString((int)state.SlotStates[slotIndex], 2).PadLeft(2, '0');
				stateStr = slotstate + stateStr;
			}
			string blackScore = Convert.ToString(state.BlackScore, 2).PadLeft(3, '0');
			string whiteScore = Convert.ToString(state.WhiteScore, 2).PadLeft(3, '0');

			return new AgentState(blackScore + whiteScore + stateStr);
		}

		public static ReadableState TranslateState(AgentState state)
		{
			var readableState = new ReadableState();
			int slotIndex = 0;
			for (; slotIndex < 61; ++slotIndex)
			{
				int i = 127 - (slotIndex << 1);
				int slotstate = Convert.ToInt32(state.State.Substring(i - 1, 2), 2);
				readableState.SlotStates[slotIndex] = (ReadableState.SlotState)slotstate;
			}
			readableState.BlackScore = Convert.ToInt32(state.State.Substring(0, 3), 2);
			readableState.WhiteScore = Convert.ToInt32(state.State.Substring(3, 3), 2);
			return readableState;
		}

	}
}
