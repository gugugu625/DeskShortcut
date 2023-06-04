#include "Include.h"

String StoreResult = "";

void StoreTree(){
	GetTreeString(&Menu);
	WriteConfigToFile(StoreResult);
}

void ProcessNodeString(MenuTree* Node,String str,int paraPos){
	//Serial.print(str+" ");
	//Serial.println(paraPos);
	if(paraPos == 0){
		Node->id = str.toInt();
	}else if(paraPos == 1){
		Node->DisplayPosition = str.toInt();
	}else if(paraPos == 2){
		Node->DisplayName = Base64Decode(str);
	}else if(paraPos == 3){
		Node->NodeType = str;
	}else if(paraPos == 4){
		Node->NodeCommand = str;
	}else if(paraPos == 5){
		Node->NodeData = Base64Decode(str);
	}else if(paraPos == 6){
		for(int i = 0;i<MenuList.size();i++){
			if(String(MenuList.at(i)->id) == str){
				MenuList.at(i)->children.push_back(Node);
				Node->Parent = MenuList.at(i);
			}
		}
	}
}

MenuTree* GenerateNode(String line){
	//Serial.println(line);
	//Serial.println("");
	MenuTree* Node = new MenuTree(0,0,"","","","");
	int lastIndex = 0;
  int index = 0;
  int paraPos = 0;
  while (index != -1) {
    index = line.indexOf('/', lastIndex);
    if (index == -1) {
      ProcessNodeString(Node,line.substring(lastIndex),paraPos);
    } else {
      ProcessNodeString(Node,line.substring(lastIndex, index),paraPos);

      lastIndex = index + 1;
    }
		paraPos++;
  }
	//Serial.println(Node->toString());
	return Node;
}

void GenerateTree(){
	String str = ReadConfigFromFile();//"1/0/cd1/List///0/\r\n5/0/菜单5/List///1/\r\n6/1/菜单6/List///1/\r\n2/5/菜单2/List///0/\r\n3/8/菜单3/List///0/\r\n4/13/菜单4/List///0/";
	if(str == ""){
    return;
  }
	int startPos = 0;
  int endPos = str.indexOf(";");
  
  while (endPos != -1) {
    String line = str.substring(startPos, endPos);
	  line.trim();
    if(line!=""){
      MenuList.push_back(GenerateNode(line));
    }
    
    startPos = endPos + 1;
    endPos = str.indexOf(";", startPos);
  }
	String line = str.substring(startPos);
	line.trim();
	if(line!=""){
		MenuList.push_back(GenerateNode(line));
	}
	
}

void GetTreeString(MenuTree* Menu){
	
	for(int i = 0;i<Menu->children.size();i++){
		String line = "";
		MenuTree* p = Menu->children.at(i);
		line += String(p->id)+"/";
		line += String(p->DisplayPosition)+"/";
		line += Base64Encode(p->DisplayName)+"/";
		line += p->NodeType+"/";
		line += p->NodeCommand+"/";
		line += Base64Encode(p->NodeData)+"/";
		line += String(p->Parent->id)+"/";
		line += ";";
		StoreResult += line;
		GetTreeString(Menu->children.at(i));
	}
}
