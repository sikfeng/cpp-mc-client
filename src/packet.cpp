#include "packet.hpp"
#include "zlib_strings.hpp"

/* from tnze
Packet::Marshall(char ID, std::vector<char> fields) {
  this->ID = ID;
  for (int idx = 0; i < fields.size(); ++i) {
    this->Data->push_back(fields.second.Encode());
  }
}
*/

std::vector<char> Packet::Encode(VarInt v) {
  std::vector<char> vi;
  while (true) {
    VarInt b = v & 0x7F;
    v >>= 7;
    if (v != 0) {
      b |= 0x80;
    }
    vi.push_back(char(b.val));
    if (b == 0) {
      break;
    }
  }
  return vi;
}

std::vector<char> Packet::Encode(uint64_t v) {
  std::vector<char> vi;
  vi.push_back(v >> 56);
  vi.push_back(v >> 48);
  vi.push_back(v >> 40);
  vi.push_back(v >> 32);
  vi.push_back(v >> 24);
  vi.push_back(v >> 16);
  vi.push_back(v >> 8);
  vi.push_back(v);
  return vi;
}

std::vector<char> Packet::Compress(std::vector<char> data) {
  std::string data_string(data.begin(), data.end());
  std::string compressed = compress_string(data_string);
  std::vector<char> b(compressed.begin(), compressed.end());
  return b;
}

std::vector<char> Packet::Pack(int threshold) {
  std::vector<char> data;
  data.push_back(this->PacketID);
  data.insert(data.end(), this->Data.begin(), this->Data.end());
  std::vector<char> pack;
  /*
  if (threshold > 0) {
    if (data.size() > threshold) {
      int Len = data.size();
      int VarLen = VarInt(Len).Encode();
      data = Compress(data);
  
      pack = append(pack, VarInt(len(VarLen)+len(data)).Encode()...)
      pack = append(pack, VarLen...)
      pack = append(pack, data...)
    } else {
      pack = append(pack, VarInt(int32(len(data)+1)).Encode()...)
      pack = append(pack, 0x00)
      pack = append(pack, data...)
    }
  } else {
    pack = append(pack, VarInt(int32(len(data))).Encode()...) //len
    pack = append(pack, data...)
  }
  */
  return pack;  
}
