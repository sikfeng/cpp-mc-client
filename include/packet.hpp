#ifndef __PACKET_HPP__
#define __PACKET_HPP__

#include "stream.hpp"
#include <easylogging++.h>

enum PacketNameHandshakingSB {
    Handshake = 0x00,
};
enum PacketNameStatusCB {
    Response = 0x00,
    Pong = 0x01,
};
enum PacketNameStatusSB {
    Request = 0x00,
    Ping = 0x01,
};
enum PacketNameLoginCB {
    Disconnect = 0x00,
    EncryptionRequest = 0x01,
    LoginSuccess = 0x02,
    SetCompression = 0x03,
};
enum PacketNameLoginSB {
    LoginStart = 0x00,
    EncryptionResponse = 0x01,
};
enum PacketNamePlayCB {
    SpawnEntity = 0x00,
    SpawnExperienceOrb = 0x01,
    SpawnWeatherEntity = 0x02,
    SpawnLivingEntity = 0x03,
    SpawnPainting = 0x04,
    SpawnPlayer = 0x05,
    EntityAnimationCB = 0x06,
    Statistics = 0x07,
    AcknowledgePlayerDigging = 0x08,
    BlockBreakAnimation = 0x09,
    BlockEntityData = 0x0A,
    BlockAction = 0x0B,
    BlockChange = 0x0C,
    BossBar = 0x0D,
    ServerDifficulty = 0x0E,
    ChatMessageCB = 0x0F,
    MultiBlockChange = 0x10,
    TabCompleteCB = 0x11,
    DeclareCommands = 0x12,
    WindowConfirmationCB = 0x13,
    CloseWindowCB = 0x14,
    WindowItems = 0x15,
    WindowProperty = 0x16,
    SetSlot = 0x17,
    SetCooldown = 0x18,
    PluginMessageCB = 0x19,
    NamedSoundEffect = 0x1A,
    DisconnectPlay = 0x1B,
    EntityStatus = 0x1C,
    Explosion = 0x1D,
    UnloadChunk = 0x1E,
    ChangeGameState = 0x1F,
    OpenHorseWindow = 0x20,
    KeepAliveCB = 0x21,
    ChunkData = 0x22,
    Effect = 0x23,
    Particle = 0x24,
    UpdateLight = 0x25,
    JoinGame = 0x26,
    MapData = 0x27,
    TradeList = 0x28,
    EntityPosition = 0x29,
    EntityPositionAndRotation = 0x2A,
    EntityRotation = 0x2B,
    EntityMovement = 0x2C,
    VehicleMove = 0x2D,
    OpenBook = 0x2E,
    OpenWindow = 0x2F,
    OpenSignEditor = 0x30,
    CraftRecipeResponse = 0x31,
    PlayerAbilitiesCB = 0x32,
    CombatEvent = 0x33,
    PlayerInfo = 0x34,
    FacePlayer = 0x35,
    PlayerPositionAndLookCB = 0x36,
    UnlockRecipes = 0x37,
    DestroyEntities = 0x38,
    RemoveEntityEffect = 0x39,
    ResourcePackSend = 0x3A,
    Respawn = 0x3B,
    EntityHeadLook = 0x3C,
    SelectAdvancementTab = 0x3D,
    WorldBorder = 0x3E,
    Camera = 0x3F,
    HeldItemChangeCB = 0x40,
    UpdateViewPosition = 0x41, 
    UpdateViewDistance = 0x42,
    DisplayScoreboard = 0x43,
    EntityMetadata = 0x44,
    AttachEntity = 0x45,
    EntityVelocity = 0x46,
    EntityEquipment = 0x47,
    SetExperience = 0x48,
    UpdateHealth = 0x49,
    ScoreboardObjective = 0x4A,
    SetPassengers = 0x4B,
    Teams = 0x4C,
    UpdateScore = 0x4D,
    SpawnPosition = 0x4E,
    TimeUpdate = 0x4F,
    Title = 0x50,
    EntitySoundEffect = 0x51,
    SoundEffect = 0x52,
    StopSound = 0x53,
    PlayerListHeaderAndFooter = 0x54,
    NBTQueryResponse = 0x55,
    CollectItem = 0x56,
    EntityTeleport = 0x57,
    Advancements = 0x58,
    EntityProperties = 0x59,
    EntityEffect = 0x5A,
    DeclareRecipes = 0x5B,
    Tags = 0x5C,
};
enum PacketNamePlaySB {
    TeleportConfirm = 0x00,
    QueryBlockNBT = 0x01,
    ChatMessageSB = 0x02,
    ClientStatus = 0x03,
    ClientSettings = 0x04,
    TabCompleteSB = 0x05,
    WindowConfirmationSB = 0x06,
    ClickWindowButton = 0x07,
    ClickWindow = 0x08,
    CloseWindowSB = 0x09,
    PluginMessageSB = 0x0A,
    EditBook = 0x0B,
    EntityNBTRequest = 0x0C,
    InteractEntity = 0x0D,
    KeepAliveSB = 0x0E,
    LockDificulty = 0x0F,
    PlayerPosition = 0x10,
    PlayerPositionAndRotationSB = 0x11,
    PlayerRotation = 0x12,
    PlayerMovement = 0x13,
    VehicleMoveSB = 0x14,
    SteerBoat = 0x15,
    PickItem = 0x16,
    CraftRecipeRequest = 0x17,
    PlayerAbilitiesSB = 0x18,
    PlayerDigging = 0x19,
    EntityAction = 0x1A,
    SteerVehicle = 0x1B,
    RecipeBookData = 0x1C,
    NameItem = 0x1E,
    ResourcePackStatus = 0x1F,
    AdvancementTab = 0x20,
    SelectTrade = 0x21,
    SetBeaconEffect = 0x22,
    HeldItemChangeSB = 0x23,
    UpdateCommandBlock = 0x24,
    UpdateCommandBlockCart = 0x25,
    CreativeInventoryAction = 0x27,
    UpdateJigsaw = 0x27,
    UpdateStructureBlock = 0x28,
    UpdateSign = 0x29,
    EntityAnimationSB = 0x2A,
    Spectate = 0x2B,
    PlayerBlockPlacement = 0x2C,
    UseItem = 0x2D,
};


