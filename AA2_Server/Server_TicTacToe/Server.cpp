#include "Server.h"
#include "bcrypt.h"

Server::Server(unsigned short port) : port(port), isRunning(false), driver(nullptr), con(nullptr) {
}

Server::~Server() {
    Stop();
}

bool Server::ConnectDatabase() {
    try {
        driver = get_driver_instance();
        con = driver->connect(dbHost, dbUser, dbPass);
        con->setSchema(dbName);
        std::cout << "[BD] Conectado a la base de datos con exito." << std::endl;
        return true;
    }
    catch (sql::SQLException& e) {
        std::cout << "[BD] Error de conexion: " << e.what() << std::endl;
        return false;
    }
}

void Server::DisconnectDatabase() {
    if (con != nullptr) {
        con->close();
        delete con;
        con = nullptr;
        std::cout << "[BD] Conexion cerrada." << std::endl;
    }
}

bool Server::CheckUserLogin(const std::string& user, const std::string& password) {
    std::cout << "[DEBUG] Entrando a CheckUserLogin..." << std::endl;
    try {
        sql::Statement* stmt = con->createStatement();

        //buscamos usuario por  nombre
        std::string query = "SELECT password FROM users WHERE userName = '" + user + "'";

        std::cout << "[DEBUG] Ejecutando SELECT de Login..." << std::endl;
        sql::ResultSet* res = stmt->executeQuery(query);

        std::cout << "[DEBUG] Leyendo resultado del Login..." << std::endl;
        bool loginSuccess = false;

        // leemos su Hash de la base de datos
        if (res->next()) {
            std::string hashFromDB = res->getString("password");

            //  BCrypt Compara la contraseña limpia que escribe el jugador con el Hash de la BD // ayuda de IA
            if (bcrypt::validatePassword(password, hashFromDB)) {
                loginSuccess = true; 
            }
        }

        delete res;
        delete stmt;

        std::cout << "[DEBUG] Login completado. Exito: " << (loginSuccess ? "SI" : "NO") << std::endl;
        return loginSuccess;
    }
    catch (sql::SQLException& e) {
        std::cout << "[BD] Error SQL en Login: " << e.what() << std::endl;
        return false;
    }
    catch (...) {
        std::cout << "[BD] Error critico desconocido en Login." << std::endl;
        return false;
    }
}

bool Server::RegisterUser(const std::string& user, const std::string& password) {
    std::cout << "[DEBUG] Entrando a RegisterUser..." << std::endl;
    try {
        // --- PASO 1: COMPROBAR CON COUNT(*) ---
        std::cout << "[DEBUG] Creando Statement para SELECT COUNT..." << std::endl;
        sql::Statement* checkStmt = con->createStatement();

        // Le pedimos a MySQL que cuente cuántos usuarios se llaman así
        std::string checkQuery = "SELECT COUNT(*) FROM users WHERE userName = '" + user + "'";

        std::cout << "[DEBUG] Ejecutando SELECT de Registro..." << std::endl;
        sql::ResultSet* res = checkStmt->executeQuery(checkQuery);

        std::cout << "[DEBUG] Leyendo conteo..." << std::endl;
        int count = 0;
        if (res->next()) {
            count = res->getInt(1); // Leemos el número exacto
        }

        delete res;
        delete checkStmt;

        if (count > 0) {
            std::cout << "[DEBUG] El usuario ya existe (Count: " << count << "). Abortando registro." << std::endl;
            return false;
        }

        // INSERTAR CON BCRYPT //con ayuda de ia:
        std::cout << "[DEBUG] El usuario es nuevo. Generando Hash..." << std::endl;

        // Generamos el Hash de la contraseña 
        std::string hashedPassword = bcrypt::generateHash(password);

        std::cout << "[DEBUG] Creando Statement para INSERT..." << std::endl;
        sql::Statement* insertStmt = con->createStatement();

        // Metemos el Hash en la base de datos
        std::string insertQuery = "INSERT INTO users (userName, password) VALUES ('" + user + "', '" + hashedPassword + "')";

        std::cout << "[DEBUG] Ejecutando INSERT..." << std::endl;
        int rows = insertStmt->executeUpdate(insertQuery);

        delete insertStmt;

        std::cout << "[DEBUG] INSERT terminado. Filas afectadas: " << rows << std::endl;
        return rows > 0;
    }
    catch (sql::SQLException& e) {
        std::cout << "[BD] Error SQL en Registro: " << e.what() << std::endl;
        return false;
    }
    catch (...) {
        std::cout << "[BD] Error critico desconocido en Registro." << std::endl;
        return false;
    }
}

