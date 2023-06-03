#include "Include.h"

Arduino_DataBus *bus = new Arduino_ESP32S2PAR16(21/* DC */, 33 /* CS */, 16 /* WR */, 17 /* RD */);
Arduino_GFX *gfx = new Arduino_ILI9488(
  bus, 18 /* RST */, 0 /* rotation */, false /* IPS */);
Adafruit_MCP23X17 mcp;
USBCDC USBSerial;
MenuTree Menu(0,0,"","ROOT","","");
Vector<MenuTree*>* CurrentLevelMenu;
MenuTree* MenuListVec[100];
Vector<MenuTree*> MenuList(MenuListVec);

int16_t MenuPosition[8] = {L1,L2,L3,L4,L5,L6,L7,L8};
volatile bool ButtonPressed = false;
bool TimeOutFlag = 0;
unsigned long LastBeat = 0;

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

  MenuList.push_back(&Menu);

  /*MenuTree* cd1 = new MenuTree(1,0,"菜单1","List","","",&Menu);
  Menu.children.push_back(cd1);
  Menu.children.push_back(new MenuTree(2,5,"菜单2","List","","",&Menu));
  Menu.children.push_back(new MenuTree(3,8,"菜单3","List","","",&Menu));
  Menu.children.push_back(new MenuTree(4,13,"菜单4","List","","",&Menu));
  cd1->children.push_back(new MenuTree(5,0,"菜单5","List","","",cd1));
  cd1->children.push_back(new MenuTree(6,1,"菜单6","List","","",cd1));*/

  //writeFile(FFat, CONFIG_PATH, "1/0/cd1/List///0/\r\n5/0/菜单5/List///1/\r\n6/1/菜单6/List///1/\r\n2/5/菜单2/List///0/\r\n3/8/菜单3/List///0/\r\n4/13/菜单4/List///0/");
  GenerateTree();
  //StoreTree();
  DisplayInitMenu();

  Serial.println(ESP.getFreeHeap());
  //Serial.println(Base64Decode("5rWL6K+V"));
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

  /*while(Serial.available()){
    size_t l = Serial.available();
    uint8_t b[l];
    l = Serial.read(b, l);
    USBSerial.write(b, l);
  }*/
  String SerialData = "";
  while(USBSerial.available()){
    SerialData += char(USBSerial.read());
  }
  SerialData.trim();
  if(SerialData!=""){
    if(SerialData.startsWith("SetMenu")){
      
      SerialData.replace("SetMenu", "");
      HandleSetMenu(SerialData);
      USBSerial.println("SetSuccessful");
      //sleep(1);
      //ESP.restart();
    }else if(SerialData=="GetDeviceName"){
      USBSerial.println("DeskShortCut");
    }else if(SerialData == "HeartBeat"){
      LastBeat = millis();
      if(TimeOutFlag){
        digitalWrite(34,HIGH);
        TimeOutFlag = false;
        DisplayMenu(CurrentLevelMenu);
      }
    }
    Serial.println(SerialData);
  }
  if(millis()-LastBeat>=5000&&(!TimeOutFlag)){
    Serial.println("TimeOutFlag");
    TimeOutFlag = true;
    gfx->fillScreen(BLACK);
    digitalWrite(34,LOW);
  }
}