#ifndef Include
#define Include

#include <Arduino.h>
#include "FS.h"
#include "FFat.h"
#include <Adafruit_MCP23X17.h>
#include <Arduino_GFX_Library.h>
#include <U8g2lib.h>
#include "USB.h"
#include "u8g2_simhei_20_chinese3.h"
#include "config.h"
#include <Vector.h>
#include "MenuTree.h"
#include <Base64.h>

#define TL_DATUM 0
#define TC_DATUM 1
#define TR_DATUM 2
#define CL_DATUM 3
#define CC_DATUM 4
#define CR_DATUM 5
#define BL_DATUM 6
#define BC_DATUM 7
#define BR_DATUM 8

extern Adafruit_MCP23X17 mcp;
extern USBCDC USBSerial;
extern Arduino_DataBus *bus;
extern Arduino_GFX *gfx;
extern MenuTree Menu;
extern Vector<MenuTree*>* CurrentLevelMenu;
extern Vector<MenuTree*> MenuList;
extern int16_t MenuPosition[8];
extern String StoreResult;
extern MenuTree* MenuListVec[100];
extern bool TimeOutFlag;
extern unsigned long LastBeat;
extern String SerialData;

#define INT_PIN 37
#define IIC_SDA 36
#define IIC_SCL 35

String Base64Encode(String str);
String Base64Decode(String str);

void drawString(String str,int16_t x, int16_t y,uint8_t s = 1,uint8_t DATUM = 0,uint16_t c = WHITE,const uint8_t *font = DEFAULT_FONT);

void DisplayMenu(Vector<MenuTree*>* CurrentLevelMenu);
void DisplayInitMenu();

void HandlePreviousMenu();
void HandleMainMenu();
void HandleButton(uint8_t btn);

void GenerateTree();
void StoreTree();
void GetTreeString(MenuTree* Menu);

void writeFile(fs::FS &fs, const char * path, const char * message);
String ReadConfigFromFile();
void WriteConfigToFile(String message);

void HandleSetMenu(String str);

void usbEventCallback(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data);
#endif