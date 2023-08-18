#pragma once
#include <mysql_driver.h>
#include <mysql_connection.h>

class Database
{
protected:
	sql::mysql::MySQL_Driver* Driver;
	sql::Connection* Connection;
	const std::list<std::string> DatabaseNames = {"Users"};
private:
	void CreateDatabase();
	void StartDatabase();
	void CreateTables();
	std::string ToLower(const std::string& input);
public:
	Database();
	~Database();
};