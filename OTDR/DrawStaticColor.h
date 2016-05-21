// DrawStaticColor.h : 头文件
//

#pragma once

class CDrawStaticColor : public CStatic
{
	DECLARE_DYNAMIC(CDrawStaticColor)

public:

	CDrawStaticColor();
	virtual ~CDrawStaticColor();

	void DrawBackGround(CDC *pDC); //绘制背景
	void DrawGridLine(CDC *pDC); //绘制网格线
	void DrawLine(CDC *pDC); //绘制AB线
	void DrawCursor(CDC *pDC); //绘制光标
	COLORREF GetCurveColor(int nIndex);

public:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();

public:
	CRect m_drawRect;
};
