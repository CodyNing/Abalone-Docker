# Abalone
 
## Goal
+ A game Abalone implementation using JavaScript, C# and C++. 
 See [wiki](https://en.wikipedia.org/wiki/Abalone_(board_game)) for game introduction.
 
+ Except the basic implementation, the game also supports a simple game playing AI.
Players can tune difficulty by limiting the AI searching time.
 
*** 
## Languages
 
### C++
 
+ Major Game logic implementations
+ Game AI searching algorithm
 
### C# ASP.NET
 
+ Scalable backend implementation
+ Bridging the communication between frontend user and AI Agent 
 
### JavaScript ES6
 
+ Interactive UI for game Abalone
+ Communicate only with ASP.NET backend via Rest API call

*** 
## Communication Methods
+ [ZeroMQ](https://zeromq.org/) (NetMQ in C#, cppzmq in C++)
+ Rest API call from JavaScript frontend to ASP.NET backend.
 
*** 
## Build
    docker-compose build
 
## Run
    docker-compose up
 
**After successfully initialize both GameBackend and GameAgent, the frontend can be access at http://localhost:8888/**
 
Note: Game AI default uses 4 threads. Please make sure that your machine supports at least 4 virtual threads. 

## Common Issue

When running

    docker-compose up

in CSIL, you might encounter the following error:

    ERROR: could not find an available, non-overlapping IPv4 address pool among the defaults to assign to the network

I prepared the following code commented out in docker-compose.yml

```
networks:
   default:
     driver: bridge
     ipam:
       config:
         - subnet: 10.10.1.0/24
```
After adding this block, I was able to run the images normally in CSIL.

*** 
## Features
### Game AI
 
+ Supports min-max searching with alpha-beta pruning
+ Supports node ordering for better pruning efficiency
+ Supports concurrent min-max searching
+ Supports predictive early returning
+ Game representations are optimized into 128 bit unsigned integer for future implementation of table searching
+ Search space is reduced by using a set of unique action representations
 
Note for TAs: Some logics are adapted from my old project: https://github.com/CodyNing/AbaloneAI
 
### Game Server
 
+ Supports translation between AI friendly game representation and human friendly representation
+ Supports session caching
+ Supports out-dated session cleaning
+ Communicates with Game AI using [ZeroMQ](https://zeromq.org/)
 
### Game Frontend
 
+ Asynchronized implementation. All UI interactions are smooth.
+ Intuitive UI control
+ Smart hint for valid movements
+ Clean game board UI design

