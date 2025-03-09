#pragma once
#include <iostream>
#include <sstream>
#include <thread>
#include <vector>
#include "client.h"


class ParserCmd
{
	std::thread thread;
	Client* client;
public:
	ParserCmd(Client* cl) : client(cl)
	{
		thread = std::thread(&ParserCmd::parseCmd, this);
		std::cout << "Type \"help\" for cmd list\n";
	}
	~ParserCmd()
	{
		thread.join();
	}

	void parseCmd()
	{
		while (true)
		{
			std::cout << "->";
			std::vector<std::string> vec(parseString(getInput(), ' '));

			if (vec.empty())
			{
				std::cout << "Invalid command" << std::endl;
				continue;
			}

			if (vec[0] == "exit")
			{
				client->close();
				return;
			}
			else if (vec[0] == "help")
			{
				std::cout << "ip 192.168.0.108 8080 - first connection to server\n"
					<< "exit - shutdown app\n" 
					<< "log - to get logs from server\n"
					<< "info - request server status\n" 
					<< "uplaod [path to file] - upload file to server. Note! [path to file] without spaces and with .extension" << std::endl;
				continue;
			}
			else if (vec[0] == "ip" && vec.size() == 3)
			{
				client->run(vec[1], vec[2], "");
				continue;
			}
			else if (vec[0] == "info" || vec[0] == "log")
			{
				client->run(vec[0]);
				continue;
			}
			else if (vec[0] == "upload" && vec.size() == 2)
			{
				client->upload(vec[0], vec[1]);
				continue;
			}


			std::cout << "Invalid command" << std::endl;
		}

		std::this_thread::yield();
	}
private:
	std::string getInput()
	{
		std::stringstream ss;
		std::string cmd;
		std::cin.clear();
		std::getline(std::cin, cmd);
		return cmd;
	}

	std::vector<std::string> parseString(std::string&& str, char delimiter)
	{
		std::vector<std::string> res;

		if (str.empty())
			return res;

		std::stringstream ss(str);
		std::string token;

		while (std::getline(ss, token, delimiter))
			res.push_back(token);
		return res;
	}
};

