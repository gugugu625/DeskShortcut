#include "MenuTree.h"

MenuTree::MenuTree(uint8_t pos,String display_name,String type,String command,String data,MenuTree* parent) : DisplayPosition(pos),DisplayName(display_name),NodeType(type),NodeCommand(command),NodeData(data),Parent(parent){
  children.setStorage(VecArray);
}