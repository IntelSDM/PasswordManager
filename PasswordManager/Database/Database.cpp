#include "pch.h"
#include "Database.h"
#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#pragma comment(lib, "mysqlcppconn.lib")
Database::Database()
{

	Database::CreateDatabase();
}
void Database::CreateDatabase()
{
	sql::mysql::MySQL_Driver* driver;
	sql::Connection* connection;

	driver = sql::mysql::get_mysql_driver_instance();
	connection = driver->connect("tcp://127.0.0.1:3306", "root", "CoolPassword");

}
void Database::StartDatabase()
{

}