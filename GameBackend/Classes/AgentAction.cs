using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace GameBackend
{
    public class AgentAction
    {
        public int Count { get; set; }
        public int Index { get; set; }
        public int Direction { get; set; }
        public int AlignDirection { get; set; }
        public int TeamCount { get; set; }
        public int OppCount { get; set; }

        public AgentAction(string actionStr)
        {
            var eles = actionStr.Split(",");
            Count = int.Parse(eles[0]);
            Index = int.Parse(eles[1]);
            Direction = int.Parse(eles[2]);
            AlignDirection = int.Parse(eles[3]);
            TeamCount = int.Parse(eles[4]);
            OppCount = int.Parse(eles[5]);
        }
    }
}
