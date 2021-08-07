using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using GameBackend.Models;

namespace GameBackend.Classes
{
    public class CreateGameArgs
    {
        public Game.InitialState InitState { get; set; }
        public int AITimeSec { get; set; }
        public int MoveLeft { get; set; }
    }
}
