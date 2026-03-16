#include <stdlib.h>
#include <iostream>

#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>


#define SERVER "127.0.0.1:3306"
#define USERNAME "root"
#define PASSWORD "enti"
#define DATABASE "videogame"
void ConnectDatabase(sql::Driver*& driver, sql::Connection*& con) {
	try {
		driver = get_driver_instance();
		con = driver->connect(SERVER, USERNAME, PASSWORD);
		std::cout << "Connection Done" << std::endl;
	}

	catch (sql::SQLException e){
		std::cout << "Could not connect to server. Error message: " << e.what() << std::endl;
	}
}

void DisconnectDatabase(sql::Connection* con) {
	con->close();

	if (con->isClosed()) {
		std::cout << "Connection Close" << std::endl;
		delete con;
	}
}

void GetAllUsers(sql::Connection* con) {
	
	try {
		sql::Statement* stmt = con->createStatement();
		sql::ResultSet* res = stmt->executeQuery("SELECT userName FROM users");

		std::cout << "users in the database: " << std::endl;

		while (res->next()) {
			std::cout << res->getString("users") << std::endl;
		}

		delete res;
		delete stmt;
	}

	catch (sql::SQLException& e) {
		std::cout << "Error while fetching users: " << e.what() << std::endl;
	}
	
}

void UpdatePassword(sql::Connection* con, std::string user, std::string newPassword)
{
	try
	{
		//sql::Statement* stmt = con->createStatement();
		std::string query = "UPDATE users SET password = ? WHERE userName = ?";
		sql::PreparedStatement* stmt = con->prepareStatement(query);

		stmt->setString(1, newPassword);
		stmt->setString(2, user);

		//int affected_rows = stmt->executeUpdate(query);
		int affected_rows = stmt->executeUpdate();

		std::cout << "Number of rows affected: " << affected_rows << std::endl;

		delete stmt;
	}
	catch (sql::SQLException& e)
	{
		std::cerr << "Error updating password: " << e.what() << std::endl;
	}
}

void DeleteByUser(sql::Connection* con, std::string user) {
	try {
		sql::Statement* stmt = con->createStatement();
		std::string query = "DELETE FROM users WHERE userName='" + user + "'";

		int affected_rows = stmt->executeUpdate(query);
		std::cout << "Number of users deleted" << affected_rows << std::endl;
		delete stmt;
	}
	catch (sql::SQLException& e)
	{
		std::cerr << "Error Error while deleting user: " << e.what() << std::endl;
	}
}

void CreateUsers(sql::Connection* con, std::string user, std::string password) {
	try {
		sql::Statement* stmt = con->createStatement();
		std::string query = "INSERT INTO users (userName, password) VALUES('" + user + "', '" + password + "')";

		int affected_rows = stmt->executeUpdate(query);

		if (affected_rows > 0) {
			std::cout << "User created successfully" << std::endl;
		}

		delete stmt;
	}
	catch (sql::SQLException& e)
	{
		std::cerr << "Error while creating user " << e.what() << std::endl;
	}
}

void main()
{
	sql::Driver* driver;
	sql::Connection* con;
	ConnectDatabase(driver, con);
	con->setSchema(DATABASE);

	//GetAllUsers(con);
	UpdatePassword(con, "Radev", "suspenderAlumnos");

	//DeleteByUser(con, "Edu");

	//CreateUsers(con, "Edu", "123");
	DisconnectDatabase(con);

	system("pause");
}