#include "stream.hpp"

#include <easylogging++.h>

#include "socket.hpp"
#include "utility.hpp"

const int MAX_VARINT_LENGTH = 5;

/*
enum NBTTag {
  TagEnd = 0,
  TagByte = 1,
  TagShort = 2,
  TagInt = 3,
  TagLong = 4,
  TagFloat = 5,
  TagDouble = 6,
  TagByteArray = 7,
  TagString = 8,
  TagList = 9,
  TagCompound = 10,
  TagIntArray = 11,
  TagLongArray = 12,
};

struct NBTTagType {
  virtual void ReadPayload();

  void ReadName() {
    unsigned char NameLength;
    socket->ReadData(&NameLength, 2);
    for (int i = 0; i < NameLength; ++i) {
      char c;
      socket->ReadData(&c, 1);
      name += c;
    }
  }

  std::string name;
  StreamInput *socket;

  NBTTagType(StreamSocket *socket) : socket(socket) {}
};

struct NBTTagByte : NBTTagType {
  void ReadPayload() override {
    socket->ReadData((signed char) &Payload, 1);
  }

  signed char Payload;
};

struct NBTTagShort : NBTTagType {
  void ReadPayload() override {
    socket->ReadData(&Payload, 2);
  }

  short Payload;
};

struct NBTTagInt : NBTTagType {
  void ReadPayload() override {
    socket->ReadData(&Payload, 4);
  }

  int Payload;
};

struct NBTTagLong : NBTTagType {
  void ReadPayload() override {
    socket->ReadData(&Payload, 8);
  }

  long Payload;
};

struct NBTTagFloat : NBTTagType {
  void ReadPayload() override {
    socket->ReadData(&Payload, 4);
  }

  float Payload;
};

struct NBTTagDouble : NBTTagType {
  void ReadPayload() override {
    socket->ReadData(&Payload, 8);
  }

  double Payload;
};

struct NBTTagByteArray : NBTTagType {
  void ReadPayload() override {
    int size;
    socket->ReadData(&size, 4);
    for (int i = 0; i < size; ++i) {
      char c;
      socket->ReadData(&size, 2);
      Payload.push_back(c);
    }
  }

  std::vector<char> Payload;
};

struct NBTTagString : NBTTagType {
  void ReadPayload() override {
    unsigned short PayloadLength;
    for (int i = 0; i < PayloadLength; ++i) {
      char c;
      socket->ReadData(&c, 1);
      Payload += c;
    }
  }

  std::string Payload;
};

struct NBTTagList : NBTTagType {
  void ReadPayload() override {
    unsigned short PayloadLength;
    for (int i = 0; i < PayloadLength; ++i) {
      signed char TagType;
      TagType = socket->ReadByte();
      NBTTagType NBTTag;
      switch (TagType) {
        case 0:
          break;
        case 1:
          NBTTag = NBTTagByte(this->socket);
          break;
        case 2:
          NBTTag = NBTTagShort(this->socket);
          break;
        case 3:
          NBTTag = NBTTagInt(this->socket);
          break;
        case 4:
          NBTTag = NBTTagLong(this->socket);
          break;
        case 5:
          NBTTag = NBTTagFloat(this->socket);
          break;
        case 6:
          NBTTag = NBTTagDouble(this->socket);
          break;
        case 7:
          NBTTag = NBTTagByteArray(this->socket);
          break;
        case 8:
          NBTTag = NBTTagString(this->socket);
          break;
        case 9:
          NBTTag = NBTTagList(this->socket);
          break;
        case 10:
          NBTTag = NBTTagCompound(this->socket);
          break;
        case 11:
          NBTTag = NBTTagIntArray(this->socket);
          break;
        case 12:
          NBTTag = NBTTagLongArray(this->socket);
          break;
        default:
          LOG(INFO) << "Unrecognized NBT Tag ID: " << TagType;
      }
      NBTTag.ReadPayload();
      Payload.push_back(NBTTag);
    }
  }

  std::vector<NBTTagType> Payload;
};

struct NBTTagCompound : NBTTagType {
  void ReadPayload() override {
    while (true) {
      signed char TagType;
      TagType = socket->ReadByte();
      NBTTagType NBTTag;
      switch (TagType) {
        case 0:
          break;
        case 1:
          NBTTag = NBTTagByte(this->socket);
          break;
        case 2:
          NBTTag = NBTTagShort(this->socket);
          break;
        case 3:
          NBTTag = NBTTagInt(this->socket);
          break;
        case 4:
          NBTTag = NBTTagLong(this->socket);
          break;
        case 5:
          NBTTag = NBTTagFloat(this->socket);
          break;
        case 6:
          NBTTag = NBTTagDouble(this->socket);
          break;
        case 7:
          NBTTag = NBTTagByteArray(this->socket);
          break;
        case 8:
          NBTTag = NBTTagString(this->socket);
          break;
        case 9:
          NBTTag = NBTTagList(this->socket);
          break;
        case 10:
          NBTTag = NBTTagCompound(this->socket);
          break;
        case 11:
          NBTTag = NBTTagIntArray(this->socket);
          break;
        case 12:
          NBTTag = NBTTagLongArray(this->socket);
          break;
        default:
          LOG(INFO) << "Unrecognized NBT Tag ID: " << TagType;
      }
      NBTTag.ReadPayload();
      NBTTag.ReadName();
      Payload.push_back(NBTTag);
      }
    }
  }

  std::vector<NBTTagType> Payload;
};

struct NBTTagIntArray : NBTTagType {
  void ReadPayload() override {
    int size;
    ReadData(&size, 4);
    for (int i = 0; i < size; ++i) {
      int c;
      socket->ReadData(&size, 4);
      Payload.push_back(c);
    }
  }

  std::vector<int> Payload;
};

struct NBTTagLongArray : NBTTagType {
  void ReadPayload() override {
    int size;
    ReadData(&size, 8);
    for (int i = 0; i < size; ++i) {
      char c;
      socket->ReadData(&size, 2);
      Payload.push_back(c);
    }
  }

  std::vector<long> Payload;
};
*/

