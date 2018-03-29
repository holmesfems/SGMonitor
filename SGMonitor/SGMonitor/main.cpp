
/*
* Author: zhai <holmesfems@gmail.com>
* Copyright (C) 2018 zhai <holmesfems@gmail.com>
*
* Distributed under terms of the MIT license.
*/
#include <iostream>
#include <thread>
#include <vector>
#include <algorithm>
#include <functional>
#include <fstream>
#include "tcpClient.h"
#include "CmdHelper.h"
#include "third_include/nlohmann/json.hpp"

//using CmdGeneratorFunc = std::function<std::string(ParamSet::Params &)>;

uint16_t sgPort = 5025;
std::string sgIP = "127.0.0.1";
CmdHelper::CmdMap cmdMap;
CmdHelper::CmdHelper cmdHelper(cmdMap);
boost::asio::io_service io_service;
TcpClient::TcpClient client(io_service);

nlohmann::json configJson;
std::string  configJsonName = "conf.json";

std::string askFreqCmd()
{
	return ":FREQ?";
}

std::string askFreq(ParamSet::Params &params)
{
	client.send(askFreqCmd());
	return client.lastRecv();
}

std::string setFreq(ParamSet::Params &params)
{
	ParamSet::ParamHelper pHelper;
	std::string value;
	pHelper.bind("", &value, ParamSet::ParamHelper::TEXT);
	pHelper.bind("value", &value, ParamSet::ParamHelper::TEXT);
	pHelper.set(params);
	if (value.empty())
		return "Invalid setfreq cmd";
	std::string cmd = ":FREQ:CENT " + value;
	client.send(cmd);
	return askFreq(params);
}

int saveConfig()
{
	std::ofstream ofs(configJsonName);
	if (ofs.is_open())
	{
		ofs << configJson;
		std::cout << "Save config succeed!" << std::endl;
		ofs.close();
	}
	else
	{
		std::cout << "Can't open file \"" + configJsonName + "\"";
	}
	return 0;
}

int initialize()
{
	//bind commands
	cmdHelper.registCmd("askfreq", &askFreq, "get the centre frequency");
	cmdHelper.registCmd("setfreq", &setFreq, "set the centre frequency to a value");
	std::ifstream ifs(configJsonName);
	if (ifs.is_open())
	{
		std::cout << "Read Configuation file succeed!" << std::endl;
		ifs >> configJson;
		sgIP = configJson["sgIP"];
		sgPort = configJson["sgPort"];
	}
	else
	{
		configJson["sgIP"] = sgIP;
		configJson["sgPort"] = sgPort;
		saveConfig();
	}
	ifs.close();
	return 0;
}

int makeClient(std::string ip, uint16_t port)
{
    std::string msg;

    client.connect(ip, port);
    std::thread thread
    (
        []()
    {
        io_service.run();
    });
    while (true)
    {
        std::string msg;
        std::getline(std::cin, msg);

        if (msg == "exit") break;
        if (msg == "clientExit") break;
		std::string reply = cmdHelper.exec(msg);
		std::cout << "Reply: " << reply << std::endl;
    }
	client.exit();
    std::cout << "client exit" << std::endl;
    thread.join();
    return 0;
}

int main(int argc, char *argv[])
{
	initialize();
	if (argc >= 2)
	{
		sgIP = argv[1];
		if (argc >= 3)
			sgPort = std::stoi(argv[2]);
		saveConfig();
	}
	makeClient(sgIP, sgPort);
	return 0;
}
