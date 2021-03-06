

#include <StandardCplusplus.h>
#include <Controllino.h>

#define DBG_OUTPUT_PORT Serial
#define DEBUG_ESP_HTTP_SERVER
#define DEBUG_ESP_PORT Serial
#define UDP_PORT 8889
#define HOSTNAME "Controller1"
unsigned int localPort = UDP_PORT; // local port to listen on


#ifdef DEBUG_ESP_PORT
#define DEBUG_MSG(...) DEBUG_ESP_PORT.printf( __VA_ARGS__ )
#else
#define DEBUG_MSG(...) 
#endif

#include <Ethernet.h>
#include <EthernetClient.h>
#include <EthernetServer.h>
#include <EthernetUdp.h>
#include <EthernetWebServer.h> //https://github.com/burner-/EthernetWebServer
#include <avr/wdt.h>

//#include "syslog.h"
#include <LinkedList.h>
#include <EEPROM.h>
#include <ArduinoJson.h>
#include "rtc.h"
#include "ntp.h"
#include "syslog.h"
#include "global.h"
#include "config.h"


/*
#include <pb_encode.h>
#include <pb_decode.h>
#include <pb.h>
#include "communication.h"
#include "scada.h"

*/
#include "webserver.h"
#include "lights.h";
#include "motion.h";

//Test

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xAF, 0xFB, 0xE2 };
IPAddress ip(10, 220, 2, 23);
IPAddress ip_gw(10, 220, 0, 1);
IPAddress ip_dns(8, 8, 8, 8);
IPAddress ip_mask(255, 255, 0, 0);

IPAddress bcast(10, 220, 255, 255);
byte loghost[] = { 37, 16, 111, 10 };

EthernetUDP Udp;

void setup() {
  //DBG_OUTPUT_PORT
  DBG_OUTPUT_PORT.begin(115200);
  DBG_OUTPUT_PORT.println(F("Initialize ver 2 ...")); 
  loadConfig();
  Ethernet.begin(mac,ip,ip_dns,ip_gw,ip_mask);
  Udp.begin(localPort);
  Syslog.setLoghost(loghost,Udp);
  
  InitWebserver();
  initLights();
  initMotionInputs();
  Controllino_RTC_init(0);
  
  getNTPTime();
  
}

void loop() {
    server.handleClient();
    handleLights();
    readMotionSensors();
    printRTCTime();
    delay(1000);
}
