#ifndef __CONN_H__
#define __CONN_H__

#include <string>
#include <iostream>
#include <asio.hpp>

#include "packet.hpp"

enum ConnectionState : unsigned char{
  Handshaking,
  Status,
  Login,
  Play,
};

class Conn {
  public:
    asio::io_context io_context;
    asio::ip::tcp::socket Socket;

    Conn() : Socket(io_context) {};
    void Open(const std::string &addr, int port = 25565);
    void Close();
    void ServerListPing();
    void WritePacket(const std::string &msg);
    Packet ReadPacket();
    int ReadVarInt();
    int ReadVarInt(const std::vector<char> &PacketData);
    Packet ReceivePacketByPacketId(int PacketID, const std::vector<char> &PacketData);

    std::string addr;
    int port;
    int threshold = 0;
    ConnectionState state;
};

#endif
