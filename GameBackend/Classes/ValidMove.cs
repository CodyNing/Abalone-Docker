using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace GameBackend
{
    public class ValidMove
    {
        public ReadableAction Action { get; set; }
        public ReadableState State { get; set; }

        public ValidMove(ReadableAction action, ReadableState state)
        {
            Action = action;
            State = state;
        }
    }
}
