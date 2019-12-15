#include "conn.hpp"
#include "packet.hpp"


std::string make_string(asio::streambuf& streambuf)
{
    return {asio::buffers_begin(streambuf.data()), 
                asio::buffers_end(streambuf.data())};
}

void Conn::DialMC(std::string &addr, int port) {
  try {
    asio::io_service io_service;
    asio::ip::tcp::resolver resolver(io_service);
    asio::ip::tcp::resolver::query query(addr, std::to_string(port));
    asio::ip::tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
    this->Socket.connect(*endpoint_iterator);
  } catch (std::exception &e) {
    std::cerr << "Conn::DialMC() " << e.what() << std::endl;
  }
  return;
}

void Conn::Close() {
  try {
    this->Socket.close();
  } catch (std::exception &e) {
    std::cerr << "Conn::Close() " << e.what() << std::endl;
  }
  return;
}

void Conn::WritePacket(const std::string &msg) {
  try {
    this->Socket.write_some(asio::buffer(msg));
    //this->Socket.write_some(p.Pack(this->threshold));
  } catch (std::exception &e) {
    std::cerr << "Conn::WritePacket() " << e.what() << std::endl;
  }
}

void Conn::ReadPacket() {
  try {
    asio::streambuf streambuf;
    //this->Socket.read_some(streambuf);
    asio::read(this->Socket, streambuf.prepare(100));
    std::cout << make_string(streambuf) << std::endl;
  } catch (std::exception &e) {
    std::cerr << "Conn::WritePacket() " << e.what() << std::endl;
  }
}

