#pragma once

//GroupBox样式1
//#define CLR_GBX_BG_T1 RGB(205,221,234)//背景//OTDR用的这个
#define CLR_GBX_BG_T1 RGB(58,127,182)//背景//OTDR用的这个wcq2012.02.13
//GroupBox样式2
#define CLR_GBX_BG_T2 RGB(236,245,254)//背景Groupbox的背景色用的这个
//GroupBox选中样式
#define CLR_GBX_BG_SELECTED RGB(255,0,0)//背景

//主界面背景
#define CLR_MAIN_BG RGB(153,183,207)//
//其它界面，如系统设置界面 背景
#define CLR_OTHER_SYSTEM_BG CLR_GBX_BG_T1//wcq2012.02.28

//界面字体颜色
//#define CLR_TEXT_T1 RGB(255,255,255)//字体颜色1
#define CLR_TEXT_T1 RGB(45,107,164)//字体颜色1 wcq2012.02.09
#define CLR_TEXT_T2 RGB(0,0,0)//字体颜色2

//状态栏
#define CLR_BAR_BG RGB(45,107,164)//背景

//按钮
#define CLR_BTN_BG_T1 RGB(154,186,211)//背景1
#define CLR_BTN_BG_T2 RGB(0,130,191)//背景2
#define CLR_BTN_BG_FOCUS RGB(154,186,211)

//wcq2011.12.08
//OTDR绘图相关颜色
#define CLR_RED            RGB(255,0,0)//通用，红色
#define CLR_GREEN            RGB(0,255,0)//通用，绿色
#define CLR_BLUE            RGB(0,0,255)//通用，蓝色
#define CLR_WHITE            RGB(255,255,255)//通用，白色
#define CLR_YELLOW            RGB(255,255,0)//通用，白色
#define CLR_OTDR_CURVE_1310 CLR_GREEN//1310曲线
#define CLR_OTDR_CURVE_1550 RGB(255,128,0)//1550曲线
#define CLR_OTDR_CURVE_2000 RGB(2,253,234)//从文件加载的曲线
#define CLR_OTDR_CURVE_SELECT CLR_YELLOW//绿色 选中曲线，前面三个都有可能选中
#define CLR_OTDR_CURVE_EVENT_LINE CLR_RED//事件点直线
#define CLR_OTDR_CURVE_EVENT_LINE_NUM RGB(128,64,64)//事件点直线下面表示编号的矩形框
#define CLR_OTDR_CURVE_TRANSPARENT  RGB(0,0,0)//绘制曲线时的透明色wcq2011.12.13
#define CLR_OTDR_GRID       RGB(70,70,70)//背景Gridline
#define CLR_OTDR_Coord      RGB(0,0,0)//矩形框
#define CLR_OTDR_BACKGROUND RGB(0,0,0)//背景
#define CLR_OTDR_TESTING_INFO CLR_RED//RGB(128,128,192)//测试过程中一些相关提示文字，如：有光检测 参数配置 连接状态 正在测试中... 
#define CLR_OTDR_THUMB_ZOOM   CLR_RED//缩略图中间变化的矩形框
#define CLR_OTDR_RESULT_INFO RGB(0,128,0)//RGB(255,255,128)//参数显示,如B-A:33km
#define CLR_OTDR_RESULT_INFO_A   RGB(255,0,0)//A距离 DB wcq2012.03.08
#define CLR_OTDR_RESULT_INFO_B   RGB(0,0,255)//B距离 DB wcq2012.03.08
#define CLR_OTDR_RESULT_INFO_XY   RGB(0,255,0)//xy div DB wcq2012.03.08
#define CLR_OTDR_RESULT_INFO_PW   RGB(0,111,0)//脉宽 wcq2012.03.08
#define CLR_OTDR_RESULT_INFO_WL   RGB(0,77,0)//波长 wcq2012.03.08