void Server::Run() {
    if (!ConnectDatabase()) return;

    if (listener.listen(port) != sf::Socket::Status::Done) {
        std::cout << "[SERVER] Error fatal: No se pudo abrir el puerto " << port << std::endl;
        return;
    }

    std::cout << "[SERVER] Encendido y escuchando en el puerto " << port << "..." << std::endl;
    selector.add(listener);
    isRunning = true;

    while (isRunning) {
        if (selector.wait()) {
            if (selector.isReady(listener)) {
                AcceptNewClient();
            }
            else {
                HandleClientPackets();
            }
        }
    }
}

void Server::AcceptNewClient() {
    sf::TcpSocket* newClient = new sf::TcpSocket();
    if (listener.accept(*newClient) == sf::Socket::Status::Done) {
        clients.push_back(newClient);
        selector.add(*newClient);
        std::cout << "[SERVER] Nuevo cliente conectado desde: " << newClient->getRemoteAddress().value() << std::endl;
    }
    else {
        delete newClient;
    }
}

void Server::HandleClientPackets() {
    for (auto it = clients.begin(); it != clients.end(); ) {
        sf::TcpSocket* client = *it;

        if (selector.isReady(*client)) {
            sf::Packet packet;
            sf::Socket::Status status = client->receive(packet);

            if (status == sf::Socket::Status::Done) {
                int typeInt;
                if (packet >> typeInt) {
                    PacketType type = static_cast<PacketType>(typeInt);

                    if (type == PacketType::LoginRequest) {
                        std::string user, pass;
                        packet >> user >> pass;
                        std::cout << "[SERVER] Peticion de Login -> Usuario: " << user << std::endl;

                        bool isValid = CheckUserLogin(user, pass);

                        sf::Packet response;
                        if (isValid) {
                            std::cout << "[SERVER] Login correcto." << std::endl;
                            loggedInUsers[client] = user;
                            response << static_cast<int>(PacketType::LoginSuccess);
                        }
                        else {
                            std::cout << "[SERVER] Login incorrecto." << std::endl;
                            response << static_cast<int>(PacketType::LoginFailed);
                        }
                        client->send(response);
                    }
                    else if (type == PacketType::RegisterRequest) {
                        std::string user, pass;
                        packet >> user >> pass;
                        std::cout << "[SERVER] Peticion de Registro -> Usuario: " << user << std::endl;

                        bool isRegistered = RegisterUser(user, pass);

                        sf::Packet response;
                        if (isRegistered) {
                            std::cout << "[SERVER] Registro correcto. Usuario guardado en BD." << std::endl;
                            response << static_cast<int>(PacketType::RegisterSuccess);
                        }
                        else {
                            std::cout << "[SERVER] Registro fallido (el usuario ya existe)." << std::endl;
                            response << static_cast<int>(PacketType::RegisterFailed);
                        }
                        (void)client->send(response);
                    }
                    else if (type == PacketType::RankingRequest) {
                        std::cout << "[SERVER] " << loggedInUsers[client] << " ha pedido el Ranking." << std::endl;
                        SendRankingToClient(client);
                    }
                    else if (type == PacketType::CreateRoomRequest) {
                        std::string roomName;
                        packet >> roomName;
                        HandleCreateRoom(client, roomName);
                    }
                    else if (type == PacketType::JoinRoomRequest) {
                        std::string roomName;
                        packet >> roomName;
                        HandleJoinRoom(client, roomName);
                    } 

                    
                 
                               // --- EL BLOQUE VITAL PARA EL ELO QUE FALTABA ---
                    else if (type == PacketType::ReportResult) {
                        std::string roomName;
                        int p1, p2, p3, p4;
                        // Leemos la sala y los 4 IDs numéricos
                        packet >> roomName >> p1 >> p2 >> p3 >> p4;

                        std::cout << "[DEBUG] Paquete de final de partida recibido. Sala: " << roomName << std::endl;

                        std::vector<int> finalStandings = { p1, p2, p3, p4 };
                        HandleMatchResult(client, roomName, finalStandings);
                    }

                   /* else if (type == PacketType::GameMove) {
                        int row = 0;
                        int col = 0;
                        packet >> row >> col;
                        std::cout << "[SERVER] " << loggedInUsers[client] << " ha intentado mover en Fila: " << row << ", Columna: " << col << std::endl;
                        HandleGameMove(client, row, col);
                    }*/
                }
                ++it;
            }
            else if (status == sf::Socket::Status::Disconnected || status == sf::Socket::Status::Error) {
                std::cout << "[SERVER] Un cliente se ha desconectado." << std::endl;
                loggedInUsers.erase(client);
                selector.remove(*client);
                client->disconnect();
                delete client;
                it = clients.erase(it);
            }
            else {
                ++it;
            }
        }
        else {
            ++it;
        }
    }
}

