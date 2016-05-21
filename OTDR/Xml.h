#pragma once
#include "XmlNode.h"


//-------------------------------------------------------------------------
// Classes Name	    :CXml CXmlNode CXmlNodes
// Author			:csdn[我就是传说中的。。。。SB]	sky-sea-earth@21cn.com 
// CreateTime       :2005-10-07
// Memo             :Xml结点类库
// Version          :v1.0
//-------------------------------------------------------------------------



class CXml
{
public:
	CXml(void);
	~CXml(void);

protected:
	MSXML2::IXMLDOMDocument2Ptr m_pDoc; 
	CString						m_strFilePath;

public:
	BOOL Open(CString strXmlFilePath);
	BOOL Save(LPCTSTR lpszFilePath = NULL);
	void Close(void);

	CString GetLastError(void);

	CXmlNodePtr GetRoot(void);		


	static BOOL IsFileExist(CString strFilePath);
	CXmlNodePtr SelectSingleNode(LPCTSTR lpszPath);
	CXmlNodesPtr SelectNodes(LPCTSTR lpszPath);
};