struct Packet {
  virtual ~Packet() = default;
  virtual void ToStream(StreamOutput *stream) = 0;
  virtual void FromStream(StreamInput *stream) = 0;
  virtual int GetPacketId() = 0;
};

struct PacketHandshake : Packet {
  void ToStream(StreamOutput *stream) override {
    stream->WriteVarInt(protocolVersion);
    stream->WriteString(serverAddress);
    stream->WriteUShort(serverPort);
    stream->WriteVarInt(nextState);
  }

  void FromStream(StreamInput *stream) override {
    protocolVersion = stream->ReadVarInt();
    serverAddress = stream->ReadString();
    serverPort = stream->ReadUShort();
    nextState = stream->ReadVarInt();
  }

  int GetPacketId() override {
    return PacketNameHandshakingSB::Handshake;
  }

  int protocolVersion;
  std::string serverAddress;
  unsigned short serverPort;
  int nextState;
};

struct PacketStatusResponse : Packet {
  void ToStream(StreamOutput *stream) override {
    stream->WriteVarInt(JSONLength);
    stream->WriteString(JSONResponse);
  }

  void FromStream(StreamInput *stream) override {
    JSONLength = stream->ReadVarInt();
    JSONResponse = stream->ReadString();
  }

  int GetPacketId() override {
    return PacketNameStatusCB::Response;
  }

  int JSONLength;
  std::string JSONResponse;
};

struct PacketStatusPong : Packet {
  void ToStream(StreamOutput *stream) override {
    stream->WriteLong(payload);
  }

  void FromStream(StreamInput *stream) override {
    payload = stream->ReadLong();
  }

  int GetPacketId() override {
    return PacketNameStatusCB::Pong;
  }

  int payload;
};

struct PacketStatusRequest : Packet {
  void ToStream(StreamOutput *stream) override {
  }

  void FromStream(StreamInput *stream) override {
  }

  int GetPacketId() override {
      return PacketNameStatusSB::Request;
  }

};

struct PacketStatusPing : Packet {
  void ToStream(StreamOutput *stream) override {
    stream->WriteLong(payload);
  }

  void FromStream(StreamInput *stream) override {
    payload = stream->ReadLong();
  }

  int GetPacketId() override {
    return PacketNameStatusSB::Ping;
  }

  int payload;
};

struct PacketLoginSuccess : Packet {
    void ToStream(StreamOutput *stream) override {
        stream->WriteString(UUID);
        stream->WriteString(Username);
    }

    void FromStream(StreamInput *stream) override {
        UUID = stream->ReadString();
        Username = stream->ReadString();
    }

    int GetPacketId() override {
        return PacketNameLoginCB::LoginSuccess;
    }

    std::string UUID;
    std::string Username;
};

struct PacketDisconnect : Packet {
    void ToStream(StreamOutput *stream) override {

    }

    void FromStream(StreamInput *stream) override {
        Reason = stream->ReadChat().text;
    }

    int GetPacketId() override {
        return PacketNameLoginCB::Disconnect;
    }

    std::string Reason;
};

