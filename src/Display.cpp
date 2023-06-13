#include "Include.h"

void DisplayInitMenu(){
  //一次显示的基本操作：将要显示的菜单列表保存在变量中，调用DisplayMenu显示。
  CurrentLevelMenu = &Menu.children;
  DisplayMenu(CurrentLevelMenu);
}


void DisplayMenu(Vector<MenuTree*>* CurrentLevelMenu){
  gfx->fillScreen(BLACK);
  drawString("上一级菜单",L_x,L8,1,CL_DATUM);
  drawString("主菜单",R_x,L8,1,CR_DATUM);
  drawString("音量",R_x,L7,1,CR_DATUM);
  //清屏后显示固定项目

  //显示父节点的名称
  if(CurrentLevelMenu->at(0)->Parent!=NULL){
    drawString(CurrentLevelMenu->at(0)->Parent->DisplayName,L_x,L7,1,CL_DATUM);
  }

  //循环所有的列表
  for(int i = 0;i<CurrentLevelMenu->size();i++){
    uint16_t XPos = 0;
    uint16_t YPos = 0;
    uint8_t datum = 0;
    //读取并赋值相关的位置信息，DATUM是基准点位置，见DisplayFunction.cpp
    if(CurrentLevelMenu->at(i)->DisplayPosition < 8){
      XPos = L_x;
      YPos = MenuPosition[CurrentLevelMenu->at(i)->DisplayPosition];
      datum = CL_DATUM;
    }else{
      XPos = R_x;
      YPos = MenuPosition[CurrentLevelMenu->at(i)->DisplayPosition-8];
      datum = CR_DATUM;
    }
    drawString(CurrentLevelMenu->at(i)->DisplayName,XPos,YPos,1,datum);
  }
}

void DisplaySpecialPage(uint8_t PageNumber){
  InSpecialPages = true;
  gfx->fillScreen(BLACK);
  drawString("上一级菜单",L_x,L8,1,CL_DATUM);
  drawString("主菜单",R_x,L8,1,CR_DATUM);
  drawString("音量",R_x,L7,1,CR_DATUM);
  drawString(CurrentLevelMenu->at(0)->Parent->DisplayName,L_x,L7,1,CL_DATUM);
  SpecialPageNumber = PageNumber;
  /*if(timerRefreshVolume){
    timerEnd(timerRefreshVolume);
    timerRefreshVolume = NULL;
  }*/
  

  if(PageNumber == VolPage){
    //drawString("CESHI 测试",160,240,1,CC_DATUM);
    drawString("-1",L_x,L1,1,CL_DATUM);
    drawString("-10",L_x,L2,1,CL_DATUM);
    drawString("静音",L_x,L3,1,CL_DATUM);
    drawString("+1",R_x,L1,1,CR_DATUM);
    drawString("+10",R_x,L2,1,CR_DATUM);
    drawString("取消静音",R_x,L3,1,CR_DATUM);
  }else{
    drawString("ERROR PAGE",160,240,1,CC_DATUM);
  }
}