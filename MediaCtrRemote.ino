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
#ifndef DEBUG_SERIAL
IRsend ir_send;
#endif

// struct IrCode
// {
//   int type;
//   int nbits;
//   unsigned long ex1;
//   unsigned long ex2;
//   unsigned long long value;
// };

IrCode packet;
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

    if (packet_size == sizeof(IrCode))
    {
      Udp.read(rawUdp, sizeof(IrCode));

      switch (packet.type) {
      case NEC:
#ifdef DEBUG_SERIAL
        Serial.println(F("NEC"));
#else
        ir_send.sendNEC(packet);
#endif
        break;
      case SONY:
#ifdef DEBUG_SERIAL
        Serial.println(F("SONY"));
#else
        ir_send.sendSony(packet);
#endif
        break;
      case RC5:
#ifdef DEBUG_SERIAL
        Serial.println(F("RC5"));
#else
        ir_send.sendRC5(packet);
#endif
        break;
      case RC6:
#ifdef DEBUG_SERIAL
        Serial.println(F("RC6"));
#else
        ir_send.sendRC6(packet);
#endif
        break;
      case DISH:
#ifdef DEBUG_SERIAL
        Serial.println(F("DISH"));
#else
        ir_send.sendDISH(packet);
#endif
        break;
      case SHARP:
#ifdef DEBUG_SERIAL
        Serial.println(F("SHARP"));
#else
        ir_send.sendSharp(packet);
#endif
        break;
      case PANASONIC:
#ifdef DEBUG_SERIAL
        Serial.println(F("PANASONIC"));
#else
        ir_send.sendPanasonic(packet);
#endif
        break;
      case JVC:
#ifdef DEBUG_SERIAL
        Serial.println(F("JVC"));
#else
        ir_send.sendJVC(packet);
#endif
        break;
      default:
        break;
      }

#ifdef DEBUG_SERIAL
      printBigInt(&Serial, packet.value);
#endif

    }
  }

  // Wait a bit before parsing the next packet
  delay(10);
}