bool StreamInput::ReadBool() {
	unsigned char value;
	ReadData(&value, 1);
	return value != 0;
}

signed char StreamInput::ReadByte() {
	signed char value;
	ReadData((unsigned char *) &value, 1);
	endswap(value);
	return value;
}

unsigned char StreamInput::ReadUByte() {
	unsigned char value;
	ReadData(&value, 1);
	endswap(value);
	return value;
}

short StreamInput::ReadShort() {
	unsigned short value;
	ReadData((unsigned char *) &value, 2);
	endswap(value);
	return value;
}

unsigned short StreamInput::ReadUShort() {
	unsigned short value;
	ReadData((unsigned char *) &value, 2);
	endswap(value);
	return value;
}

int StreamInput::ReadInt() {
	int value;
	ReadData((unsigned char *) &value, 4);
	endswap(value);
	return value;
}

long long StreamInput::ReadLong() {
	long long value;
	ReadData((unsigned char *) &value, 8);
	endswap(value);
	return value;
}

float StreamInput::ReadFloat() {
	float value;
	ReadData((unsigned char *) &value, 4);
	endswap(value);
	return value;
}

double StreamInput::ReadDouble() {
	double value;
	ReadData((unsigned char *) &value, 8);
	endswap(value);
	return value;
}

std::string StreamInput::ReadString() {
	int strLength = ReadVarInt();
	std::vector<unsigned char> buff(strLength + 1);
	ReadData(buff.data(), strLength);
	buff[strLength] = 0;
	std::string str((char *) buff.data());
	return str;
}

Chat StreamInput::ReadChat() {
	/*std::string str, jsonStr = ReadString();
	nlohmann::json json;
	try {
		json = nlohmann::json::parse(jsonStr);
	} catch (std::exception &e) {
		LOG(WARNING) << "Chat json parsing failed: " << e.what();
		LOG(WARNING) << "Corrupted json: " << jsonStr;
		return "";
	}
	if (json.find("translate") != json.end())
		if (json["translate"].get<std::string>() == "multiplayer.disconnect.kicked")
			return "kicked by operator";
	for (auto &it:json["extra"]) {
		str += it["text"].get<std::string>();
	}*/
  Chat str(ReadString());
	return str;
}

int StreamInput::ReadVarInt() {
	unsigned char data[MAX_VARINT_LENGTH] = {0};
	size_t dataLen = 0;
	do {
		ReadData(&data[dataLen], 1);
	} while ((data[dataLen++] & 0x80) != 0);

	int readed = 0;
	int result = 0;
	char read;
	do {
		read = data[readed];
		int value = (read & 0b01111111);
		result |= (value << (7 * readed));
		readed++;
	} while ((read & 0b10000000) != 0);

	return result;
}

long long StreamInput::ReadVarLong() {
	return 0;
}

std::vector<unsigned char> StreamInput::ReadEntityMetadata() {
  LOG(FATAL) << "Reading EntityMetadata is not implemented";
	return std::vector<unsigned char>();
}

