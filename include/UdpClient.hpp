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

#pragma once

#include <netdb.h>
#include <stdexcept>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <cstdio>
#include <errno.h>
#include <iostream>

namespace udp_socket{

	class UdpClientRuntimeError : public std::runtime_error
	{
        public:
            UdpClientRuntimeError(const std::string &msg) : std::runtime_error(msg) {}
	};

	class UdpClient
	{
        public:
            /**
             * \brief Initialize a UDP client object.
             *
             * This function initializes the UDP client object using the address and the
             * port as specified.
             *
             * The port is expected to be a host side port number (i.e. 59200).
             *
             * The \p address parameter is a textual address. It may be an IPv4 or IPv6
             * address and it can represent a host name or an address defined with
             * just numbers. If the address cannot be resolved then an error occurs
             * and constructor throws.
             *
             * \note
             * The socket is open in this process. If you fork() or exec() then the
             * socket will be closed by the operating system.
             *
             * \warning
             * We only make use of the first address found by getaddrinfo(). All
             * the other addresses are ignored.
             *
             * \exception UdpClientRuntimeError
             * The server could not be initialized properly. Either the address cannot be
             * resolved, the port is incompatible or not available, or the socket could
             * not be created.
             *
             * \param[in] address  The address to send to.
             * \param[in] port  The port to send to.
             * \param[in] debug  If true, the client will print debug information.
            */
            UdpClient(const std::string& address, int port, bool debug = false);
            /** \brief Clean up the UDP client.
             *
             * This function frees the address info structures and close the socket.
             */
            virtual ~UdpClient();
            /** \brief Retrieve a copy of the socket identifier.
             *
             * This function return the socket identifier as returned by the socket()
             * function. This can be used to change some flags.
             *
             * \return The socket used by this UDP client.
             */
            virtual int GetSocket() const;
            /** \brief Retrieve the port number.
             *
             * This function returns the port number as specified in the constructor.
             *
             * \return The port number.
             */
            virtual int GetPort() const;
            /** \brief Retrieve a copy of the address.
             *
             * This function returns a copy of the address as it was specified in the
             * constructor. This does not return a canonalized version of the address.
             *
             * The address cannot be modified. If you need to send data on a different
             * address, create a new UDP client.
             *
             * \return A string with a copy of the constructor input address.
             */
            virtual std::string GetAddress() const;
           
            virtual int Send(const std::string& message);
            /** \brief Send a message through this UDP client.
             *
             * This function sends the \p buffer data through the UDP client socket.
             * The function cannot be used to change the destination as it was defined
             * when creating the UdpClient object.
             *
             * The size must be small enough for the message to fit.
             *
             * \param[in] buffer  The buffer containing the data to send.
             * \param[in] size  The number of bytes contained within the buffer to send.
             *
             * \return -1 if an error occurs, otherwise the number of bytes sent. errno
             * is set accordingly on error.
             */  
            virtual int Send(void *buffer, size_t size);

        private:
            int _socket;
            int _port;
            std::string _address;
            struct addrinfo *_addrInfo;
            bool _debug = false;
	};

} 

