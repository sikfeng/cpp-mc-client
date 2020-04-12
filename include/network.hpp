#ifndef __NETWORK_HPP__
#define __NETWORK_HPP__

#include <memory>
#include "packet.hpp"

enum ConnectionState : unsigned char {
	Handshaking,
	Status,
	Login,
	Play,
};

class Network {
	std::unique_ptr<Socket> socket;
	std::unique_ptr<StreamSocket> stream;

	std::shared_ptr<Packet> ReceivePacketByPacketId(int packetId, ConnectionState state, StreamInput &stream);

public:
	Network(std::string address, unsigned short port);
	~Network();

	std::shared_ptr<Packet> ReceivePacket(ConnectionState state = Play, bool useCompression = false);
	void SendPacket(Packet &packet, int compressionThreshold = -1);
	std::shared_ptr<Packet> ParsePacketPlay(PacketNamePlayCB id);
};

#endif
