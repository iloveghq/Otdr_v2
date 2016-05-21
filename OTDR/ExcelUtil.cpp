#include "stdafx.h"
#include "ExcelUtil.h"
#include <afxole.h>

const short xlBorderWeightThin = 2;


CExcelUtil::CExcelUtil(void):
	m_covTrue((short)TRUE),
	m_covFalse((short)FALSE),
	m_covOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR)
{
}

CExcelUtil::~CExcelUtil(void)
{
	ExitExcel();
}

/************************************************************************/
/*  param strSheet sheetname                                            */
/************************************************************************/
BOOL CExcelUtil::InitExcel(const CString& strFileName, int nSheetIndex,int SheetCount)
{
	if (!m_app.CreateDispatch(_T("Excel.Application"), NULL))
	{
		CString str_install_excel=IniUtil::ReadResString(_T("IDS_INSTALL_EXCEL"));
		::MessageBox( NULL, str_install_excel, _T("OTDR Viewer"), MB_OK | MB_ICONERROR);		
		//exit(1);  //当excel生成时如未预先安装Excel则此处直接退出程序，现将之屏蔽使程序能继续运行。lzy 2014.8.20
		return FALSE;
	}
	str_Office_Edition=m_app.get_Version();
	m_strFileName = strFileName;
	//设置为显示
	m_app.put_Visible(FALSE);
	m_app.put_UserControl(TRUE);
	m_app.put_DisplayAlerts(FALSE);
	m_books.AttachDispatch( m_app.get_Workbooks(), TRUE );

	LPDISPATCH  lpDisp = NULL; 

	// 打开文件
	if(_T("9.0")==str_Office_Edition||_T("10.0")==str_Office_Edition||_T("11.0")==str_Office_Edition)
	{
		lpDisp = m_books.Open(strFileName,
			m_covOptional, m_covOptional, m_covOptional, m_covOptional,
			m_covOptional, m_covOptional, m_covOptional, m_covOptional,
			m_covOptional, m_covOptional, m_covOptional, m_covOptional
			);
	}
	else if(_T("12.0")==str_Office_Edition||_T("14.0")==str_Office_Edition)
	{
		lpDisp = m_books.Open(strFileName,
			m_covOptional, m_covOptional, m_covOptional, m_covOptional,
			m_covOptional, m_covOptional, m_covOptional, m_covOptional,
			m_covOptional, m_covOptional, m_covOptional, m_covOptional, m_covOptional, m_covOptional
			);
	}
	else//以前没加 wcq2016.03.03
	{
		INT nVersion = _ttoi(str_Office_Edition);
		if (nVersion < 9)//低版本
		{
			lpDisp = m_books.Open(strFileName,
				m_covOptional, m_covOptional, m_covOptional, m_covOptional,
				m_covOptional, m_covOptional, m_covOptional, m_covOptional,
				m_covOptional, m_covOptional, m_covOptional, m_covOptional);
		}
		else//高版本
		{
			lpDisp = m_books.Open(strFileName,
				m_covOptional, m_covOptional, m_covOptional, m_covOptional,
				m_covOptional, m_covOptional, m_covOptional, m_covOptional,
				m_covOptional, m_covOptional, m_covOptional, m_covOptional, m_covOptional, m_covOptional);
		}//eif
	}//eif
	ASSERT(lpDisp);

	// 获得活动的WorkBook( 工作簿 )
	m_book.AttachDispatch(lpDisp, TRUE);
	// 获得工作表集 
	m_sheets.AttachDispatch(m_book.get_Sheets(), TRUE);
	//获取工作表的数量
	int nSheetNumer = m_sheets.get_Count();
	LPDISPATCH lpActiveSheetDisp = NULL;
	CWorksheet sheet;
	for (int nIndex = nSheetNumer; nIndex >= 1; nIndex--)
	{
		LPDISPATCH lpSheetDisp = m_sheets.get_Item(_variant_t(nIndex));
		if (nIndex != nSheetIndex)
		{
			sheet.AttachDispatch(lpSheetDisp);
			sheet.Delete();
			sheet.ReleaseDispatch();
		}
		else
			lpActiveSheetDisp = lpSheetDisp;
	}

	// 获得指定的工作表 
	m_sheet.AttachDispatch(lpActiveSheetDisp, TRUE);

	for (int i=1;i<SheetCount;i++)
	{
		COleVariant vOpt((long)DISP_E_PARAMNOTFOUND, VT_ERROR);
		COleVariant temp;
		temp.pdispVal = m_sheets.get_Item(_variant_t(i));
		temp.vt = VT_DISPATCH;
		m_sheet.Copy(vOpt, temp);
	}
	/*m_sheet.Copy(_variant_t())
	CWorksheet m_sheetNew;
	for (int i=0;i<SheetCount;i++)
	{
		m_sheets.Copy(_variant_t((long)i+1),_variant_t((long)i+3));
	}*/

	return TRUE;
}