struct PacketSetCompression : Packet {
    void ToStream(StreamOutput *stream) override {

    }

    void FromStream(StreamInput *stream) override {
        Threshold = stream->ReadVarInt();
    }

    int GetPacketId() override {
        return PacketNameLoginCB::SetCompression;
    }

    int Threshold;
};

struct PacketLoginStart : Packet {
    void ToStream(StreamOutput *stream) override {
        stream->WriteString(Username);
    }

    void FromStream(StreamInput *stream) override {
        Username = stream->ReadString();
    }

    int GetPacketId() override {
        return PacketNameLoginSB::LoginStart;
    }

    std::string Username;
};

/*
 * Packet Play Client Bound
 */

struct PacketSpawnEntity : Packet {
    void ToStream(StreamOutput *stream) override {
    }

    void FromStream(StreamInput *stream) override {
        EntityId = stream->ReadVarInt();
        ObjectUUID = stream->ReadUUID();
        Type = stream->ReadByte();
        X = stream->ReadDouble();
        Y = stream->ReadDouble();
        Z = stream->ReadDouble();
        Pitch = stream->ReadAngle();
        Yaw = stream->ReadAngle();
        Data = stream->ReadInt();
        VelocityX = stream->ReadShort();
        VelocityY = stream->ReadShort();
        VelocityZ = stream->ReadShort();
    }

    int GetPacketId() override {
        return PacketNamePlayCB::SpawnEntity;
    }

    int EntityId;
    UUID ObjectUUID;
    unsigned char Type;
    double X, Y, Z;
    unsigned char Pitch, Yaw;
    int Data;
    short VelocityX;
    short VelocityY;
    short VelocityZ;
};

struct PacketSpawnLivingEntity : Packet {
    void ToStream(StreamOutput *stream) override {
    }

    void FromStream(StreamInput *stream) override {
        EntityId = stream->ReadVarInt();
        EntityUUID = stream->ReadUUID();
        type = stream->ReadVarInt();
        X = stream->ReadDouble();
        Y = stream->ReadDouble();
        Z = stream->ReadDouble();
        Yaw = stream->ReadAngle();
        Pitch = stream->ReadAngle();
        HeadPitch = stream->ReadAngle();
        VelocityX = stream->ReadShort();
        VelocityY = stream->ReadShort();
        VelocityZ = stream->ReadShort();
    }

    int GetPacketId() override {
        return PacketNamePlayCB::SpawnLivingEntity;
    }

    unsigned int EntityId;
    UUID EntityUUID;
    int type;
    double X, Y, Z;
    unsigned char Yaw, Pitch, HeadPitch;
    short VelocityX, VelocityY, VelocityZ;
};

struct PacketSpawnPlayer : Packet {
    void ToStream(StreamOutput *stream) override {
    }

    void FromStream(StreamInput *stream) override {
        EntityId = stream->ReadVarInt();
        PlayerUUID = stream->ReadUUID();
        X = stream->ReadDouble();
        Y = stream->ReadDouble();
        Z = stream->ReadDouble();
        Yaw = stream->ReadAngle();
        Pitch = stream->ReadAngle();
    }

    int GetPacketId() override {
        return PacketNamePlayCB::SpawnPlayer;
    }

    int EntityId;
    UUID PlayerUUID;
    double X, Y, Z;
    unsigned char Yaw, Pitch;


};

struct PacketBlockChange : Packet {
    void ToStream(StreamOutput *stream) override {
    }

    void FromStream(StreamInput *stream) override {
        Position = stream->ReadPosition();
        BlockId = stream->ReadVarInt();
    }

    int GetPacketId() override {
        return PacketNamePlayCB::BlockChange;
    }

    Vector Position;
    int BlockId;
};

struct PacketBossBar : Packet {
    void ToStream(StreamOutput *stream) override {
    }

    void FromStream(StreamInput *stream) override {
      Uuid = stream->ReadUUID();
      Action = stream->ReadVarInt();
      switch (Action) {
        case 0: {
          Title = stream->ReadChat();
          Health = stream->ReadFloat();
          Color = stream->ReadVarInt();
          Division = stream->ReadVarInt();
          Flags = stream->ReadUByte();
          break;
        }
        case 1: {
          break;
        }
        case 2: {
          Health = stream->ReadFloat();
          break;
        }
        case 3: {
          Title = stream->ReadChat();
          break;
        }
        case 4: {
          Color = stream->ReadVarInt();
          Division = stream->ReadVarInt();
          break;
        }
        case 5: {
          Flags = stream->ReadUByte();
          break;
        }
      }
    }

    int GetPacketId() override {
        return PacketNamePlayCB::BossBar;
    }

