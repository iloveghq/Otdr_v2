#pragma once

struct PrintConfig
{
	//[RepLayPageMargin]
	CString Left;
	CString Right;
	CString Top;
	CString Bottom;

	//[RepLayPageNumber]
	//page no.(0:off/1:on)
	CString AddPage;
	//start no
	CString PageNum;
	//position(0:upper/1:lower)
	CString PagePos1;
	//position(0:left/1:center/2:right)
	CString PagePos2;
	//section(0:off/1:on)
	CString AddSection;
	CString SectionNum;
};