#pragma once
#include <mysql_driver.h>
#include <mysql_connection.h>
#include "Manager.h"
enum class UserRegistrationResult
{
	Success,
	InvalidUsername,
};
enum class UserVerificationResult
{
	Success,
	InvalidUsername,
	InvalidPassword,
};
enum class AddManagerResult
{
	Success,
	InvalidUser,
	NoInstances
};
enum class GetManagerResult
{
	Success,
	InvalidUser,
	NoInstances
};
class Database
{
protected:
	sql::mysql::MySQL_Driver* Driver;
	sql::Connection* Connection;
	const std::list<std::string> DatabaseNames = {"DevBuild"};
	std::wstring Username;
	std::wstring Password;
	bool LoggedIn = false;
private:
	void CreateDatabase();
	void StartDatabase();
	void CreateTables();
	std::string ToLower(const std::string& input);
	sql::SQLString ToSQLString(const std::wstring& input);
	std::wstring SQLStringToWString(const sql::SQLString& sqlstring);
	std::wstring GenerateSalt();

public:
	Database();
	~Database();
	UserRegistrationResult AddUser(const std::wstring& username, const std::wstring& password);
	UserVerificationResult VerifyUser(const std::wstring& username, const std::wstring& password);
	AddManagerResult AddManager(const std::wstring& username, const std::wstring& password, const std::wstring& name);
	GetManagerResult GetManagers(std::vector<Manager>* Managers);
};