#define DEFAULT_FONT u8g2_simhei_20_chinese4 //默认字体，在修改后请去Include.h更改引用的头文件，可选项有：u8g2_simhei_18_chinese4，u8g2_simhei_20_chinese4，u8g2_simhei_24_chinese4
#define FORMAT_FFAT false //是否格式化FFAT分区，如果首次刷入出现FFAT无法读取问题请将这项打开刷入一次再关闭刷入
#define CONFIG_PATH "/config" //配置（菜单）文件路径
#define DEFAULT_COLOR WHITE//0x0561 //默认字体颜色

/*
字体相关文件可以自行设置，详情请参考U8G2的字体文件生成。
*/

//屏幕相关坐标，L_x是左列左侧位置，R_x是右列右侧位置。L1-8是每行文字中心位置。
#define L_x 10
#define R_x 310
#define L1 44
#define L2 100
#define L3 153
#define L4 210
#define L5 265
#define L6 323
#define L7 383
#define L8 438

#define VolPage 0