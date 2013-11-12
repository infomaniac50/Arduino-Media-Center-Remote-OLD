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

struct IrPacket {
  int type;
  IrCode code;
};

IrPacket packet;
char * rawUdp;
void setup()
{
  rawUdp = (char*)(&packet);

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
  size_t packet_size = Udp.parsePacket();
  if (packet_size) {

    #ifdef DEBUG_SERIAL
    Serial.print(F("Got packet of size: "));
    Serial.print(packet_size);
    Serial.println(F(" bytes"));
    #endif

    if (packet_size == sizeof(IrPacket))
    {
      Udp.read(rawUdp, sizeof(IrPacket));

      switch (packet.type) {
          case NEC:
            ir_send.sendNEC(packet.code);
            break;
          case SONY:
            ir_send.sendSony(packet.code);
            break;
          case RC5:
            ir_send.sendRC5(packet.code);
            break;
          case RC6:
            ir_send.sendRC6(packet.code);
            break;
          case DISH:
            ir_send.sendDISH(packet.code);
            break;
          case SHARP:
            ir_send.sendSharp(packet.code);
            break;
          case PANASONIC:
            ir_send.sendPanasonic(packet.code);
            break;
          case JVC:
            ir_send.sendJVC(packet.code);
            break;
          default: 
            break;
      }

      #ifdef DEBUG_SERIAL
      Serial.println(packet.type);
      printBigInt(&Serial, packet.value);
      #endif
    
    }
  }

  // Wait a bit before parsing the next packet
  delay(10);
}


