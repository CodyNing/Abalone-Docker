using GameBackend.Classes;
using GameBackend.Models;
using Microsoft.AspNetCore.Mvc;
using NetMQ;
using NetMQ.Sockets;
using Newtonsoft.Json;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text.Json;
using System.Threading.Tasks;

namespace GameBackend.Controllers
{
    public class GameController : Controller
    {
        public IActionResult Index()
        {
            return View();
        }

        [HttpPost]
        [Route("[controller]/CreateGame")]
        public IActionResult CreateGame([FromBody]object body)
        {
            var args = JsonConvert.DeserializeObject<CreateGameArgs>(body.ToString());
            var game = new Game(Request.Host.Host, args.InitState, args.AITimeSec, args.MoveLeft);
            return Json(game);
        }

        [HttpPost]
        [Route("[controller]/GetGame")]
        public IActionResult GetGame([FromBody] object body)
        {
            var args = JsonConvert.DeserializeObject<GetGameArgs>(body.ToString());
            Game game = null;
            var sucess = Game.Games.TryGetValue(args.SessionId, out game);
            if (sucess)
            {
                return Json(game);
            }
            else
            {
                return StatusCode(410);
            }
        }

        [HttpPost]
        [Route("[controller]/TerminateGame")]
        public IActionResult TerminateGame([FromBody] object body)
        {
            var args = JsonConvert.DeserializeObject<GetGameArgs>(body.ToString());
            var sucess = Game.Games.Remove(args.SessionId);
            if (sucess)
            {
                return Ok();
            }
            else
            {
                return StatusCode(410);
            }
        }

        //[HttpPost]
        //[Route("CreateGame")]
        //public IActionResult Move([FromBody] object body)
        //{
        //    var args = JsonConvert.DeserializeObject<CreateGameArgs>(body.ToString());
        //    var game = new Game(Request.Host.Host, args.InitState, args.AITimeSec, args.MoveLeft);
        //    return Json(game);
        //}

        [HttpPost]
        [Route("[controller]/GetValidMoves")]
        public IActionResult GetValidMoves([FromBody]object body)
        {
            var state = JsonConvert.DeserializeObject<ReadableState>(body.ToString());
            var agentstate = GameUtil.TranslateState(state);
            var requestStr = $"GetValidMoves\n{agentstate.State},true";
            var agentRes = AgentRequester.GetInstance().Send(requestStr);
            var lines = agentRes.Split('\n');
            var moves = new List<ValidMove>(lines.Length);
            foreach(var line in lines)
            {
                var actState = line.Split(":");
                if(actState.Length != 2)
                {
                    return StatusCode(500);
                }
                var aact = new AgentAction(actState[0]);
                var astate = new AgentState(actState[1]);
                ReadableState nstate;
                ReadableAction nact;
                GameUtil.TranslateAction(aact, astate, state, out nact, out nstate);
                var move = new ValidMove(nact, nstate);
                moves.Add(move);
            }
            //return Ok(JsonConvert.SerializeObject(moves));
            return Json(moves);
        }

        [HttpPost]
        [Route("[controller]/GetAINextMove")]
        public IActionResult GetAINextMove([FromBody] object body)
        {
            var args = JsonConvert.DeserializeObject<GetAINextMoveArgs>(body.ToString());
            var agentstate = GameUtil.TranslateState(args.State);
            var requestStr = $"GetAINextMove\n{agentstate.State},true,{args.MoveLeft},{args.TimeLeft}";
            var agentRes = AgentRequester.GetInstance().Send(requestStr);
            var nstate = GameUtil.TranslateState(new AgentState(agentRes));
            //return Ok(JsonConvert.SerializeObject(nstate));
            return Json(nstate);
        }

        [HttpPost]
        [Route("[controller]/TranslateState")]
        public IActionResult TranslateState([FromBody]object body)
        {
            var astate = JsonConvert.DeserializeObject<AgentState>(body.ToString());
            var state = GameUtil.TranslateState(astate);
            //return Json(JsonConvert.SerializeObject(state));
            return Json(state);
        }
    }
}
