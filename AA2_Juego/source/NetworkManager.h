#pragma once
#include <SFML/Network.hpp>
#include <iostream>
#include <string>
#include "NetworkProtocol.h"
#include "SceneManager.h"
#include "TicTacToe.h"

#define NM NetworkManager::Instance()

class NetworkManager {
private:
    sf::TcpSocket socket;
    sf::TcpListener p2pListener;
    std::vector<sf::TcpSocket*> p2pPeers;
    bool isConnected = false;

    NetworkManager() {
        socket.setBlocking(false);
    }

public:
    bool nextGameMyTurn = false;
    std::string nextGameOpponent = "";
    int nextGamePlayerID = 1;
    static NetworkManager& Instance() {
        static NetworkManager instance;
        return instance;
    }

    bool isP2PHost = false;
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
        socket.setBlocking(true);

        sf::Socket::Status status = socket.connect(sf::IpAddress::resolve(ip).value(), port, sf::seconds(2.0f));

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
                lastRanking.clear(); 

                for (int i = 0; i < numPlayers; i++) {
                    PlayerRecord rec;
                    packet >> rec.pos >> rec.name >> rec.pts >> rec.v >> rec.d;
                    lastRanking.push_back(rec);
                }
                newRankingAvailable = true; 
                break;
                }
            case PacketType::RoomSuccess:
                std::cout << "[CLIENTE] Operacion de sala correcta. Esperando jugadores..." << std::endl;
                break;
            case PacketType::RoomError:
                std::cout << "[CLIENTE] Error: La sala ya existe, esta llena o no se encontro." << std::endl;
                break;
            /*case PacketType::GameStart:
            {
                bool isMyTurn;
                std::string opponentName;
                packet >> isMyTurn >> opponentName;

                std::cout << "\n=====================================" << std::endl;
                std::cout << "  PARTIDA ENCONTRADA! VS " << opponentName << std::endl;
                std::cout << "  Tu turno inicial: " << (isMyTurn ? "SI" : "NO") << std::endl;
                std::cout << "=====================================\n" << std::endl;

                // IMPORTANTE: Aquí cambiamos de escena automáticamente al empezar la partida
                SceneManager::Instance().SetNextScene("TicTacToe");
                break;
            }*/
            /*case PacketType::GameStart:
            {
                bool myTurn;
                std::string opponent;
                packet >> myTurn >> opponent;

                this->nextGameMyTurn = myTurn;
                this->nextGameOpponent = opponent;
                this->nextGamePlayerID = myTurn ? 1 : 2;

                std::cout << "[CLIENTE] Empieza la partida contra " << opponent << ". Mi turno: " << (myTurn ? "SI" : "NO") << std::endl;

                SceneManager::Instance().SetNextScene("TicTacToe");
                break;
            }*/

            case PacketType::GameStart:
            {
                bool isHost;
                std::string hostIP;
                packet >> isHost >> hostIP;

                this->isP2PHost = isHost;

                socket.disconnect();
                isConnected = false;
                std::cout << "[CLIENTE] Desconectado del servidor principal (Bootstrap)." << std::endl;

                //Iniciar P2P
                if (isHost) {
                    socket.setBlocking(false);
                    if (p2pListener.listen(54000) == sf::Socket::Status::Done) {
                        std::cout << "[P2P] Soy el HOST. Puertas abiertas en el puerto 54000." << std::endl;
                    }
                    else {
                        std::cout << "[P2P] No se pudo abrir el puerto de Host." << std::endl;
                    }
                }
                else {

                    socket.setBlocking(true);
                    sf::Socket::Status status = socket.connect(sf::IpAddress::resolve(hostIP).value(), 54000, sf::seconds(5.0f)); 
                    //Linea de arriba generada por IA, debido a que petaba la IP del Host: linea original:
                    //sf::Socket::Status status = socket.connect(hostIP, 54000, sf::seconds(5.0f));
                    socket.setBlocking(false);

                    if (status == sf::Socket::Status::Done) {
                        std::cout << "[P2P] Conectado al HOST directamente con exito!" << std::endl;
                        isConnected = true; 
                    }
                    else {
                        std::cout << "[P2P ERROR] No se pudo conectar al Host en la IP: " << hostIP << std::endl;
                    }
                }

            }
            case PacketType::UpdateBoard:
            {
                int r, c, playerID;
                bool nextTurnIsMine;
                packet >> r >> c >> playerID >> nextTurnIsMine;

                // Buscamos la escena actual y aplicamos el movimiento
                TicTacToe* gameScene = dynamic_cast<TicTacToe*>(SceneManager::Instance().GetCurrentScene());
                if (gameScene) {
                    gameScene->ApplyMoveFromServer(r, c, playerID, nextTurnIsMine);
                }
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

    void SendCreateRoom(const std::string& roomName) {
        if (!isConnected) return;
        sf::Packet packet;
        packet << static_cast<int>(PacketType::CreateRoomRequest) << roomName;

        if (socket.send(packet) == sf::Socket::Status::Done) {
            std::cout << "[CLIENTE] Peticion para CREAR sala '" << roomName << "' enviada." << std::endl;
        }
    }

    void SendJoinRoom(const std::string& roomName) {
        if (!isConnected) return;
        sf::Packet packet;
        packet << static_cast<int>(PacketType::JoinRoomRequest) << roomName;

        if (socket.send(packet) == sf::Socket::Status::Done) {
            std::cout << "[CLIENTE] Peticion para UNIRSE a sala '" << roomName << "' enviada." << std::endl;
        }
    }

    void SendGameMove(int row, int col) {
        if (!isConnected) return;
        std::cout << "[CLIENTE] Pieza clickada en fila " << row << "y columna " << col << std::endl;
        sf::Packet packet;
        packet << static_cast<int>(PacketType::GameMove) << row << col;
        (void)socket.send(packet);
    }
};