SlotDataType StreamInput::ReadSlot() {
    SlotDataType slot;
    slot.BlockId = ReadShort();

    if (slot.BlockId == -1)
        return slot;

    slot.ItemCount = ReadByte();
    slot.ItemDamage = ReadShort();

    //ReadNbtTag();

    return slot;
}

/*
std::vector<unsigned char> StreamInput::ReadNbtTag() {
    int NBTTagID = ReadByte();
    switch (NBTTagID) {
      case 0:
        break;
      case 1:
        ReadString();
        ReadByte();
        //throw std::runtime_error("Slot data with NBT not supported");
        break;
      case 2:
        ReadString();
        ReadShort();
        //throw std::runtime_error("Slot data with NBT not supported");
        break;
      case 3:
        ReadString();
        ReadInt();
        //throw std::runtime_error("Slot data with NBT not supported");
        break;
      case 4:
        ReadString();
        ReadLong();
        //throw std::runtime_error("Slot data with NBT not supported");
        break;
      case 5:
        ReadString();
        ReadFloat();
        //throw std::runtime_error("Slot data with NBT not supported");
        break;
      case 6:
        ReadString();
        ReadDouble();
        //throw std::runtime_error("Slot data with NBT not supported");
        break;
      case 7: {
        ReadString();
        int arrayLength = ReadInt();
        ReadByteArray(arrayLength);
        //throw std::runtime_error("Slot data with NBT not supported");
        break;
      }
      case 8:
        ReadString();
        ReadString();
        //throw std::runtime_error("Slot data with NBT not supported");
        break;
      case 9: {
        ReadString();
        int listType = ReadByte();
        int listhLength = ReadInt();
        //ReadList();
        //throw std::runtime_error("Slot data with NBT not supported");
        break;
      }
      case 10:
        ReadString();
        ReadNbtTag();
        //throw std::runtime_error("Slot data with NBT not supported");
        break;
      case 11: {
        ReadString();
        int arrayLength = ReadInt();
        //ReadIntArray(arrayLength);
        //throw std::runtime_error("Slot data with NBT not supported");
        break;
      }
      case 12: {
        int arrayLength = ReadInt();
        //ReadLongArray(arrayLength);
        //throw std::runtime_error("Slot data with NBT not supported");
        break;
      }
      default:
        LOG(WARNING) << "Unknown NBT Tag ID " << NBTTagID;
        //throw std::runtime_error("Unknown NBT Tag");
        break;
    }

  //LOG(FATAL) << "Reading NBT is not implemented";
	return std::vector<unsigned char>();
}
*/

Vector StreamInput::ReadPosition() {
	unsigned long long t = ReadLong();
	int x = t >> 38;
	int y = (t >> 26) & 0xFFF;
	int z = t << 38 >> 38;
	if (x >= pow(2, 25)) {
		x -= pow(2, 26);
	}
	if (y >= pow(2, 11)) {
		y -= pow(2, 12);
	}
	if (z >= pow(2, 25)) {
		z -= pow(2, 26);
	}
	return Vector(x, y, z);
}

unsigned char StreamInput::ReadAngle() {
	return ReadUByte();
}

UUID StreamInput::ReadUUID() {
	unsigned char buff[16];
  LOG(INFO) << "Start Reading UUID";
	ReadData(buff, 16);
  LOG(INFO) << "Done Reading UUID";
	endswap(buff, 16);
  return UUID(buff,buff+16);
}

std::vector<unsigned char> StreamInput::ReadByteArray(size_t arrLength) {
	std::vector<unsigned char> buffer(arrLength);
	ReadData(buffer.data(), arrLength);
	return buffer;

}

void StreamOutput::WriteBool(bool value) {
	unsigned char val = value ? 1 : 0;
	endswap(val);
	WriteData(&val, 1);
}

void StreamOutput::WriteByte(signed char value) {
	endswap(value);
	WriteData((unsigned char *) &value, 1);
}

void StreamOutput::WriteUByte(unsigned char value) {
	endswap(value);
	WriteData(&value, 1);
}

void StreamOutput::WriteShort(short value) {
	endswap(value);
	WriteData((unsigned char *) &value, 2);
}

void StreamOutput::WriteUShort(unsigned short value) {
	endswap(value);
	WriteData((unsigned char *) &value, 2);
}

void StreamOutput::WriteInt(int value) {
	endswap(value);
	WriteData((unsigned char *) &value, 4);
}

void StreamOutput::WriteLong(long long value) {
	endswap(value);
	WriteData((unsigned char *) &value, 8);
}

void StreamOutput::WriteFloat(float value) {
	endswap(value);
	WriteData((unsigned char *) &value, 4);
}

