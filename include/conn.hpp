#include <string>
#include <iostream>

#include <asio.hpp>

#include "packet.hpp"

class Conn {
  public:
    asio::io_context io_context;
    asio::ip::tcp::socket Socket;
    Conn() : Socket(io_context) {};
    void DialMC(std::string &addr, int port = 25565);
    void Close();
    void WritePacket(const std::string&);
    void ReadPacket();
    //io.ByteReader;
    //io.Writer;

    int threshold;
};
