#include <string>
#include <chrono>
#include <thread>
#include <iostream>
#include "automata.h"
#include "util.h"

#include <zmq.hpp>

int main() 
{
//    using namespace std::chrono_literals;
    using namespace std;

    // initialize the zmq context with a single IO thread
    zmq::context_t context{1};

    // construct a REP (reply) socket and bind to interface
    zmq::socket_t socket{context, zmq::socket_type::rep};
    socket.bind("tcp://0.0.0.0:5555");

    for (;;) 
    {
        zmq::message_t request;
        automata ai = automata{};

        socket.recv(request, zmq::recv_flags::none);
        string requestStr = request.to_string();

        std::cout << "Received " << requestStr << std::endl;

        auto res = util::route(requestStr, ai);

        std::cout << res << std::endl;

        // send the reply to the client
        socket.send(zmq::buffer(res), zmq::send_flags::none);
    }

    return 0;
}
