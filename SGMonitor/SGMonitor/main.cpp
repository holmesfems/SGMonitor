
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

std::string askFreq(ParamSet::Params &e)
{
	return ":FREQ?";
}

std::string setFreq(ParamSet::Params &e)
{
	ParamSet::ParamHelper pHelper;
	std::string value;
	
}

int initialize()
{
	return 0;
}

int makeClient(std::string ip, uint16_t port)
{
    std::string msg;
    boost::asio::io_service io_service;
    TcpClient::TcpClient client(io_service);
    client.connect(ip, port);
    std::thread thread
    (
        [&io_service]()
    {
        io_service.run();
    });
    while (true)
    {
        std::string msg;
        std::getline(std::cin, msg);
        client.send(msg);
        if (msg == "exit") break;
        if (msg == "clientExit") break;
    }
    std::cout << "client exit" << std::endl;
    thread.join();
    return 0;
}



int main(int argc,char *argv[])
{
    
    return 0;
}
