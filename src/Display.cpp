#include "Include.h"


void DisplayInitMenu(){
  CurrentLevelMenu = &Menu.children;
  DisplayMenu(CurrentLevelMenu);
}


void DisplayMenu(Vector<MenuTree*>* CurrentLevelMenu){
  gfx->fillScreen(BLACK);
  drawString("上一级菜单",L_x,L7,1,CL_DATUM);
  drawString("主菜单",R_x,L7,1,CR_DATUM);
  drawString("时间",R_x,L8,1,CR_DATUM);
  if(CurrentLevelMenu->at(0)->Parent!=NULL){
    drawString(CurrentLevelMenu->at(0)->Parent->DisplayName,L_x,L8,1,CL_DATUM);
  }
  for(int i = 0;i<CurrentLevelMenu->size();i++){
    uint16_t XPos = 0;
    uint16_t YPos = 0;
    uint8_t datum = 0;
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