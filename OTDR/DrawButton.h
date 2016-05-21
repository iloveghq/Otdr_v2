#pragma once
#include "afxwin.h"
#include "PrintView.h"

class CDrawButton : public CButton
{
public:
	CDrawButton(void);
	~CDrawButton(void);
	void Draw(unsigned int nStep, unsigned int nCurveNumber);//供外部调用
	void SetDrawMode(BOOL bReportDraw); //设置绘制模式，TRUE绘制报表，FALSE绘制波形
	void Setlayout(BOOL bHorLayout); //设置打印排列方式
	void SetListFormat(int nListFormat); //0:A-1,1:A-2,2:A-3,3:B-1
	void ReDrawRect(int nStep/*当前步骤*/); //重绘矩形区
	void OutPutBmp();
	void SaveCurveToLocal(int nCurveIndex);//绘trace
	void DeleteBmpFiles(); //删除位图文件

private:
	void DrawReport(CDC* pDC);
	void Draw1Curve(CDC* pDC); //每页1个波形
	void Draw1CurveTitle(CDC* pDC); //画波形标题矩形
	void Draw2Curve(CDC* pDC); //每页2个波形
	void Draw2CurveTitle(CDC* pDC); //画波形标题矩形
	void Draw4Curve(CDC* pDC); //每页4个波形
	void Draw4CurveTitle(CDC* pDC); //画波形标题矩形
	void Draw6Curve(CDC* pDC); //每页6个波形
	void Draw6CurveTitle(CDC* pDC); //画波形标题矩形
	void Draw8Curve(CDC* pDC); //每页8个波形
	void Draw8CurveTitle(CDC* pDC); //画波形标题矩形
	void InnerDraw(CDC* pDC); //内部绘图只用
	void DrawCurveTitleRect(CDC* pDC); //绘制标题框
	void DrawLittlesCurveTitleRect(CDC* pDC); //绘制小标题框
	void DrawRptTitleRect(CDC* pDC); //绘制总表标题框
	void DrawReportFailRect(CDC* pDC); //绘制总表合格不合格框
	void DrawReportRptRect(CDC* pDC); //绘制总表总结表框
	void DrawCurve(CDC* pDC, CRect& rect, int nCurveIndex); //绘制曲线

	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();

public:
	CRect m_rectDraw;//绘制矩形区域大小
	BOOL m_bReportDraw;//是否为绘制总表TRUE是，FALSE，绘制波形
	unsigned int m_nStep; //当前步骤
	unsigned int m_nCurveNumber; //每页波形个数
	unsigned int m_nListFormat; //列表格式
	BOOL m_bHorLayout; //打印布局方式，是否为横向
public:
	CPrintView m_PrintView;
};