    UUID Uuid;
    int Action;
    Chat Title;
    float Health;
    int Color;
    int Division;
    unsigned char Flags;
};

struct PacketServerDifficulty : Packet {
    void ToStream(StreamOutput *stream) override {
    }

    void FromStream(StreamInput *stream) override {
      Difficulty = stream->ReadUByte();
      DifficultyLocked = stream->ReadBool();
    }

    int GetPacketId() override {
        return PacketNamePlayCB::ServerDifficulty;
    }

    unsigned char Difficulty;
    bool DifficultyLocked;
};

struct PacketChatMessageCB : Packet {
    void ToStream(StreamOutput *stream) override {
    }

    void FromStream(StreamInput *stream) override {
        JsonData = stream->ReadChat();
        Position = stream->ReadByte();
    }

    int GetPacketId() override {
        return PacketNamePlayCB::ChatMessageCB;
    }

    Chat JsonData;
    unsigned char Position;
};

struct PacketMultiBlockChange : Packet {
    void ToStream(StreamOutput *stream) override {
    }

    void FromStream(StreamInput *stream) override {
        ChunkX = stream->ReadInt();
        ChunkZ = stream->ReadInt();
        int recordCount = stream->ReadVarInt();
        for (int i = 0; i < recordCount; i++) {
            Record record;
            record.HorizontalPosition = stream->ReadUByte();
            record.YCoordinate = stream->ReadUByte();
            record.BlockId = stream->ReadVarInt();
            Records.push_back(record);
        }
    }

    int GetPacketId() override {
        return PacketNamePlayCB::MultiBlockChange;
    }

    int ChunkX;
    int ChunkZ;
    struct Record {
        unsigned char HorizontalPosition;
        unsigned char YCoordinate;
        int BlockId;
    };
    std::vector<Record> Records;
};

struct PacketDisconnectPlay : Packet {
    void ToStream(StreamOutput *stream) override {
        stream->WriteString(Reason); //TODO: Implement chat-wrapper
    }

    void FromStream(StreamInput *stream) override {
        Reason = stream->ReadChat().text;
    }

    int GetPacketId() override {
        return PacketNamePlayCB::DisconnectPlay;
    }

    std::string Reason;
};

struct PacketKeepAliveCB : Packet {
    void ToStream(StreamOutput *stream) override {
    }

    void FromStream(StreamInput *stream) override {
        KeepAliveId = stream->ReadLong();
    }

    int GetPacketId() override {
        return PacketNamePlayCB::KeepAliveCB;
    }

    long long KeepAliveId;
};

struct PacketChunkData : Packet {
    void ToStream(StreamOutput *stream) override {
    }

    void FromStream(StreamInput *stream) override {
        ChunkX = stream->ReadInt();
        ChunkZ = stream->ReadInt();
        GroundUpContinuous = stream->ReadBool();
        PrimaryBitMask = stream->ReadVarInt();
        int Size = stream->ReadVarInt();
        Data = stream->ReadByteArray(Size);
        int NumberOfBlockEntities = stream->ReadVarInt(); //TODO: Need NBT
        for (int i = 0; i < NumberOfBlockEntities; i++) {
            //BlockEntities[i] = stream->ReadNbt();
        }
    }

    int GetPacketId() override {
        return PacketNamePlayCB::ChunkData;
    }

    int ChunkX;
    int ChunkZ;
    bool GroundUpContinuous;
    int PrimaryBitMask;
    //int Size;
    std::vector<unsigned char> Data;
    //int NumberOfBlockEntities;
    std::vector<int> BlockEntities; //TODO: Replace int with NbtTag and implement NbtTree
};

struct PacketEffect : Packet {
    void ToStream(StreamOutput *stream) override {
    }

    void FromStream(StreamInput *stream) override {
      EffectID = stream->ReadInt();
      Location = stream->ReadPosition();
      Data = stream->ReadInt();
      DisableRelativeVolume = stream->ReadBool();
    }

    int GetPacketId() override {
        return PacketNamePlayCB::Effect;
    }

    int EffectID;
    Vector Location;
    int Data;
    bool DisableRelativeVolume;
};

struct PacketJoinGame : Packet {
    void ToStream(StreamOutput *stream) override {
    }

    void FromStream(StreamInput *stream) override {
        EntityId = stream->ReadInt();
        Gamemode = stream->ReadUByte();
        Dimension = stream->ReadInt();
        HashedSeed = stream->ReadLong();
        MaxPlayers = stream->ReadUByte();
        LevelType = stream->ReadString();
        ReducedDebugInfo = stream->ReadBool();
        EnableRespawnScreen = stream->ReadBool();
    }

    int GetPacketId() override {
        return PacketNamePlayCB::JoinGame;
    }

