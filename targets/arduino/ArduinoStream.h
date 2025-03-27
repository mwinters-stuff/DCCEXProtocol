#ifndef ARDUINOSTREAM_H
#define ARDUINOSTREAM_H
#include "../../src/DCCStream.h"
#include <Arduino.h>
#include <WiFi.h>

class ArduinoStream : public DCCExController::DCCStream {
  private:
    WiFiClient &client;
  public:
  
  ArduinoStream(WifiClient &client): client(client){

  }
   ~ArduinoStream() {}
  
   int available() { return client.available(); }
   int read() {return client.read();}
   size_t write(const uint8_t *buffer, size_t size){
    return client.write(buffer,size);
   };
   void flush() {return client.flush(); };
   void println(const char *format, ...){
    Serial.println(format, ...);
   };
   void print(const char *format, ...){
    Serial.print(format, ...);
   };
};

#endif // ARDUINOSTREAM_H