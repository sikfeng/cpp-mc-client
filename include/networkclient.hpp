#ifndef __NETWORKCLIENT_HPP__
#define __NETWORKCLIENT_HPP__

#include <memory>
#include <queue>
#include <string>
#include <chrono>
#include <thread>

#include "event.hpp"

class Network;
struct Packet;
enum ConnectionState : unsigned char;

class NetworkClient {
  const std::string address = "127.0.0.1";
  const unsigned short port = 25565;
  std::unique_ptr<Network> network;
	std::queue<std::shared_ptr<Packet>> toSend;
	std::queue<std::shared_ptr<Packet>> toReceive;
	ConnectionState state;
  int compressionThreshold = -1;
  std::chrono::steady_clock::time_point timeOfLastKeepAlivePacket;
	std::thread thread;
	bool isRunning = false;
	void ExecNs();
  void UpdatePacket(std::shared_ptr<Packet> ptr);
public:
	NetworkClient(const std::string &address, const unsigned short port);
	void Connect(const std::string &username);
	void ServerListPing();
	~NetworkClient();
};

#endif
