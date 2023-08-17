#pragma once
class Database
{
protected:
	const std::string Name = "Database.db";
private:
	void CreateDatabase();
	void StartDatabase();
public:
	Database();

};