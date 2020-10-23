#include "networkclient.hpp"

#include <easylogging++.h>

#include "network.hpp"
#include "event.hpp"

NetworkClient::NetworkClient(const std::string &address, const unsigned short port) 
  : address(address), port(port) {
  network = std::make_unique<Network>(address, port);
	state = Handshaking;
  LOG(INFO) << "Initialized NetworkClient";
}

void NetworkClient::ServerListPing() {
	PacketHandshake handshake;
	handshake.protocolVersion = 578;
	handshake.serverAddress = address;
	handshake.serverPort = port;
	handshake.nextState = 1;
	network->SendPacket(handshake);
	state = Status;

	PacketStatusRequest request;
	network->SendPacket(request);

  auto packet = network->ReceivePacket(Status);

  while (!packet) {
    packet = network->ReceivePacket(Status);
  }

	auto response = std::static_pointer_cast<PacketStatusResponse>(packet);
  LOG(INFO) << response->JSONResponse;

	state = Handshaking;
}
void NetworkClient::Connect(const std::string &username) {
	PacketHandshake handshake;
	handshake.protocolVersion = 578;
	handshake.serverAddress = address;
	handshake.serverPort = port;
	handshake.nextState = 2;
  LOG(INFO) << "Sent Handshake";
	network->SendPacket(handshake);
	state = Login;

	PacketLoginStart loginStart;
	loginStart.Username = username;
  LOG(INFO) << "Sent Login Start";
	network->SendPacket(loginStart);

  auto packet = network->ReceivePacket(Login);

  while (!packet) {
    packet = network->ReceivePacket(Login);
  }

  if (packet->GetPacketId() == PacketNameLoginCB::SetCompression) {
    auto compPacket = std::static_pointer_cast<PacketSetCompression>(packet);
    LOG(INFO) << "Compression threshold: " << compPacket->Threshold;
    compressionThreshold = compPacket->Threshold;
    packet.reset();
    while (!packet) {
      packet = network->ReceivePacket(Login, compressionThreshold >= 0);
    }
  }

	auto response = std::static_pointer_cast<PacketLoginSuccess>(packet);

	if (response->Username != username) {
		throw std::logic_error("Received username is not the username sent : " + response->Username + " != " + username);
	}

  LOG(INFO) << "Logged In as " << response->Username;
  LOG(INFO) << "UUID: " << response->UUID;

  timeOfLastKeepAlivePacket = std::chrono::steady_clock::now();

	state = Play;
  isRunning = true;
  thread = std::thread(&NetworkClient::ExecNs, this);
}

NetworkClient::~NetworkClient() {
  if (isRunning) {
    isRunning = false;
    thread.join();
  }
}

void NetworkClient::ExecNs() {
  el::Helpers::setThreadName("network thread");
  EventListener listener;

  listener.RegisterHandler("SendPacket", [&](const Event& eventData) {
    std::shared_ptr<Packet> packet = eventData.get<std::shared_ptr<Packet>>();
    network->SendPacket(*packet,compressionThreshold);
  });

	listener.RegisterHandler("ReceivedPacket", [&](const Event& eventData) {
		std::shared_ptr<Packet> packet = eventData.get<std::shared_ptr<Packet>>();
		this->UpdatePacket(packet);
	});

	listener.RegisterHandler("SendChatMessage", [](const Event& eventData) {
		auto message = eventData.get<std::string>();
		auto packet = std::static_pointer_cast<Packet>(std::make_shared<PacketChatMessageSB>(message));
		PUSH_EVENT("SendPacket", packet);
	});

	listener.RegisterHandler("NetworkClientException", [](const Event& eventData) {
		auto data = eventData.get<std::string>();
		PUSH_EVENT("Disconnect", data);
	});

	try {
		while (isRunning) {
			listener.HandleAllEvents();

			std::shared_ptr<Packet> packet = network->ReceivePacket(state, compressionThreshold >= 0);
			if (packet != nullptr) {
				if (packet->GetPacketId() != PacketNamePlayCB::KeepAliveCB) {
					PUSH_EVENT("ReceivedPacket", packet);
				} else {
          LOG(INFO) << "KeepAliveCB Received";
					timeOfLastKeepAlivePacket = std::chrono::steady_clock::now();
					auto packetKeepAlive = std::static_pointer_cast<PacketKeepAliveCB>(packet);
					auto packetKeepAliveSB = std::make_shared<PacketKeepAliveSB>(packetKeepAlive->KeepAliveId);
					network->SendPacket(*packetKeepAliveSB, compressionThreshold);
          LOG(INFO) << "KeepAliveSB Sent";
				}
			}
			using namespace std::chrono_literals;
			if (std::chrono::steady_clock::now() - timeOfLastKeepAlivePacket > 60s) {
				packet = std::make_shared<PacketDisconnectPlay>();
				std::static_pointer_cast<PacketDisconnectPlay>(packet)->Reason = "Timeout: server not responding";
				PUSH_EVENT("ReceivedPacket", packet);
			}
		}
	} catch (std::exception &e) {
		PUSH_EVENT("NetworkClientException", std::string(e.what()));
	}
}


