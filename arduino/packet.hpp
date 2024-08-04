#include <cstdint>
#define HEADER_LENGTH 3

// Lets make a mavlink lol
struct __attribute__((packed)) Payload
{ 
  uint16_t teensyID;
  uint16_t packetID;
  int16_t temperature;
};  

struct __attribute__((packed)) Packet
{ 
  const uint8_t start = 0xAA;
  uint8_t header[HEADER_LENGTH] = {0xBB, 0xCC, 0xDD};
  const uint32_t payloadLength = sizeof(Payload);
  Payload data;
  uint16_t crc;
  uint8_t end=0xFF;
};
