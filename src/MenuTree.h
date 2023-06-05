#include <Arduino.h>
#include <Vector.h>

/*
树的基本节点
*/
class MenuTree{
  public:
    uint id;
    uint8_t DisplayPosition;
    String DisplayName;
    String NodeType;
    String NodeCommand;
    String NodeData;
    MenuTree* Parent;
    Vector<MenuTree*> children;
    MenuTree(uint id,uint8_t pos,String display_name,String type,String command,String data,MenuTree* parent=NULL);
    String toString();
  private:
    MenuTree* VecArray[16];
};
/*
id对于节点是唯一且递增的，由上位机生成。DisplayPosition用于指示显示位置，位置从0-15共16个。
整个菜单由一颗树存储，每个节点有指向其父节点的指针及其子节点的列表。
*/