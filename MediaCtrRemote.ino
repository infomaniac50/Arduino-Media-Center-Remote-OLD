#include <SPI.h>
#include <Ethernet.h>
#include <EthernetUdp.h>


byte mac[] = { 0x90, 0xA2, 0xDA, 0x00, 0x3F, 0x5A };
IPAddress ip(192, 168, 2, 88);
const unsigned int socket_port = 2001;

EthernetUDP socket;

void setup()
{
  Ethernet.begin(mac, ip);
  udp.begin(socket_port);
}

void loop()
{
  int packet_size = udp.parsePacket();

  if (packet_size)
  {
    //Do stuff here
  }

  // Wait a bit before parsing the next packet
  delay(10);
}