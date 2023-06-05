#include "Include.h"

String StoreResult = "";

void StoreTree(){
	GetTreeString(&Menu);
	WriteConfigToFile(StoreResult);
}

//将每个节点的每个参数传入并根据位置判断
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
      //要生成子节点列表，由子节点查找父节点并加入父节点的子节点列表
			if(String(MenuList.at(i)->id) == str){
				MenuList.at(i)->children.push_back(Node);
				Node->Parent = MenuList.at(i);
			}
		}
	}
}

//传入每一行，将这一行每个以/分开的参数生成树节点并返回
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

//读取树文件，按;分行并生成树
void GenerateTree(){
	String str = ReadConfigFromFile();
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

//递归将树生成字符串并存储至变量
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
