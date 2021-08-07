using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace GameBackend
{
    public class AgentState
    {
        public string State { get; set; }

        public AgentState(string stateStr)
        {
            State = stateStr;
        }
    }
}
