#include "Include.h"

void HandlePreviousMenu(){
	CurrentLevelMenu = &(CurrentLevelMenu->at(0)->Parent->Parent->children);
	DisplayMenu(CurrentLevelMenu);
}

void HandleMainMenu(){
	CurrentLevelMenu = &Menu.children;
	DisplayMenu(CurrentLevelMenu);
}

void HandleButton(uint8_t btn){
  for(int i = 0;i<CurrentLevelMenu->size();i++){
    if(CurrentLevelMenu->at(i)->DisplayPosition == btn){
			if(CurrentLevelMenu->at(i)->children.size()!=0){
				CurrentLevelMenu = &(CurrentLevelMenu->at(i)->children);
				DisplayMenu(CurrentLevelMenu);
			}
		}
  }
}