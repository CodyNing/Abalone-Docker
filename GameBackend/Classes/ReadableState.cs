using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace GameBackend
{
    public class ReadableState
    {
        public enum SlotState
        {
            Empty = 0,
            Black = 1,
            White = 2,
        }

        public int BlackScore { get; set; }
        public int WhiteScore { get; set; }
        public SlotState[] SlotStates { get; set; }

        public ReadableState()
        {
            SlotStates = new SlotState[61];
        }
    }
}
