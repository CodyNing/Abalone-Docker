using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace GameBackend.Classes
{
    public class MoveArgs
    {
        public Guid SessionId { get; set; }
        public ValidMove Move { get; set; }
    }
}
