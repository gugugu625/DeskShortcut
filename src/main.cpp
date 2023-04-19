#include <Arduino.h>
#include <Adafruit_MCP23X17.h>
#include <Arduino_GFX_Library.h>
#include "USB.h"

Adafruit_MCP23X17 mcp;
USBCDC USBSerial;
#define INT_PIN 37
#define IIC_SDA 36
#define IIC_SCL 35
volatile bool ButtonPressed = false;

void ButtonInterrupt(){
  ButtonPressed = true;
}


void MCPButtonInit(){
  Wire.begin(IIC_SDA, IIC_SCL);
  while (!mcp.begin_I2C()) {}

  pinMode(INT_PIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(INT_PIN), ButtonInterrupt, FALLING);
  mcp.setupInterrupts(true, false, LOW);

  for(int i = 0;i<16;i++){
    //Serial.println(i);
    mcp.pinMode(i, INPUT);
    mcp.setupInterruptPin(i, CHANGE);
  }
}

void setup() {
  Serial.begin(115200);
  MCPButtonInit();

  USBSerial.begin();
  USB.begin();

  Arduino_DataBus *bus = new Arduino_ESP32S2PAR16(21/* DC */, 33 /* CS */, 16 /* WR */, 17 /* RD */);
  Arduino_GFX *gfx = new Arduino_ILI9488(
    bus, 18 /* RST */, 0 /* rotation */, false /* IPS */);
  gfx->begin();
  gfx->println(F("Arduino GFX Test"));
}

void loop() {
  if (ButtonPressed) {
    int InterruptPin = mcp.getLastInterruptPin();
    if(mcp.digitalRead(InterruptPin) == LOW){
      Serial.println(InterruptPin);
    }
    ButtonPressed = false;
    mcp.clearInterrupts();
  }

  while(Serial.available()){
    size_t l = Serial.available();
    uint8_t b[l];
    l = Serial.read(b, l);
    USBSerial.write(b, l);
  }

  /*while(USBSerial.available()){
    size_t l = USBSerial.available();
    uint8_t b[l];
    l = USBSerial.read(b, l);
    Serial.write(b, l);
  }*/
}