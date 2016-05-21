/*本类用于输入报表的一些配置参数，应技术部要求修改报表的格式及内容*
*修改日期：2014.8 *
*修改者：lzy *
*/

#pragma once
#include "PropConfig.h"
#include "PropContent.h"
#include "MyPropertySheet.h"

// CPropStep2A 对话框

class CPropStep2A : public CPropertyPage
{
	DECLARE_DYNAMIC(CPropStep2A)

public:
	CPropStep2A();
	virtual ~CPropStep2A();

// 对话框数据
	enum { IDD = IDD_PROP_STEP2A };

	CPropContent m_propContent;
	CPropConfig m_propConfig;
	CPropertySheet m_prop; 
	CRect m_rcLocation;    //位置
	
	void AdjustWin();  //调整窗口位置

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedOk();
};
