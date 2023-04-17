#include <Arduino.h>
#include <Adafruit_MCP23X17.h>
#include <Arduino_GFX_Library.h>

Adafruit_MCP23X17 mcp;
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

  //Arduino_DataBus *bus = new Arduino_ESP32PAR16(
  //    21 /* DC */,33 /* CS */, 16 /* WR */, 17 /* RD */,
  //    0 /* D0 */, 1 /* D1 */, 2 /* D2 */, 3 /* D3 */, 4 /* D4 */, 5 /* D5 */, 6 /* D6 */, 7 /* D7 */,
  //    8 /* D8 */, 9 /* D9 */, 10 /* D10 */, 11 /* D11 */, 12 /* D12 */, 13 /* D13 */, 14 /* D14 */, 15 /* D15 */);
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
}