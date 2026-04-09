#pragma once
#include "Scene.h"
#include "SceneManager.h"
#include "RenderManager.h"
#include <SFML/Network.hpp>
#include <iostream>
#include "Button.h"
#include "TextObject.h"
#define SERVER_PORT 55002
class Login : public Scene {
private:
    float windowWidth = RM->WINDOW_WIDTH;
    float windowHeight = RM->WINDOW_HEIGHT;

	Object* textMenu = nullptr;

	Button* buttonLogin = nullptr;
	Button* buttonRegister = nullptr;
	Button* buttonExit = nullptr;

	Object* buttonLoginText = nullptr;
	Object* buttonAudioText = nullptr;
	Object* buttonExitText = nullptr;
    const sf::IpAddress SERVER_IP = sf::IpAddress(10, 40, 1, 214);
    enum tipoPaquete { HANDSHAKE, LOGIN, MOVIMIENTO };

    sf::Packet& operator<<(sf::Packet& packet, tipoPaquete& tipo){
        int temp;
        packet >> temp;
        tipo = static_cast <tipoPaquete>(temp);

        return packet;
    }

    void HandShake(sf::Packet data) {
        std::string receiveMessage;
        data >> receiveMessage;

        std::cout << "MEnsaje enviado del servidor: " << receiveMessage << std::endl;
    }

    void LoginUser(sf::Packet data) {
        std::string user;
        std::string psswrd;

        data >> user;
        data >> psswrd;

        std::cout << "Username: " << user << std::endl;
        std::cout << "Password: " << psswrd << std::endl;
    }

public:
    void OnEnter() override {
        // Aquí puedes agregar los objetos que quieras que aparezcan en la pantalla de login
        // Por ejemplo, un fondo, un título, campos de texto para el usuario y la contraseña, etc.
		AM.PlaySong("menuMusic");

		textMenu = new TextObject("MAIN MENU");
		textMenu->GetTransform()->position = Vector2(windowWidth / 2 - 75.0f, windowHeight / 2 - 150);
		SPAWN.SpawnObject(textMenu);

		float btn1_Y = windowHeight / 2 - 50;
		float btn2_Y = windowHeight / 2 + 30;
		float btn3_Y = windowHeight / 2 + 110;

		float btn_X = windowWidth / 2 - 125.0f;

		buttonLogin = new Button(Vector2(btn_X, btn1_Y), Vector2(250, 50), sf::Color(255, 0, 255, 255), "", Button::ActionType::ChangeScene, "Space Invaders");
		buttonRegister = new Button(Vector2(btn_X, btn2_Y), Vector2(250, 50), sf::Color(255, 0, 255, 255), "", Button::ActionType::ToggleAudio);
		buttonExit = new Button(Vector2(btn_X, btn3_Y), Vector2(250, 50), sf::Color(255, 0, 255, 255), "", Button::ActionType::ExitGame);

		SPAWN.SpawnObject(buttonLogin);
		SPAWN.SpawnObject(buttonRegister);
		SPAWN.SpawnObject(buttonExit);

		buttonLoginText = new TextObject("TIC TAC TOE");
		buttonLoginText->GetTransform()->position = Vector2(windowWidth / 2 - 80.0f, btn1_Y + 10);
		SPAWN.SpawnObject(buttonLoginText);

		buttonAudioText = new TextObject("TOGGLE AUDIO");
		buttonAudioText->GetTransform()->position = Vector2(windowWidth / 2 - 90.0f, btn2_Y + 10);
		SPAWN.SpawnObject(buttonAudioText);

		buttonExitText = new TextObject("EXIT GAME");
		buttonExitText->GetTransform()->position = Vector2(windowWidth / 2 - 71.0f, btn3_Y + 10);
		SPAWN.SpawnObject(buttonExitText);

	}

    ~Login() {

    }
    void OnExit() override {
        // Aquí puedes limpiar los objetos específicos de la pantalla de login si es necesario
        Scene::OnExit(); // Llama al método base para limpiar los objetos comunes
    }
    void Update() override {
        // Aquí puedes manejar la lógica de actualización específica de la pantalla de login
		sf::TcpSocket socket;

		if (socket.connect(SERVER_IP, SERVER_PORT) != sf::Socket::Status::Done) {
			std::cerr << "Error al conectar con el servidor" << std::endl;
		}
		else {
			std::cout << "Conectado con el servidor" << std::endl;
			socket.setBlocking(false);

			sf::Packet packet;

			bool gameOver = false;

			while (!gameOver) {

				std::string message;
				std::cout << "Inserta mensaje para el servidor, -1 para salir" << std::endl;
				std::cin >> message;

				if (message == "-1") {
					std::cout << "Desconectando..." << std::endl;
					gameOver = true;
				}
				else {
					sf::Packet packet;
					packet << message;
					if (socket.send(packet) != sf::Socket::Status::Done) {
						std::cerr << "Error al enviar el paquete al servidor" << std::endl;
					}
					else {
						std::cout << "Mensaje enviado: " << message << std::endl;
					}
				}
				//sf::sleep(sf::seconds(1));
				//if (socket.receive(packet) == sf::Socket::Status::Done) {
				//	//std::string receiveMessage;
				//	// packet >> receiveMessage;

				//	tipoPaquete tipo;
				//	packet >> tipo;

				//	switch (tipo) {
				//	case HANDSHAKE:
				//		HandShake(packet);
				//		break;
				//	case LOGIN:
				//		Login(packet);
				//		break;
				//	case MOVIMIENTO:
				//		break;
				//	}
				//	//std::cout << "Mensaje recibido del servidor: " << receiveMessage << std::endl;
				//	packet.clear();
				//}
				//if (socket.receive(packet) == sf::Socket::Status::Disconnected) {
				//	gameOver = true;
				//}
				//else {
				//	std::cerr << "Error al recibir el mensaje del servidor" << std::endl;
				//}
			}
		}

		if(buttonLogin)
        Scene::Update(); // Llama al método base para manejar la actualización común
    }
    void Render() override {
        // Aquí puedes manejar la lógica de renderizado específica de la pantalla de login
        Scene::Render(); // Llama al método base para manejar el renderizado común
	}
};

