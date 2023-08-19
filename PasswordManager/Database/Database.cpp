#include "pch.h"
#include "Database.h"
#include <json.hpp>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/sqlstring.h>
#pragma comment(lib, "mysqlcppconn.lib")
#include "SHA256.h"
Database::Database()
{
	Database::StartDatabase();
	Database::CreateDatabase();
	Database::CreateTables();
	Database::AddUser(L"test1", L"Test");
}
void Database::CreateTables()
{
	// we do this for all our databases so we can set up a database such as a development database and commercial database with the same tables
	for (std::string database : Database::DatabaseNames)
	{
		Database:Connection->setSchema(database);

		sql::ResultSet* res;
		res = Connection->createStatement()->executeQuery("SHOW TABLES");
		std::map<std::string, bool> usedtables;
		while (res->next())
		{
			std::cout << "Found Table: " << res->getString(1) << std::endl;
			usedtables[Database::ToLower(res->getString(1))] = true;
		}
		// create our tables here
		if (usedtables[Database::ToLower("Users")] == false)
		{

			Connection->createStatement()->execute("CREATE TABLE Users ("
				"id INT AUTO_INCREMENT PRIMARY KEY,"
				"Username VARCHAR(255),"
				"Password VARCHAR(255),"
				"Salt VARCHAR(255))");

		}

		delete res;
	}
}
std::string Database::ToLower(const std::string& input)
{
	std::string result = input;
	std::transform(result.begin(), result.end(), result.begin(), [](unsigned char c)
		{
		return std::tolower(c);
		});
	return result;
}
void Database::CreateDatabase()
{
	sql::ResultSet* result = Connection->createStatement()->executeQuery("SHOW DATABASES");
	std::map<std::string, bool> databaseexistance;

	// set up datavasexistance with our databases
	for (std::string databasename : Database::DatabaseNames)
	{
			databaseexistance[Database::ToLower(databasename)] = false;
	}

	// set all the current databases we got from the query to true
	while (result->next())
	{
		std::string existingdatabasename = result->getString(1);
		databaseexistance[existingdatabasename] = true;
	}

	// check if databases exist
	bool dbresult = false;
	for (auto existancepair : databaseexistance)
	{
		if (existancepair.second == false)
		{
			// database not found, create it!
			std::cout<< "Creating " << existancepair.first << std::endl;
			dbresult = Connection->createStatement()->execute("CREATE DATABASE " + existancepair.first);
			if (dbresult)
			{
				std::cout << existancepair.first << " Created Successfully." << std::endl;
			}
			else 
			{
				std::cout << "Error Creating " << existancepair.first << std::endl;
			}
		}
		else
		{
			std::cout << existancepair.first << " Found\n";
		}
	}
}
std::wstring Database::GenerateSalt()
{
	const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
	const int length = 16;

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> distribution(0, sizeof(charset) - 2);

	std::wstring salt;
	salt.reserve(length);

	for (int i = 0; i < length; ++i)
	{
		salt += charset[distribution(gen)];
	}

	return salt;
}
void Database::StartDatabase()
{
	Driver = sql::mysql::get_mysql_driver_instance();
	Connection = Driver->connect("tcp://127.0.0.1:3306", "root", "CoolPassword");
	
}

Database::~Database()
{
	Connection->close();
	delete Connection;
}
sql::SQLString Database::ToSQLString(const std::wstring& input)
{
	std::string utf8(input.begin(), input.end());
	return sql::SQLString(utf8.c_str());
}
void Database::AddUser(const std::wstring& username,const std::wstring& password)
{
	Connection->setSchema("DevBuild");

	sql::PreparedStatement* statement = Connection->prepareStatement("INSERT INTO Users (Username, Password, Salt) VALUES (?, ?, ?)");

	std::wstring salt = Database::GenerateSalt();
	statement->setString(1, ToSQLString(username));
	statement->setString(2, ToSQLString(sha256(password + salt)));
	statement->setString(3, ToSQLString(salt));
	statement->execute();
	delete statement;
}