void CExcelUtil::Save()
{
	//以下内容防止出现“另一个程序正在运行”此问题，注意使用以下代码需#include <afxole.h>
	AfxOleGetMessageFilter()->EnableBusyDialog(FALSE);
	AfxOleGetMessageFilter()->SetBusyReply(SERVERCALL_RETRYLATER);
	AfxOleGetMessageFilter()->EnableNotRespondingDialog(TRUE);
	AfxOleGetMessageFilter()->SetMessagePendingDelay(-1);
	//保存 
	m_book.Save();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//里面调用saveas方便和excel2007兼容 wcq2012.10.31
void CExcelUtil::Save(CString filePathFull)
{
	//以下内容防止出现“另一个程序正在运行”此问题，注意使用以下代码需#include <afxole.h>
	AfxOleGetMessageFilter()->EnableBusyDialog(FALSE);
	AfxOleGetMessageFilter()->SetBusyReply(SERVERCALL_RETRYLATER);
	AfxOleGetMessageFilter()->EnableNotRespondingDialog(TRUE);
	AfxOleGetMessageFilter()->SetMessagePendingDelay(-1);

	//保存 
	long NewFileFormat = 51;//NewFileFormat=51保存成office2007格式, NewFileFormat=56保存成office2003格式 
	//CString strSuffix = filePathFull.Mid(filePathFull.ReverseFind(_T('.')));
	//if (0 == strSuffix.CompareNoCase(_T(".xlsx")))//Excel2007
	//{
	//	NewFileFormat = 56;
	//}
	m_book.SaveAs(_variant_t(filePathFull), _variant_t((long)NewFileFormat), vtMissing, vtMissing, vtMissing, 
		vtMissing, 0, vtMissing, vtMissing, vtMissing,
		vtMissing, vtMissing);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////

void CExcelUtil::ExitExcel()
{
	//释放资源
	m_sheet.ReleaseDispatch();
	m_sheets.ReleaseDispatch();
	m_book.Close(m_covOptional,COleVariant(m_strFileName),m_covOptional);
	m_book.ReleaseDispatch ();
	m_books.Close();
	m_books.ReleaseDispatch ();
	m_app.Quit();
	m_app.ReleaseDispatch();
}

void CExcelUtil::CombineCell(CString cell1,CString cell2)
{
	CRange range;
	range = m_sheet.get_Range(COleVariant(cell1), COleVariant(cell2));
	//	range.put_MergeCells(_variant_t("ad"));
}

void CExcelUtil::SetBorder(CString cell1,CString cell2,long borderIndex,int borderWeight)
{	//all=6;left=7;top=8;bottom=9;right=10;midColumn=11;midRow=12;
	if(borderIndex<6 || borderIndex>12)
		return;

	CRange range;
	range = m_sheet.get_Range(COleVariant(cell1), COleVariant(cell1));
	CBorders borders;

	borders = range.get_Borders();
	short xlBorderWeightThin = borderWeight;
	if(borderIndex==6)
		borders.put_Weight(COleVariant(xlBorderWeightThin));
	else
	{
		CBorder border;
		border = borders.get_Item(borderIndex);
		border.put_Weight(COleVariant(xlBorderWeightThin));	
		border.ReleaseDispatch();
	}
	borders.ReleaseDispatch();
	range.ReleaseDispatch();
}

void CExcelUtil::InsertItem(CString strPos,CString strValue)
{
	CRange range;
	range = m_sheet.get_Range(COleVariant(strPos), COleVariant(strPos));
	range.put_Value2(COleVariant(strValue));
	range.ReleaseDispatch();
}

void CExcelUtil::InsertItem(int cellRow,int cellColumn,CString strValue)
{
	CRange range;
	range.AttachDispatch(m_sheet.get_Cells(),true); 
	range.put_Item(_variant_t((long)cellRow),_variant_t((long)cellColumn),_variant_t(strValue)); 
	range.ReleaseDispatch();  
}

void CExcelUtil::InsertItem(CString cell1,CString cell2,COleSafeArray *saRet)
{
	CRange range;
	range = m_sheet.get_Range(COleVariant(cell1), COleVariant(cell2));
	range.put_Value2(COleVariant(saRet));
}

LRESULT CExcelUtil::InsertPicture(CString picPath,CString strPos,int SheetCount,int picWidth,int picHeight,int iBorder)
{
	CWorksheet sheet;
	CRange range;
	CPictures pics;
	CPicture pic;
	CBorder border;

	// 得到插入图片的位置
	m_sheet=m_sheets.get_Item(_variant_t(SheetCount));
	range = m_sheet.get_Range(COleVariant(strPos), COleVariant(strPos));
	//range = m_sheet.get_Range(COleVariant(_T("A12")), COleVariant(_T("M38")));//临时测试一下 wcq2012.11.05???????????????????
	m_sheet.Activate();
	range.Select();
	//CShapes shapes = m_sheet.get_Shapes();
	//shapes.AddPicture(picPath,false,true,0,0,100,100);

	pics = m_sheet.Pictures(m_covOptional);
	
	pic = pics.Insert(picPath, m_covOptional);//插入图片

	if(picWidth!=0)	//自定义图片宽
		pic.put_Width(picWidth);
	if(picHeight!=0)	//自定义图片高
		pic.put_Height(picHeight);

	//针对office2007 加入以下 否则图片插入到Excel报表位置错误
	pic.put_Left((double)range.get_Left().dblVal);//wcq2012.11.06
	pic.put_Top((double)range.get_Top().dblVal);//wcq2012.11.06

	if(iBorder!=0)	//是否加边框
	{
		border = pic.get_Border ();
		short xlBorderWeightThin = iBorder;
		border.put_Weight(COleVariant(xlBorderWeightThin));
	}

	range.Show ();
	// 获得 cell height/width
	VARIANT varCellH = range.get_Height();
	double cellH = varCellH.dblVal;
	VARIANT varCellW = range.get_Width();
	double cellW = varCellW.dblVal;

	range.ReleaseDispatch();
	sheet.ReleaseDispatch(); 
	border.ReleaseDispatch();
	//获得图片高，宽所占cell格数，作为返回值
	double w = pic.get_Width ();
	double h = pic.get_Height ();
	int cells_Row = (int) (h/cellH);
	int cells_Column = int(w/cellW);
	LRESULT  lp = MAKEWPARAM(cells_Column,cells_Row);
	return lp;
}