    int EntityId;
    unsigned char Gamemode;
    int Dimension;
    long long HashedSeed;
    unsigned char MaxPlayers;
    std::string LevelType;
    bool ReducedDebugInfo;
    bool EnableRespawnScreen;
};

struct PacketEntityPosition : Packet {
    void ToStream(StreamOutput *stream) override {

    }

    void FromStream(StreamInput *stream) override {
        EntityId = stream->ReadVarInt();
        DeltaX = stream->ReadShort();
        DeltaY = stream->ReadShort();
        DeltaZ = stream->ReadShort();
        OnGround = stream->ReadBool();
    }

    int GetPacketId() override {
        return PacketNamePlayCB::EntityPosition;
    }

    int EntityId;
    short DeltaX, DeltaY, DeltaZ;
    bool OnGround;
};

struct PacketEntityPositionAndRotation : Packet {
    void ToStream(StreamOutput *stream) override {

    }

    void FromStream(StreamInput *stream) override {
        EntityId = stream->ReadVarInt();
        DeltaX = stream->ReadShort();
        DeltaY = stream->ReadShort();
        DeltaZ = stream->ReadShort();
        Yaw = stream->ReadAngle();
        Pitch = stream->ReadAngle();
        OnGround = stream->ReadBool();
    }

    int GetPacketId() override {
        return PacketNamePlayCB::EntityPositionAndRotation;
    }

    int EntityId;
    short DeltaX, DeltaY, DeltaZ;
    unsigned char Yaw, Pitch;
    bool OnGround;
};

struct PacketEntityRotation : Packet {
    void ToStream(StreamOutput *stream) override {

    }

    void FromStream(StreamInput *stream) override {
        EntityId = stream->ReadVarInt();
        Yaw = stream->ReadAngle();
        Pitch = stream->ReadAngle();
        OnGround = stream->ReadBool();
    }

    int GetPacketId() override {
        return PacketNamePlayCB::EntityRotation;
    }

    int EntityId;
    unsigned char Yaw, Pitch;
    bool OnGround;
};

struct PacketPlayerPositionAndLookCB : Packet {
    void ToStream(StreamOutput *stream) override {
    }

    void FromStream(StreamInput *stream) override {
        X = stream->ReadDouble();
        Y = stream->ReadDouble();
        Z = stream->ReadDouble();
        Yaw = stream->ReadFloat();
        Pitch = stream->ReadFloat();
        Flags = stream->ReadUByte();
        TeleportId = stream->ReadVarInt();
    }

    int GetPacketId() override {
        return PacketNamePlayCB::PlayerPositionAndLookCB;
    }

    double X;
    double Y;
    double Z;
    float Yaw;
    float Pitch;
    unsigned char Flags;
    int TeleportId;
};

struct PacketUpdateHealth : Packet {
    void ToStream(StreamOutput *stream) override {
        stream->WriteFloat(Health);
        stream->WriteVarInt(Food);
        stream->WriteFloat(FoodSaturation);
    }

    void FromStream(StreamInput *stream) override {
        Health = stream->ReadFloat();
        Food = stream->ReadVarInt();
        FoodSaturation = stream->ReadFloat();
    }

    int GetPacketId() override {
        return PacketNamePlayCB::UpdateHealth;
    }

    float Health;
    int Food;
    float FoodSaturation;
};

struct PacketSpawnPosition : Packet {
    void ToStream(StreamOutput *stream) override {
    }

    void FromStream(StreamInput *stream) override {
        Location = stream->ReadPosition();
    }

    int GetPacketId() override {
        return PacketNamePlayCB::SpawnPosition;
    }

    Vector Location;
};


struct PacketUnloadChunk : Packet {
  void ToStream(StreamOutput *stream) override {

  }

  void FromStream(StreamInput *stream) override {
    ChunkX = stream->ReadInt();
    ChunkZ = stream->ReadInt();
  }

  int GetPacketId() override {
    return PacketNamePlayCB::UnloadChunk;
  }

  int ChunkX;
  int ChunkZ;
};

struct PacketWindowConfirmationCB : Packet {
  void ToStream(StreamOutput *stream) override {
  }

  void FromStream(StreamInput *stream) override {
    WindowId = stream->ReadByte();
    ActionNumber = stream->ReadShort();
    Accepted = stream->ReadBool();
  }

  int GetPacketId() override {
    return PacketNamePlayCB::WindowConfirmationCB;
  }

  signed char WindowId;
  short ActionNumber;
  bool Accepted;
};

struct PacketCloseWindowCB : Packet {
  void ToStream(StreamOutput *stream) override {
  }

  void FromStream(StreamInput *stream) override {
    WindowId = stream->ReadUByte();
  }

