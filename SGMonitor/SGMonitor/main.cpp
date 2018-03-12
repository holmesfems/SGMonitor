
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

//using CmdGeneratorFunc = std::function<std::string(ParamSet::Params &)>;

uint16_t sgPort = 5025;
CmdHelper::CmdHelper cmdHelper;
boost::asio::io_service io_service;
TcpClient::TcpClient client(io_service);

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
	std::string cmd = ":FREQ:CENT " + value;
	client.send(cmd);
	return askFreq(params);
}

int initialize()
{
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
    }
	client.exit();
    std::cout << "client exit" << std::endl;
    thread.join();
    return 0;
}



int main(int argc,char *argv[])
{
    
    return 0;
}
