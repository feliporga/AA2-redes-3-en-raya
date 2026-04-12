#pragma once
#include <SFML/Network.hpp>
#include <iostream>
#include <vector>
#include <string>

#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>

#include "NetworkProtocol.h"

class Server {
private:
    unsigned short port;
    bool isRunning;
    sf::TcpListener listener;
    sf::SocketSelector selector;
    std::vector<sf::TcpSocket*> clients;
    std::map<sf::TcpSocket*, std::string> loggedInUsers;

    sql::Driver* driver;
    sql::Connection* con;
    std::string dbHost = "127.0.0.1:3306";
    std::string dbUser = "root";
    std::string dbPass = "enti";
    std::string dbName = "tictactoe_db";

    bool ConnectDatabase();
    void DisconnectDatabase();
    bool CheckUserLogin(const std::string& user, const std::string& password);
    bool RegisterUser(const std::string& user, const std::string& password);

    void AcceptNewClient();
    void HandleClientPackets();

    void SendRankingToClient(sf::TcpSocket* client);

public:
    Server(unsigned short port);
    ~Server();

    void Run();
    void Stop();
};