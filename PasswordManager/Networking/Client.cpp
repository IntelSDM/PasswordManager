#include "pch.h"
#include "Client.h"
#include <locale>
#include <codecvt> 
#include "Database.h"
#include "Manager.h"
constexpr int BufferSize = 10000;
Client::Client(SOCKET socket)
{
	Client::Socket = socket;
	std::thread thread([&]
		{
			Client::MessageHandler (); // create a thread for the client
		});
	thread.detach();
}
void Client::SendText(std::string text)
{
	ByteArray plaintext(text.begin(), text.end());
	int32_t Result = send(Client::Socket, (char*)plaintext.data(), (int)plaintext.size(), 0);
}

std::string Client::ReceiveText()
{
	ByteArray	receivedbytes;
	uint8_t		recvbuffer[BufferSize];

	while (true)
	{
		int32_t received = recv(Client::Socket, (char*)recvbuffer, BufferSize, 0);

		if (received < 0)
			break;

		for (int n = 0; n < received; ++n)
		{
			receivedbytes.push_back(recvbuffer[n]);
		}
		auto breaker = std::find(receivedbytes.begin(), receivedbytes.end(), '|');

		if (breaker != receivedbytes.end())
		{
			std::string str(receivedbytes.begin(), breaker);
			return str;
		}
		if (received <= BufferSize)
			break;

	}
	std::string str(receivedbytes.begin(), receivedbytes.end());
	return str;
}
void Client::MessageHandler()
{
	while (true)
	{
		std::string message = Client::ReceiveText();
		if (message == "" || message.size() == 0)
			continue;
		std::cout << message << std::endl;
		json jsoned = json::parse(message);
		if (jsoned["Type"] == "Login")
		{
			UserVerificationResult identifier = Client::Database.VerifyUser(jsoned["Username"],jsoned["Password"]);
			if (identifier == UserVerificationResult::Success)
			{
				Client::SendText("LoginSuccess");
			}
			
		}
		if (jsoned["Type"] == "Register")
		{

			UserRegistrationResult identifier = Client::Database.AddUser(jsoned["Username"], jsoned["Password"]);
			if (identifier == UserRegistrationResult::Success)
			{
				Client::SendText("RegisterSuccess");
				std::vector<Manager> managers;
				Client::Database.GetManagers(&managers);
				json jsoned;
				for (const Manager& manager : managers) {
					json entry;
					manager.ToJson(entry);

					jsoned.push_back(entry);
				}
				Client::SendText(jsoned.dump());
			}
		}
	}
}