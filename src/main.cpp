#include "Include.h"

Arduino_DataBus *bus = new Arduino_ESP32S2PAR16(21/* DC */, 33 /* CS */, 16 /* WR */, 17 /* RD */);
Arduino_GFX *gfx = new Arduino_ILI9488(
  bus, 18 /* RST */, 0 /* rotation */, false /* IPS */);
//使用16位并口驱动IL9488
Adafruit_MCP23X17 mcp;//MCP23017扩展16个IO口
USBCDC USBSerial;//USB串口
MenuTree Menu(0,0,"","ROOT","","");//菜单树的根节点
Vector<MenuTree*>* CurrentLevelMenu;//保存指向当前所在屏幕的菜单的层级列表
MenuTree* MenuListVec[100];//这个Vector库需要使用静态数组存储
Vector<MenuTree*> MenuList(MenuListVec);//所有菜单的列表

int16_t MenuPosition[8] = {L1,L2,L3,L4,L5,L6,L7,L8};//将菜单位置转换为数组方便访问
volatile bool ButtonPressed = false;
bool TimeOutFlag = 0;
unsigned long LastBeat = 0;
String SerialData = "";

void ButtonInterrupt(){
  ButtonPressed = true;
  //在这里只赋值因为后续操作包含对屏幕的写入，导致中断时间过长内核错误
}

/*
初始化MCP23017，相关内容参考库文件示例
*/
void MCPButtonInit(){
  Wire.begin(IIC_SDA, IIC_SCL);
  while (!mcp.begin_I2C()) {}

  pinMode(INT_PIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(INT_PIN), ButtonInterrupt, FALLING);
  mcp.setupInterrupts(true, false, LOW);//可以设置将两个中断引脚合为一个，处理起来相对容易

  for(int i = 0;i<16;i++){
    mcp.pinMode(i, INPUT);
    mcp.setupInterruptPin(i, CHANGE);
  }
}


/*
初始化
*/
void setup() {
  Serial.begin(115200);
  MCPButtonInit();

  USBSerial.begin();
  USB.begin();
  USBSerial.onEvent(usbEventCallback);
  //初始化串口及USB串口
  if (FORMAT_FFAT) FFat.format();
  if(!FFat.begin()){
      Serial.println("FFat Mount Failed");
      return;
  }
  //初始化FFAT
  gfx->begin();
  gfx->fillScreen(BLACK);
  gfx->setUTF8Print(true);
  //初始化屏幕并启用UTF8
  MenuList.push_back(&Menu);//所有的节点需要父节点，将这个根节点推入列表

  从文件读入并初始化这棵树。
  GenerateTree();
  
  初始化显示首页菜单
  DisplayInitMenu();

  //Serial.println(ESP.getFreeHeap());
}

void loop() {
  //中断后该变量为true，处理相关动作
  if (ButtonPressed) {
    int InterruptPin = mcp.getLastInterruptPin();
    if(mcp.digitalRead(InterruptPin) == LOW){
      Serial.println(InterruptPin);
      if(InterruptPin==6){
        HandlePreviousMenu();//点击上一级菜单时
      }else if(InterruptPin==7){
        //预留位
      }else if(InterruptPin==14){
        HandleMainMenu();//点击主菜单时
      }else if(InterruptPin==15){
        //预留位
      }else{
        HandleButton(InterruptPin);//点击其他按键时
      }
    }
    ButtonPressed = false;//重置变量和扩展芯片的中断状态
    mcp.clearInterrupts();
  }

  /*串口数据的处理。在串口接收到中断后会将数据存入这个数组。
    在写这个部分时，我们认为在执行完上一次这部分到执行完下一次这部分中的时间，不会有两个命令输入，这段时间应远小于上位机发送的间隔。其他情况我认为基本不可能发生。
    单纯循环读取缓冲区有可能造成缓冲区溢出，这在测试时发生过。
    还有一种可能性是逐行发送，上位机也逐行发送，增加了数据之前的时间防止缓冲区溢出。由于代码改动较大并未实行
  */
  if(SerialData!=""){
    //SetMenu相关的数据由SetMenuStart开头，SetMenuEnd结尾。
    if(SerialData.indexOf("SetMenuStart") >= 0&&SerialData.indexOf("SetMenuEnd") >= 0){//当indexOf返回值为-1时代表没有查到相关数据，这里用于检测是否存在该字符串
      SerialData = SerialData.substring(SerialData.indexOf("SetMenuStart"), SerialData.indexOf("SetMenuEnd"));//我们只截取这个区间部分
      SerialData.replace("SetMenuStart", "");
      SerialData.replace("SetMenuEnd", "");//删除头尾标识符。由用户输入的部分此时被base64包裹
      
      SerialData.trim();
      HandleSetMenu(SerialData);//传递输入的数据
      SerialData = "";
      USBSerial.println("SetSuccessful");//清空串口接收字符串并返回值
    }else if(SerialData.indexOf("GetDeviceName") >= 0){
      USBSerial.println("DeskShortCut");//用于上位机自动识别设备
      SerialData = "";
    }else if(SerialData.indexOf("HeartBeat") >= 0){
      //上位机发送心跳包，如果处于超时（黑屏）状态就重新显示内容
      SerialData = "";
      LastBeat = millis();
      if(TimeOutFlag){
        digitalWrite(34,HIGH);
        TimeOutFlag = false;
        DisplayMenu(CurrentLevelMenu);
      }
    }
  }
  /*
  判断距离上次心跳包的时间
  当未接收到心跳包时认为上位机下线并关闭屏幕（写黑屏）
  */
  if(millis()-LastBeat>=5000&&(!TimeOutFlag)){
    Serial.println("TimeOutFlag");
    TimeOutFlag = true;
    gfx->fillScreen(BLACK);
    digitalWrite(34,LOW);
  }
}