#include "Include.h"

void DisplayMenu(Vector<MenuTree*>* CurrentLevelMenu){
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