#include <gtest/gtest.h>
#include <UdpClient.hpp>
#include <UdpServer.hpp>

TEST( testUdpSocket, testUdpSocketClientServer) {
    udp_socket::UdpClient client("127.0.0.1", 5832);
    udp_socket::UdpServer server("127.0.0.1", 5832);
    std::string testMsg = "Testing";
    client.Send(testMsg);
    auto msg =  server.stringTimedReceive(100);
    EXPECT_EQ(testMsg, msg);
    EXPECT_EQ(client.GetAddress(), "127.0.0.1");
    EXPECT_EQ(client.GetPort(), 5832);  
    testMsg = "1Testing";
    client.Send(testMsg);
    msg =  server.stringTimedReceive(100);
    EXPECT_EQ(testMsg, msg);

}

TEST( testUdpSocket, testUdpSocketClientServerDebug) {
    udp_socket::UdpClient client("127.0.0.1", 5832, true);
    udp_socket::UdpServer server("127.0.0.1", 5832, true);
    std::string testMsg = "Testing";
    client.Send(testMsg);
    auto msg =  server.stringTimedReceive(100);
    EXPECT_EQ(testMsg, msg);
    EXPECT_EQ(client.GetAddress(), "127.0.0.1");
    EXPECT_EQ(client.GetPort(), 5832);  
    testMsg = "1Testing";
    client.Send(testMsg);
    msg =  server.stringTimedReceive(100);
    EXPECT_EQ(testMsg, msg);
}