void Server::SendRankingToClient(sf::TcpSocket* client) {
    try {
        std::string currentUser = loggedInUsers[client];
        sql::Statement* stmt = con->createStatement();

        // ranking top 10
        std::string queryTop10 = "SELECT userName, points, wins, losses FROM users ORDER BY points DESC, wins DESC LIMIT 10";
        sql::ResultSet* res = stmt->executeQuery(queryTop10);

        sf::Packet response;
        response << static_cast<int>(PacketType::RankingResponse);

        struct PlayerData { int pos; std::string name; int pts; int v; int d; };
        std::vector<PlayerData> ranking;

        bool userInTop10 = false;
        int currentPos = 1;

        while (res->next()) {
            std::string name = res->getString("userName");
            int pts = res->getInt("points");
            int v = res->getInt("wins");
            int d = res->getInt("losses");

            ranking.push_back({ currentPos, name, pts, v, d });
            if (name == currentUser) userInTop10 = true;
            currentPos++;
        }
        delete res;

		// si no esta en el top 10, buscamos su posicion exacta
        if (!userInTop10 && currentUser != "") {
            std::string queryMyRank =
                "SELECT points, wins, losses, "
                "(SELECT COUNT(*) + 1 FROM users WHERE points > u.points) AS myPos "
                "FROM users u WHERE userName = '" + currentUser + "'";

            sql::ResultSet* resMe = stmt->executeQuery(queryMyRank);
            if (resMe->next()) {
                int myPos = resMe->getInt("myPos");
                int pts = resMe->getInt("points");
                int v = resMe->getInt("wins");
                int d = resMe->getInt("losses");
                ranking.push_back({ myPos, currentUser, pts, v, d });
            }
            delete resMe;
        }
        delete stmt;

		// enviamos al cliente el ranking completo (top 10 + su posicion si no esta)
        response << static_cast<int>(ranking.size());
        for (const auto& p : ranking) {
            response << p.pos << p.name << p.pts << p.v << p.d;
        }

        (void)client->send(response);
        std::cout << "[SERVER] Ranking enviado a " << currentUser << std::endl;

    }
    catch (sql::SQLException& e) {
        std::cout << "[BD] Error SQL enviando ranking: " << e.what() << std::endl;
    }
}

