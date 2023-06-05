#include "MenuTree.h"

MenuTree::MenuTree(uint id,uint8_t pos,String display_name,String type,String command,String data,MenuTree* parent) : id(id),DisplayPosition(pos),DisplayName(display_name),NodeType(type),NodeCommand(command),NodeData(data),Parent(parent){
  children.setStorage(VecArray);//初始化Vector
}

/*
输出Node为字符串
*/
String MenuTree::toString(){
	String line = "";
	line += String(this->id)+" ";
	line += String(this->DisplayPosition)+" ";
	line += this->DisplayName+" ";
	line += this->NodeType+" ";
	line += this->NodeCommand+" ";
	line += this->NodeData+" ";
	if(this->Parent!=NULL){
		line += String(this->Parent->id)+" ";
	}else{
		line += "NULL ";
	}
	return line;
}