  int GetPacketId() override {
    return PacketNamePlayCB::CloseWindowCB;
  }

  unsigned char WindowId;
};

struct PacketWindowItems : Packet {
  void ToStream(StreamOutput *stream) override {
  }

  void FromStream(StreamInput *stream) override {
    WindowId = stream->ReadUByte();
    Count = stream->ReadShort();
    for (int i = 0; i < Count; ++i) {
      SlotData.push_back(stream->ReadSlot());
    }
  }

  int GetPacketId() override {
    return PacketNamePlayCB::WindowItems;
  }

  unsigned char WindowId;
  short Count;
  std::vector<SlotDataType> SlotData;
};

struct PacketWindowProperty : Packet {
  void ToStream(StreamOutput *stream) override {
  }

  void FromStream(StreamInput *stream) override {
    WindowId = stream->ReadUByte();
    Property = stream->ReadShort();
    Value = stream->ReadShort();
  }

  int GetPacketId() override {
    return PacketNamePlayCB::WindowProperty;
  }

  unsigned char WindowId;
  short Property;
  short Value;
};

struct PacketSetSlot : Packet {
  void ToStream(StreamOutput *stream) override {

  }

  void FromStream(StreamInput *stream) override {
    WindowId = stream->ReadByte();
    LOG(INFO) << "SetSlot " << (int) WindowId;
    Slot = stream->ReadShort();
    LOG(INFO) << "SetSlot " << Slot;
    SlotData = stream->ReadSlot();
    LOG(INFO) << "SetSlot slotdata";
  }

  int GetPacketId() override {
    return PacketNamePlayCB::SetSlot;
  }

  signed char WindowId;
  short Slot;
  SlotDataType SlotData;
};

struct PacketSetCooldown : Packet {
    void ToStream(StreamOutput *stream) override {
    }

    void FromStream(StreamInput *stream) override {
      ItemID = stream->ReadVarInt();
      CooldownTicks = stream->ReadVarInt();
    }

    int GetPacketId() override {
        return PacketNamePlayCB::SetCooldown;
    }

    int ItemID;
    int CooldownTicks;
};

struct PacketOpenWindow : Packet {
    void ToStream(StreamOutput *stream) override {

    }

    void FromStream(StreamInput *stream) override {
        WindowId = stream->ReadUByte();
        WindowType = stream->ReadString();
        WindowTitle = stream->ReadChat().text;
        NumberOfSlots = stream->ReadUByte();

        if (WindowType == "EntityHorse")
            EntityId = stream->ReadInt();
    }

    int GetPacketId() override {
        return PacketNamePlayCB::OpenWindow;
    }

    unsigned char WindowId;
    std::string WindowType;
    std::string WindowTitle;
    unsigned char NumberOfSlots;
    int EntityId;
};

struct PacketDestroyEntities : Packet {
    void ToStream(StreamOutput *stream) override {

    }

    void FromStream(StreamInput *stream) override {
        int count = stream->ReadVarInt();
        EntityIds.reserve(count);
        for (int i = 0; i < count; i++) {
            int entityId = stream->ReadVarInt();
            EntityIds.push_back(entityId);
        }
    }

    int GetPacketId() override {
        return PacketNamePlayCB::DestroyEntities;
    }

    std::vector <unsigned int> EntityIds;
};

struct PacketEntityVelocity : Packet {
    void ToStream(StreamOutput *stream) override {
    }

    void FromStream(StreamInput *stream) override {
        EntityId = stream->ReadVarInt();
        VelocityX = stream->ReadShort();
        VelocityY = stream->ReadShort();
        VelocityZ = stream->ReadShort();
    }

    int GetPacketId() override {
        return PacketNamePlayCB::EntityVelocity;
    }

    int EntityId;
    short VelocityX;
    short VelocityY;
    short VelocityZ;
};

struct PacketTimeUpdate : Packet {
    void ToStream(StreamOutput *stream) override {

    }

    void FromStream(StreamInput *stream) override {
        WorldAge = stream->ReadLong();
        TimeOfDay = stream->ReadLong();
    }

    int GetPacketId() override {
        return PacketNamePlayCB::TimeUpdate;
    }

    long long WorldAge;
    long long TimeOfDay;
};

struct PacketSoundEffect : Packet {
  void ToStream(StreamOutput *stream) override {

  }

  void FromStream(StreamInput *stream) override {
    SoundID = stream->ReadVarInt();
    SoundCategory = stream->ReadVarInt();
    EffectPositionX = stream->ReadInt();
    EffectPositionY = stream->ReadInt();
    EffectPositionZ = stream->ReadInt();
    Volume = stream->ReadFloat();
    Pitch = stream->ReadFloat();
  }

