#pragma once

struct WaveConfig 
{
	COLORREF ColorCurve1;  //曲线1颜色
	COLORREF ColorCurve2;  //曲线2颜色
	COLORREF ColorCurve3;  //曲线3颜色
	COLORREF ColorCurve4;  //曲线4颜色
	COLORREF ColorCurve5;  //曲线5颜色
	COLORREF ColorCurve6;  //曲线6颜色
	COLORREF ColorCurve7;  //曲线7颜色
	COLORREF ColorCurve8;  //曲线8颜色
	COLORREF ColorBack;  //背景颜色
	COLORREF ColorGrid;  //栅格颜色
	COLORREF ColorEvent;  //事件颜色
	COLORREF ColorCursor;  //光标颜色
	BOOL ShowGrid; //是否显示网格
	BOOL ShowCursor; //是否显示光标
	BOOL ShowDoubleCursor; //是否显示双光标
	BOOL MultiMode; //是否为多曲线显示模式
	//Unit Page
	int UnitDbFig; //Dt刻度单位精度5\4\3
	int UnitDistFig; //距离刻度单位精度
	CString DistUnit; //距离单位
};