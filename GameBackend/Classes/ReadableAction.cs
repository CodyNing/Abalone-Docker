using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace GameBackend
{
    public class ReadableAction
    {
        public class Movement
        {
            public int FromIndex { get; set; }
            public int ToIndex { get; set; }

            public Movement(int from, int to)
            {
                FromIndex = from;
                ToIndex = to;
            }
        }

        public Movement[] Movements { get; set; }
        public int Direction { get; set; }
    }
}