  int GetPacketId() override {
    return PacketNamePlayCB::SoundEffect;
  }

  int SoundID;
  int SoundCategory;
  int EffectPositionX;
  int EffectPositionY;
  int EffectPositionZ;
  float Volume;
  float Pitch;
};

struct PacketEntityTeleport : Packet {
    void ToStream(StreamOutput *stream) override {
    }

    void FromStream(StreamInput *stream) override {
        EntityId = stream->ReadVarInt();
        X = stream->ReadDouble();
        Y = stream->ReadDouble();
        Z = stream->ReadDouble();
        Yaw = stream->ReadAngle();
        Pitch = stream->ReadAngle();
        OnGround = stream->ReadBool();
    }

    int GetPacketId() override {
        return PacketNamePlayCB::EntityTeleport;
    }

    int EntityId;
    double X, Y, Z;
    unsigned char Yaw, Pitch;
    bool OnGround;
};


/*
 * Packet Play Server Bound
 */

struct PacketKeepAliveSB : Packet {
    void ToStream(StreamOutput *stream) override {
        stream->WriteLong(KeepAliveId);
    }

    void FromStream(StreamInput *stream) override {
        KeepAliveId = stream->ReadLong();
    }

    int GetPacketId() override {
        return PacketNamePlaySB::KeepAliveSB;
    }

    long long KeepAliveId;

    PacketKeepAliveSB(int KeepAliveId) : KeepAliveId(KeepAliveId) {}
};

struct PacketTeleportConfirm : Packet {
    void ToStream(StreamOutput *stream) override {
        stream->WriteVarInt(TeleportId);
    }

    void FromStream(StreamInput *stream) override {
        TeleportId = stream->ReadVarInt();
    }

    int GetPacketId() override {
        return PacketNamePlaySB::TeleportConfirm;
    }

    int TeleportId;

    PacketTeleportConfirm(int TeleportId) : TeleportId(TeleportId) {}
};

struct PacketClientStatus : Packet {
    void ToStream(StreamOutput *stream) override {
        stream->WriteVarInt(ActionId);
    }

    void FromStream(StreamInput *stream) override {
        ActionId = stream->ReadVarInt();
    }

    int GetPacketId() override {
        return PacketNamePlaySB::ClientStatus;
    }

    int ActionId;

    PacketClientStatus(int ActionId) : ActionId(ActionId) {}
};

struct PacketPlayerPositionAndRotationSB : Packet {
    void ToStream(StreamOutput *stream) override {
        stream->WriteDouble(X);
        stream->WriteDouble(FeetY);
        stream->WriteDouble(Z);
        stream->WriteFloat(Yaw);
        stream->WriteFloat(Pitch);
        stream->WriteBool(OnGround);
    }

    void FromStream(StreamInput *stream) override {
        X = stream->ReadDouble();
        FeetY = stream->ReadDouble();
        Z = stream->ReadDouble();
        Yaw = stream->ReadFloat();
        Pitch = stream->ReadFloat();
        OnGround = stream->ReadBool();
    }

    int GetPacketId() override {
        return PacketNamePlaySB::PlayerPositionAndRotationSB;
    }


    double X;
    double FeetY;
    double Z;
    float Yaw;
    float Pitch;
    bool OnGround;

    PacketPlayerPositionAndRotationSB(double X, double FeetY, double Z,
                                      float Yaw, float Pitch, bool OnGround) : X(X), FeetY(FeetY), Z(Z), Yaw(Yaw),
                                                                               Pitch(Pitch), OnGround(OnGround) {}
};

struct PacketPlayerPosition : Packet {
    void ToStream(StreamOutput *stream) override {
        stream->WriteDouble(X);
        stream->WriteDouble(FeetY);
        stream->WriteDouble(Z);
        stream->WriteBool(OnGround);
    }

    void FromStream(StreamInput *stream) override {
        X = stream->ReadDouble();
        FeetY = stream->ReadDouble();
        Z = stream->ReadDouble();
        OnGround = stream->ReadBool();
    }

    int GetPacketId() override {
        return PacketNamePlaySB::PlayerPosition;
    }

    double X;
    double FeetY;
    double Z;
    bool OnGround;

    PacketPlayerPosition(double X, double Y, double Z, bool ground) : X(X), FeetY(Y), Z(Z), OnGround(ground) {}
};
struct PacketPlayerRotation : Packet {
    void ToStream(StreamOutput *stream) override {
        stream->WriteFloat(Yaw);
        stream->WriteFloat(Pitch);
        stream->WriteBool(OnGround);
    }

