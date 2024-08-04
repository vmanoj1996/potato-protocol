// 1. Upload this code into teensy 4.1 or any other arduino board
// 2. Check if the arduino board prints gibberish on serial monitor. Close the serial monitor
// 3. Compile and run the host receiver code

//  dont compile this code with LTO optimizations. or it will likely crash while running on teensy
// https://forum.pjrc.com/index.php?threads/meaning-of-with-lto.60211/post-234163

#include "crc16.h"
#include "packet.hpp"

void setup()
{
  // baud rate does not matter for teensy 4.0 - Communication uses full USB 2.0 speeds
  Serial.begin(115200); 
}

static int packetID = 0;
void loop()
{
  Packet packet;
  packet.data.teensyID = 0x1234;
  packet.data.temperature = 25;
  packet.data.packetID = packetID++;
  CRC16 crc_calc;
  // make the new object to clear any previous memory
  crc_calc.processBuffer((uint8_t*) (&packet.data), packet.payloadLength);
  packet.crc =  crc_calc.getCrc();

  Serial.write( (uint8_t*)&packet, sizeof(packet) );

  delay(100);
}