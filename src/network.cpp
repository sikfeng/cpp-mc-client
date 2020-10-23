#include "network.hpp"

#include <zlib.h>
#include <easylogging++.h>

#include "socket.hpp"

Network::Network(std::string address, unsigned short port) {
	try {
		socket = std::make_unique<Socket>(address, port);
		stream = std::make_unique<StreamSocket>(socket.get());
	} catch (std::exception &e) {
		LOG(WARNING) << "Connection failed: " << e.what();
		throw;
	}	
}

Network::~Network() {
}

std::shared_ptr<Packet> Network::ReceivePacket(ConnectionState state, bool useCompression) {
	if (useCompression) {
		int packetLength = stream->ReadVarInt();
		auto packetData = stream->ReadByteArray(packetLength);
		StreamBuffer streamBuffer(packetData.data(), packetData.size());
		
		int dataLength = streamBuffer.ReadVarInt();
		if (dataLength == 0) {
			auto packetData = streamBuffer.ReadByteArray(packetLength - streamBuffer.GetReadedLength());
			StreamBuffer streamBuffer(packetData.data(), packetData.size());
			int packetId = streamBuffer.ReadVarInt();
			auto packet = ReceivePacketByPacketId(packetId, state, streamBuffer);
			return packet;
		} else {
			std::vector<unsigned char> compressedData = streamBuffer.ReadByteArray(packetLength - streamBuffer.GetReadedLength());
			std::vector<unsigned char> uncompressedData;
			uncompressedData.resize(dataLength);

			z_stream stream;
			stream.avail_in = compressedData.size();
			stream.next_in = compressedData.data();
			stream.avail_out = uncompressedData.size();
			stream.next_out = uncompressedData.data();
			stream.zalloc = Z_NULL;
			stream.zfree = Z_NULL;
			stream.opaque = Z_NULL;
			if (inflateInit(&stream) != Z_OK)
				throw std::runtime_error("Zlib decompression initalization error");

			int status = inflate(&stream, Z_FINISH);
			switch (status) {
        case Z_STREAM_END:
          break;
        case Z_OK:                            
        case Z_STREAM_ERROR:
        case Z_BUF_ERROR:
          throw std::runtime_error("Zlib decompression error: " + std::to_string(status));
			}

			if (inflateEnd(&stream) != Z_OK)
				throw std::runtime_error("Zlib decompression end error");

			StreamBuffer streamBuffer(uncompressedData.data(), uncompressedData.size());
			int packetId = streamBuffer.ReadVarInt();
			auto packet = ReceivePacketByPacketId(packetId, state, streamBuffer);
			return packet;            
		}
	} else {
		int packetSize = stream->ReadVarInt();
		auto packetData = stream->ReadByteArray(packetSize);
		StreamBuffer streamBuffer(packetData.data(), packetData.size());
		int packetId = streamBuffer.ReadVarInt();
		auto packet = ReceivePacketByPacketId(packetId, state, streamBuffer);
		return packet;
	}	
}

void Network::SendPacket(Packet &packet, int compressionThreshold) {
  LOG(INFO) << "Sending Packet";
	if (compressionThreshold >= 0) {
		StreamCounter packetSize;
		packetSize.WriteVarInt(packet.GetPacketId());
		packetSize.WriteVarInt(0);
		packet.ToStream(&packetSize);
		if (packetSize.GetCountedSize() < compressionThreshold) {
			stream->WriteVarInt(packetSize.GetCountedSize());
			stream->WriteVarInt(0);
			stream->WriteVarInt(packet.GetPacketId());
			packet.ToStream(stream.get());
		} else {
			throw std::runtime_error("Compressing send data not supported");
		}
	}	else {
		StreamCounter packetSize;
		packetSize.WriteVarInt(packet.GetPacketId());
		packet.ToStream(&packetSize);
		stream->WriteVarInt(packetSize.GetCountedSize());
		stream->WriteVarInt(packet.GetPacketId());
		packet.ToStream(stream.get());
	}	
  LOG(INFO) << "Sent Packet ID: " << packet.GetPacketId();
}

std::shared_ptr<Packet> Network::ReceivePacketByPacketId(int packetId, ConnectionState state, StreamInput &stream) {
	std::shared_ptr<Packet> packet(nullptr);
	switch (state) {
		case Handshaking:
      switch (packetId) {
      case PacketNameHandshakingSB::Handshake:
        packet = std::make_shared<PacketHandshake>();
        break;
      }
      break;
		case Status:
      switch (packetId) {
      case PacketNameStatusCB::Response:
        packet = std::make_shared<PacketStatusResponse>();
        break;
      case PacketNameStatusCB::Pong:
        packet = std::make_shared<PacketStatusPong>();
        break;
      }
			break;
		case Login:
      switch (packetId) {
      case PacketNameLoginCB::LoginSuccess:
        packet = std::make_shared<PacketLoginSuccess>();
        break;
      case PacketNameLoginCB::SetCompression:
        packet = std::make_shared<PacketSetCompression>();
        break;
      case PacketNameLoginCB::Disconnect:
        packet = std::make_shared<PacketDisconnect>();
        break;
      }
			break;
		case Play:
			packet = ParsePacketPlay((PacketNamePlayCB) packetId);
			break;
	}
	if (packet.get() != nullptr) {
		packet->FromStream(&stream);
  }
	return packet;
}

