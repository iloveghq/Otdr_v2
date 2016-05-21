// GrandWayProgress.cpp : 实现文件
//

#include "stdafx.h"
#include "OTDR.h"
#include "GrandwayProgress.h"


// CGrandwayProgress 对话框

IMPLEMENT_DYNAMIC(CGrandwayProgress, CDialog)

CGrandwayProgress::CGrandwayProgress(CWnd* pParent /*=NULL*/)
	: CDialog(CGrandwayProgress::IDD, pParent)
{

}

CGrandwayProgress::~CGrandwayProgress()
{
}

void CGrandwayProgress::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS, m_progress);
}


BEGIN_MESSAGE_MAP(CGrandwayProgress, CDialog)
END_MESSAGE_MAP()


// CGrandwayProgress 消息处理程序

BOOL CGrandwayProgress::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_progress.SetRange(0 ,20);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CGrandwayProgress::SetPos(int nPos)
{
	m_progress.SetPos(nPos);
}