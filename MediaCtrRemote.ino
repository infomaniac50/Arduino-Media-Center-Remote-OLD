#include <SPI.h>
#include <Ethernet.h>
#include <EthernetUdp.h>
#include <IRremote.h>
#include <util.h>

// #define DEBUG_SERIAL

#define PORT 8888
#define NODHCP

byte mac[] = { 0x90, 0xA2, 0xDA, 0x00, 0x3F, 0x5A };

#ifdef NODHCP
IPAddress ip(192, 168, 2, 88);
#endif

EthernetUDP Udp;
IRsend ir_send;

struct IrCode {
  unsigned long long value;
};

union IrPacket {
  IrCode code;
  char udp[sizeof(IrCode)];
} packet;


void setup()
{
  pinMode(SS, OUTPUT);

#ifdef NODHCP  
  Ethernet.begin(mac, ip);
#else
  if (Ethernet.begin(mac) == 0)
  {
    while(true);
  }
#endif

#ifdef DEBUG_SERIAL
  Serial.begin(9600);

  while (Serial.available() <= 0 && Serial.read() != 'g' && Serial.read() != 'o')
    delay(100);

  Serial.println("Go Code Received");
#endif

  Udp.begin(PORT);
}

void loop()
{
  int packet_size = Udp.parsePacket();
  if (packet_size) {

  #ifdef DEBUG_SERIAL
    Serial.print(F("Got packet of size: "));
    Serial.print(packet_size);
    Serial.println(F(" bytes"));
  #endif

    if (packet_size == sizeof(IrPacket))
    {
      Udp.read(packet.udp, packet_size);

      ir_send.sendNEC((unsigned long)packet.code.value, 32);
  #ifdef DEBUG_SERIAL
      printBigInt(&Serial, packet.code.value);
      delay(10);
  #endif
    }
  }

  // Wait a bit before parsing the next packet
  delay(10);
}


