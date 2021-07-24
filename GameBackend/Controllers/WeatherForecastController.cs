using Microsoft.AspNetCore.Mvc;
using Microsoft.Extensions.Logging;
using NetMQ;
using NetMQ.Sockets;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace GameBackend.Controllers
{
    [ApiController]
    [Route("[controller]")]
    public class WeatherForecastController : ControllerBase
    {
        private static readonly string[] Summaries = new[]
        {
            "Freezing", "Bracing", "Chilly", "Cool", "Mild", "Warm", "Balmy", "Hot", "Sweltering", "Scorching"
        };

        private readonly ILogger<WeatherForecastController> _logger;

        public WeatherForecastController(ILogger<WeatherForecastController> logger)
        {
            _logger = logger;
        }

        [HttpGet]
        public IEnumerable<WeatherForecast> Get()
        {
            var rng = new Random();
            return Enumerable.Range(1, 5).Select(index => new WeatherForecast
            {
                Date = DateTime.Now.AddDays(index),
                TemperatureC = rng.Next(-20, 55),
                Summary = Summaries[rng.Next(Summaries.Length)]
            })
            .ToArray();
        }

        [HttpGet]
        [Route("TestRPC")]
        public string TestRPC()
        {
            var address = Environment.GetEnvironmentVariable("SERVER_CONNECT_URI");
            var res = "Connecting to hello world server…\n";
            using (var requester = new RequestSocket())
            {
                requester.Connect(address);

                int requestNumber;
                for (requestNumber = 0; requestNumber != 10; requestNumber++)
                {
                    res += $"Sending Hello {requestNumber}...\n";
                    requester.SendFrame("Hello");
                    string str = requester.ReceiveFrameString();
                    res += $"Received {str} {requestNumber}\n";
                }
            }

            return res;
        }
    }
}
