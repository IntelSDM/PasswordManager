#pragma once
using ByteArray = std::vector<uint8_t>;
class Client
{
	protected:
		SOCKET Socket;
		Database Database;
public:
	Client(SOCKET socket);

	std::string IpAddress;
	void SendText(std::string Text);
	std::string ReceiveText();
	void MessageHandler();

};