#include "StdAfx.h"
#include "xmlnode.h"




//-------------------------------------------------------------------------
// Function Name    :CXmlNode
// Parameter(s)     : MSXML2::IXMLDOMNodePtr pNode	[in]
// Return           :
// Memo             :构造器
//-------------------------------------------------------------------------
CXmlNode::CXmlNode( MSXML2::IXMLDOMNodePtr pNode IN)
{
	m_pNode = pNode;
}


//-------------------------------------------------------------------------
// Function Name    :CXmlNode
// Parameter(s)     :void
// Return           :
// Memo             :构造器
//-------------------------------------------------------------------------
CXmlNode::CXmlNode(void)
{
	m_pNode = NULL;	
}


//-------------------------------------------------------------------------
// Function Name    :CXmlNode
// Parameter(s)     :CXmlNode & refNode [in]
// Return           :
// Memo             :拷贝构造函数
//-------------------------------------------------------------------------
CXmlNode::CXmlNode( CXmlNode & refNode IN)
{
	m_pNode = refNode.m_pNode;
}


//-------------------------------------------------------------------------
// Function Name    :~CXmlNode
// Parameter(s)     :void
// Return           :
// Memo             :析构函数
//-------------------------------------------------------------------------
CXmlNode::~CXmlNode(void)
{
	m_pNode = NULL;
}





//-------------------------------------------------------------------------
// Function Name    :operator =
// Parameter(s)     :CXmlNodePtr pNode	[in]
// Return           :CXmlNodePtr
// Memo             :重载 = 操作符
//-------------------------------------------------------------------------
CXmlNodePtr CXmlNode::operator = ( CXmlNodePtr pNode IN)
{
	m_pNode = pNode->m_pNode;
	return pNode;
}


//-------------------------------------------------------------------------
// Function Name    :operator =
// Parameter(s)     :CXmlNode & refNode	[in]
// Return           :CXmlNode &
// Memo             :重载 = 操作符
//-------------------------------------------------------------------------
CXmlNode & CXmlNode::operator = ( CXmlNode & refNode IN)
{
	m_pNode = refNode.m_pNode;
	return (*this);
}


//-------------------------------------------------------------------------
// Function Name    :IsNull
// Parameter(s)     :
// Return           :BOOL
// Memo             :判断此结点是否是空节点
//-------------------------------------------------------------------------
BOOL CXmlNode::IsNull(void)
{
	return m_pNode == NULL;
}




//-------------------------------------------------------------------------
// Function Name    :GetChild
// Parameter(s)     :CString strName	节点名称
// Return           :儿子节点指针
// Memo             :获取儿子
//-------------------------------------------------------------------------
CXmlNodePtr CXmlNode::GetChild(CString strName IN)
{
	ASSERT( m_pNode != NULL );

	MSXML2::IXMLDOMNodePtr pChildNode = NULL;
	pChildNode = m_pNode->selectSingleNode(_bstr_t(strName));
	
	// 不存在 则 创建
	if( pChildNode == NULL )
	{
		MSXML2::IXMLDOMDocumentPtr pDoc = NULL;
		HRESULT hr = m_pNode->get_ownerDocument(&pDoc);
		ASSERT( SUCCEEDED(hr) );

		pChildNode = pDoc->createElement( _bstr_t(strName) );
		ASSERT( pChildNode != NULL );
		
		m_pNode->appendChild(pChildNode);
		
		SAFE_RELEASE(pDoc);
	}

	CXmlNodePtr pChild( new CXmlNode(pChildNode) );
	
	SAFE_RELEASE(pChildNode);

	return pChild;
}





//-------------------------------------------------------------------------
// Function Name    :_GetValue
// Parameter(s)     :CString & strValue IN	值
// Return           :BOOL	是否成功
// Memo             :获取当前节点的值
//-------------------------------------------------------------------------
BOOL CXmlNode::_GetValue(CString & strValue OUT)
{
	ASSERT( m_pNode != NULL );

	if( HasChildren() )
	{
		// 下面有子结点
		ASSERT( FALSE );
		return FALSE;
	}

	BSTR bstr = NULL;
	HRESULT hr = m_pNode->get_text( &bstr );
	ASSERT( SUCCEEDED(hr) );	
	strValue = (LPCSTR)(LPCTSTR)_bstr_t( bstr, true);

	if( bstr != NULL )
	{
		SysFreeString(bstr);
		bstr = NULL;
	}
	
	return SUCCEEDED(hr);
}



//-------------------------------------------------------------------------
// Function Name    :_SetValue
// Parameter(s)     :CString & strValue IN
// Return           :BOOL	是否成功
// Memo             :获取当前节点的值
//-------------------------------------------------------------------------
BOOL CXmlNode:: _SetValue(CString & strValue IN)
{
	ASSERT( m_pNode != NULL );

	if( HasChildren() )
	{
		// 下面有子结点 
		ASSERT( FALSE );
		return FALSE;
	}
	
	HRESULT hr = S_OK;
	hr = m_pNode->put_text( _bstr_t(strValue) );
	ASSERT( SUCCEEDED(hr) );

	return SUCCEEDED(hr);
}



