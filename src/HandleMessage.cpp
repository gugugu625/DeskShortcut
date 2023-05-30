#include "Include.h"

void HandleSetMenu(String msg){
    WriteConfigToFile(msg);
    Menu.children.clear();
    MenuList.clear();
    Serial.println(MenuList.size());
    MenuList.push_back(&Menu);
    GenerateTree();
    DisplayInitMenu();
}