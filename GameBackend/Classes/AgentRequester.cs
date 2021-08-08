using NetMQ;
using NetMQ.Sockets;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace GameBackend
{
    public class AgentRequester
    {
        public static readonly string Address = Environment.GetEnvironmentVariable("SERVER_CONNECT_URI");

        private static AgentRequester Instance = null;

        private static readonly object myLock = new object();

        private readonly RequestSocket Requester;

        public static AgentRequester GetInstance()
        {
            if(Instance == null)
            {
                Instance = new AgentRequester();
            }
            return Instance;
        }
        private AgentRequester()
        {
            Requester = new RequestSocket();
            Requester.Connect(Address);
        }

        public string Send(string message)
        {
            lock (myLock)
            {
                Requester.SendFrame(message);
                return Requester.ReceiveFrameString();
            }
        }
    }
}
