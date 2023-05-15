#include "Include.h"

String StoreResult = "";

void GetTreeString(MenuTree* Menu){
	
	for(int i = 0;i<Menu->children.size();i++){
		String line = "";
		line += String(Menu->children.at(i)->id);

		line += "\r\n";
		StoreResult += line;
		GetTreeString(Menu->children.at(i));
	}
}