void StreamOutput::WriteDouble(double value) {
	endswap(value);
	WriteData((unsigned char *) &value, 8);
}

void StreamOutput::WriteString(const std::string &value) {
	WriteVarInt(value.size());
	WriteData((unsigned char *) value.data(), value.size());
}

void StreamOutput::WriteChat(const Chat &value) {
	WriteString(value.ToJson());
}

void StreamOutput::WriteVarInt(int value) {
	unsigned char buff[5];
	size_t len = 0;
	do {
		unsigned char temp = (unsigned char) (value & 0b01111111);
		value >>= 7;
		if (value != 0) {
			temp |= 0b10000000;
		}
		buff[len] = temp;
		len++;
	} while (value != 0);
	WriteData(buff, len);
}

void StreamOutput::WriteVarLong(long long value) {
	unsigned char buff[10];
	size_t len = 0;
	do {
		unsigned char temp = (unsigned char) (value & 0b01111111);
		value >>= 7;
		if (value != 0) {
			temp |= 0b10000000;
		}
		buff[len] = temp;
		len++;
	} while (value != 0);
	WriteData(buff, len);
}

void StreamOutput::WriteEntityMetadata(const std::vector<unsigned char> &value) {
	LOG(FATAL) << "Used unimplemented WriteEntityMetadata: " << value.size();
}

void StreamOutput::WriteSlot(const SlotDataType &value) {
    WriteShort(value.BlockId);
    if (value.BlockId == -1)
        return;
    WriteByte(value.ItemCount);
    WriteShort(value.ItemDamage);
    WriteByte(0);
}

/*
void StreamOutput::WriteNbtTag(const std::vector<unsigned char> &value) {
	LOG(FATAL) << "Used unimplemented WriteNbtTag " << value.size();
}
*/

void StreamOutput::WritePosition(const Vector &value) {
	unsigned long long pos = ((value.x & 0x3FFFFFF) << 38) | ((value.y & 0xFFF) << 26) | (value.z & 0x3FFFFFF);
	WriteLong(pos);
}

void StreamOutput::WriteAngle(unsigned char value) {
	WriteUByte(value);
}

void StreamOutput::WriteUUID(const UUID &value) {
	WriteByteArray(value);
}

void StreamOutput::WriteByteArray(const std::vector<unsigned char> &value) {
    auto& val = const_cast<std::vector<unsigned char>&>(value);
	WriteData(val.data(), val.size());
}

void StreamBuffer::ReadData(unsigned char *buffPtr, size_t buffLen) {
	size_t bufferLengthLeft = buffer.data() + buffer.size() - bufferPtr;

  if (bufferLengthLeft < buffLen) {
    throw std::runtime_error("Internal error: StreamBuffer reader out of data");
  }
	std::memcpy(buffPtr, bufferPtr, buffLen);
	bufferPtr += buffLen;
}

void StreamBuffer::WriteData(unsigned char *buffPtr, size_t buffLen) {
	size_t bufferLengthLeft = buffer.data() + buffer.size() - bufferPtr;
	if (bufferLengthLeft < buffLen)
		throw std::runtime_error("Internal error: StreamBuffer writer out of data");
	std::memcpy(bufferPtr, buffPtr, buffLen);
	bufferPtr += buffLen;
}

StreamBuffer::StreamBuffer(unsigned char *data, size_t dataLen) : buffer(data,data+dataLen) {
	bufferPtr = buffer.data();
}

StreamBuffer::StreamBuffer(size_t bufferLen) : buffer(bufferLen) {	
	bufferPtr = buffer.data();
	for (auto &it : buffer)
		it = 0;
}

std::vector<unsigned char> StreamBuffer::GetBuffer() {
	return buffer;
}

size_t StreamBuffer::GetReadedLength() {
    return bufferPtr - buffer.data();
}

void StreamCounter::WriteData(unsigned char *buffPtr, size_t buffLen) {
	buffPtr++;
	size += buffLen;
}

StreamCounter::StreamCounter(size_t initialSize) : size(initialSize) {

}

StreamCounter::~StreamCounter() {

}

size_t StreamCounter::GetCountedSize() {
	return size;
}

void StreamSocket::ReadData(unsigned char *buffPtr, size_t buffLen) {
	socket->Read(buffPtr, buffLen);
}

void StreamSocket::WriteData(unsigned char *buffPtr, size_t buffLen) {
	socket->Write(buffPtr, buffLen);
}

StreamSocket::StreamSocket(Socket *socketPtr) : socket(socketPtr) {

}
