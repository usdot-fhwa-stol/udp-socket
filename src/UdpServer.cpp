/*
 * UdpServer.cpp
 *
 *  Created on: Aug 27, 2015
 *      Author: ivp
 */

#include "UdpServer.hpp"



namespace udp_socket {

    
    UdpServer::UdpServer(const std::string& address, int port)
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
            throw UdpServerRuntimeError(("invalid address or port for UDP socket: \"" + address + ":" + decimalPort + "\"").c_str());
        }

        _socket = socket(_addrInfo->ai_family, SOCK_DGRAM | SOCK_CLOEXEC, IPPROTO_UDP);
        if (_socket == -1)
        {
            freeaddrinfo(_addrInfo);
            throw UdpServerRuntimeError(("could not create UDP socket for: \"" + address + ":" + decimalPort + "\"").c_str());
        }

        r = bind(_socket, _addrInfo->ai_addr, _addrInfo->ai_addrlen);
        if (r != 0)
        {
            freeaddrinfo(_addrInfo);
            close(_socket);
            throw UdpServerRuntimeError(("could not bind UDP socket with: \"" + address + ":" + decimalPort + "\"").c_str());
        }
    }

 
    UdpServer::~UdpServer()
    {
        freeaddrinfo(_addrInfo);
        close(_socket);
    }

   
    int UdpServer::GetSocket() const
    {
        return _socket;
    }

   
    int UdpServer::GetPort() const
    {
        return _port;
    }

 
    std::string UdpServer::GetAddress() const
    {
        return _address;
    }

   
    int UdpServer::Receive(char *msg, size_t maxSize)
    {
        return ::recv(_socket, msg, maxSize, 0);
    }

   
    int UdpServer::TimedReceive(char *msg, size_t maxSize, int maxWait_ms)
    {
        fd_set s;
        FD_ZERO(&s);
        FD_SET(_socket, &s);
        struct timeval timeout;
        timeout.tv_sec = maxWait_ms / 1000;
        timeout.tv_usec = (maxWait_ms % 1000) * 1000;

        int retval = select(_socket + 1, &s, &s, &s, &timeout);
        if (retval == -1)
        {
            // select() set errno accordingly
            return -1;
        }
        if (retval > 0)
        {
            // The socket has data.
            return ::recv(_socket, msg, maxSize, 0);
        }

        // The socket has no data.
        errno = EAGAIN;
        return -1;
    }

    std::string UdpServer::stringTimedReceive(int maxWait_ms) {
        std::vector<char> msg(4000);
        int num_of_bytes = this->TimedReceive(msg.data(),4000, maxWait_ms);
        if (num_of_bytes > 0 ) {
            msg.resize(num_of_bytes);
            std::string ret(msg.data());
            std::cout << "UDP Server message received : " << ret << " of size " << num_of_bytes << std::endl;
            return ret;
        }
        else if ( num_of_bytes == 0 ) {
            throw UdpServerRuntimeError("Received empty message!");
        }
        else {
            throw UdpServerRuntimeError("Listen timed out after 5 ms!");
        }
        return "";
    }

} 