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
String SerialData = "";

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
  USBSerial.onEvent(usbEventCallback);
  
  if (FORMAT_FFAT) FFat.format();
  if(!FFat.begin()){
      Serial.println("FFat Mount Failed");
      return;
  }
  gfx->begin();
  gfx->fillScreen(BLACK);
  gfx->setUTF8Print(true);
  MenuList.push_back(&Menu);

  //writeFile(FFat,CONFIG_PATH,"4/0/6LWE5rqQ566h55CG5Zmo/Command/OpenFile/RXhwbG9yZXI=/0/;14/1/SG9tZUFzc2lzdGFudCAgICA=/Command/OpenFile/aHR0cDovLzE5Mi4xNjguMS4xOTg6ODEyMw==/0/;15/2/S2xpcHBlcg==/Command/OpenFile/aHR0cDovLzE5Mi4xNjguMS4xOTc=/0/;16/3/UHJvamVjdA==/List///0/;17/0/RGVza1Nob3J0Q3V0/List///16/;18/0/5LiK5L2N5py65bel56iL/Command/OpenFile/RjpcbW95dVxEZXNrU2hvcnRDdXRNYXN0ZXI=/17/;19/1/5LiL5L2N5py65bel56iL/Command/OpenFile/RjpcbW95dVxEZXNrU2hvcnRjdXQ=/17/;20/4/572R5Z2A/List///0/;21/0/VGhpbmdpdmVyc2U=/Command/OpenFile/aHR0cHM6Ly93d3cudGhpbmdpdmVyc2UuY29tLw==/20/;22/1/U2FrdXJhRnJw/Command/OpenFile/aHR0cHM6Ly93d3cubmF0ZnJwLmNvbS8=/20/;23/2/5p6c55qu57OW/Command/OpenFile/aHR0cHM6Ly93d3cuZ3VvcGl0YW5nLmNvbS8=/20/;24/3/RjE05p2w5ZOl5Lit5paH6I+c5Y2V/Command/OpenFile//20/");
  GenerateTree();
  //StoreTree();
  DisplayInitMenu();

  Serial.println(ESP.getFreeHeap());
  //Serial.println(Base64Decode("aHR0cHM6Ly9hbHBoYWJldC1naG9zdC5naXRlZS5pby9qZXN0ZXItcmFkaWFsLW1lbnUtdnVl"));
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

  
  if(SerialData!=""){
    if(SerialData.indexOf("SetMenuStart") >= 0&&SerialData.indexOf("SetMenuEnd") >= 0){
      SerialData = SerialData.substring(SerialData.indexOf("SetMenuStart"), SerialData.indexOf("SetMenuEnd"));
      SerialData.replace("SetMenuStart", "");
      SerialData.replace("SetMenuEnd", "");
      
      SerialData.trim();
      HandleSetMenu(SerialData);
      SerialData = "";
      USBSerial.println("SetSuccessful");
      //sleep(1);
      //ESP.restart();
    }else if(SerialData.indexOf("GetDeviceName") >= 0){
      USBSerial.println("DeskShortCut");
      SerialData = "";
    }else if(SerialData.indexOf("HeartBeat") >= 0){
      SerialData = "";
      LastBeat = millis();
      if(TimeOutFlag){
        digitalWrite(34,HIGH);
        TimeOutFlag = false;
        DisplayMenu(CurrentLevelMenu);
      }
    }
    //Serial.println(SerialData);
  }
  /*while(Serial.available()){
    size_t l = Serial.available();
    uint8_t b[l];
    l = Serial.read(b, l);
    USBSerial.write(b, l);
  }*/
  if(millis()-LastBeat>=5000&&(!TimeOutFlag)){
    Serial.println("TimeOutFlag");
    TimeOutFlag = true;
    gfx->fillScreen(BLACK);
    digitalWrite(34,LOW);
  }
}