//-------------------------------------------------------------------------
// Function Name    :GetName
// Parameter(s)     :void
// Return           :CString
// Memo             :获取当前节点名称
//-------------------------------------------------------------------------
CString CXmlNode::GetName(void)
{
	ASSERT( m_pNode != NULL );

	BSTR bstr = NULL;
	HRESULT hr = m_pNode->get_nodeName(&bstr);
	ASSERT( SUCCEEDED(hr) );	
	CString strRet( (LPCSTR)(LPCTSTR)_bstr_t( bstr, true) );

	if( bstr != NULL )
	{
		SysFreeString(bstr);
		bstr = NULL;
	}

	return strRet;
}






//-------------------------------------------------------------------------
// Function Name    :GetAttribute
// Parameter(s)     :CString strName		属性名
//					:LPCTSTR lpszDefault	默认值
// Return           :CString
// Memo             :获取属性值
//-------------------------------------------------------------------------
CString CXmlNode::GetAttribute( CString strName IN
							   , LPCTSTR lpszDefault /* = NULL */ IN)
{
	ASSERT( m_pNode != NULL );

	HRESULT hr = S_OK;
	MSXML2::IXMLDOMNamedNodeMapPtr pIXMLDOMNamedNodeMap = NULL;
	hr = m_pNode->get_attributes(&pIXMLDOMNamedNodeMap);
	ASSERT( SUCCEEDED(hr) );

	MSXML2::IXMLDOMNodePtr pIXMLDOMNode = NULL;
	pIXMLDOMNode = pIXMLDOMNamedNodeMap->getNamedItem( _bstr_t(strName) );
	
	CString strValue;
	if( pIXMLDOMNode == NULL )
	{
		strValue = lpszDefault;
		SetAttribute( strName, strValue);		
	}
	else
	{
		VARIANT varValue;
		hr = pIXMLDOMNode->get_nodeValue(&varValue);
		ASSERT( SUCCEEDED(hr) );

		strValue = (LPCTSTR)(_bstr_t)varValue;
	}

	pIXMLDOMNode = NULL;

	pIXMLDOMNamedNodeMap = NULL;
	return strValue;
}


//-------------------------------------------------------------------------
// Function Name    :SetAttribute
// Parameter(s)     :CString strValue	值
// Return           :BOOL
// Memo             :设置属性值
//-------------------------------------------------------------------------
BOOL CXmlNode::SetAttribute( CString strName IN
							, CString strValue IN)
{
	ASSERT( m_pNode != NULL );

	HRESULT hr = S_OK;
	MSXML2::IXMLDOMElementPtr pElement = NULL;
	pElement = static_cast<IXMLDOMElementPtr> (m_pNode);
	hr = pElement->setAttribute( _bstr_t(strName), _variant_t(strValue) );
	ASSERT( SUCCEEDED(hr) );

	SAFE_RELEASE(pElement);
    
	return SUCCEEDED(hr);
}

//-------------------------------------------------------------------------
// Function Name    :DelAttribute
// Parameter(s)     :CString strName IN 属性名称
// Return           :BOOL
// Memo             :删除属性
//-------------------------------------------------------------------------
BOOL CXmlNode::DelAttribute( CString strName IN )
{
	ASSERT( m_pNode != NULL );

	HRESULT hr = S_OK;
	MSXML2::IXMLDOMNamedNodeMapPtr pIXMLDOMNamedNodeMap = NULL;
	hr = m_pNode->get_attributes(&pIXMLDOMNamedNodeMap);
	ASSERT( SUCCEEDED(hr) );

	hr = SUCCEEDED(hr) ? 
		pIXMLDOMNamedNodeMap->removeNamedItem( _bstr_t(strName) )
		: hr;
	ASSERT( SUCCEEDED(hr) );

	SAFE_RELEASE(pIXMLDOMNamedNodeMap);
	return SUCCEEDED(hr);
}



//-------------------------------------------------------------------------
// Function Name    :HasChildren
// Parameter(s)     :
// Return           :
// Memo             :是否有子结点
//-------------------------------------------------------------------------
BOOL CXmlNode::HasChildren(void)
{
	ASSERT( m_pNode != NULL );

	BOOL bHasChildren = FALSE;

	HRESULT hr = S_OK;
	MSXML2::IXMLDOMNodePtr pNode = NULL;
	MSXML2::DOMNodeType NodeType;
	MSXML2::IXMLDOMNodeListPtr pNodeList = NULL;
	hr = m_pNode->get_childNodes(&pNodeList);
	ASSERT( SUCCEEDED(hr) );

	for( int i = 0; i < pNodeList->length; i++)
	{
		pNode = pNodeList->item[i];
		
		hr = pNode->get_nodeType(&NodeType);
		ASSERT( SUCCEEDED(hr) );
		
		SAFE_RELEASE(pNode);

		if( NodeType == MSXML2::NODE_ELEMENT )
		{
			bHasChildren = TRUE;
			break;
		}
	}

	SAFE_RELEASE(pNodeList);

	return bHasChildren;
}



