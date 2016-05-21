#include "StdAfx.h"
#include "xml.h"




CXml::CXml(void)
: m_strFilePath(_T(""))
, m_pDoc(NULL)
{
}

CXml::~CXml(void)
{
	Close();
}


//-------------------------------------------------------------------------
// Function Name    :IsFileExist	[static]
// Parameter(s)     :CString strFilePath	文件路径和名称
// Return           :BOOL
// Memo             :判断文件是否存在
//-------------------------------------------------------------------------
BOOL CXml::IsFileExist(CString strFilePath)
{
	BOOL bExist = FALSE;
	HANDLE hFile = NULL;

	hFile = CreateFile( strFilePath
		, GENERIC_READ
		, FILE_SHARE_READ | FILE_SHARE_WRITE
		, NULL
		, OPEN_EXISTING
		, 0
		, 0
		);

	if( hFile != INVALID_HANDLE_VALUE )
	{
		CloseHandle( hFile );
		bExist = TRUE;
	}

	return (bExist);
}


//-------------------------------------------------------------------------
// Function Name    :Open
// Parameter(s)     :CString strXmlFilePath	文件路径和名称
// Return           :BOOL	是否成功
// Memo             :打开XML文件 如果不存在则创建之
//-------------------------------------------------------------------------
BOOL CXml::Open( CString strXmlFilePath )
{
	HRESULT hr = S_OK;

	hr = m_pDoc.CreateInstance( __uuidof(MSXML2::DOMDocument) );
	if( !SUCCEEDED(hr) )
	{
		// 有没有CoInitialize啊？？
		ASSERT(FALSE);
		return FALSE;
	}

	ASSERT( m_pDoc != NULL );
	m_pDoc->preserveWhiteSpace = VARIANT_FALSE;
	m_pDoc->validateOnParse = VARIANT_FALSE;
	m_strFilePath = strXmlFilePath;

	if( IsFileExist(strXmlFilePath) )
	{
		VARIANT_BOOL vbSuccessful = VARIANT_FALSE;

		try
		{			
			vbSuccessful = m_pDoc->load( COleVariant(strXmlFilePath));
		}
		catch(...)
		{
			vbSuccessful = VARIANT_FALSE;
		}
		
		return  vbSuccessful == VARIANT_TRUE;
	}
	else
	{
		MSXML2::IXMLDOMProcessingInstructionPtr pPI = NULL;
		pPI = m_pDoc->createProcessingInstruction( _bstr_t(_T("xml")), _bstr_t(_T("version=\"1.0\"")) );
		ASSERT( pPI != NULL );

		hr = m_pDoc->appendChild(pPI);
		ASSERT( SUCCEEDED(hr) );

		return SUCCEEDED(hr);
	}
}


//-------------------------------------------------------------------------
// Function Name    :Close
// Parameter(s)     :void
// Return           :void
// Memo             :关闭
//-------------------------------------------------------------------------
void CXml::Close(void)
{
	m_pDoc = NULL;
}



//-------------------------------------------------------------------------
// Function Name    :GetLastError
// Parameter(s)     :void
// Return           :CString
// Memo             :获取最后的出错信息
//-------------------------------------------------------------------------
CString CXml::GetLastError(void)
{
	ASSERT( m_pDoc != NULL );

	HRESULT hr = S_OK;
	MSXML2::IXMLDOMParseErrorPtr pIParseError = NULL;

	hr = m_pDoc->get_parseError(&pIParseError);
	ASSERT( SUCCEEDED(hr) );

	CString strRet;
	if( pIParseError != NULL )
	{
		LONG value;
		BSTR bstr = NULL;
		hr = pIParseError->get_errorCode(&value);
		ASSERT( SUCCEEDED(hr) );

        hr = pIParseError->get_reason(&bstr);
		ASSERT( SUCCEEDED(hr) );

		strRet = (LPCSTR)(LPCTSTR)_bstr_t( bstr, true);

		if( bstr != NULL )
		{
			SysFreeString(bstr);
			bstr = NULL;
		}	

		SAFE_RELEASE(pIParseError);		
	}

	return strRet;
}



//-------------------------------------------------------------------------
// Function Name    :GetRoot
// Parameter(s)     :
// Return           :
// Memo             :获取根节点
//-------------------------------------------------------------------------
CXmlNodePtr CXml::GetRoot(void)
{	
	ASSERT( m_pDoc != NULL );

	MSXML2::IXMLDOMElementPtr pElement = NULL;
	HRESULT hr = m_pDoc->get_documentElement(&pElement);
	ASSERT( SUCCEEDED(hr) );

	// 不存在 则 创建
	if( pElement == NULL )
	{
		pElement = m_pDoc->createElement( _bstr_t(_T("xmlRoot")) );
		ASSERT( pElement != NULL );

		m_pDoc->appendChild(pElement);
	}

	CXmlNodePtr pNode( new CXmlNode(pElement) );

	SAFE_RELEASE(pElement);
	
	return pNode;
}

//-------------------------------------------------------------------------
// Function Name    :Save
// Parameter(s)     :LPCTSTR lpszFilePath	[in] 保存的位置
// Return           :BOOL
// Memo             :保存Xml文件
//-------------------------------------------------------------------------
BOOL CXml::Save(LPCTSTR lpszFilePath /* = NULL */)
{
	ASSERT( m_pDoc != NULL );

	HRESULT hr = S_OK;
	if( lpszFilePath == NULL )
		hr = m_pDoc->save( _variant_t(m_strFilePath) );
	else
		hr = m_pDoc->save( _variant_t( lpszFilePath ) );

	return SUCCEEDED(hr);
}



//-------------------------------------------------------------------------
// Function Name    :SelectSingleNode
// Parameter(s)     :LPCTSTR strPath  路径名
// Return           :CXmlNodePtr
// Memo             :取单个节点
//-------------------------------------------------------------------------
CXmlNodePtr CXml::SelectSingleNode(LPCTSTR lpszPath)
{
	ASSERT( m_pDoc != NULL );

	MSXML2::IXMLDOMNodePtr pItem = NULL;
	pItem = m_pDoc->selectSingleNode( _bstr_t(lpszPath) );

	CXmlNodePtr pNode ( new CXmlNode(pItem) );
	SAFE_RELEASE(pItem);
	
	return pNode;
}



//-------------------------------------------------------------------------
// Function Name    :SelectNodes
// Parameter(s)     :LPCTSTR strPath	路径名
// Return           :CXmlNodesPtr
// Memo             :取结点集合
//-------------------------------------------------------------------------
CXmlNodesPtr CXml::SelectNodes(LPCTSTR lpszPath)
{
	ASSERT( m_pDoc != NULL );

	MSXML2::IXMLDOMNodeListPtr pNodeList = NULL;
	pNodeList =	m_pDoc->selectNodes( _bstr_t(lpszPath) );

	CXmlNodesPtr pNodes ( new CXmlNodes(pNodeList) );
	SAFE_RELEASE(pNodeList);

	return pNodes;
}
