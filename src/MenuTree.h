#include <Arduino.h>
#include <Vector.h>

class MenuTree{
  public:
    uint8_t DisplayPosition;
    String DisplayName;
    String NodeType;
    String NodeCommand;
    String NodeData;
    MenuTree* Parent;
    MenuTree* VecArray[16];
    Vector<MenuTree*> children;
    MenuTree(uint8_t pos,String display_name,String type,String command,String data,MenuTree* parent=NULL);
};