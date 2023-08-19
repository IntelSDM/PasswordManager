#pragma once
#include <mysql_driver.h>
#include <mysql_connection.h>

class Database
{
protected:
	sql::mysql::MySQL_Driver* Driver;
	sql::Connection* Connection;
	const std::list<std::string> DatabaseNames = {"DevBuild"};
private:
	void CreateDatabase();
	void StartDatabase();
	void CreateTables();
	std::string ToLower(const std::string& input);
	sql::SQLString ToSQLString(const std::wstring& input);
public:
	Database();
	~Database();
	void AddUser(const std::wstring& username, const std::wstring& password);
};