#pragma once
#include <time.h>
#include <string>
#include <iostream>
#include <fstream>
#include <mutex>

class Log
{
    static inline bool rwt = false;
    static inline std::mutex _m;

public:
    std::string getDateTime()
    {
        time_t timestamp = time(&timestamp);
        struct tm* dt = localtime(&timestamp);
        char buf[80];
        strftime(buf, 80, "%d/%m/%Y %T", dt);
        return std::string(buf);
    }

protected:
    void writeLog(std::string&& str)
    {
        std::lock_guard<std::mutex> lock(_m);
        std::ofstream log;
        std::string dt = getDateTime();
        dt.pop_back();

        if(!rwt) //erase previous session logging
        {
            log.open("log.txt");
            rwt = true;
        }
        else    //continue logging session
            log.open("log.txt", std::ios_base::app);

        if(log.is_open())
        {
            log << dt << " " << str << "\n";
            log.close();
        }
        std::cout << str << std::endl;
    }

    std::string getLog()
    {
        std::lock_guard<std::mutex> lock(_m);
        std::ifstream log;
        log.open("log.txt");
        std::string str;
        std::string file;

        if(log.is_open())
        {
           while(std::getline(log, str))
                file += str + "\n";
            
            log.close();
            return file;
        }
        return str;
    }
};