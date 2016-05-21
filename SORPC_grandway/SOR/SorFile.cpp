#include "StdAfx.h"
#define _CLASSINDLL//
#include "SorFile.h"
#include "SorFileBase.h"

CSorFile::CSorFile(void)
{
	m_lpVoid = new CSorFileBase();
}

CSorFile::~CSorFile(void)
{
	if (m_lpVoid)
	{
		delete[] m_lpVoid;
	}
}
////INT CSorFile::readSorFile(CString filePathAndName,TestResultInfo& testResultInfo)//cwcq2012.07.23
////{
////	return ( (CSorFileBase*)m_lpVoid )->readSorFile(filePathAndName,testResultInfo);
////}
////INT CSorFile::saveSorFile(CString filePathAndName,TestResultInfo testResultInfo)//cwcq2012.07.23
////{
////	return ( (CSorFileBase*)m_lpVoid )->saveSorFile(filePathAndName,testResultInfo);
////}

INT CSorFile::readSorFile(CString filePathAndName,SORDATA& sorData)//wcq2012.07.23
{
	return ( (CSorFileBase*)m_lpVoid )->readSorFile(filePathAndName,sorData);
}
INT CSorFile::saveSorFile(CString filePathAndName,SORDATA sorData)//wcq2012.07.23
{
	return ( (CSorFileBase*)m_lpVoid )->saveSorFile(filePathAndName,sorData);
}
INT CSorFile::readSorFile(BYTE* pData, int nLength, SORDATA &sorData)
{
	return ( (CSorFileBase*)m_lpVoid )->readSorFile(pData, nLength,sorData);

}