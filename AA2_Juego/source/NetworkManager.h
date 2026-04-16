#pragma once
#include <SFML/Network.hpp>
#include <iostream>
#include <string>
#include "NetworkProtocol.h"
#include "SceneManager.h"
#include "TicTacToe.h"

#define NM NetworkManager::Instance()
#define NUM_ROWS 6
#define NUM_COLS 6
#define INITIAL_TURN 1
#define PEER_PORT 55000
class NetworkManager {
private:
    sf::TcpSocket socket;
    sf::TcpListener p2pListener;
    std::vector<sf::TcpSocket*> p2pPeers;
    bool isConnected = false;
    int hostBoard[NUM_ROWS][NUM_COLS];
    int hostCurrentTurn = INITIAL_TURN;

    NetworkManager() {
        socket.setBlocking(false);
    }

public:
    bool nextGameMyTurn = false;
    std::string nextGameOpponent = "";
    int nextGamePlayerID = INITIAL_TURN;
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
                    p2pListener.setBlocking(true);
                    if (p2pListener.listen(PEER_PORT) == sf::Socket::Status::Done) {
                        std::cout << "[P2P] Esperando a que el peer se conecte" << std::endl;
                        // Aceptamos al P2P y metemos su conexión en socket
                        if (p2pListener.accept(socket) == sf::Socket::Status::Done) {
                            std::cout << "[P2P] PEER conectado! La partida comienza." << std::endl;
                            isConnected = true;

                            //El host es el ID 1 
                            this->nextGameMyTurn = true;
                            this->nextGameOpponent = "Jugador 2";
                            this->nextGamePlayerID = 1;

                            //Seteamos valor inicial a la jugada
                            for (int row = 0; row < NUM_ROWS; row++) {
                                for (int col = 0; col < NUM_COLS; col++) {
                                    hostBoard[row][col] = 0;
                                }
                            }
                            hostCurrentTurn = 1;
                        }
                        p2pListener.setBlocking(false);
                    } 

                    else {
                        std::cout << "[P2P] No se pudo abrir el puerto de host." << std::endl;
                    }
                }
                else {

                    socket.setBlocking(true);
                    sf::Socket::Status status = socket.connect(sf::IpAddress::resolve(hostIP).value(), PEER_PORT, sf::seconds(5.0f)); 
                    //Linea de arriba generada por IA, debido a que petaba la IP del Host: linea original:
                    //sf::Socket::Status status = socket.connect(hostIP, 55000, sf::seconds(5.0f));

                    socket.setBlocking(false);

                    if (status == sf::Socket::Status::Done) {
                        std::cout << "[P2P] Conectado al host directamente con exito" << std::endl;
                        isConnected = true; 

                        this->nextGameMyTurn = false;
                        this->nextGameOpponent = "Jugador 1";
                        this->nextGamePlayerID = 2;
                    }
                    else {
                        std::cout << "[P2P]Error: no se pudo conectar al Host en la IP: " << hostIP << std::endl;
                    }
                }
                SceneManager::Instance().SetNextScene("TicTacToe");
                break;
            }
            case PacketType::GameMove:
            {
                if (isP2PHost) {
                    int row, col;
                    packet >> row >> col;

                    // El Host revisa si el movimiento del Peer es legal
                    if (hostCurrentTurn == 2 && hostBoard[row][col] == 0) {
                        hostBoard[row][col] = 2; 
                        hostCurrentTurn = 1; 

                        //El Host lo dibuja en su pantalla (el Peer fue quien movió)
                        TicTacToe* gameScene = dynamic_cast<TicTacToe*>(SceneManager::Instance().GetCurrentScene());
                        if (gameScene) gameScene->ApplyMoveFromServer(row, col, 2, 1);

                        // El Host le confirma al Peer que el movimiento fue válido
                        sf::Packet response;
                        response << static_cast<int>(PacketType::UpdateBoard)
                            << static_cast<int>(row) << static_cast<int>(col)
                            << static_cast<int>(2)
                            << static_cast<int>(1); 
                        (void)socket.send(response);
                    }

                }
                break;
            }

            case PacketType::UpdateBoard:
            {
                int row, col, playerID, nextPlayerTurn;
                packet >> row >> col >> playerID >> nextPlayerTurn;

                // Buscamos la escena actual y aplicamos el movimiento
                TicTacToe* gameScene = dynamic_cast<TicTacToe*>(SceneManager::Instance().GetCurrentScene());
                if (gameScene) {
                    gameScene->ApplyMoveFromServer(row, col, playerID, nextPlayerTurn);
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
        if (isP2PHost) {

            // Validamos nuestra propia jugada (comprobamos que sea nuestro turno y esté vacío)
            if (hostCurrentTurn == 1 && hostBoard[row][col] == 0) {
                hostBoard[row][col] = 1; 
                hostCurrentTurn = 2;     

                // Dibujamos en nuestra propia pantalla al instante
                TicTacToe* gameScene = dynamic_cast<TicTacToe*>(SceneManager::Instance().GetCurrentScene());
                if (gameScene) gameScene->ApplyMoveFromServer(row, col, 1, 2);
        
                //Le enviamos la orden al otro jugador (el peer) de que lo dibuje (diciéndole que ahora es su turno)
                sf::Packet packet;
                packet << static_cast<int>(PacketType::UpdateBoard)
                    << static_cast<int>(row) << static_cast<int>(col)
                    << static_cast<int>(1)
                    << static_cast<int>(2);
                (void)socket.send(packet);
            }
        }
        else {
            // Al jugar su turno, el peer le envía las coordenadas al host para actualizar el movimiento
            sf::Packet packet;
            packet << static_cast<int>(PacketType::GameMove)
                << static_cast<int>(row) << static_cast<int>(col);
            (void)socket.send(packet);
        }
    }
};