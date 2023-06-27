#include "Include.h"

/*
处理按下上一级菜单时的动作
*/
void HandlePreviousMenu(){
  
	if(CurrentLevelMenu->at(0)->Parent->Parent != NULL){//要获取父节点整个一层的数据，需要访问父节点的父节点的子节点列表
		CurrentLevelMenu = &(CurrentLevelMenu->at(0)->Parent->Parent->children);
		DisplayMenu(CurrentLevelMenu);
	}
}
void SpecialPagePreviousMenu(){
  InSpecialPages = false;
  DisplayMenu(CurrentLevelMenu);
}
/*
处理回到主菜单
*/
void HandleMainMenu(){
  InSpecialPages = false;
	//直接显示根节点的子节点
	CurrentLevelMenu = &Menu.children;
	DisplayMenu(CurrentLevelMenu);
}

/*
处理按下一般按钮时
*/
void HandleButton(uint8_t btn){
  for(int i = 0;i<CurrentLevelMenu->size();i++){
    if(CurrentLevelMenu->at(i)->DisplayPosition == btn){
      //如果显示位置与按下按钮编号相同则为对应按钮
      MenuTree* t = CurrentLevelMenu->at(i);
      //判断节点类型。当类型为List时只做跳转，当类型为Command时判断Command
      if(t->NodeType=="List"){
        if(t->children.size()!=0){
          CurrentLevelMenu = &(t->children);
          DisplayMenu(CurrentLevelMenu);
        }
      }else if(t->NodeType=="Command"){
        if(t->NodeCommand=="OpenFile"){
          //直接向上位机发送动作类型与数据
          USBSerial.println(t->NodeCommand+Base64Encode(t->NodeData));
        }
      }
	  }
  }
}

void HandleSpecialPageButton(uint8_t btn){
  if(SpecialPageNumber==VolPage){
    if(btn==0){
      USBSerial.println("DecreaseVolume1");
    }else if(btn==1){
      USBSerial.println("DecreaseVolume10");
    }else if(btn==2){
      USBSerial.println("Mute");
    }else if(btn==8){
      USBSerial.println("IncreaseVolume1");
    }else if(btn==9){
      USBSerial.println("IncreaseVolume10");
    }else if(btn==10){
      USBSerial.println("CancelMute");
    }
    USBSerial.println("GetVolume");
  }

}