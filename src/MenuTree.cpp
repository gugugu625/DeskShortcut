#include "MenuTree.h"

MenuTree::MenuTree(uint id,uint8_t pos,String display_name,String type,String command,String data,MenuTree* parent) : id(id),DisplayPosition(pos),DisplayName(display_name),NodeType(type),NodeCommand(command),NodeData(data),Parent(parent){
  children.setStorage(VecArray);
}