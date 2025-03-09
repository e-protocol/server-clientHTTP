#pragma once
#include <sstream>
#include <iostream>
#include <thread>
#include "server.h"

class ParserCmd
{
    static inline short port = 8080;
    Server* server = nullptr;
    std::thread t;
public:
    ParserCmd() = delete;
    ~ParserCmd()
    {
        t.join();
    }

    ParserCmd(Server* ptr) : server(ptr)
    {
        t = std::thread(&ParserCmd::parseInput, this);
    }

    void parseInput()
    {
        while(true)
        {
            std::stringstream ss;
            std::string cmd;
            std::cin.clear();
            std::getline(std::cin, cmd);

            if(cmd == "exit")
            {
                server->close();
                break;
            }
            else
                std::cout << "Invalid cmd\n";
            std::this_thread::yield;
        }
    }

    static short parse(int argc, char** argv)
    {
        if(argc < 2)
            return port;

        std::stringstream ss(argv[1]);
        short num;
        ss >> num;
        
        if(num < 1000)
            return port;
        return port = num;
    }
};