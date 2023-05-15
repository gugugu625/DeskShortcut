#include "Include.h"

Arduino_DataBus *bus = new Arduino_ESP32S2PAR16(21/* DC */, 33 /* CS */, 16 /* WR */, 17 /* RD */);
Arduino_GFX *gfx = new Arduino_ILI9488(
  bus, 18 /* RST */, 0 /* rotation */, false /* IPS */);
Adafruit_MCP23X17 mcp;
USBCDC USBSerial;
MenuTree Menu(0,"","ROOT","","");
Vector<MenuTree*>* CurrentLevelMenu;
int16_t MenuPosition[8] = {L1,L2,L3,L4,L5,L6,L7,L8};
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

  MenuTree* cd1 = new MenuTree(0,"菜单1","List","","",&Menu);
  Menu.children.push_back(cd1);
  Menu.children.push_back(new MenuTree(5,"菜单2","List","","",&Menu));
  Menu.children.push_back(new MenuTree(8,"菜单3","List","","",&Menu));
  Menu.children.push_back(new MenuTree(13,"菜单4","List","","",&Menu));
  cd1->children.push_back(new MenuTree(0,"菜单5","List","","",cd1));
  cd1->children.push_back(new MenuTree(1,"菜单6","List","","",cd1));

  DisplayInitMenu();
}

void loop() {
  if (ButtonPressed) {
    int InterruptPin = mcp.getLastInterruptPin();
    if(mcp.digitalRead(InterruptPin) == LOW){
      Serial.println(InterruptPin);
      if(InterruptPin==6){
        HandlePreviousMenu();
      }else if(InterruptPin==7){

      }else if(InterruptPin==14){
        HandleMainMenu();
      }else if(InterruptPin==15){

      }else{
        HandleButton(InterruptPin);
      }
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