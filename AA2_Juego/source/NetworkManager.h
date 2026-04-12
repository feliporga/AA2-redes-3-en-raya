#pragma once
#include <SFML/Network.hpp>
#include <iostream>
#include <string>
#include "NetworkProtocol.h"
#include "SceneManager.h"

#define NM NetworkManager::Instance()

class NetworkManager {
private:
    sf::TcpSocket socket;
    bool isConnected = false;

    NetworkManager() {
        // Run in non-blocking mode to prevent the game from freezing while listening
        socket.setBlocking(false);
    }

public:
    static NetworkManager& Instance() {
        static NetworkManager instance;
        return instance;
    }

    // Estructura para guardar los datos del ranking temporalmente
    struct PlayerRecord {
        int pos;
        std::string name;
        int pts;
        int v;
        int d;
    };
    std::vector<PlayerRecord> lastRanking;
    bool newRankingAvailable = false;

    bool ConnectToServer(const std::string& ip, unsigned short port) {
        // Temporarily block to ensure connection is established before continuing
        socket.setBlocking(true);

        sf::Socket::Status status = socket.connect(sf::IpAddress::resolve(ip).value(), port, sf::seconds(2.0f));

        // Revert to non-blocking for normal gameplay
        socket.setBlocking(false);

        if (status == sf::Socket::Status::Done) {
            std::cout << "[CLIENTE] Conectado al servidor con exito!" << std::endl;
            isConnected = true;
            return true;
        }
        std::cout << "[CLIENTE] Error al conectar con el servidor." << std::endl;
        return false;
    }

    void SendLogin(const std::string& username, const std::string& password) {
        if (!isConnected) {
            std::cout << "[CLIENTE] No estas conectado al servidor." << std::endl;
            return;
        }

        sf::Packet packet;
        packet << static_cast<int>(PacketType::LoginRequest) << username << password;

        if (socket.send(packet) == sf::Socket::Status::Done) {
            std::cout << "[CLIENTE] Peticion de Login enviada al servidor." << std::endl;
        }
    }

    void Listen() {
        if (!isConnected) return;

        sf::Packet packet;
        // Poll for incoming packets every frame without hanging the main loop
        if (socket.receive(packet) == sf::Socket::Status::Done) {
            int typeInt;
            packet >> typeInt;
            PacketType type = static_cast<PacketType>(typeInt);

            switch (type) {
            case PacketType::LoginSuccess:
                std::cout << "[CLIENTE] El servidor acepto el Login! Entrando al juego..." << std::endl;
                SceneManager::Instance().SetNextScene("MainMenu");
                break;
            case PacketType::LoginFailed:
                std::cout << "[CLIENTE] Error: Usuario o contrasena incorrectos." << std::endl;
                break;
            case PacketType::RegisterSuccess:
                std::cout << "[CLIENTE] Cuenta creada con exito! Ahora puedes hacer Login." << std::endl;
                break;
            case PacketType::RegisterFailed:
                std::cout << "[CLIENTE] Error: El nombre de usuario ya existe." << std::endl;
                break;
            case PacketType::RankingResponse:
                {
                int numPlayers;
                packet >> numPlayers;
                lastRanking.clear(); // Limpiamos la lista vieja

                for (int i = 0; i < numPlayers; i++) {
                    PlayerRecord rec;
                    packet >> rec.pos >> rec.name >> rec.pts >> rec.v >> rec.d;
                    lastRanking.push_back(rec);
                }
                newRankingAvailable = true; // ¡Avisamos a la escena de que ya están listos!
                break;
                }
            }
        }
    }

    void SendRegister(const std::string& username, const std::string& password) {
        if (!isConnected) {
            std::cout << "[CLIENTE] No estas conectado al servidor." << std::endl;
            return;
        }

        sf::Packet packet;
        packet << static_cast<int>(PacketType::RegisterRequest) << username << password;

        if (socket.send(packet) == sf::Socket::Status::Done) {
            std::cout << "[CLIENTE] Peticion de Registro enviada." << std::endl;
        }
    }

    void RequestRanking() {
        if (!isConnected) return;
        sf::Packet packet;
        packet << static_cast<int>(PacketType::RankingRequest);
        (void)socket.send(packet);
    }
};