void Server::Stop() {
    isRunning = false;
    listener.close();
    selector.clear();
    for (auto* client : clients) {
        client->disconnect();
        delete client;
    }
    clients.clear();
    DisconnectDatabase();
    std::cout << "[SERVER] Servidor apagado." << std::endl;
}

void Server::HandleCreateRoom(sf::TcpSocket* client, const std::string& roomName) {
    // 1. Comprobar si la sala ya existe
    for (const auto& room : activeRooms) {
        if (room.name == roomName) {
            sf::Packet response;
            response << static_cast<int>(PacketType::RoomError); // Ya existe
            (void)client->send(response);
            return;
        }
    }

    // 2. Si no existe, la creamos y metemos al jugador 1
    Room newRoom;
    newRoom.name = roomName;
    newRoom.players.push_back(client);
    activeRooms.push_back(newRoom);

    std::cout << "[SERVER] Sala '" << roomName << "' creada por " << loggedInUsers[client] << std::endl;

    sf::Packet response;
    response << static_cast<int>(PacketType::RoomSuccess);
    (void)client->send(response);
}

void Server::HandleJoinRoom(sf::TcpSocket* client, const std::string& roomName) {
    // 1. Buscar la sala
    for (auto& room : activeRooms) {
        if (room.name == roomName) {
            // 2. Ver si hay hueco
            if (room.players.size() < 4) {
                room.players.push_back(client);
                std::cout << "[SERVER] " << loggedInUsers[client] << " se ha unido a '" << roomName << "'" << std::endl;

                sf::Packet successResponse;
                successResponse << static_cast<int>(PacketType::RoomSuccess);
                (void)client->send(successResponse);

                if (room.players.size() == 4) {
                    std::cout << "[SERVER] Sala llena, iniciando emparejamiento..." << std::endl;

                    // --- NUEVO: GUARDAMOS LA PARTIDA Y LOS NOMBRES REALES ---
                    OngoingMatch newMatch;
                    newMatch.roomName = room.name;

                    // Guardamos los nombres reales en orden (ID 1 = pos 0, ID 2 = pos 1...)
                    for (auto* p : room.players) {
                        newMatch.realNames.push_back(loggedInUsers[p]);
                    }

                    activeMatches.push_back(newMatch);
                    // --------------------------------------------------------

                    //Estructura de los datos de los peers
                    struct PeerData {
                        int id;
                        std::string ip;
                        unsigned short port;
                    };

                    std::vector<PeerData> peers;
                    for (int i = 0; i < room.players.size(); i++) {
                        peers.push_back({ i + 1, room.players[i]->getRemoteAddress().value().toString(), static_cast<unsigned short>(PORT + i) });
                    }

                    for (int i = 0; i < room.players.size(); i++) {
                        sf::Packet startPacket;
                        startPacket << static_cast<int>(PacketType::GameStart);
                        startPacket << peers[i].id;
                        startPacket << peers[i].port;
                        startPacket << static_cast<int>(3); // Número de oponentes

                        for (int j = 0; j < room.players.size(); j++) {
                            if (i != j) {
                                startPacket << peers[j].id << peers[j].ip << peers[j].port;
                            }
                        }

                        (void)room.players[i]->send(startPacket);

                       
                    }

                    // Limpiamos la sala 
                    room.players.clear();
                }
                return;
            }
            else {
                // La sala está llena
                sf::Packet response;
                response << static_cast<int>(PacketType::RoomError);
                (void)client->send(response);
                return;
            }
        }
    }

    // Si llega aquí, es que la sala no existe
    sf::Packet response;
    response << static_cast<int>(PacketType::RoomError);
    (void)client->send(response);
}



