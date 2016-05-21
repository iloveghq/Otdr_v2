#pragma once

#include "Xml.h"
#include "ExcelUtil.h"
#include "GrandwayProgress.h"

// CPropStep6 对话框
class CSpreadSheet;
class CPropStep6 : public CPropertyPage
{
	DECLARE_DYNAMIC(CPropStep6)

public:
	CPropStep6();
	virtual ~CPropStep6();
	void SetLocalString();
	static UINT ExportXlsThread(LPVOID lparam);
	void ExportToExcel();
	BOOL ExportXlsByRpt();
	int GetSheetIndex();
	void WriteToExcel(CSpreadSheet* excel);
	void WriteToExcel(const CString& fileName, int nSheetIndex,int SheetCount);
public:
// 对话框数据
	enum { IDD = IDD_PROP_STEP6 };

public:
	virtual BOOL OnSetActive();
	virtual BOOL OnWizardFinish();
	virtual LRESULT OnWizardBack();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnStep6Preview();
	afx_msg void OnBnClickedBtnStep6Setting();
	afx_msg void OnBnClickedBtnStep6Print();
	afx_msg void OnBnClickedBtnStep6Excel();

private:
	void ExportReport(); //总表
	void Export1Curve(); //每页1个波形
	void Export2Curve(); //每页2个波形
	void Export4Curve(); //每页4个波形
	void Export6Curve(); //每页6个波形
	void Export8Curve(); //每页8个波形
	void WriteDataToExcel(int nWaveNumber);//zll 2013.5.24 晚
	void WriteDataToExcel(int nWaveNumber, BOOL bNewRpt);//lzy 2014.8.22
	void WriteEventList(int m_nWaveNumberPerPage,int m_nCurCurve, int nRow = 33);//zll 2013.5.27 导出excel，每页一条曲线时显示事件列表主体
	void WriteEventHead(int m_nWaveNumberPerPage,int m_nCurCurve, int nRow = 32);//zll 2013.5.27 晚 显示事件列表表头
	void SetHorUnitText(int m_nWaveNumberPerPage,int m_nCurCurve); //zll 2013.6.11 设置横坐标刻度文本
	void SetVerUnitText(int m_nWaveNumberPerPage,int m_nCurCurve); //zll 2013.6.11 设置纵坐标刻度文本
	void GetHorLocation(int m_nWaveNumberPerPage,int m_nCurCurve,CString m_strStartDis,CString m_strUnitDis,CString m_strEndDis);//zll 2013.6.11 得到横坐标位置并赋刻度文本
	void SetSorName(int m_nWaveNumberPerPage,int m_nCurCurve,CString m_sorName);//zll 2013.6.11 设置.sor曲线文件名

private:
	CGrandwayProgress m_progress; //进度条
	CWinThread* m_pThread;
	CXml m_xml;
	CString m_strXmlFile;
	CExcelUtil m_ExcelUtil;

public:
	CString m_strStartDis; //横坐标开始值
	CString m_strEndDis; //横坐标结束值
	CString m_strUnitDis; //横坐标单位长度

	CString m_strUnitDiv; //纵坐标单位值
};