void NetworkClient::UpdatePacket(std::shared_ptr<Packet> ptr) {
  LOG(INFO) << "Packet ID: " << ptr->GetPacketId() << " Received";
	switch ((PacketNamePlayCB) ptr->GetPacketId()) {
		case SpawnEntity: {
      auto packet = std::static_pointer_cast<PacketSpawnEntity>(ptr);
      break;
		}
		case SpawnExperienceOrb:
      break;
		case SpawnLivingEntity: {
      auto packet = std::static_pointer_cast<PacketSpawnLivingEntity>(ptr);
      break;
		}
		case SpawnPainting:
      break;
		case SpawnPlayer: {
      auto packet = std::static_pointer_cast<PacketSpawnPlayer>(ptr);
      break;
		}
		case EntityAnimationCB:
      break;
		case Statistics:
      break;
		case BlockBreakAnimation:
      break;
		case BlockEntityData:
      break;
		case BlockAction:
      break;
		case BlockChange: {
      auto packet = std::static_pointer_cast<PacketBlockChange>(ptr);
      break;
		}
		case BossBar:
      break;
		case ServerDifficulty: {
      auto packet = std::static_pointer_cast<PacketServerDifficulty>(ptr);
      LOG(INFO) << "Difficulty is " << packet->Difficulty;
      break;
    }
		case TabCompleteCB:
      break;
		case ChatMessageCB: {
      auto packet = std::static_pointer_cast<PacketChatMessageCB>(ptr);
			PUSH_EVENT("ReceivedChatMessage", packet->JsonData.text);
      break;
		}
		case MultiBlockChange: {
      auto packet = std::static_pointer_cast<PacketMultiBlockChange>(ptr);
      break;
		}
		case WindowConfirmationCB: {
      auto packet = std::static_pointer_cast<PacketWindowConfirmationCB>(ptr);
      break;
		}
		case CloseWindowCB:
      break;
		case OpenWindow: {
      auto packet = std::static_pointer_cast<PacketOpenWindow>(ptr);
      LOG(INFO) << "Open new window " << packet->WindowTitle << ": " << packet->WindowId;
      break;
		}
		case WindowItems: {
      auto packet = std::static_pointer_cast<PacketWindowItems>(ptr);
      if (packet->WindowId == 0) {
        LOG(INFO) << "Player Inventory " << packet->Count << " items";
      }
      break;
		}
		case WindowProperty:
      break;
		case SetSlot: {
      auto packet = std::static_pointer_cast<PacketSetSlot>(ptr);
      if (packet->WindowId == 0) {
      }
      break;
		}
		case SetCooldown:
      break;
		case PluginMessageCB:
      break;
		case NamedSoundEffect:
      break;
		case DisconnectPlay: {
      auto packet = std::static_pointer_cast<PacketDisconnectPlay>(ptr);
      LOG(INFO) << "Disconnect reason: " << packet->Reason;
      PUSH_EVENT("Disconnected", packet->Reason);
      break;
		}
		case EntityStatus:
      break;
		case Explosion:
      break;
		case UnloadChunk: {
      auto packet = std::static_pointer_cast<PacketUnloadChunk>(ptr);
      break;
		}
		case ChangeGameState:
      break;
		case KeepAliveCB: {
      LOG(WARNING) << "Receive KeepAlive packet in GameState handler";
      break;
		}
		case ChunkData: {
      auto packet = std::static_pointer_cast<PacketChunkData>(ptr);
      break;
		}
		case Effect: {
      auto packet = std::static_pointer_cast<PacketEffect>(ptr);
      LOG(INFO) << "Effect ID: " << packet->EffectID;
      break;
    }
		case Particle:
      break;
		case JoinGame: {
      auto packet = std::static_pointer_cast<PacketJoinGame>(ptr);
      LOG(INFO) << "Gamemode is " << (packet->Gamemode & 0b11111011) << ", Dimension is " << packet->Dimension << ", Level Type is " << packet->LevelType;
      PUSH_EVENT("PlayerConnected", 0);
      break;
		}
		case MapData:
      break;
		case EntityPosition:
      break;
		case EntityPositionAndRotation:
      break;
		case EntityRotation:
      break;
		case VehicleMove:
      break;
		case OpenSignEditor:
      break;
		case PlayerAbilitiesCB:
      break;
		case CombatEvent:
      break;
		case PlayerPositionAndLookCB: {
      auto packet = std::static_pointer_cast<PacketPlayerPositionAndLookCB>(ptr);

      LOG(INFO) << "player pos and look cb";
      auto packetResponse = std::make_shared<PacketTeleportConfirm>(packet->TeleportId);
      auto packetPerformRespawn = std::make_shared<PacketClientStatus>(0);

      PUSH_EVENT("SendPacket",std::static_pointer_cast<Packet>(packetResponse));
      PUSH_EVENT("SendPacket",std::static_pointer_cast<Packet>(packetPerformRespawn));
      break;
		}
		case UnlockRecipes:
      break;
		case DestroyEntities:
      break;
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
		case DisplayScoreboard:
      break;
		case EntityMetadata:
      break;
		case AttachEntity:
      break;
		case EntityVelocity:
      break;
		case EntityEquipment:
      break;
		case SetExperience:
      break;
		case UpdateHealth: {
      auto packet = std::static_pointer_cast<PacketUpdateHealth>(ptr);
      int playerHealth = packet->Health;
      if (playerHealth <= 0) {
        LOG(INFO) << "Player is dead. Respawning...";
        auto packetPerformRespawn = std::make_shared<PacketClientStatus>(0);
        PUSH_EVENT("SendPacket",std::static_pointer_cast<Packet>(packetPerformRespawn));
      }
      break;
		}
		case ScoreboardObjective:
				break;
		case SetPassengers:
				break;
		case Teams:
				break;
		case UpdateScore:
				break;
		case SpawnPosition: {
      auto packet = std::static_pointer_cast<PacketSpawnPosition>(ptr);
      break;
		}
		case TimeUpdate: {
      break;
    }
		case Title:
				break;
		case SoundEffect: {
      auto packet = std::static_pointer_cast<PacketSoundEffect>(ptr);
      PUSH_EVENT("SoundEffect", packet->SoundID);
      break;
    }
		case PlayerListHeaderAndFooter:
      break;
		case CollectItem:
      break;
		case EntityTeleport:
			break;
		case Advancements:
			break;
		case EntityProperties:
			break;
		case EntityEffect:
			break;
    default: {
      LOG(INFO) << "Unknown Packet ID: " << ptr->GetPacketId();
    }
	}
}