void Server::HandleMatchResult(sf::TcpSocket* client, const std::string& roomName, const std::vector<int>& placements) {
    bool roomFound = false; 
    for (auto it = activeMatches.begin(); it != activeMatches.end(); ++it) {
        if (it->roomName == roomName) {
            roomFound = true;
            bool matched = false;

            // Peer Verification // debugs hechos con ia 
            for (auto& report : it->reportedResults) {
                if (report.placements == placements) {
                    matched = true;
                    break;
                }
            }

            if (matched) {
                std::cout << "[SERVER] Resultados validados por pares para la sala: " << roomName << ". Actualizando ELO..." << std::endl;

                std::string winner1 = it->realNames[placements[0] - 1];
                std::string winner2 = it->realNames[placements[1] - 1];
                std::string loser3 = it->realNames[placements[2] - 1];
                std::string loser4 = it->realNames[placements[3] - 1];

                UpdatePlayerStats(winner1, 30, 1, 0);
                UpdatePlayerStats(winner2, 15, 1, 0);
                UpdatePlayerStats(loser3, -15, 0, 1);
                UpdatePlayerStats(loser4, -30, 0, 1);

                activeMatches.erase(it);
                return;
            }
            else {
                MatchResult newReport;
                newReport.placements = placements;
                it->reportedResults.push_back(newReport);
                std::cout << "[SERVER] Resultado parcial recibido de '" << roomName << "'. Esperando validacion de otro jugador..." << std::endl;
                return;
            }
        }
    }

    if (!roomFound) {
        std::cout << "[SERVER ERROR] Me ha llegado un resultado para la sala '" << roomName << "' pero no la tengo en memoria!" << std::endl;
    }
}

void Server::UpdatePlayerStats(const std::string& user, int pointsOffset, int winOffset, int lossOffset) {
    try {
        sql::Statement* stmt = con->createStatement();

        // Lógica de puntos blindada anti-crasheos de MySQL hecho con ia 
        std::string pointsLogic;
        if (pointsOffset >= 0) {
            pointsLogic = "points = points + " + std::to_string(pointsOffset);
        }
        else {
            // Si vamos a restar, primero comprobamos si tiene suficientes puntos para no bajar de cero
            int absOffset = std::abs(pointsOffset);
            pointsLogic = "points = CASE WHEN points < " + std::to_string(absOffset) + " THEN 0 ELSE points - " + std::to_string(absOffset) + " END";
        }

        std::string query = "UPDATE users SET " +
            pointsLogic + ", " +
            "wins = wins + " + std::to_string(winOffset) + ", " +
            "losses = losses + " + std::to_string(lossOffset) + " " +
            "WHERE userName = '" + user + "'";

        stmt->executeUpdate(query);
        delete stmt;
        std::cout << "[BD] Stats actualizados para: " << user << " (Pts: " << pointsOffset << ")" << std::endl;
    }
    catch (sql::SQLException& e) {
        std::cout << "[BD] Error actualizando stats: " << e.what() << std::endl;
    }
}


//void Server::HandleGameMove(sf::TcpSocket* client, int row, int col) {
//    // Buscamos en qué sala está este cliente
//    for (auto& room : activeRooms) {
//        if (room.player1 == client || room.player2 == client) {
//
//            int playerID = (room.player1 == client) ? 1 : 2;
//
//            // 1. Validar que sea su turno
//            if (room.currentTurn != playerID) return;
//
//            // 2. Validar que la casilla esté vacía
//            if (room.board[row][col] != 0) return;
//
//            // 3. Aplicar el movimiento en el servidor
//            room.board[row][col] = playerID;
//
//            // 4. Cambiar el turno
//            room.currentTurn = (playerID == 1) ? 2 : 1;
//
//            // 5. Avisar a AMBOS jugadores del movimiento y de quién le toca ahora
//            sf::Packet p1Packet, p2Packet;
//
//            p1Packet << static_cast<int>(PacketType::UpdateBoard) << row << col << playerID << (room.currentTurn == 1);
//            p2Packet << static_cast<int>(PacketType::UpdateBoard) << row << col << playerID << (room.currentTurn == 2);
//
//            (void)room.player1->send(p1Packet);
//            (void)room.player2->send(p2Packet);
//
//            return;
//        }
//    }
//}