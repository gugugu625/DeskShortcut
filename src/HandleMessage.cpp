#include "Include.h"

/*
处理设置菜单时的相关操作
*/
void HandleSetMenu(String msg){
    WriteConfigToFile(msg);//首先要向文件系统写入文件
    Menu.children.clear();
    MenuList.clear();//清空相关数组，保留根节点
    //Serial.println(msg);
    MenuList.push_back(&Menu);//清空之后仍需推入根节点
    GenerateTree();//直接从文件系统生成树
    DisplayInitMenu();//显示菜单
}