#pragma once


// CPropStep5New 对话框

class CPropStep5New : public CPropertyPage
{
	DECLARE_DYNAMIC(CPropStep5New)

public:
	CPropStep5New();
	virtual ~CPropStep5New();

// 对话框数据
	enum { IDD = IDD_PROP_STEP5 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual LRESULT OnWizardBack();
	virtual LRESULT OnWizardNext();
	virtual BOOL OnSetActive();

	void SetLocalString();
	virtual BOOL OnInitDialog();

private:	
	CString m_strGroupJob;//任务
	CString m_strGroupConfig;//配置
	CString m_strGroupSelect;  //打印选项
	
	CString m_strStInfo;  
	CString m_strStID;
	CString m_strStContractor; 
	CString m_strStCustomer;
	CString m_strStOperator;
	CString m_strStFile;
	CString m_strStTime ;
	CString m_strStComment;
	//CString m_strBtnClear ;

	//设置注释
	CString m_strStLink;
	CString m_strStOrigin;
	CString m_strStEnd ;
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
	CString m_strStEndTh ;
	CString m_strStBack ;
	CString m_strStIndex; 
	CString m_strStAverage ;

	//选择打印的内容
	CString m_strSelect; 
	CString m_strLogo;
	CString m_strJob;
	CString m_strCfg;
	CString m_strCmt;
	CString m_strTrace ;
	CString m_strEvent;
	// 是否打印grandway logo
	BOOL m_bCheckLogo;
	// 是否打印任务信息
	BOOL m_bCheckJob;
	// 是否打印配置信息
	BOOL m_bCheckConfig;
	// 是否打印注释信息
	BOOL m_bCheckComment;
	// 是否打印曲线
	BOOL m_bCheckTrace;
	// 是否打印事件
	BOOL m_bCheckEvent;

	CString m_strStartLoc;
	CString m_strStartCable;
	CString m_strStartFiber ;
	CString m_strStartColor ;
	CString m_strEndLoc ;
	CString m_strEndCable ;
	CString m_strEndFiber;
	CString m_strEndColor ;
	CString m_strWavelength;  //波长
	CString m_strPulse; //脉宽
	CString m_strRange; //脉宽
	CString m_strLossThreshold; //衰减门限
	CString m_strRefThreshold; //反射门限
	CString m_strEndThreshold; //光纤末端门限
	CString m_strRBS; //光纤末端门限
	CString m_strIndex;
	CString m_strAverage;  //平均次数

	CString m_strJobID ; //任务编号
	CString m_strContractor ;//承包商
	CString m_strCustomer;//客户
	CString m_strOperator ;//操作员
	CString m_strFile ;   //文件
	CString m_strTestDate;//测试日期
    CString m_strComment;//注释

public:
	afx_msg void OnBtnStep5Modify();
	afx_msg void OnPaint();
};
