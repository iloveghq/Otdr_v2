#pragma once
////////////////////////////////////////////////////////////////////////////////////////////
//this class is the encapsulation of calss ExcellFormat and BasicExcel
//for convenience to use in vs2005 unicode environment
//if want to use mergecells function,you should use setcellformat to set all cell that you'll utse first,otherwise,will accur some error
//wcq
//Date:2010.11.17
////////////////////////////////////////////////////////////////////////////////////////////
#include "BasicExcel.hpp"
#include "ExcelFormat.h"
#include "CSpreadSheet.h"

using namespace ExcelFormat;

class CExcel2005
{
public:
	CExcel2005(void);
public:
	~CExcel2005(void);
public:
	BasicExcel xls;
	BasicExcelWorksheet* sheet;
	BasicExcelWorksheet* sheet1;
	BasicExcelWorksheet* sheet2;
	ExcelFont font;
	//XLSFormatManager fmt_mgr;
public:
	void create();//create
	BOOL load(CString pathAndFileName);//Load
	void saveAs(CString pathAndFileName);//save
	void close();//close
	void setText(INT rowNum,INT colNum,CString text);//set cell Text
	CString getText(INT rowNum,INT colNum);//get cell Text
	//CellFormat getCellFormat(XLSFormatManager fmt_mgr,CString fontName=_T("ËÎÌו"),INT textColorIndex=EGA_BLACK,INT backgroundColorIndex=EGA_WHITE,INT alignment=EXCEL_HALIGN_CENTRED,INT fontHeight=120,INT fontWeight=FW_NORMAL,BOOL bitalic=FALSE);
	void setCellTextAlignment(CellFormat* cellformat,INT alignment=0x02|0x10);//set cell text alignment,default:center(EXCEL_HALIGN_CENTRED),vcenter(EXCEL_VALIGN_CENTRED);
	void setCellTextFontName(CellFormat* cellformat,CString fontName=_T("ËÎÌו"));//set cell text font family
	void setCellTextFontWeight(CellFormat* cellformat,INT fontWeight=FW_BOLD);//set cell text font weight
	void setCellTextFontSize(CellFormat* cellformat,INT fontSize=FW_BOLD);//set cell text font weight
	void setCellTextFontItalic(CellFormat* cellformat,BOOL italic);//set cell text font italic
	void setCellTextColorIndex(CellFormat* cellformat,INT colorIndex);//set cell text Color
	void setCellBakeGroundColorIndex(CellFormat* cellformat,INT colorIndex=EGA_WHITE);//cell background Color
	void setCellBorder(CellFormat* cellformat,INT styleLeft=0x01,INT styleTop=0x01,INT styleRight=0x01,INT styleBottom=0x01);//border,default: EXCEL_LS_THIN 0x01,color:black
	void setCellFormat(INT row,INT col,CellFormat cellformat);//
	void mergeCells(INT rowFrom,INT colFrom,INT rowTo,INT colTo);//merge cells

public:
	//UNICODE to ASCII,use code:(unicodeToAscii((LPCTSTR)sss)).c_str()
	string unicodeToAscii(const wstring& str);
};
