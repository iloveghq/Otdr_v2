#pragma once


#import "msxml6.dll"
#include <memory>
using namespace MSXML2;


#define SAFE_RELEASE(x)	\
	if( x != NULL )		\
	{					\
		x.Release();	\
		x = NULL;		\
	}


class CXml;

class CXmlNode;
typedef std::auto_ptr<CXmlNode>  CXmlNodePtr;

class CXmlNodes;
typedef std::auto_ptr<CXmlNodes>  CXmlNodesPtr;



#include "XmlNodes.h"



class CXmlNode
{
	friend class CXml;
	friend class CXmlNode;
	friend class CXmlNodes;


public:	
	~CXmlNode(void);

protected:
	
	MSXML2::IXMLDOMNodePtr   m_pNode;

	CXmlNode( MSXML2::IXMLDOMNodePtr pNode);	
	BOOL _GetValue(CString & strValue);
	BOOL _SetValue(CString & strValue);

public:

	//		_________________
	//______|	构造函数	|___________________________________________________________________
	CXmlNode(void);
	CXmlNode( CXmlNode & refNode IN);


	//		_________________
	//______|	重载 = 号	|___________________________________________________________________
	CXmlNodePtr operator = ( CXmlNodePtr pNode);
	CXmlNode & operator = ( CXmlNode & refNode);



	//		_________________
	//______|  获取结点名称	|___________________________________________________________________
	CString GetName(void);
	


	//		_________________
	//______|	属性操作	|___________________________________________________________________
	CString GetAttribute( CString strName, LPCTSTR lpszDefault = NULL);
	BOOL SetAttribute(  CString strName, CString strValue);
	BOOL DelAttribute( CString strName );



	//		_________________
	//______|	子结点操作	|___________________________________________________________________
	CXmlNodePtr GetChild(CString strName);
	CXmlNodesPtr GetChildren();
	BOOL HasChildren(void);
	BOOL RemoveChildren(void);
	


	//		_________________
	//______|	删除结点	|___________________________________________________________________
	BOOL Remove(void);
	


	//		_________________
	//______|	取值操作	|___________________________________________________________________	
	CString GetString( LPCTSTR lpszDefault = NULL );
	BOOL GetBOOL( BOOL bDefault = FALSE );
	INT GetInt( INT nDefault = 0 );
	DOUBLE GetFloat( DOUBLE fDefault = 0.0f );
	


	//		_________________
	//______|	赋值操作	|___________________________________________________________________
	BOOL SetString( LPCTSTR lpszValue );
	BOOL SetBOOL( BOOL bValue );
	BOOL SetInt( INT nValue );
	BOOL SetFloat( FLOAT fValue );
	
	BOOL IsNull(void);
};
