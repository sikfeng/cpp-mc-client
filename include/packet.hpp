#ifndef __PACKET_H__
#define __PACKET_H__

#include <iostream>
#include <vector>
#include <sstream>

class VarInt {
  public:
    int val = 0;
    VarInt() {};
    VarInt(int v) : val(v) {};
    VarInt& operator+=(VarInt const rhs) {
      this->val += rhs.val;
      return *this;
    }
    VarInt& operator>>=(int rhs) {
      this->val >>= rhs;
      return *this;
    }
    VarInt& operator|=(int rhs) {
      this->val |= rhs;
      return *this;
    }
    VarInt operator&(int rhs) {
      VarInt v;
      v.val = this->val & rhs;
      return v;
    }
    bool operator!=(VarInt const rhs) {
      return this->val != rhs.val;
    }
    bool operator!=(int rhs) {
      return this->val != rhs;
    }
    bool operator==(VarInt const rhs) {
      return this->val == rhs.val;
    }
    bool operator==(int rhs) {
      return this->val == rhs;
    }
};

class Packet {
  public:
    int PacketID;
    int PacketLength;
    int DataLength;
    std::vector<char> Data;

    Packet() {};
    std::vector<char> Pack(int threshold);
    std::vector<char> Encode(uint32_t v);
    std::vector<char> Encode(VarInt v);
    std::vector<char> Encode(uint64_t v);
    std::vector<char> Compress(std::vector<char>);
};

#endif
