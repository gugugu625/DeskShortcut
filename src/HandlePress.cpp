#include "Include.h"

void HandlePreviousMenu(){
	if(CurrentLevelMenu->at(0)->Parent->Parent != NULL){
		CurrentLevelMenu = &(CurrentLevelMenu->at(0)->Parent->Parent->children);
		DisplayMenu(CurrentLevelMenu);
	}
}

void HandleMainMenu(){
	
	CurrentLevelMenu = &Menu.children;
	DisplayMenu(CurrentLevelMenu);
}

void HandleButton(uint8_t btn){
  for(int i = 0;i<CurrentLevelMenu->size();i++){
    if(CurrentLevelMenu->at(i)->DisplayPosition == btn){
      MenuTree* t = CurrentLevelMenu->at(i);
      
      if(t->NodeType=="List"){
        if(t->children.size()!=0){
          CurrentLevelMenu = &(t->children);
          DisplayMenu(CurrentLevelMenu);
        }
      }else if(t->NodeType=="Command"){
        if(t->NodeCommand=="OpenFile"){
          USBSerial.println(t->NodeCommand+t->NodeData);
        }
      }
	  }
  }
}