#include "Server.h"

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
        std::string query = "SELECT * FROM users WHERE userName = '" + user + "' AND password = '" + password + "'";

        std::cout << "[DEBUG] Ejecutando SELECT de Login..." << std::endl;
        sql::ResultSet* res = stmt->executeQuery(query);

        std::cout << "[DEBUG] Leyendo resultado del Login..." << std::endl;
        bool loginSuccess = res->next();

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

        // Al destruir aquí, como era un COUNT, el candado se libera 100% seguro
        delete res;
        delete checkStmt;

        if (count > 0) {
            std::cout << "[DEBUG] El usuario ya existe (Count: " << count << "). Abortando registro." << std::endl;
            return false;
        }

        // --- PASO 2: INSERTAR ---
        std::cout << "[DEBUG] El usuario es nuevo. Creando Statement para INSERT..." << std::endl;
        sql::Statement* insertStmt = con->createStatement();
        std::string insertQuery = "INSERT INTO users (userName, password) VALUES ('" + user + "', '" + password + "')";

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