std::shared_ptr<Packet> Network::ParsePacketPlay(PacketNamePlayCB id) {
  LOG(INFO) << "Packet ID Received: " << id;
	switch (id) {
		case SpawnEntity:
      return std::make_shared<PacketSpawnEntity>();
		case SpawnExperienceOrb:
			break;
    case SpawnWeatherEntity:
			break;
    case SpawnLivingEntity:
			break;
    case SpawnPainting:
			break;
		case SpawnPlayer:
      return std::make_shared<PacketSpawnPlayer>();
		case EntityAnimationCB:
			break;
		case Statistics:
			break;
    case AcknowledgePlayerDigging:
      break;
		case BlockBreakAnimation:
      break;
    case BlockEntityData:
			break;
		case BlockAction:
			break;
		case BlockChange:
      return std::make_shared<PacketBlockChange>();
		case BossBar:
      break;
      return std::make_shared<PacketBossBar>();
		case ServerDifficulty:
			break;
		case ChatMessageCB:
      return std::make_shared<PacketChatMessageCB>();
		case MultiBlockChange:
      return std::make_shared<PacketMultiBlockChange>();
		case TabCompleteCB:
			break;
    case DeclareCommands:
      break;
		case WindowConfirmationCB:
      return std::make_shared<PacketWindowConfirmationCB>();
		case CloseWindowCB:
      return std::make_shared<PacketCloseWindowCB>();
		case WindowItems:
      return std::make_shared<PacketWindowItems>();
		case WindowProperty:
      return std::make_shared<PacketWindowProperty>();
		case SetSlot:
      return std::make_shared<PacketSetSlot>();
		case SetCooldown:
      return std::make_shared<PacketSetCooldown>();
		case PluginMessageCB:
			break;
		case NamedSoundEffect:
			break;
		case DisconnectPlay:
			return std::make_shared<PacketDisconnectPlay>();
		case EntityStatus:
			break;
		case Explosion:
			break;
		case UnloadChunk:
      return std::make_shared<PacketUnloadChunk>();
		case ChangeGameState:
			break;
		case OpenHorseWindow:
      break;
		case KeepAliveCB:
			return std::make_shared<PacketKeepAliveCB>();
		case ChunkData:
			return std::make_shared<PacketChunkData>();
		case Effect:
			return std::make_shared<PacketEffect>();
			break;
		case Particle:
			break;
    case UpdateLight:
      break;
		case JoinGame:
			return std::make_shared<PacketJoinGame>();
		case MapData:
			break;
    case TradeList:
      break;
		case EntityPosition:
      return std::make_shared<PacketEntityPosition>();
		case EntityPositionAndRotation:
      return std::make_shared<PacketEntityPositionAndRotation>();
		case EntityRotation:
      return std::make_shared<PacketEntityRotation>();
    case EntityMovement:
      break;
		case VehicleMove:
			break;
		case OpenBook:
			break;
		case OpenWindow:
			break;
		case OpenSignEditor:
			break;
    case CraftRecipeResponse:
      break;
		case PlayerAbilitiesCB:
			break;
		case CombatEvent:
			break;
    case PlayerInfo:
      break;
    case FacePlayer:
      break;
		case PlayerPositionAndLookCB:
			return std::make_shared<PacketPlayerPositionAndLookCB>();
    case UnlockRecipes:
      break;
		case DestroyEntities:
      return std::make_shared<PacketDestroyEntities>();
		case RemoveEntityEffect:
			break;
		case ResourcePackSend:
			break;
		case Respawn:
			break;
		case EntityHeadLook:
			break;
    case SelectAdvancementTab:
      break;
		case WorldBorder:
			break;
		case Camera:
			break;
		case HeldItemChangeCB:
			break;
    case UpdateViewPosition:
      break;
    case UpdateViewDistance:
      break;
		case DisplayScoreboard:
			break;
		case EntityMetadata:
			break;
		case AttachEntity:
			break;
		case EntityVelocity:
      return std::make_shared<PacketEntityVelocity>();
		case EntityEquipment:
			break;
		case SetExperience:
			break;
		case UpdateHealth:
			return std::make_shared<PacketUpdateHealth>();
		case ScoreboardObjective:
			break;
		case SetPassengers:
			break;
		case Teams:
			break;
		case UpdateScore:
			break;
		case SpawnPosition:
			return std::make_shared<PacketSpawnPosition>();
		case TimeUpdate:
      return std::make_shared<PacketTimeUpdate>();
		case Title:
			break;
		case EntitySoundEffect:
      break;
		case SoundEffect:
      return std::make_shared<PacketSoundEffect>();
    case StopSound:
      break;
		case PlayerListHeaderAndFooter:
			break;
		case CollectItem:
			break;
		case EntityTeleport:
      return std::make_shared<PacketEntityTeleport>();
    case Advancements:
      break;
		case EntityProperties:
			break;
		case EntityEffect:
			break;
		case DeclareRecipes:
			break;
		case Tags:
			break;
    default:
      LOG(INFO) << "Unidentifiable Packet ID: " << id;
      return nullptr;
	}
	return nullptr;
}