//-------------------------------------------------------------------------
// Function Name    :Remove
// Parameter(s)     :
// Return           :
// Memo             :删除此结点
//-------------------------------------------------------------------------
BOOL CXmlNode::Remove(void)
{
	ASSERT( m_pNode != NULL );

	HRESULT hr = S_OK;
	MSXML2::IXMLDOMNodePtr pNode = NULL;
	hr = m_pNode->get_parentNode(&pNode);
	ASSERT( SUCCEEDED(hr) );
	
	pNode->removeChild(m_pNode);

	return TRUE;
}

//-------------------------------------------------------------------------
// Function Name    :RemoveChildren
// Parameter(s)     :
// Return           :BOOL
// Memo             :删除子结点
//-------------------------------------------------------------------------
BOOL CXmlNode::RemoveChildren(void)
{
	ASSERT( m_pNode != NULL );

	if( !HasChildren() )
		return FALSE;
	
	HRESULT hr = S_OK;
	MSXML2::IXMLDOMNodePtr pNode = NULL;
	MSXML2::DOMNodeType NodeType;
	MSXML2::IXMLDOMNodeListPtr pNodeList = NULL;
	hr = m_pNode->get_childNodes(&pNodeList);
	ASSERT( SUCCEEDED(hr) );

	for( int i = 0; i < pNodeList->length; i++)
	{
		pNode = pNodeList->item[i];
		
		hr = pNode->get_nodeType(&NodeType);
		ASSERT( SUCCEEDED(hr) );

		if( NodeType == MSXML2::NODE_ELEMENT )
		{
			m_pNode->removeChild(pNode);
		}

		SAFE_RELEASE(pNode);
	}

	SAFE_RELEASE(pNodeList);
	return TRUE;
}


//-------------------------------------------------------------------------
// Function Name    :GetChildren
// Parameter(s)     :void
// Return           :CXmlNodesPtr
// Memo             :获取子结点
//-------------------------------------------------------------------------
CXmlNodesPtr CXmlNode::GetChildren()
{
	ASSERT( m_pNode != NULL );

	HRESULT hr = S_OK;
	MSXML2::IXMLDOMNodeListPtr pNodeList = NULL;
	hr = m_pNode->get_childNodes(&pNodeList);
	ASSERT( SUCCEEDED(hr) );

	CXmlNodesPtr pNodes(new CXmlNodes(pNodeList));

	return pNodes;
}




//////////////////////////////////////////////////////////////////////////
// 下列为取值的重载方法

CString CXmlNode::GetString(LPCTSTR lpszValue /* = NULL */ IN)
{
	CString strValue;
	_GetValue(strValue);

	if( strValue.IsEmpty() &&
		lpszValue != NULL )
	{
		strValue = lpszValue;
		_SetValue(strValue);
	}

	return strValue;
}



BOOL CXmlNode::GetBOOL( BOOL bDefault /* = FALSE */ IN)
{
	CString strValue;
	_GetValue(strValue);

	if( strValue.CompareNoCase(_T("true")) == 0 )
	{
		return TRUE;
	}
	else if( strValue.CompareNoCase(_T("false")) == 0 )
	{
		return FALSE;
	}
	else
	{
		strValue = bDefault ? "true" : "false";
		_SetValue(strValue);
		return bDefault;
	}	
}


INT CXmlNode::GetInt( INT nDefault /* = 0 */ IN)
{
	CString strValue;
	_GetValue(strValue);

	if( strValue.IsEmpty() )
	{
		strValue.Format(_T("%ld"), nDefault);
		_SetValue(strValue);
	}

	return _wtoi(strValue);
}



DOUBLE CXmlNode::GetFloat( DOUBLE fDefault /* = 0.0 */)
{
	CString strValue;
	_GetValue(strValue);

	if( strValue.IsEmpty() )
	{
		strValue.Format(_T("%f"), fDefault);
		_SetValue(strValue);
	}

	return _wtof(strValue);
}


//////////////////////////////////////////////////////////////////////////
// 下列为赋值的重载方法

BOOL CXmlNode::SetString( LPCTSTR lpszValue )
{
	CString strValue(lpszValue);
	return _SetValue(strValue);
}


BOOL CXmlNode::SetBOOL( BOOL bValue )
{
	CString strValue;
	strValue = bValue ? _T("true") : _T("false");

	return _SetValue(strValue);
}

BOOL CXmlNode::SetInt( INT nValue )
{
	CString strValue;
	strValue.Format(_T("%ld"), nValue);

	return _SetValue(strValue);
}

BOOL CXmlNode::SetFloat( FLOAT fValue )
{
	CString strValue;
	strValue.Format(_T("%f"), fValue);

	return _SetValue(strValue);
}

//////////////////////////////////////////////////////////////////////////

