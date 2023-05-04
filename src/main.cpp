#include "Include.h"

Arduino_DataBus *bus = new Arduino_ESP32S2PAR16(21/* DC */, 33 /* CS */, 16 /* WR */, 17 /* RD */);
Arduino_GFX *gfx = new Arduino_ILI9488(
  bus, 18 /* RST */, 0 /* rotation */, false /* IPS */);
Adafruit_MCP23X17 mcp;
USBCDC USBSerial;
MenuTree Menu("ROOT","","");
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
    mcp.pinMode(i, INPUT);
    mcp.setupInterruptPin(i, CHANGE);
  }
}



void setup() {
  Serial.begin(115200);
  MCPButtonInit();

  USBSerial.begin();
  USB.begin();

  if (FORMAT_FFAT) FFat.format();
  if(!FFat.begin()){
      Serial.println("FFat Mount Failed");
      return;
  }
  gfx->begin();
  gfx->fillScreen(BLACK);
  gfx->setUTF8Print(true);

  
  drawString("菜单1",L_x,L1,1,CL_DATUM);
  drawString("菜单2",L_x,L2,1,CL_DATUM);
  drawString("菜单3",L_x,L3,1,CL_DATUM);
  drawString("菜单4",L_x,L4,1,CL_DATUM);
  drawString("菜单5",L_x,L5,1,CL_DATUM);
  drawString("菜单6",L_x,L6,1,CL_DATUM);
  drawString("菜单7",L_x,L7,1,CL_DATUM);
  drawString("菜单8",L_x,L8,1,CL_DATUM);
  drawString("菜单9",R_x,L1,1,CR_DATUM);
  drawString("菜单10",R_x,L2,1,CR_DATUM);
  drawString("菜单11",R_x,L3,1,CR_DATUM);
  drawString("菜单12",R_x,L4,1,CR_DATUM);
  drawString("菜单13",R_x,L5,1,CR_DATUM);
  drawString("菜单14",R_x,L6,1,CR_DATUM);
  drawString("菜单15",R_x,L7,1,CR_DATUM);
  drawString("菜单16",R_x,L8,1,CR_DATUM);
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