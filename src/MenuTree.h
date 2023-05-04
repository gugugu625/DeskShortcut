#include <Arduino.h>
#include <Vector.h>

class MenuTree{
    public:
        String NodeType;
        String NodeCommand;
        String NodeData;
        Vector<MenuTree*> children;
        MenuTree(String type,String command,String data) : NodeType(type),NodeCommand(command),NodeData(data){}
};