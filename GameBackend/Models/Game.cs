using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace GameBackend.Models
{
    public class Game
    {
        /*
                1010101010
               101010101010
              00001010100000
             0000000000000000
            000000000000000000
             0000000000000000
              00000101010000
               010101010101
                0101010101
        */

        public static readonly string STANDARD_STR = "00000010101010101010101010100000101010000000000000000000000000000000000000000000000000000000000001010100000101010101010101010101";

        /*
                0000000000
               010100001010
              01010100101010
             0001010000101000
            000000000000000000
             0010100000010100
              10101000010101
               101000000101
                0000000000
        */
        public static readonly string GERMAN_DAISY_STR = "00000000000000000101000010100101010010101000010100001010000000000000000000000010100000010100101010000101011010000001010000000000";

        /*
                0101001010
               010101101010
              00010100101000
             0000000000000000
            000000000000000000
             0000000000000000
              00101000010100
               101010010101
                1010000101
        */
        public static readonly string BELGAIN_DAISY_STR = "00000001010010100101011010100001010010100000000000000000000000000000000000000000000000000000001010000101001010100101011010000101";


        public static Dictionary<Guid, Game> Games { get; private set; }

        public const int MAX_SESSION = 100;
        public const int MAX_SESSION_LIFESPAN = 60;


        public enum GameState
        {
            NOT_STARTED = 0,
            HUMAN_THINKING = 1,
            PAUSED = 1 << 1,
            AI_SEARCHING = 1 << 2,
            BLACK_WINNED = 1 << 3,
            WHITE_WINNED = 1 << 4
        }

        public enum InitialState
        {
            STANDARD = 1,
            GERMAN_DAISY = 2,
            BELGAIN_DAISY = 3
        }

        public Guid SessionId { get; set; }
        public string IPAddress { get; set; }
        public GameState State { get; set; }
        public ReadableState BoardState { get; set; }
        public int AITimeSec { get; set; }
        public int MoveLeft { get; set; }
        public DateTime InitTime { get; set; }
        public DateTime LastUpdatedTime { get; set; }

        static Game()
        {
            Games = new Dictionary<Guid, Game>();
        }

        public Game(string ip, InitialState initState, int aiTime, int MoveLimit)
        {
            this.IPAddress = ip;
            this.AITimeSec = aiTime;
            this.MoveLeft = MoveLimit;
            switch (initState)
            {
                case InitialState.STANDARD:
                    BoardState = GameUtil.TranslateState(new AgentState(STANDARD_STR));
                    break;
                case InitialState.BELGAIN_DAISY:
                    BoardState = GameUtil.TranslateState(new AgentState(BELGAIN_DAISY_STR));
                    break;
                case InitialState.GERMAN_DAISY:
                    BoardState = GameUtil.TranslateState(new AgentState(GERMAN_DAISY_STR));
                    break;
            }
            State = GameState.HUMAN_THINKING;
            InitTime = DateTime.Now;
            LastUpdatedTime = DateTime.Now;
            SessionId = Guid.NewGuid();
            AddGameSession(this);
        }

        public static void AddGameSession(Game game)
        {
            //clean up sessions
            if(Games.Count >= MAX_SESSION)
            {
                var killSessions = Games.Where(s =>
                {
                    var span = DateTime.Now - s.Value.LastUpdatedTime;
                    return span.TotalMinutes > MAX_SESSION_LIFESPAN;
                }).ToList();

                foreach(var s in killSessions)
                {
                    Games.Remove(s.Key);
                }
            }

            Games[game.SessionId] = game;
        }
    }
}
