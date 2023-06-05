#include "Include.h"
/*
绘制文字，由于在库文件中并未找到直接绘制的相关内容，在下面函数中首先根据基准点信息计算左下（BL）坐标位置，之后设置游标绘制
T：Top
B：Bottom
C：Center
L： Left
R：Right
各种组合
*/
void drawString(String str,int16_t x, int16_t y,uint8_t s,uint8_t DATUM,uint16_t c,const uint8_t *font){
  gfx->setTextColor(c);
  gfx->setFont(font);
  gfx->setTextSize(s);
  int16_t mx = 0,my = 0;
  uint16_t w = 0,h = 0;
  gfx->getTextBounds(str,x,y,&mx,&my,&w,&h);//获取文字绘制的边界
  int16_t DisplayX = x,DisplayY = y;
  if(DATUM == TL_DATUM){
    DisplayX = x;
    DisplayY = y+h;
  }else if(DATUM == TC_DATUM){
    DisplayX = x-(w/2);
    DisplayY = y+h;
  }else if(DATUM == TR_DATUM){
    DisplayX = x-w;
    DisplayY = y+h;
  }else if(DATUM == CL_DATUM){
    DisplayX = x;
    DisplayY = y+(h/2);
  }else if(DATUM == CC_DATUM){
    DisplayX = x-(w/2);
    DisplayY = y+(h/2);
  }else if(DATUM == CR_DATUM){
    DisplayX = x-w;
    DisplayY = y+(h/2);
  }else if(DATUM == BL_DATUM){
    DisplayX = x;
    DisplayY = y;
  }else if(DATUM == BC_DATUM){
    DisplayX = x-(w/2);
    DisplayY = y;
  }else if(DATUM == BR_DATUM){
    DisplayX = x-w;
    DisplayY = y;
  }
  gfx->setCursor(DisplayX, DisplayY);
  gfx->print(str);
}