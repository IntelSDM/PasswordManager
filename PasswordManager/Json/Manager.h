#pragma once
#include "pch.h"
class Manager
{

public:
    Manager(std::string username,std::string password, std::string name)
    {
      	Username = username;
		Password = password;
		Name = name;
    }
    std::string Type = "Manager";
    std::string Username;
    std::string Password;
    std::string Name;
    bool PasswordVisible = false;
    void ToJson(json& j) const
    {
        j = json{
            {"Type", Type},
            {"Username", Username},
            {"Password", Password},
            {"Name",Name},
            {"PasswordVisible",PasswordVisible}
        };
    }

    void FromJson(const json& j)
    {
        Type = j["Type"];
        Username = j["Username"];
        Password = j["Password"];
        Name = j["Name"];
        PasswordVisible = j["PasswordVisible"];
    }
};