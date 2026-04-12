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

        // --- PASO 2: INSERTAR CON BCRYPT --- //con ayuda de ia:
        std::cout << "[DEBUG] El usuario es nuevo. Generando Hash..." << std::endl;

        // 1. Generamos el Hash de la contraseña 
        std::string hashedPassword = bcrypt::generateHash(password);

        std::cout << "[DEBUG] Creando Statement para INSERT..." << std::endl;
        sql::Statement* insertStmt = con->createStatement();

        // 2. Metemos el Hash en la base de datos, NUNCA la contraseña real
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
                }
                ++it;
            }
            else if (status == sf::Socket::Status::Disconnected || status == sf::Socket::Status::Error) {
                std::cout << "[SERVER] Un cliente se ha desconectado." << std::endl;
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