#include <string>
#include <iostream>

#include <conn.hpp>
#include <packet.hpp>

int main() {
  try {
    std::cout << "yay?" << std::endl;
    Conn conn;
    std::string server_addr = "therealorange.com";
    conn.DialMC(server_addr, 80);
    //conn.ReadPacket();
    //conn.WritePacket("lol");
    conn.Close();
    //conn.Socket.read_some(receive_buffer, error);
    std::cout << "yay?" << std::endl;
  } catch (std::exception &e) {
    std::cerr << e.what() << std::endl;
  }
  return 0;
}
