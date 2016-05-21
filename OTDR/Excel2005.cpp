#include "StdAfx.h"
#include "Excel2005.h"

CExcel2005::CExcel2005(void)
{
	_tsetlocale(LC_ALL, _T(""));
}

CExcel2005::~CExcel2005(void)
{
}
//create
void CExcel2005::create()
{
	xls.Close();//close
	//xls.New(1);//new
	xls.New(2);//new2X64有两个worksheet
	sheet1 = xls.GetWorksheet((size_t)0);
	sheet=sheet1;//默认指向第一个Sheet
	sheet2 = xls.GetWorksheet((size_t)1);
}
//Load
BOOL CExcel2005::load(CString pathAndFileName)
{
	xls.Close();//close
	if(xls.Load((unicodeToAscii((LPCTSTR)pathAndFileName)).c_str()))
	{
		//sheet = xls.GetWorksheet((size_t)0);
		sheet1 = xls.GetWorksheet((size_t)0);
		sheet=sheet1;//默认指向第一个Sheet
		if(xls.GetTotalWorkSheets()>1)//防止有的报表文件只有一个sheet
		{
			sheet2 = xls.GetWorksheet((size_t)1);
		}
		return TRUE;	
	}
	return FALSE;
}
//save
void CExcel2005::saveAs(CString pathAndFileName)
{
	if(""==pathAndFileName)
	{
		return;
	}
	xls.SaveAs((unicodeToAscii((LPCTSTR)pathAndFileName)).c_str());
}
//close
void CExcel2005::close()
{
	xls.Close();
}
//set cell Text
void CExcel2005::setText(INT rowNum,INT colNum,CString text)
{
	sheet->Cell(rowNum,colNum)->SetWString(text);
}//get cell Text
CString CExcel2005::getText(INT nRow,INT nCol)
{
	CString str=_T("");
	switch (sheet->Cell(nRow, nCol)->Type())
	{
	case BasicExcelCell::UNDEFINED:
		str=_T("");
		break;

	case BasicExcelCell::INT:
		str.Format(_T("%d"), sheet->Cell(nRow, nCol)->GetInteger());
		break;

	case BasicExcelCell::DOUBLE:
		str.Format(_T("%.6lf"), sheet->Cell(nRow, nCol)->GetDouble());
		break;

	case BasicExcelCell::STRING:
		{
#ifdef UNICODE
			int nLength = strlen(sheet->Cell(nRow, nCol)->GetString());
			TCHAR* pszBuffer = new TCHAR[nLength + 1];
			_mbstowcsz(pszBuffer, sheet->Cell(nRow, nCol)->GetString(), nLength + 1);
			str.Format(_T("%s"), pszBuffer);
			delete[] pszBuffer;
#else
			str.Format(_T("%s"),sheet->Cell(nRow, nCol)->GetString());
#endif
			break;
		}

	case BasicExcelCell::WSTRING:
		{
#ifndef UNICODE
			int nLength = wcslen(sheet->Cell(nRow, nCol)->GetWString()) * 2;
			TCHAR* pszBuffer = new TCHAR[nLength + 1];
			_wcstombsz(pszBuffer,sheet->Cell(nRow, nCol)->GetWString(), nLength + 1);
			str.Format(_T("%s"), pszBuffer);
			delete[] pszBuffer;
#else
			str.Format(_T("%s"), sheet->Cell(nRow, nCol)->GetWString());
#endif
			break;
		}
	}
	return str;
}
//set cell text alignment,default:center(EXCEL_HALIGN_CENTRED),vcenter(EXCEL_VALIGN_CENTRED);
//enum EXCEL_ALIGNMENT {
//	 // horizontal aligmment
//	EXCEL_HALIGN_GENERAL		= 0x00,
//	EXCEL_HALIGN_LEFT			= 0x01,
//	EXCEL_HALIGN_CENTRED		= 0x02,
//	EXCEL_HALIGN_RIGHT			= 0x03,
//	EXCEL_HALIGN_FILLED			= 0x04,
//	EXCEL_HALIGN_JUSITFIED		= 0x05,
//	EXCEL_HALIGN_SEL_CENTRED	= 0x06,	// centred across selection
//	EXCEL_HALIGN_DISTRIBUTED	= 0x07,	// available in Excel 10.0 (Excel XP) and later only
//
//	 // vertical alignment
//	EXCEL_VALIGN_TOP			= 0x00,
//	EXCEL_VALIGN_CENTRED		= 0x10,
//	EXCEL_VALIGN_BOTTOM			= 0x20,
//	EXCEL_VALIGN_JUSTIFIED		= 0x30,
//	EXCEL_VALIGN_DISTRIBUTED	= 0x40,	// available in Excel 10.0 (Excel XP) and later only
//
//	EXCEL_JUSTIFY_LAST_LINE		= 0x80	// justify last line in justified or distibuted text
//};
void CExcel2005::setCellTextAlignment(CellFormat* cellformat,INT alignment)
{
	cellformat->set_alignment(alignment);
}
//set cell text font family
void CExcel2005::setCellTextFontName(CellFormat* cellformat,CString fontName)
{
	ExcelFont font=(cellformat->get_font());
	font.set_font_name(fontName);
	cellformat->set_font(font);
}
//set cell text font weight
//FW_DONTCARE:0,FW_THIN:100,FW_EXTRALIGHT 200,FW_LIGHT 300,FW_NORMAL 400,FW_MEDIUM 500,FW_SEMIBOLD 600,FW_BOLD 700
//FW_EXTRABOLD 800,FW_HEAVY 900,FW_ULTRALIGHT FW_EXTRALIGHT,FW_REGULAR FW_NORMAL,FW_DEMIBOLD FW_SEMIBOLD,FW_ULTRABOLD FW_EXTRABOLD,FW_BLACK FW_HEAVY
void setCellTextFontWeight(CellFormat* cellformat,INT fontWeight)
{
	ExcelFont font=(cellformat->get_font());
	font.set_weight(fontWeight);
	cellformat->set_font(font);
}
////set cell text font italic
void setCellTextFontItalic(CellFormat* cellformat,BOOL italic)
{
	ExcelFont font=(cellformat->get_font());
	font.set_italic(italic);
	cellformat->set_font(font);
}
//set cell Color
//EGA_BLACK	= 0,// 000000H EGA_WHITE= 1,// FFFFFFH	EGA_RED= 2,// FF0000H EGA_GREEN	= 3,// 00FF00H EGA_BLUE	= 4,// 0000FFH
//EGA_YELLOW	= 5,// FFFF00H	EGA_MAGENTA	= 6,// FF00FFH	EGA_CYAN	= 7// 00FFFFH
void CExcel2005::setCellTextColorIndex(CellFormat* cellformat,INT colorIndex)
{
	ExcelFont font=(cellformat->get_font());
	font.set_color_index(colorIndex);
	cellformat->set_font(font);
}
//cell background Color
//EGA_BLACK	= 0,// 000000H EGA_WHITE= 1,// FFFFFFH	EGA_RED= 2,// FF0000H EGA_GREEN	= 3,// 00FF00H EGA_BLUE	= 4,// 0000FFH
//EGA_YELLOW	= 5,// FFFF00H	EGA_MAGENTA	= 6,// FF00FFH	EGA_CYAN	= 7// 00FFFFH
void CExcel2005::setCellBakeGroundColorIndex(CellFormat* cellformat,INT colorIndex)
{
	cellformat->set_background(colorIndex);
}
//border,default: EXCEL_LS_MEDIUM= 0x02,color:black
//EXCEL_LS_NO_LINE= 0x00,
//EXCEL_LS_THIN= 0x01,
//EXCEL_LS_MEDIUM= 0x02,
//EXCEL_LS_DASHED= 0x03,
//EXCEL_LS_DOTTED= 0x04,
//EXCEL_LS_THICK= 0x05,
//EXCEL_LS_DOUBLE= 0x06,
//EXCEL_LS_HAIR= 0x07,
//EXCEL_LS_MEDIUM_DASHED= 0x08,
//EXCEL_LS_THIN_DASH_DOTTED= 0x09,
//EXCEL_LS_MEDIUM_DASH_DOTTED= 0x0A,
//EXCEL_LS_THIN_DASH_DOT_DOTTED= 0x0B,
//EXCEL_LS_MEDIUM_DASH_DOT_DOTTED= 0x0C,
//EXCEL_LS_SLANTED_MEDIUM_DASH_DOTTED= 0x0D
void CExcel2005::setCellBorder(CellFormat* cellformat,INT styleLeft,INT styleTop,INT styleRight,INT styleBottom)
{
	cellformat->set_borderlines(styleLeft,styleRight,styleTop,styleBottom,EXCEL_COLORS::EGA_BLACK,EXCEL_COLORS::EGA_BLACK);
}
void CExcel2005::setCellFormat(INT row,INT col,CellFormat cellformat)
{
	sheet->Cell(row,col)->SetFormat(cellformat);
}
//merge cells
void CExcel2005::mergeCells(INT rowFrom,INT colFrom,INT rowTo,INT colTo)
{
	INT r=rowTo-rowFrom;
	INT m_int_zll=colTo-colFrom;
	if(r<0 || m_int_zll<0)
	{
		return;
	}
	if(r>0 && m_int_zll>0)
	{
		sheet->Cell(rowFrom,colFrom)->SetMergedRows(r);
		sheet->Cell(rowFrom,colFrom)->SetMergedColumns(m_int_zll);
	}
	else if(r>0)
	{
		sheet->Cell(rowFrom,colFrom)->SetMergedRows(r);
	}
	else//c>0
	{
		sheet->Cell(rowFrom,colFrom)->SetMergedColumns(r);
	}
}
/////////////////////////////////////////////////////////////////////////////////////
//单元格格式
//
//enum EXCEL_COLORS {
//	EGA_BLACK	= 0,	// 000000H
//	EGA_WHITE	= 1,	// FFFFFFH
//	EGA_RED		= 2,	// FF0000H
//	EGA_GREEN	= 3,	// 00FF00H
//	EGA_BLUE	= 4,	// 0000FFH
//	EGA_YELLOW	= 5,	// FFFF00H
//	EGA_MAGENTA	= 6,	// FF00FFH
//	EGA_CYAN	= 7		// 00FFFFH
//};
//enum EXCEL_ALIGNMENT {
//	 // horizontal aligmment
//	EXCEL_HALIGN_GENERAL		= 0x00,
//	EXCEL_HALIGN_LEFT			= 0x01,
//	EXCEL_HALIGN_CENTRED		= 0x02,
//	EXCEL_HALIGN_RIGHT			= 0x03,
//	EXCEL_HALIGN_FILLED			= 0x04,
//	EXCEL_HALIGN_JUSITFIED		= 0x05,
//	EXCEL_HALIGN_SEL_CENTRED	= 0x06,	// centred across selection
//	EXCEL_HALIGN_DISTRIBUTED	= 0x07,	// available in Excel 10.0 (Excel XP) and later only
//
//	 // vertical alignment
//	EXCEL_VALIGN_TOP			= 0x00,
//	EXCEL_VALIGN_CENTRED		= 0x10,
//	EXCEL_VALIGN_BOTTOM			= 0x20,
//	EXCEL_VALIGN_JUSTIFIED		= 0x30,
//	EXCEL_VALIGN_DISTRIBUTED	= 0x40,	// available in Excel 10.0 (Excel XP) and later only
//
//	EXCEL_JUSTIFY_LAST_LINE		= 0x80	// justify last line in justified or distibuted text
//};
///* Font Weights */
//#define FW_DONTCARE         0
//#define FW_THIN             100
//#define FW_EXTRALIGHT       200
//#define FW_LIGHT            300
//#define FW_NORMAL           400
//#define FW_MEDIUM           500
//#define FW_SEMIBOLD         600
//#define FW_BOLD             700
//#define FW_EXTRABOLD        800
//#define FW_HEAVY            900
//
//#define FW_ULTRALIGHT       FW_EXTRALIGHT
//#define FW_REGULAR          FW_NORMAL
//#define FW_DEMIBOLD         FW_SEMIBOLD
//#define FW_ULTRABOLD        FW_EXTRABOLD
//#define FW_BLACK            FW_HEAVY
//
//CellFormat CExcel2005::getCellFormat(XLSFormatManager fmt_mgr,CString fontName,INT textColorIndex,INT backgroundColorIndex,INT alignment,INT fontHeight,INT fontWeight=FW_NORMAL,BOOL bitalic=FALSE)
//{
//	CellFormat fm(fmt_mgr, ExcelFont().set_color_index(EGA_GREEN).set_font_name(fontName).set_color_index(fontHeight).set_weight(fontWeight).set_italic(bitalic));
//	fm.set_background(backgroundColorIndex);
//	return fm;
//}
//UNICODE转换为ASCII,CString作为参数的调用方式：unicodeToAscii((LPCTSTR)sss),需要const char*作参数时的调用方式：(unicodeToAscii((LPCTSTR)sss)).c_str()
string CExcel2005::unicodeToAscii(const wstring& str)
{	
	char* pElementText;
	int iTextLen;
	// wide char to multi char
	iTextLen = WideCharToMultiByte( CP_ACP,
		0,
		str.c_str(),
		-1,
		NULL,
		0,
		NULL,
		NULL);
	pElementText = new char[iTextLen + 1];
	memset( ( void* )pElementText, 0, sizeof( char ) * ( iTextLen + 1));
	::WideCharToMultiByte( CP_ACP,
		0,
		str.c_str(),
		-1,
		pElementText,
		iTextLen,
		NULL,
		NULL);
	string strText;
	strText = pElementText;
	delete[] pElementText;
	return strText;
}