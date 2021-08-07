using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace GameBackend
{
    public class GetAINextMoveArgs
    {
        public ReadableState State { get; set; }
        public int MoveLeft { get; set; }
        public int TimeLeft { get; set; }
    }
}
