#pragma once

extern SorFileArray g_sorFileArray;
// CPropConfig 对话框

class CPropConfig : public CPropertyPage
{
	DECLARE_DYNAMIC(CPropConfig)

public:
	CPropConfig();
	virtual ~CPropConfig();

// 对话框数据
	enum { IDD = IDD_PROP_STEP2_CONFIG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	// 起点位置，EDIT控件变量
	CString m_strStartLoc;
	// 起点光缆号
	CString m_strStartCable;
	// 起点光纤号
	CString m_strStartFiber;
	// 起点光纤颜色
	CString m_strStartColor;
	// 终点位置，EDIT控件变量
	CString m_strEndLoc;
	// 终点光缆号
	CString m_strEndCable;
	// 终点光纤号
	CString m_strEndFiber;
	// 终点光纤颜色
	CString m_strEndColor;
	// 波长
	CString m_strWavelength;
	// 折射率
	CString m_strIndex;
	// 脉宽
	CString m_strPulse;
	// 解析度
	CString m_strResolution;
	// 背向散射因子
	CString m_strRBS;
	// 范围
	CString m_strRange;
	// 衰减门限
	CString m_strLossThreshold;
	// 反射门限
	CString m_strRefThreshold;
	// 光纤末端门限
	CString m_strEndThreshold;
	//平均次数
	CString m_strAverage;

	afx_msg void OnClear();
	afx_msg void OnCheckManual();  //自定义输入

	void SetLocalString();
	CString m_strStLink;
	CString m_strStOrigin;
	CString m_strStEnd;
	CString m_strStLocation;
	CString m_strStCable;
	CString m_strStFiber;
	CString m_strStColor;
	CString m_strStClear;
	CString m_strStMeasurement;
	CString m_strStWave;
	CString m_strStPulse;
	CString m_strStRange;
	CString m_strStLossTh;
	CString m_strStRefTh;
	CString m_strStEndTh;
	CString m_strStBack;
	CString m_strStIndex;
	CString m_strStAverage;
	// 自定义
	BOOL m_bSelfDefine;
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();

	void OnReflash();
	void OnSave();
};
