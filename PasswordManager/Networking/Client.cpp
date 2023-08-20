#include "pch.h"
#include "Client.h"
#include <locale>
#include <codecvt> 
constexpr int BufferSize = 1000000;
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

}