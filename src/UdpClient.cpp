/**
 * Copyright (C) 2025 LEIDOS.
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may not
 * use this except in compliance with the License. You may obtain a copy of
 * the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
 * License for the specific language governing permissions and limitations under
 * the License.
 */


#include "UdpClient.hpp"

namespace udp_socket{


	UdpClient::UdpClient(const std::string& address, int port)
		: _port(port)
		, _address(address)
	{
		char decimalPort[16];
		snprintf(decimalPort, sizeof(decimalPort), "%d", _port);
		decimalPort[sizeof(decimalPort) / sizeof(decimalPort[0]) - 1] = '\0';

		struct addrinfo hints;
		memset(&hints, 0, sizeof(hints));
		hints.ai_family = AF_UNSPEC;
		hints.ai_socktype = SOCK_DGRAM;
		hints.ai_protocol = IPPROTO_UDP;

		int r(getaddrinfo(address.c_str(), decimalPort, &hints, &_addrInfo));
		if (r != 0 || _addrInfo == NULL)
		{
			throw UdpClientRuntimeError(("invalid address or port: \"" + address + ":" + decimalPort + "\"").c_str());
		}

		_socket = socket(_addrInfo->ai_family, SOCK_DGRAM | SOCK_CLOEXEC, IPPROTO_UDP);
		if (_socket == -1)
		{
			freeaddrinfo(_addrInfo);
			throw UdpClientRuntimeError(("could not create UDP socket for: \"" + address + ":" + decimalPort + "\"").c_str());
		}
	}


	UdpClient::~UdpClient()
	{
		freeaddrinfo(_addrInfo);
		close(_socket);
	}


	int UdpClient::GetSocket() const
	{
		return _socket;
	}

	int UdpClient::GetPort() const
	{
		return _port;
	}


	std::string UdpClient::GetAddress() const
	{
		return _address;
	}


	int UdpClient::Send(void *buffer, size_t size)
	{
		return sendto(_socket, buffer, size, 0, _addrInfo->ai_addr, _addrInfo->ai_addrlen);
	}

	int UdpClient::Send(const std::string& message)
	{
		return sendto(_socket, message.c_str(), message.length(), 0, _addrInfo->ai_addr, _addrInfo->ai_addrlen);
	}

}