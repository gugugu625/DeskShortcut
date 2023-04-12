#include <Arduino.h>
#include <Adafruit_MCP23X17.h>

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