    void FromStream(StreamInput *stream) override {
        Yaw = stream->ReadFloat();
        Pitch = stream->ReadFloat();
        OnGround = stream->ReadBool();
    }

    int GetPacketId() override {
        return PacketNamePlaySB::PlayerRotation;
    }

    float Yaw;
    float Pitch;
    bool OnGround;

    PacketPlayerRotation(float Yaw, float Pitch, bool ground) : Yaw(Yaw), Pitch(Pitch), OnGround(ground) {}
};

struct PacketWindowConfirmationSB : Packet {
    void ToStream(StreamOutput *stream) override {
        stream->WriteByte(WindowId);
        stream->WriteShort(ActionNumber);
        stream->WriteBool(Accepted);
    }

    void FromStream(StreamInput *stream) override {
        WindowId = stream->ReadByte();
        ActionNumber = stream->ReadShort();
        Accepted = stream->ReadBool();
    }

    int GetPacketId() override {
        return PacketNamePlaySB::WindowConfirmationSB;
    }

    signed char WindowId;
    short ActionNumber;
    bool Accepted;
};

struct PacketClickWindow : Packet {
    void ToStream(StreamOutput *stream) override {
        stream->WriteUByte(WindowId);
        stream->WriteShort(Slot);
        stream->WriteByte(Button);
        stream->WriteShort(ActionNumber);
        stream->WriteVarInt(Mode);
        stream->WriteSlot(ClickedItem);
    }

    void FromStream(StreamInput *stream) override {

    }

    int GetPacketId() override {
        return PacketNamePlaySB::ClickWindow;
    }

    unsigned char WindowId;
    short Slot;
    signed char Button;
    short ActionNumber;
    int Mode;
    SlotDataType ClickedItem;

    PacketClickWindow(unsigned char windowId, short slot, signed char button, short actionNumber, int mode, SlotDataType ClickedItem) : WindowId(windowId), Slot(slot),
        Button(button), ActionNumber(actionNumber), Mode(mode), ClickedItem(ClickedItem) {};
};

struct PacketCloseWindowSB : Packet {
    void ToStream(StreamOutput *stream) override {
        stream->WriteUByte(WindowId);
    }

    void FromStream(StreamInput *stream) override {

    }

    int GetPacketId() override {
        return PacketNamePlaySB::CloseWindowSB;
    }

    unsigned char WindowId;
};

struct PacketChatMessageSB : Packet {
    void ToStream(StreamOutput *stream) override {
        stream->WriteString(Message);
    }

    void FromStream(StreamInput *stream) override {

    }

    int GetPacketId() override {
        return PacketNamePlaySB::ChatMessageSB;
    }

    std::string Message;

    PacketChatMessageSB(const std::string &msg) : Message(msg) {};
};

struct PacketPlayerDigging : Packet {
    void ToStream(StreamOutput *stream) override {
        stream->WriteVarInt(Status);
        stream->WritePosition(Location);
        stream->WriteByte(Face);
    }

    void FromStream(StreamInput *stream) override {

    }

    int GetPacketId() override {
        return PacketNamePlaySB::PlayerDigging;
    }

    int Status;
    Vector Location;
    signed char Face;

    PacketPlayerDigging(int status, const Vector& location, signed char face) : Status(status),Location(location),Face(face) {};
};

struct PacketPlayerBlockPlacement : Packet {
    void ToStream(StreamOutput *stream) override {
        stream->WritePosition(location);
        stream->WriteByte(face);
        stream->WriteByte(hand);
        stream->WriteFloat(cursorPositionX);
        stream->WriteFloat(cursorPositionY);
        stream->WriteFloat(cursorPositionZ);
    }

    void FromStream(StreamInput *stream) override {

    }

    int GetPacketId() override {
        return PacketNamePlaySB::PlayerBlockPlacement;
    }

    PacketPlayerBlockPlacement(
        const Vector& location, signed char face, unsigned char hand,
        float cursorPositionX, float cursorPositionY, float cursorPositionZ)
            : location(location), face(face), hand(hand), cursorPositionX(cursorPositionX),
              cursorPositionY(cursorPositionY), cursorPositionZ(cursorPositionZ) {};

    Vector location;
    signed char face;
    unsigned char hand;
    float cursorPositionX;
    float cursorPositionY;
    float cursorPositionZ;
};

struct PacketUseItem : Packet {
  void ToStream(StreamOutput *stream) override {
    stream->WriteVarInt(hand);
  }

  void FromStream(StreamInput *stream) override {
    hand = stream->ReadVarInt();
  }

  int GetPacketId() override {
    return PacketNamePlaySB::UseItem;
  }

  int hand;

  PacketUseItem(const int hand) : hand(hand) {};
};
#endif
