#define _AFXEXT
#include "StdAfx.h"
#include "SorFileBase.h"
void writeShort(vector<BYTE>& vData,short nToWrite);
void writeInt(vector<BYTE>& vData,INT nToWrite);
void writeString(vector<BYTE>& vData,vector<char> vChar);//写入字符串，加'\0'
void writeString(vector<BYTE>& vData,vector<char> vChar,int len);//写入固定长度字符，不加'\0'

CSorFileBase::CSorFileBase(void)
{
}

CSorFileBase::~CSorFileBase(void)
{
}
//初始化一些参数，给一些变量赋默认值，刚测试完的数据保存时需要初始化
void CSorFileBase::init()
{
	////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////
	//Map 
	memset(&m_mapBlock,0,sizeof(MapBlock));
	vchar2string(m_mapBlock.mapBlock.ID,string("Map"));
	m_mapBlock.mapBlock.RevisionNo = REVISION_NUM;

	MapBlock::blockInfo bInfo;
	const int paramsCount = 6;//不包含LnkParams
	string paras[paramsCount]={"GenParams","SupParams","FxdParams","KeyEvents",/*"LnkParams",*/"DataPts","Cksum"};
	for (int i=0;i<paramsCount;i++)
	{
		vchar2string(bInfo.ID,paras[i]);
		bInfo.RevisionNo = REVISION_NUM;
		m_mapBlock.vBlockInfo.push_back(bInfo);
	}
	m_mapBlock.blockCount = paramsCount +1 ;//包含map本身

	////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////
	//GenParams/GeneralParas
	memset(&m_generalParas,0,sizeof(GeneralParas));
	vchar2string(m_generalParas.ID,string("GenParams"));
	vchar2string(m_generalParas.languageCode,string("CH"));
	//m_generalParas.languageCode.push_back('C');
	//m_generalParas.languageCode.push_back('H');
	vchar2string(m_generalParas.currentDataFlag,string("BC"));

	////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////
	//SupParams/SuppliersParas
	memset(&m_suppliersParas,0,sizeof(SuppliersParas));
	vchar2string(m_suppliersParas.ID,string("SupParams"));
	vchar2string(m_suppliersParas.name,string("Grandway"));

	////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////
	//FxdParams/FixedParas
	memset(&m_fixedParas,0,sizeof(FixedParas));
	vchar2string(m_fixedParas.ID,string("FxdParams"));
	m_fixedParas.vPulseWidth.push_back(10000);//默认10us
	m_fixedParas.vDataSpacing.push_back(0);
	m_fixedParas.vDataPointsEach.push_back(0);
	vchar2string(m_fixedParas.distanceUnit,string("km"));
	vchar2string(m_fixedParas.traceType,string("ST"));
	m_fixedParas.pulseWidthNum=1;//脉宽个数为1
	m_fixedParas.windowCoor1 = 0;
	m_fixedParas.windowCoor2 = 0;
	m_fixedParas.windowCoor3 = 0;
	m_fixedParas.windowCoor4 = 0;

	////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////
	//KeyEvents
	memset(&m_keyEvent,0,sizeof(KeyEvent));
	vchar2string(m_keyEvent.ID,string("KeyEvents"));
	m_keyEvent.keyEventNum = 0;//事件点个数默认为0

#ifdef B_INCLUDE_LNKPARAMS
	////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////
	//LnkParams/linkParams
	memset(&m_linkParas,0,sizeof(LinkParas));
	vchar2string(m_linkParas.ID,string("LnkParams"));
#endif

	////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////
	//DataPts
	memset(&m_dataPoints,0,sizeof(DataPoints));
	vchar2string(m_dataPoints.ID,string("DataPts"));
	m_dataPoints.dataPointNum=0;//
	m_dataPoints.totalScaleFactors=1;
	DataPoints::DataUseScaleFactor dus;
	dus.scaleFactor = 1000;
	m_dataPoints.vAllData.push_back(dus);

	////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////
	//Cksum
	memset(&m_checksum,0,sizeof(Checksum));
	vchar2string(m_checksum.ID,string("Cksum"));
}

////将解析后的数据赋给传入的参数
//BOOL CSorFileBase::setReturnData(TestResultInfo& testResultInfo)
//{
//	//
//	if (NULL !=testResultInfo.pDataPoints)//先清空原有的各点数据
//	{
//		delete[] testResultInfo.pDataPoints;//
//		testResultInfo.pDataPoints= NULL;
//	}
//	testResultInfo.DataPointsNum = m_dataPoints.dataPointNum;//数据点个数
//	if (0 != m_dataPoints.dataPointNum)
//	{
//		testResultInfo.pDataPoints = new WORD[m_dataPoints.vAllData[0].count];//只考虑只有一个缩放因子情形????????????????????????
//		memcpy(testResultInfo.pDataPoints,&(m_dataPoints.vAllData[0].vData[0]),m_dataPoints.vAllData[0].count * 2);////memcpy按字节，个数要乘2
//		//if (1000 != m_dataPoints.vAllData[0].scaleFactor)//如果系数不一样，则乘以相应的系数//wcq2011.11.30 先不考虑????
//		//{
//		//	int count=m_dataPoints.vAllData[0].count;
//		//	float factor = m_dataPoints.vAllData[0].scaleFactor * 1.0/1000;
//		//	for(int i=0;i<count;i++)
//		//	{
//		//		testResultInfo.pDataPoints[i] = testResultInfo.pDataPoints[i] * factor;
//		//	}
//		//}
//	}
//	//缩放系数wcq2011.10.21
//	testResultInfo.ScaleFactor = m_dataPoints.vAllData[0].scaleFactor;
//
//	//计算测量距离
//	float fC=299792458.0;
//	float fReflect=m_fixedParas.groupIndex /100000.0;//折射率
//	float numEachTime=m_fixedParas.vDataSpacing[0]/100000000000000.0;//两点间时间//wcq2011.11.25
//	//DEBUGMSG(true,(_T("numEachTime :%.2f\n"),numEachTime));
//	numEachTime = C_LIGHT_SPEED * (numEachTime) / fReflect;//每点距离 =两点间时间 * 光在光纤速度//wcq2011.11.25
//	//DEBUGMSG(true,(_T("numEachTime 2:%.2f\n"),numEachTime));
//	//testResultInfo.Length =C_LIGHT_SPEED * (numEachTime) * m_fixedParas.vDataPointsEach[0] /fReflect;//
//	testResultInfo.Length = numEachTime * m_fixedParas.vDataPointsEach[0];//每点距离 * 点数//wcq2011.11.25
//	////DataSpacing:for each pulse width used:one-way time(units of 100 psec) for 10000 data points//wcq2011.10.19理解为两点间距离
//	//折射率 两点间距离
//	testResultInfo.Refraction = fReflect;
//	//testResultInfo.DataSpacing = numEachTime;
//	testResultInfo.DataSpacing = numEachTime;//wcq2011.11.25//m_fixedParas.vDataSpacing[0];//2011.10.21
//	//DEBUGMSG(true,(_T("numEachTime :%.2f   DataSpacing :%.2f\n"),testResultInfo.DataSpacing,testResultInfo.DataSpacing));
//	//波长
//	//testResultInfo.WaveLength = m_fixedParas.actualWavelength;
//	testResultInfo.WaveLength = m_fixedParas.actualWavelength/10;//wcq2012.02.07
//
//	//脉宽
//	testResultInfo.PulseWidth = m_fixedParas.vPulseWidth[0];
//
//	//
//	//采样频率
//	//testResultInfo.Frequency = (1/numEachTime)*1000000;
//	testResultInfo.Frequency = 100000000.0 / (m_fixedParas.vDataSpacing[0] * 2);//wcq2012.06.12
//
//	//事件列表 wcq2012.06.11
//	testResultInfo.EventList.vEventInfo.clear();
//	int count = m_keyEvent.vEvent.size();
//	if (count > 0)//如果文件中有，则赋值
//	{
//		for (int i=0;i<count;i++)
//		{
//			EventInfo ei;
//			if (m_keyEvent.vEvent[i].code.size()>5)//实际长度应该为6
//			{
//				ei.nEventType = m_keyEvent.vEvent[i].code[0] - '0';//事件类型
//				ei.cEventType = m_keyEvent.vEvent[i].code[1];//事件是生成的还是用户添加或是尾端事件等
//			}//eif
//			///ei.fLocation = (m_keyEvent.vEvent[i].propagationTime / (testResultInfo.Refraction  * pow(10.0,-10))) * C_LIGHT_SPEED;//位置
//
//			ei.fLocation = m_keyEvent.vEvent[i].propagationTime * pow(10.0,-10) * C_LIGHT_SPEED / (testResultInfo.Refraction ) ;//位置
//			//event.propagationTime = (testResultInfo.EventList.vEventInfo[i].fLocation * pow(10.0,-10)/C_LIGHT_SPEED ) * testResultInfo.Refraction;//距离/光速(km/ps) * 折射率 位置
//			ei.index = (ei.fLocation / testResultInfo.Length)*testResultInfo.DataPointsNum;//曲线数据数组下标,这里没有计算?????????????????????????????????????
//			//防止超出范围
//			if (ei.index <0)
//			{
//				ei.index =0;
//			}
//			else if (ei.index > testResultInfo.DataPointsNum)
//			{
//				ei.index = testResultInfo.DataPointsNum;//
//			}//eif
//			ei.fEventloss = m_keyEvent.vEvent[i].eventLoss / 1000.0;//损耗
//			ei.fEventHigh = m_keyEvent.vEvent[i].reflectance /1000.0;//反射dB在otdr曲线事件列表
//			ei.fLineK = m_keyEvent.vEvent[i].attenCoefficent / 1000.0;//斜率
//			//ei.fTotalLoss = m_keyEvent.vEvent[i]//总损耗????????????????????
//			if (0 == i)
//			{
//				ei.fTotalLoss = ei.fEventloss;
//			}
//			else
//			{
//				ei.fTotalLoss = testResultInfo.EventList.vEventInfo[i-1].fTotalLoss/*前一事件总损耗*/ + ei.fEventloss/*当前事件损耗*/ + (ei.fLineK/1000) *(ei.fLocation - testResultInfo.EventList.vEventInfo[i-1].fLocation);
//			}//eif
//			ei.fReflect = 0;//这个参数没有用到
//			ei.fTrailEnd =0;////这个参数没有用到C/(2nf))*index
//			testResultInfo.EventList.vEventInfo.push_back(ei);
//		}//efor
//	}//eif count > 0
//	return TRUE;
//}
////////////////////////////////////////////////////////////////////////////////////
//函数说明://打开文件后解析各个block，会调用下面几个read函数
//入口参数:data:文件数据;len：文件长度
//出口参数:
//建立时间:2011.10.11
//Write By:wcq
////////////////////////////////////////////////////////////////////////////////////
BOOL CSorFileBase::parseData(BYTE fileData[],int len)
{
	//清空其他OTDR设备厂商自定义的Block数据
	for(int i=0;i<m_vUnknownParas.size();i++)//各个Block清空
	{
		m_vUnknownParas[i].clear();
	}
	m_vUnknownParas.clear();//本身清空
	int index=0;//文件下标
	//string temp;
	char* pChar;
	int IDLen=0;
	if(readMap(fileData))//查看是否为.sor文件格式，如果是，读取Map数据块
	{
		if (!checkSize(len))//文件大小和各个Block之和不一样，则认为文件无效,防止文件读取有bug.
		{
			//delete[] fileData;//删除临时变量2011.12.01
			return FALSE;
		}

		index += m_mapBlock.mapBlock.Size;//加上map Block的大小
		int count=m_mapBlock.blockCount-1;//总共有多少个Block,减去Map Block本身
		for (int i=0;i<count;i++)
		{
			//vchar2string(temp,m_mapBlock.vBlockInfo[i].ID);
			IDLen=m_mapBlock.vBlockInfo[i].ID.size();
			pChar=&m_mapBlock.vBlockInfo[i].ID[0];
			if (0 == strncmp(pChar,"GenParams",IDLen))//
			{
				readGeneralParas(fileData + index);
			}
			else if (0 == strncmp(pChar,"SupParams",IDLen))
			{
				readSuppliersParas(fileData + index);
			}
			else if (0 == strncmp(pChar,"FxdParams",IDLen))
			{
				readFixedParas(fileData + index);
			}
			else if (0 == strncmp(pChar,"KeyEvents",IDLen))
			{
				readKeyEvents(fileData + index);
			}
			else if (0 == strncmp(pChar,"LnkParams",IDLen))
			{
				readLinkParas(fileData + index);
			}
			else if (0 == strncmp(pChar,"DataPts",IDLen))
			{
				readDataPts(fileData + index);
			}
			else if (0 == strncmp(pChar,"Cksum",IDLen) || 0 == strncmp(pChar,"CKsum",IDLen))//标准应为Cksum,但是得力的用的是CKsum
			{
				readCKsum(fileData + index);
			}
			else
			{
				vector<BYTE> vParams(fileData + index,fileData + index + m_mapBlock.vBlockInfo[i].Size);
				m_vUnknownParas.push_back(vParams);
			}////eif
			index += m_mapBlock.vBlockInfo[i].Size;
		}//efor
	}//eif
	return TRUE;
}
////////////////////////////////////////////////////////////////////////////////////
//函数说明:读取MapBlock。
//入口参数:data:BYTE数组;
//出口参数:
//建立时间:2011.7.8
//Write By:wcq
////////////////////////////////////////////////////////////////////////////////////
INT CSorFileBase::readMap(BYTE data[])//
{
	vchar2string(m_mapBlock.mapBlock.ID,string("Map"));
	int blockLen=0;//block长度
	//Map标志
	//if(_T("Map")!=readString(data,0))//改为判断
	if('M'!=data[0] || 'a'!=data[1] || 'p'!=data[2] || '\0'!=data[3])
	{
		return FALSE;
	}
	//
	//m_parsePos=4;//修订版本号起始位置
	blockLen=4;
	m_mapBlock.mapBlock.RevisionNo=TWOINT(data,blockLen);
	//m_parsePos=6;//长度起始位置
	blockLen=6;
	m_mapBlock.mapBlock.Size=FOURINT(data,blockLen);
	//m_parsePos=10;//Block个数起始位置
	blockLen=10;//Block个数起始位置
	INT count=TWOINT(data,blockLen);//Block个数
	m_mapBlock.blockCount=count;
	count--;//减去Map Block本身
	MapBlock::blockInfo block;
	//m_parsePos=12;//第一个block位置
	blockLen=12;//第一个block位置
	m_mapBlock.vBlockInfo.clear();//清空以前的数据块记录
	int strLen=0;
	for (int i=0;i<count;i++)
	{//int readString(BYTE data[],vector<char> vChar)
		//block.ID=readString(data,m_parsePos);//名称
		strLen=readString(data+blockLen,block.ID);
		//m_parsePos+=block.ID.GetLength()+1;//修订版本号起始位置
		blockLen+=strLen+1;//修订版本号起始位置
		//block.RevisionNo=TWOINT(data,m_parsePos);
		block.RevisionNo=TWOINT(data,blockLen);
		//m_parsePos+=2;//长度起始位置
		blockLen+=2;//长度起始位置
		//block.Size=FOURINT(data,m_parsePos);
		block.Size=FOURINT(data,blockLen);
		//m_parsePos+=4;//
		blockLen+=4;//
		m_mapBlock.vBlockInfo.push_back(block);
	}
	return blockLen;
}

////////////////////////////////////////////////////////////////////////////////////
//函数说明:读取GeneralParas Block。测试通过7.8
//入口参数:data:BYTE数组
//出口参数:
//建立时间:2011.7.8
//Write By:wcq
////////////////////////////////////////////////////////////////////////////////////
INT CSorFileBase::readGeneralParas(BYTE data[])
{
	vchar2string(m_generalParas.ID,string("GenParams"));
	int blockLen=0;//block长度
	int strLen=0;
	blockLen = 10;//语言编码起始位置,跳过GenParams\0
	m_generalParas.languageCode.clear();
	m_generalParas.languageCode.push_back(data[blockLen]);
	m_generalParas.languageCode.push_back(data[blockLen+1]);
	//m_parsePos+=2;//电缆编号起始位置
	blockLen += 2;//电缆编号起始位置

	//m_generalParas.cableID=readString(data,m_parsePos);//电缆编号
	strLen=readString(data + blockLen,m_generalParas.cableID);
	blockLen += strLen+1;//光纤编号起始位置

	//m_generalParas.fiberID=readString(data,m_parsePos);//光纤编号
	strLen=readString(data + blockLen,m_generalParas.fiberID);
	//m_parsePos+=m_generalParas.fiberID.GetLength()+1;//光纤编号起始位置
	blockLen += strLen+1;//光纤编号起始位置

	//m_generalParas.fiberType=TWOINT(data,m_parsePos);//光纤类型
	m_generalParas.fiberType=TWOINT(data,blockLen);//光纤类型

	//m_parsePos+=2;//额定波长起始位置
	blockLen+=2;//额定波长起始位置
	m_generalParas.nominalWaveLength=TWOINT(data,blockLen);//额定波长

	//m_parsePos+=2;//起始位置
	blockLen+=2;//起始位置
	//m_generalParas.OLocation=readString(data,m_parsePos);//Originating Location
	strLen=readString(data + blockLen,m_generalParas.OLocation);

	//m_parsePos+=m_generalParas.OLocation.GetLength()+1;//起始位置
	blockLen += strLen+1;//起始位置
	//m_generalParas.TLocation=readString(data,m_parsePos);//Terminating Location
	strLen=readString(data + blockLen,m_generalParas.TLocation);

	//m_parsePos+=m_generalParas.TLocation.GetLength()+1;//起始位置
	blockLen += strLen+1;//起始位置
	//m_generalParas.cableCode=readString(data,m_parsePos);//
	strLen=readString(data + blockLen,m_generalParas.cableCode);

	//m_parsePos+=m_generalParas.cableCode.GetLength()+1;//起始位置
	blockLen += strLen+1;//起始位置
	//temp.Format(_T("%C%C"),data[m_parsePos],data[m_parsePos+1]);
	//m_generalParas.currentDataFlag=temp;//Terminating Location
	m_generalParas.currentDataFlag.clear();
	m_generalParas.currentDataFlag.push_back(data[blockLen]);
	m_generalParas.currentDataFlag.push_back(data[blockLen+1]);

	//m_parsePos+=2;//起始位置
	blockLen += 2;//起始位置
	//m_generalParas.userOffset=FOURINT(data,m_parsePos);//
	m_generalParas.userOffset=FOURINT(data,blockLen);//

	//m_parsePos+=4;//起始位置
	blockLen += 4;//起始位置
	m_generalParas.uODistance=FOURINT(data,blockLen);//user Offset Distance

	//m_parsePos+=4;//操作人员起始位置
	blockLen += 4;//起始位置
	//m_generalParas.operater=readString(data,m_parsePos);//操作人员
	strLen=readString(data + blockLen,m_generalParas.operater);

	//m_parsePos+=m_generalParas.operater.GetLength()+1;//注释起始位置
	blockLen += strLen+1;//起始位置
	//m_generalParas.comment=readString(data,m_parsePos);//注释
	strLen=readString(data + blockLen,m_generalParas.comment);

	//m_parsePos+=m_generalParas.comment.GetLength()+1;//下一记录起始位置
	blockLen += strLen+1;//起始位置

	return blockLen;
}

////////////////////////////////////////////////////////////////////////////////////
//函数说明:读取SuppliersParas Block。测试通过7.8
//入口参数:data:BYTE数组
//出口参数:
//建立时间:2011.7.8
//Write By:wcq
////////////////////////////////////////////////////////////////////////////////////
INT CSorFileBase::readSuppliersParas(BYTE data[])
{
	vchar2string(m_suppliersParas.ID,string("SupParams"));
	int blockLen=0;//block长度
	int strLen=0;
	blockLen+=10;//厂商名称起始位置,跳过SupParams\0
	strLen=readString(data + blockLen,m_suppliersParas.name);

	blockLen+=strLen+1;//主面板型号起始位置
	strLen=readString(data + blockLen,m_suppliersParas.mainframeID);//主面板型号

	blockLen+=strLen+1;//主面板序列号起始位置
	strLen=readString(data + blockLen,m_suppliersParas.mainframeSn);//主面板序列号

	blockLen+=strLen+1;//插件模板号起始位置
	strLen=readString(data + blockLen,m_suppliersParas.moduleID);//插件模板号

	blockLen+=strLen+1;//插件（plug-in）序列号起始位置
	strLen=readString(data + blockLen,m_suppliersParas.moduleSn);//插件（plug-in）序列号

	blockLen+=strLen+1;//软硬件版本号起始位置
	strLen=readString(data + blockLen,m_suppliersParas.softVersion);//软硬件版本号

	blockLen+=strLen+1;//其它信息起始位置
	strLen=readString(data + blockLen,m_suppliersParas.other);//其它信息

	blockLen+=strLen+1;//下一记录起始位置
	return blockLen;
	//m_parsePos+=10;//厂商名称起始位置,跳过SupParams\0
	//m_suppliersParas.name=readString(data,m_parsePos);//厂商名称

	//m_parsePos+=m_suppliersParas.name.GetLength()+1;//主面板型号起始位置
	//m_suppliersParas.mainframeID=readString(data,m_parsePos);//主面板型号

	//m_parsePos+=m_suppliersParas.mainframeID.GetLength()+1;//主面板序列号起始位置
	//m_suppliersParas.mainframeSn=readString(data,m_parsePos);//主面板序列号

	//m_parsePos+=m_suppliersParas.mainframeSn.GetLength()+1;//插件模板号起始位置
	//m_suppliersParas.moduleID=readString(data,m_parsePos);//插件模板号

	//m_parsePos+=m_suppliersParas.moduleID.GetLength()+1;//插件（plug-in）序列号起始位置
	//m_suppliersParas.moduleSn=readString(data,m_parsePos);//插件（plug-in）序列号

	//m_parsePos+=m_suppliersParas.moduleSn.GetLength()+1;//软硬件版本号起始位置
	//m_suppliersParas.softVersion=readString(data,m_parsePos);//软硬件版本号

	//m_parsePos+=m_suppliersParas.softVersion.GetLength()+1;//其它信息起始位置
	//m_suppliersParas.other=readString(data,m_parsePos);//其它信息

	//m_parsePos+=m_suppliersParas.other.GetLength()+1;//下一记录起始位置

	//return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////
//函数说明:读取FxdParams Block。
//入口参数:data:BYTE数组
//出口参数:
//建立时间:2011.7.8
//Write By:wcq
////////////////////////////////////////////////////////////////////////////////////
INT CSorFileBase::readFixedParas(BYTE data[])
{
	vchar2string(m_fixedParas.ID,string("FxdParams"));
	int blockLen=0;//block长度
	int strLen=0;
	blockLen+=10;//时间起始位置,跳过FxdParams\0
	m_fixedParas.timestamp=FOURINT(data,blockLen);//

	blockLen+=4;//单位起始位置
	m_fixedParas.distanceUnit.clear();
	m_fixedParas.distanceUnit.push_back(data[blockLen]);
	m_fixedParas.distanceUnit.push_back(data[blockLen+1]);

	blockLen+=2;//实际测量波长起始位置
	m_fixedParas.actualWavelength=TWOINT(data,blockLen);//

	blockLen+=2;//acquisition Offset波长起始位置
	m_fixedParas.acquisitionOffset=FOURINT(data,blockLen);//

	blockLen+=4;//acquisition Offset Distance起始位置
	m_fixedParas.AODistance=FOURINT(data,blockLen);//

	blockLen+=4;//脉宽个数起始位置
	int itemp=TWOINT(data,blockLen);
	m_fixedParas.pulseWidthNum=itemp;//

	blockLen+=2;//起始位置
	m_fixedParas.vPulseWidth.clear();
	for(int i=0;i<itemp;i++)
	{
		m_fixedParas.vPulseWidth.push_back(TWOINT(data,blockLen));
		blockLen+=2;//下一个位置
	}

	//blockLen+=2;//已在上面循环了加过
	//不同脉宽，10000个数据点时间起始位置
	m_fixedParas.vDataSpacing.clear();
	for(int i=0;i<itemp;i++)
	{
		m_fixedParas.vDataSpacing.push_back(FOURINT(data,blockLen));
		blockLen+=4;//下一个位置
	}

	//blockLen+=4;//已在上面循环了加过
	//每个脉宽数据点个数 起始位置
	m_fixedParas.vDataPointsEach.clear();
	for(int i=0;i<itemp;i++)
	{
		m_fixedParas.vDataPointsEach.push_back(FOURINT(data,blockLen));
		blockLen+=4;//下一个位置
	}

	//blockLen+=4;//group Index已在上面循环中加过.起始位置
	m_fixedParas.groupIndex=FOURINT(data,blockLen);

	blockLen+=4;//backscatter 起始位置
	m_fixedParas.backscatter=TWOINT(data,blockLen);

	blockLen+=2;//average number起始位置
	m_fixedParas.avgNum=FOURINT(data,blockLen);

	blockLen+=4;//average time 起始位置
	m_fixedParas.avgTime=TWOINT(data,blockLen);

	blockLen+=2;//acquisitionRange起始位置
	m_fixedParas.acquisitionRange=FOURINT(data,blockLen);

	blockLen+=4;//acquisitionRange distance起始位置
	m_fixedParas.ARDistance=FOURINT(data,blockLen);

	blockLen+=4;//front Panel Offset起始位置
	m_fixedParas.frontPanelOff=FOURINT(data,blockLen);

	blockLen+=4;//noise Floor Level 起始位置
	m_fixedParas.noiseFloorLevel=TWOINT(data,blockLen);

	blockLen+=2;//噪声级别系数 起始位置
	m_fixedParas.noiseFloorFactor=TWOINT(data,blockLen);

	blockLen+=2;//power offset first point起始位置
	m_fixedParas.powerOff=TWOINT(data,blockLen);

	blockLen+=2;//loss Threshold起始位置
	m_fixedParas.lossThreshold=TWOINT(data,blockLen);

	blockLen+=2;//反射域起始位置
	m_fixedParas.reflectanceThreshod=TWOINT(data,blockLen);

	blockLen+=2;//end-of-fiber threshold起始位置
	m_fixedParas.eofThreshold=TWOINT(data,blockLen);

	blockLen+=2;//trace Type追踪类型起始位置,标准上说是string，实际上是string 固定2字节，后面没\0
	//m_fixedParas.traceType=readString(data,blockLen);//
	m_fixedParas.traceType.clear();
	m_fixedParas.traceType.push_back(data[blockLen]);
	m_fixedParas.traceType.push_back(data[blockLen+1]);

	blockLen+=2;//window coordinates起始位置
	m_fixedParas.windowCoor1=FOURINT(data,blockLen);
	blockLen+=4;//
	m_fixedParas.windowCoor2=FOURINT(data,blockLen);
	blockLen+=4;//
	m_fixedParas.windowCoor3=FOURINT(data,blockLen);
	blockLen+=4;//
	m_fixedParas.windowCoor4=FOURINT(data,blockLen);


	blockLen+=4;//下一记录起始位置

	return blockLen;

	//CString temp;
	//m_parsePos+=10;//时间起始位置,跳过FxdParams\0
	//m_fixedParas.timestamp=FOURINT(data,m_parsePos);//

	//m_parsePos+=4;//单位起始位置
	//temp.Format(_T("%C%C"),data[m_parsePos],data[m_parsePos+1]);
	//m_fixedParas.distanceUnit=temp;//单位

	//m_parsePos+=2;//实际测量波长起始位置
	//m_fixedParas.actualWavelength=TWOINT(data,m_parsePos);//

	//m_parsePos+=2;//acquisition Offset波长起始位置
	//m_fixedParas.acquisitionOffset=FOURINT(data,m_parsePos);//

	//m_parsePos+=4;//acquisition Offset Distance起始位置
	//m_fixedParas.AODistance=FOURINT(data,m_parsePos);//

	//m_parsePos+=4;//脉宽个数起始位置
	//int itemp=TWOINT(data,m_parsePos);
	//m_fixedParas.pulseWidthNum=itemp;//

	//m_parsePos+=2;//起始位置
	//m_fixedParas.vPulseWidth.clear();
	//for(int i=0;i<itemp;i++)
	//{
	//	m_fixedParas.vPulseWidth.push_back(TWOINT(data,m_parsePos));
	//	m_parsePos+=2;//下一个位置
	//}

	////m_parsePos+=2;//已在上面循环了加过
	////不同脉宽，10000个数据点时间起始位置
	//m_fixedParas.vDataSpacing.clear();
	//for(int i=0;i<itemp;i++)
	//{
	//	m_fixedParas.vDataSpacing.push_back(FOURINT(data,m_parsePos));
	//	m_parsePos+=4;//下一个位置
	//}

	////m_parsePos+=4;//已在上面循环了加过
	////每个脉宽数据点个数 起始位置
	//m_fixedParas.vDataPointsEach.clear();
	//for(int i=0;i<itemp;i++)
	//{
	//	m_fixedParas.vDataPointsEach.push_back(FOURINT(data,m_parsePos));
	//	m_parsePos+=4;//下一个位置
	//}

	////m_parsePos+=4;//group Index已在上面循环中加过.起始位置
	//m_fixedParas.groupIndex=FOURINT(data,m_parsePos);

	//m_parsePos+=4;//backscatter 起始位置
	//m_fixedParas.backscatter=TWOINT(data,m_parsePos);

	//m_parsePos+=2;//average number起始位置
	//m_fixedParas.avgNum=FOURINT(data,m_parsePos);

	//m_parsePos+=4;//average time 起始位置
	//m_fixedParas.avgTime=TWOINT(data,m_parsePos);

	//m_parsePos+=2;//acquisitionRange起始位置
	//m_fixedParas.acquisitionRange=FOURINT(data,m_parsePos);

	//m_parsePos+=4;//acquisitionRange distance起始位置
	//m_fixedParas.ARDistance=FOURINT(data,m_parsePos);

	//m_parsePos+=4;//front Panel Offset起始位置
	//m_fixedParas.frontPanelOff=FOURINT(data,m_parsePos);

	//m_parsePos+=4;//noise Floor Level 起始位置
	//m_fixedParas.noiseFloorLevel=TWOINT(data,m_parsePos);

	//m_parsePos+=2;//噪声级别系数 起始位置
	//m_fixedParas.noiseFloorFactor=TWOINT(data,m_parsePos);

	//m_parsePos+=2;//power offset first point起始位置
	//m_fixedParas.powerOff=TWOINT(data,m_parsePos);

	//m_parsePos+=2;//loss Threshold起始位置
	//m_fixedParas.lossThreshold=TWOINT(data,m_parsePos);

	//m_parsePos+=2;//反射域起始位置
	//m_fixedParas.reflectanceThreshod=TWOINT(data,m_parsePos);

	//m_parsePos+=2;//end-of-fiber threshold起始位置
	//m_fixedParas.eofThreshold=TWOINT(data,m_parsePos);

	//m_parsePos+=2;//trace Type追踪类型起始位置
	//m_fixedParas.traceType=readString(data,m_parsePos);//

	//m_parsePos+=m_fixedParas.traceType.GetLength()+1;//window coordinates起始位置
	//m_fixedParas.windowCoor1=FOURINT(data,m_parsePos);
	//m_parsePos+=4;//
	//m_fixedParas.windowCoor2=FOURINT(data,m_parsePos);
	//m_parsePos+=4;//
	//m_fixedParas.windowCoor3=FOURINT(data,m_parsePos);
	//m_parsePos+=4;//
	//m_fixedParas.windowCoor4=FOURINT(data,m_parsePos);


	//m_parsePos+=4;//下一记录起始位置

	//return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////
//函数说明:读取readKeyEvents Block。还没测试?????????????????????????????????????????????
//入口参数:data:BYTE数组
//出口参数:
//建立时间:2011.7.8
//Write By:wcq
////////////////////////////////////////////////////////////////////////////////////
INT CSorFileBase::readKeyEvents(BYTE data[])
{
	vchar2string(m_keyEvent.ID,string("KeyEvents"));
	int blockLen=0;//block长度
	int strLen=0;
	blockLen+=10;//事件个数起始位置,跳过KeyEvents\0
	int count=TWOINT(data,blockLen);
	m_keyEvent.keyEventNum=count;//

	KeyEvent::Event event;
	blockLen+=2;//第一个事件起始位置
	m_keyEvent.vEvent.clear();
	for (int i=0;i<count;i++)
	{
		event.eventNum=TWOINT(data,blockLen);//编号

		blockLen+=2;//连接后第一个e事件点时间起始位置
		event.propagationTime=FOURINT(data,blockLen);//

		blockLen+=4;//衰减系数 起始位置
		event.attenCoefficent=TWOINT(data,blockLen);//

		blockLen+=2;//事件损耗 起始位置
		event.eventLoss=TWOINT(data,blockLen);//

		blockLen+=2;//事件反射 起始位置
		event.reflectance=FOURINT(data,blockLen);//

		blockLen+=4;//事件标志 起始位置
		//temp.Format(_T("%C%C%C%C%C%C"),data[blockLen],data[blockLen+1],data[blockLen+2],data[blockLen+3],data[blockLen+4],data[blockLen+5]);
		event.code.clear();
		for (int i=0;i<6;i++)//6字节
		{
			event.code.push_back(data[blockLen+i]);
		}

		blockLen+=6;//loss Measurement Technique起始位置
		//temp.Format(_T("%C%C"),data[blockLen],data[blockLen+1]);//2字节
		event.lossMeasurement.clear();
		event.lossMeasurement.push_back(data[blockLen]);
		event.lossMeasurement.push_back(data[blockLen+1]);
		//event.lossMeasurement=temp;

		blockLen+=2;//marker Locations起始位置
		for (int j=0;j<5;j++)
		{
			event.markerLocations[j]=FOURINT(data,blockLen);//
			blockLen+=4;//注释 起始位置
		}

		//event.comment=readString(data,blockLen);//
		strLen=readString(data + blockLen,event.comment);//

		blockLen+=strLen+1;//下一记录起始位置

		m_keyEvent.vEvent.push_back(event);
	}//efor


	///blockLen+=2;//已加过End-to-End Loss 起始位置
	m_keyEvent.eteLoss=FOURINT(data,blockLen);//

	blockLen+=4;//End-to-End Marker Positions Start 起始位置
	m_keyEvent.eteMarkerStart=FOURINT(data,blockLen);//

	blockLen+=4;//End-to-End Marker Positions Finish 起始位置
	m_keyEvent.eteMarkerFinish=FOURINT(data,blockLen);//

	blockLen+=4;//回损 起始位置
	m_keyEvent.returnLoss=TWOINT(data,blockLen);//

	blockLen+=2;//回损标志起点 起始位置
	m_keyEvent.RLMarkerStart=FOURINT(data,blockLen);//

	blockLen+=4;//回损标志结束点 起始位置
	m_keyEvent.RLMarkerFinish=FOURINT(data,blockLen);//

	blockLen+=4;//下一记录起始位置

	return blockLen;

	//m_parsePos+=10;//事件个数起始位置,跳过LnkParams\0
	//int count=TWOINT(data,m_parsePos);
	//m_keyEvent.keyEventNum=count;//

	//KeyEvent::Event event;
	//m_parsePos+=2;//第一个事件起始位置
	//CString temp;
	//m_keyEvent.vEvent.clear();
	//for (int i=0;i<count;i++)
	//{
	//	event.eventNum=TWOINT(data,m_parsePos);//编号

	//	m_parsePos+=2;//连接后第一个e事件点时间起始位置
	//	event.propagationTime=FOURINT(data,m_parsePos);//

	//	m_parsePos+=4;//衰减系数 起始位置
	//	event.attenCoefficent=TWOINT(data,m_parsePos);//

	//	m_parsePos+=2;//事件损耗 起始位置
	//	event.eventLoss=TWOINT(data,m_parsePos);//

	//	m_parsePos+=2;//事件反射 起始位置
	//	event.reflectance=FOURINT(data,m_parsePos);//

	//	m_parsePos+=4;//事件标志 起始位置
	//	temp.Format(_T("%C%C%C%C%C%C"),data[m_parsePos],data[m_parsePos+1],data[m_parsePos+2],data[m_parsePos+3],data[m_parsePos+4],data[m_parsePos+5]);
	//	event.code=temp;//6字节

	//	m_parsePos+=6;//loss Measurement Technique起始位置
	//	temp.Format(_T("%C%C"),data[m_parsePos],data[m_parsePos+1]);//2字节
	//	event.lossMeasurement=temp;

	//	m_parsePos+=2;//marker Locations起始位置
	//	event.markerLocations=FOURINT(data,m_parsePos);//

	//	m_parsePos+=4;//注释 起始位置
	//	event.comment=readString(data,m_parsePos);//

	//	m_parsePos+=event.comment.GetLength()+1;//下一记录起始位置

	//	m_keyEvent.vEvent.push_back(event);
	//}//efor


	/////m_parsePos+=2;//已加过End-to-End Loss 起始位置
	//m_keyEvent.eteLoss=FOURINT(data,m_parsePos);//

	//m_parsePos+=4;//End-to-End Marker Positions Start 起始位置
	//m_keyEvent.eteMarkerStart=FOURINT(data,m_parsePos);//

	//m_parsePos+=4;//End-to-End Marker Positions Finish 起始位置
	//m_keyEvent.eteMarkerFinish=FOURINT(data,m_parsePos);//

	//m_parsePos+=4;//回损 起始位置
	//m_keyEvent.eteLoss=TWOINT(data,m_parsePos);//

	//m_parsePos+=2;//回损标志起点 起始位置
	//m_keyEvent.RLMarkerStart=FOURINT(data,m_parsePos);//

	//m_parsePos+=4;//回损标志结束点 起始位置
	//m_keyEvent.RLMarkerFinish=FOURINT(data,m_parsePos);//

	//m_parsePos+=4;//下一记录起始位置

	//return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////
//函数说明:读取LinkParas Block。还没测试?????????????????????????????????????????????
//入口参数:data:BYTE数组
//出口参数:
//建立时间:2011.10.8
//Write By:wcq
////////////////////////////////////////////////////////////////////////////////////
BOOL CSorFileBase::readLinkParas(BYTE data[])
{
	vchar2string(m_linkParas.ID,string("LnkParams"));
	int blockLen=0;//block长度
	int strLen=0;
	blockLen+=10;//事件个数起始位置,跳过LnkParams\0
	int count=TWOINT(data,blockLen);
	m_linkParas.totalLandmarks=count;//地标个数

	LinkParas::Landmark landmark;
	blockLen+=2;//第一个事件起始位置,跳过地标个数
	CString temp;
	m_linkParas.vLandmark.clear();
	for (int i=0;i<count;i++)
	{
		landmark.num=TWOINT(data,blockLen);//编号

		blockLen+=2;//地标类别码
		//temp.Format(_T("%C%C"),data[blockLen],data[blockLen+1]);
		//landmark.code=temp;//2字节
		landmark.code.clear();
		landmark.code.push_back(data[blockLen]);
		landmark.code.push_back(data[blockLen+1]);

		blockLen+=2;//location 4字节
		landmark.location=FOURINT(data,blockLen);//


		blockLen+=4;//related Event Number相应事件编号
		landmark.relatedEventNum=TWOINT(data,blockLen);//

		blockLen+=2;//GPS Information GPS信息 4字节//GPS经度
		landmark.GPSLongitude=FOURINT(data,blockLen);//

		blockLen+=4;//GPS纬度
		landmark.GPSLatitude=FOURINT(data,blockLen);//

		blockLen+=4;//Fiber Correction Factor Lead-in Fiber  2字节
		landmark.fiberCorrFactor = TWOINT(data,blockLen);//

		blockLen+=2;//Sheath Marker Entering LandMark 4字节
		landmark.sheathEntering=FOURINT(data,blockLen);//

		blockLen+=4;////Sheath Marker Leaving LandMark 4字节 路标后面的光纤外套上的数字
		landmark.sheathLeaving=FOURINT(data,blockLen);//

		blockLen+=4;//光纤外套上表示距离的单位 2字节 feet 或 meters
		//temp.Format(_T("%C%C"),data[blockLen],data[blockLen+1]);
		//landmark.SLUnit=temp;//
		landmark.SLUnit.clear();
		landmark.SLUnit.push_back(data[blockLen]);
		landmark.SLUnit.push_back(data[blockLen+1]);

		blockLen+=2;//Mode Field Diameter Leaving LandMark 放大率 2字节
		landmark.modeFieldDiameter=TWOINT(data,blockLen);//

		blockLen+=2;//Comment注释 string
		//landmark.comment=readString(data,blockLen);//
		strLen=readString(data + blockLen,landmark.comment);//

		blockLen+=strLen+1;//下一记录起始位置
		m_linkParas.vLandmark.push_back(landmark);
	}//efor

	return blockLen;

	//m_parsePos+=10;//事件个数起始位置,跳过KeyEvents\0
	//int count=TWOINT(data,m_parsePos);
	//m_linkParas.totalLandmarks=count;//地标个数

	//LinkParas::Landmark landmark;
	//m_parsePos+=2;//第一个事件起始位置,跳过地标个数
	//CString temp;
	//m_linkParas.vLandmark.clear();
	//for (int i=0;i<count;i++)
	//{
	//	landmark.num=TWOINT(data,m_parsePos);//编号

	//	m_parsePos+=2;//地标类别码
	//	temp.Format(_T("%C%C"),data[m_parsePos],data[m_parsePos+1]);
	//	landmark.code=temp;//2字节

	//	m_parsePos+=2;//location 4字节
	//	landmark.location=FOURINT(data,m_parsePos);//


	//	m_parsePos+=4;//related Event Number相应事件编号
	//	landmark.relatedEventNum=TWOINT(data,m_parsePos);//

	//	m_parsePos+=2;//GPS Information GPS信息 4字节//GPS经度
	//	landmark.GPSLongitude=FOURINT(data,m_parsePos);//

	//	m_parsePos+=4;//GPS纬度
	//	landmark.GPSLatitude=FOURINT(data,m_parsePos);//

	//	m_parsePos+=4;//Fiber Correction Factor Lead-in Fiber  2字节
	//	landmark.fiberCorrFactor = TWOINT(data,m_parsePos);//

	//	m_parsePos+=2;//Sheath Marker Entering LandMark 4字节
	//	landmark.sheathEntering=FOURINT(data,m_parsePos);//

	//	m_parsePos+=4;////Sheath Marker Leaving LandMark 4字节 路标后面的光纤外套上的数字
	//	landmark.sheathLeaving=FOURINT(data,m_parsePos);//

	//	m_parsePos+=4;//光纤外套上表示距离的单位 2字节 feet 或 meters
	//	temp.Format(_T("%C%C"),data[m_parsePos],data[m_parsePos+1]);
	//	landmark.SLUnit=temp;//

	//	m_parsePos+=2;//Mode Field Diameter Leaving LandMark 放大率 2字节
	//	landmark.modeFieldDiameter=FOURINT(data,m_parsePos);//

	//	m_parsePos+=2;//Comment注释 string
	//	landmark.comment=readString(data,m_parsePos);//

	//	m_parsePos+=landmark.comment.GetLength()+1;//下一记录起始位置
	//	m_linkParas.vLandmark.push_back(landmark);
	//}//efor

	//return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////
//函数说明:读取DataPts Block。测试通过7.8
//入口参数:data:BYTE数组
//出口参数:
//建立时间:2011.7.8
//Write By:wcq
////////////////////////////////////////////////////////////////////////////////////
INT CSorFileBase::readDataPts(BYTE data[])
{
	vchar2string(m_dataPoints.ID,string("DataPts"));
	int blockLen=0;//block长度
	//int strLen=0;
	blockLen+=8;//事件个数起始位置,跳过DataPts\0
	int count=FOURINT(data,blockLen);
	m_dataPoints.dataPointNum=count;//

	blockLen+=4;//使用的扩展系数个数 起始位置
	count=TWOINT(data,blockLen);//
	m_dataPoints.totalScaleFactors=count;

	///int itemp;
	blockLen+=2;//第一个扩展系数相关数据-总个数 起始位置
	DataPoints::DataUseScaleFactor dataFactor;
	int len;
	FLOAT factor;
	m_dataPoints.vAllData.clear();
	unsigned short* pUnShort;
	for (int i=0;i<count;i++)
	{
		len=FOURINT(data,blockLen);
		dataFactor.count=len;//使用当前系数的数据总个数

		blockLen += 4;//系数 起始位置
		dataFactor.scaleFactor=TWOINT(data,blockLen);//1000=1.0
		////factor=dataFactor.scaleFactor/1000.0;//1000==1.0

		blockLen += 2;//具体数据 起始位置
		dataFactor.vData.clear();
		//for (int j=0;j<len;j++)//原来可以正常读取wcq2011.10.21
		//{
		//	////dataFactor.vData.push_back(TWOINT(data,blockLen)*factor);
		//	dataFactor.vData.push_back(TWOINT(data,blockLen));

		//	blockLen+=2;//下一个数据 起始位置
		//}//efor
		pUnShort = (unsigned short*)(data+blockLen);
		dataFactor.vData.assign(pUnShort,pUnShort+len);
		blockLen += len * 2;
		m_dataPoints.vAllData.push_back(dataFactor);
	}//efor

	////blockLen+=2;//下一记录起始位置,已在前面循环加过

	return blockLen;

	//m_parsePos+=8;//事件个数起始位置,跳过DataPts\0
	//int count=FOURINT(data,m_parsePos);
	//m_dataPoints.dataPointNum=count;//

	//m_parsePos+=4;//使用的扩展系数个数 起始位置
	//count=TWOINT(data,m_parsePos);//
	//m_dataPoints.totalScaleFactors=count;

	/////int itemp;
	//m_parsePos+=2;//第一个扩展系数相关数据-总个数 起始位置
	//DataPoints::DataUseScaleFactor dataFactor;
	//int len;
	//FLOAT factor;
	//m_dataPoints.vAllData.clear();
	//for (int i=0;i<count;i++)
	//{
	//	len=FOURINT(data,m_parsePos);
	//	dataFactor.count=len;//使用当前系数的数据总个数

	//	m_parsePos+=4;//系数 起始位置
	//	dataFactor.scaleFactor=TWOINT(data,m_parsePos);//1000=1.0
	//	factor=dataFactor.scaleFactor/1000.0;//1000==1.0

	//	m_parsePos+=2;//具体数据 起始位置
	//	dataFactor.vData.clear();
	//	for (int j=0;j<len;j++)
	//	{
	//		dataFactor.vData.push_back(TWOINT(data,m_parsePos)*factor);
	//		
	//		m_parsePos+=2;//下一个数据 起始位置
	//	}//efor
	//	m_dataPoints.vAllData.push_back(dataFactor);
	//}//efor

	//////m_parsePos+=2;//下一记录起始位置,已在前面循环加过

	//return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////
//函数说明:读取Cksum Block。校验码。从最后开始读取。测试通过7.8
//入口参数:data:BYTE数组,length:文件字节数
//出口参数:
//建立时间:2011.7.8
//Write By:wcq
////////////////////////////////////////////////////////////////////////////////////
INT CSorFileBase::readCKsum(BYTE data[])
{
	vchar2string(m_checksum.ID,string("Cksum"));
	int blockLen=0;//block长度
	blockLen+=6;//校验码 起始位置
	m_checksum.checksum=TWOINT(data,blockLen);//

	return blockLen;

	//m_parsePos=length-8;//事件个数起始位置,CKsum\0起始位置.CKsum总在文件的最后，所以从最后读取。
	//CString temp;
	//temp=readString(data,m_parsePos);//
	//if("CKsum"!=temp)
	//{
	//	return FALSE;
	//}

	//m_parsePos+=6;//校验码 起始位置
	//m_checksum.checksum=TWOINT(data,m_parsePos);//

	//return TRUE;
}
////////////////////////////////////////////////////////////////////////////////////
//函数说明:读取未知Block
//入口参数:data:BYTE数组,nextBlockID:下一个Block的ID，startPos:起始位置，maxLen文件最大长度
//出口参数:Block长度
//建立时间:2011.10.10
//Write By:wcq
////////////////////////////////////////////////////////////////////////////////////
////INT CSorFileBase::readUnknownParas(BYTE data[],vector<char> nextBlockID,int startPos,int maxLen)
////{
////	int blockLen=0;//block长度
////	int remainLen =maxLen - startPos;//
////	int strLen=nextBlockID.size();
////	//bool bContinue=true;
////	int j;
////	while(blockLen< remainLen)
////	{
////		if (nextBlockID[0] == data[blockLen])//和下一个block ID的第一个字符一样
////		{
////			for (j=0;j<strLen;j++)
////			{
////				if (nextBlockID[j] != data[blockLen + j])
////				{
////					break;
////				}
////			}
////			if (j == strLen)////所有字符都一样。到了下一个block
////			{
////				blockLen--;
////				break;
////			}			
////		}
////		blockLen++;
////	}//eofwhile
////
////	//保存当前Block到m_vUnknownParas
////	vector<BYTE> vcParams(data,data+blockLen-1);
////	m_vUnknownParas.push_back(vcParams);
////
////	return blockLen;
////}
////////////////////////////////////////////////////////////////////////////////////
//函数说明://读取文件时检查各个block大小之和是否大于文件总大小
//入口参数:fileLen:文件总大小
//出口参数:
//建立时间:2011.10.11
//Write By:wcq
////////////////////////////////////////////////////////////////////////////////////
BOOL CSorFileBase::checkSize(int fileLen)
{
	int len=m_mapBlock.mapBlock.Size;
	int count=m_mapBlock.blockCount-1;//总共有多少个Block,减去Map Block本身
	for (int i=0;i<count;i++)
	{
		len += m_mapBlock.vBlockInfo[i].Size;
	}
	if (len != fileLen)
	{
		return FALSE;
	}
	return TRUE;
}
////////////////////////////////////////////////////////////////////////////////////
//函数说明:读取.sor文件,没考虑超出文件位置。以1000作为最大字符串长度.转化过程是否可优化**MM**
//入口参数:data:BYTE数组;startPos:起始位置
//出口参数:读到的字符串
//建立时间:2011.7.8
//Write By:wcq
////////////////////////////////////////////////////////////////////////////////////
CString CSorFileBase::readString(BYTE data[],int startPos)
{
	CString temp=_T(""),temp2;
	int itemp=0;
	while('\0'!=data[startPos+itemp] && itemp<1000)//以1000作为最大字符串长度
	{
		temp2.Format(_T("%C"),data[startPos+itemp]);
		temp+=temp2;
		itemp++;
	}
	return temp;
}
////////////////////////////////////////////////////////////////////////////////////
//函数说明:
//入口参数:vChar:存放返回结果,data:可能加了偏移。vChar:保存字符串
//出口参数:多少个字节
//建立时间:2011.10.9
//Write By:wcq
////////////////////////////////////////////////////////////////////////////////////
int CSorFileBase::readString(BYTE data[],vector<char>& vChar)
{
	int itemp=0;
	vChar.clear();
	while('\0'!=data[itemp] && itemp<MAX_STRING_LEN)//以1000作为最大字符串长度
	{
		vChar.push_back(data[itemp]);
		itemp++;
	}
	return vChar.size();
}
unsigned short CSorFileBase::crc16(byte d[], int len)
{
	byte b = 0;
	unsigned short crc = 0xffff;
	int i, j;
	for(i=0; i<len; i++)
	{
		for(j=0; j<8; j++)
		{
			b = ((d[i]<<j)&0x80) ^ ((crc&0x8000)>>8);
			crc<<=1;
			if(b!=0)crc^=0x1021;
		}
	}
	crc = ~crc;
	return crc;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////
//获取长度////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
//主要是用于保存新的文件，如果是加载，则不会改变，也不用重新计算.???????????????????????????
INT CSorFileBase::MapBlock::getsize()
{
	INT len;
	//len=4 + 6;//Map本身
	//len+= 2;//Block个数
	//len+= (10 + 6)*4;//GenParams/SupParams/FxdParams/KeyEvents/
	/////len+= 10 + 6;//LnkParams暂时不加
	//len+= 8 + 6;//DataPts
	/////Special Propritary暂时不加
	//len+= 6 + 6;//Cksum

	len=102;//如果添加新Block需要重新计算.???????????????????????????
	return len;
}
//主要是用于保存新的文件，如果是加载，则不会改变，也不用重新计算
INT CSorFileBase::GeneralParas::getsize()
{
	INT len = 10;//:GenParams本身
	len += 16;//不变的相加
	len += cableID.size()+1;
	len += fiberID.size()+1;
	len += OLocation.size()+1;
	len += TLocation.size()+1;
	len += cableCode.size()+1;
	len += operater.size()+1;
	len += comment.size()+1;
	return len;
}
INT CSorFileBase::SuppliersParas::getsize()
{
	INT len = 10;//:SupParams本身
	len += name.size()+1;
	len += mainframeID.size()+1;
	len += mainframeSn.size()+1;
	len += moduleID.size()+1;
	len += moduleSn.size()+1;
	len += softVersion.size()+1;
	len += other.size()+1;
	return len;
}
INT CSorFileBase::FixedParas::getsize()
{
	INT len = 10;//:FixedParas本身
	//len += 74;//
	len += 72;//
	len += pulseWidthNum * 10;
	return len;
}
INT CSorFileBase::KeyEvent::getsize()
{
	INT len = 10;//KeyEvents本身
	len += 24;//
	
	for (int i=0;i<keyEventNum;i++)
	{
		len += 42 + vEvent[i].comment.size()+1;
	}
	return len;
}
INT CSorFileBase::LinkParas::getsize()
{
	INT len = 10;//KeyEvents本身
	len += 2;//

	for (int i=0;i<totalLandmarks;i++)
	{
		len += 32 + vLandmark[i].comment.size()+1;
	}
	return len;
}
INT CSorFileBase::DataPoints::getsize()
{
	INT len = 8;//KeyEvents本身
	len += 6;//dataPointNum + totalScaleFactors

	//for (int i=0;i<totalScaleFactors;i++)
	for (int i=0;i<vAllData.size();i++)
	{
		len += 6 + vAllData[i].count * 2;
	}
	return len;
}
INT CSorFileBase::Checksum::getsize()
{
	return 8;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////
//保存////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
//

void writeShort(vector<BYTE>& vData,short nToWrite)
{
	vData.push_back((unsigned char)nToWrite);
	nToWrite >>= 8;
	vData.push_back((unsigned char)nToWrite);
}
void writeInt(vector<BYTE>& vData,INT nToWrite)
{
	for(int i=0;i<4;i++)
	{
		vData.push_back((unsigned char)nToWrite);
		nToWrite >>= 8;
	}
}
//写入字符串，加'\0'
void writeString(vector<BYTE>& vData,vector<char> vChar)
{
	int len= vChar.size();
	for(int i=0;i<len;i++)
	{
		vData.push_back(vChar[i]);
	}
	vData.push_back('\0');
}
//写入固定长度字符串，不加'\0'
void writeString(vector<BYTE>& vData,vector<char> vChar,int len)
{
	if (vChar.size()<len)//防止出错wcq2011.10.21
	{
		while(vChar.size()<len)
		{
			vChar.push_back('0');
		}
	}
	for(int i=0;i<len;i++)
	{
		vData.push_back(vChar[i]);
	}
}
//主要是用于保存新的文件，如果是加载，则不会改变，也不用重新计算.???????????????????????????
void CSorFileBase::writeMapBlock(vector<BYTE>& vData)
{
	INT len;
	writeString(vData,m_mapBlock.mapBlock.ID);//4
	writeShort(vData,m_mapBlock.mapBlock.RevisionNo);//2 REVISION_NUM
	writeInt(vData,1);//先占个位等会还要修改//4
	int nLen=m_mapBlock.vBlockInfo.size();
	writeShort(vData,m_mapBlock.vBlockInfo.size()+1);//加上Map本身
	len=12;//前面几个的和
	int nLenTemp=0;
	char* pChar;
	int IDLen=0;
	for(int i=0;i<nLen;i++)//各个block大小有可能变化，所以要重新计算
	{
		writeString(vData,m_mapBlock.vBlockInfo[i].ID);//4
		writeShort(vData,m_mapBlock.vBlockInfo[i].RevisionNo);//2

		IDLen=m_mapBlock.vBlockInfo[i].ID.size();
		pChar=&m_mapBlock.vBlockInfo[i].ID[0];
		if (0 == strncmp(pChar,"GenParams",IDLen))//
		{
			nLenTemp = m_generalParas.getsize();
		}
		else if (0 == strncmp(pChar,"SupParams",IDLen))
		{
			nLenTemp = m_suppliersParas.getsize();
		}
		else if (0 == strncmp(pChar,"FxdParams",IDLen))
		{
			nLenTemp = m_fixedParas.getsize();
		}
		else if (0 == strncmp(pChar,"KeyEvents",IDLen))
		{
			nLenTemp = m_keyEvent.getsize();
		}
		else if (0 == strncmp(pChar,"LnkParams",IDLen))
		{
			nLenTemp = m_linkParas.getsize();
		}
		else if (0 == strncmp(pChar,"DataPts",IDLen))
		{
			nLenTemp = m_dataPoints.getsize();
		}
		else if (0 == strncmp(pChar,"Cksum",IDLen) || 0 == strncmp(pChar,"CKsum",IDLen))//标准应为Cksum,但是得力的用的是CKsum
		{
			nLenTemp = m_checksum.getsize();
		}
		else
		{
			nLenTemp = m_mapBlock.vBlockInfo[i].Size;
		}////eif
		writeInt(vData,nLenTemp);//
		len += 6 + m_mapBlock.vBlockInfo[i].ID.size()+1;
	}//efor
	
	//map block大小重新赋值	
	for(int i=0;i<4;i++)
	{
		vData[6+i] = (unsigned char)len;
		len >>= 8;
	}
}//写入GeneralParas Block
void CSorFileBase::GeneralParas::write(vector<BYTE>& vData)
{
	writeString(vData,ID);//
	writeString(vData,languageCode,2);//
	writeString(vData,cableID);//
	writeString(vData,fiberID);//
	writeShort(vData,fiberType);//
	writeShort(vData,nominalWaveLength);//

	writeString(vData,OLocation);//
	writeString(vData,TLocation);//
	writeString(vData,cableCode);//
	writeString(vData,currentDataFlag,2);//

	writeInt(vData,userOffset);//
	writeInt(vData,uODistance);//

	writeString(vData,operater);//
	writeString(vData,comment);//
}
//写入SuppliersParas Block
void CSorFileBase::SuppliersParas::write(vector<BYTE>& vData)
{
	writeString(vData,ID);//
	writeString(vData,name);//
	writeString(vData,mainframeID);//
	writeString(vData,mainframeSn);//
	writeString(vData,moduleID);//
	writeString(vData,moduleSn);//
	writeString(vData,softVersion);//
	writeString(vData,other);//
}
//写入FxdParams Block
void CSorFileBase::FixedParas::write(vector<BYTE>& vData)
{
	writeString(vData,ID);//
	writeInt(vData,timestamp);//
	writeString(vData,distanceUnit,2);//
	writeShort(vData,actualWavelength);//
	writeInt(vData,acquisitionOffset);//
	writeInt(vData,AODistance);//

	writeShort(vData,pulseWidthNum);//

	//for(int i=0;i<pulseWidthNum;i++)//改为下面的 cwcq2012.08.24
	for(int i=0;i<vPulseWidth.size();i++)//wcq2012.08.24
	{
		writeShort(vData,vPulseWidth[i]);//
		writeInt(vData,vDataSpacing[i]);//
		writeInt(vData,vDataPointsEach[i]);//
	}

	writeInt(vData,groupIndex);//
	writeShort(vData,backscatter);//
	writeInt(vData,avgNum);//
	writeShort(vData,avgTime);//

	writeInt(vData,acquisitionRange);//
	writeInt(vData,ARDistance);//
	writeInt(vData,frontPanelOff);//

	writeShort(vData,noiseFloorLevel);//
	writeShort(vData,noiseFloorFactor);//
	writeShort(vData,powerOff);//
	writeShort(vData,lossThreshold);//
	writeShort(vData,reflectanceThreshod);//
	writeShort(vData,eofThreshold);//

	writeString(vData,traceType,2);//

	writeInt(vData,windowCoor1);//
	writeInt(vData,windowCoor2);//
	writeInt(vData,windowCoor3);//
	writeInt(vData,windowCoor4);//
}
//写入KeyEvent Block
void CSorFileBase::KeyEvent::write(vector<BYTE>& vData)
{
	writeString(vData,ID);//
	writeShort(vData,keyEventNum);//
	//for (int i=0;i<keyEventNum;i++)//改为下面的 cwcq2012.08.24
	for (int i=0; i<vEvent.size(); i++)//wcq2012.08.24
	{
		writeShort(vData,vEvent[i].eventNum);//
		writeInt(vData,vEvent[i].propagationTime);//
		writeShort(vData,vEvent[i].attenCoefficent);//
		writeShort(vData,vEvent[i].eventLoss);//
		writeInt(vData,vEvent[i].reflectance);//
		writeString(vData,vEvent[i].code,6);//
		writeString(vData,vEvent[i].lossMeasurement,2);//
		for (int j=0;j<5;j++)
		{
			writeInt(vData,vEvent[i].markerLocations[j]);//
		}
		writeString(vData,vEvent[i].comment);//
	}
	writeInt(vData,eteLoss);//
	writeInt(vData,eteMarkerStart);//
	writeInt(vData,eteMarkerFinish);//
	writeShort(vData,returnLoss);//
	writeInt(vData,RLMarkerStart);//
	writeInt(vData,RLMarkerFinish);//
}
//清空事件列表wcq2012.06.11
void CSorFileBase::KeyEvent::clearEvent()
{
	keyEventNum = 0;
	for (int i=0;i<vEvent.size();i++)
	{
		vEvent[i].code.clear();
		vEvent[i].lossMeasurement.clear();
		vEvent[i].comment.clear();
	}
	vEvent.clear();
}
//写入LinkParas 
void CSorFileBase::LinkParas::write(vector<BYTE>& vData)
{
	writeString(vData,ID);//
	writeShort(vData,totalLandmarks);//
	//for (int i=0;i<totalLandmarks;i++)//改为下面的 cwcq2012.08.24
	for (int i=0;i<vLandmark.size();i++)//wcq2012.08.24
	{
		writeShort(vData,vLandmark[i].num);//
		writeString(vData,vLandmark[i].code,2);//
		writeInt(vData,vLandmark[i].location);//
		writeShort(vData,vLandmark[i].relatedEventNum);//

		writeInt(vData,vLandmark[i].GPSLongitude);//
		writeInt(vData,vLandmark[i].GPSLatitude);//
		writeShort(vData,vLandmark[i].fiberCorrFactor);//
		writeInt(vData,vLandmark[i].sheathEntering);//
		writeInt(vData,vLandmark[i].sheathLeaving);//

		writeString(vData,vLandmark[i].SLUnit,2);//

		writeShort(vData,vLandmark[i].modeFieldDiameter);//
		writeString(vData,vLandmark[i].comment);//
	}
}
//写入DataPts Block
void CSorFileBase::DataPoints::write(vector<BYTE>& vData)
{
	writeString(vData,ID);//
	writeInt(vData,dataPointNum);//
	writeShort(vData,totalScaleFactors);//
	int num;
	unsigned short sValue;
	BYTE* pB;
	int count = vAllData.size();
	//for (int i=0;i<totalScaleFactors;i++)//改为下面的 cwcq2012.08.24
	for (int i=0;i<count;i++)//wcq2012.08.24
	{
		writeInt(vData,vAllData[i].count);//
		writeShort(vData,vAllData[i].scaleFactor);//
		/*num = vAllData[i].count;
		for (int j=0;j<num;j++)
		{
			sValue = vAllData[i].vData[j];
			vData.push_back((unsigned char)sValue);
			sValue >>= 8;
			vData.push_back((unsigned char)sValue);
		}*/
		pB=(BYTE*)&vAllData[i].vData[0];
		vData.insert(vData.end(),pB,pB + vAllData[i].count * 2);
	}
}
//写入Cksum Block
void CSorFileBase::Checksum::write(vector<BYTE>& vData)
{
	writeString(vData,ID);//
	writeShort(vData,checksum);//
}
void CSorFileBase::writeData(vector<BYTE>& vData)
{
	writeMapBlock(vData);//Map
	int nLenTemp=0;
	char* pChar;
	int IDLen=0;
	int nLen=m_mapBlock.vBlockInfo.size();
	int indexUnkown=0;
	for(int i=0;i<nLen;i++)//各个block大小有可能变化，所以要重新计算
	{
		IDLen=m_mapBlock.vBlockInfo[i].ID.size();
		pChar=&m_mapBlock.vBlockInfo[i].ID[0];
		if (0 == strncmp(pChar,"GenParams",IDLen))//
		{
			m_generalParas.write(vData);
		}
		else if (0 == strncmp(pChar,"SupParams",IDLen))
		{
			m_suppliersParas.write(vData);
		}
		else if (0 == strncmp(pChar,"FxdParams",IDLen))
		{
			m_fixedParas.write(vData);
		}
		else if (0 == strncmp(pChar,"KeyEvents",IDLen))
		{
			m_keyEvent.write(vData);
		}
		else if (0 == strncmp(pChar,"LnkParams",IDLen))
		{
			m_linkParas.write(vData);
		}
		else if (0 == strncmp(pChar,"DataPts",IDLen))
		{
			m_dataPoints.write(vData);
		}
		else if (0 == strncmp(pChar,"Cksum",IDLen) || 0 == strncmp(pChar,"CKsum",IDLen))//标准应为Cksum,但是得力的用的是CKsum
		{
			m_checksum.write(vData);
		}
		else//自定义Block
		{
			vData.insert(vData.end(),m_vUnknownParas[indexUnkown].begin(),m_vUnknownParas[indexUnkown].end());
			indexUnkown++;
		}////eif
	}//efor
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//读写
////////////////////////////////////////////////////////////////////////////////////
//函数说明:读取.sor文件。读取时忽略自定义的数据块
//入口参数:filePathAndName:文件名，包含路径
//出口参数:
//建立时间:2011.7.8
//Write By:wcq
////////////////////////////////////////////////////////////////////////////////////
//INT CSorFileBase::readSorFile(CString filePathAndName,TestResultInfo& testResultInfo)
INT CSorFileBase::readSorFile(CString filePathAndName,SORDATA& sorData)
{
	if(""==filePathAndName)//
	{
		//m_errInfo=_T("文件名为空");
		return ERROR_EMPTY_NAME;
	}//eif

	//
	CFile file;
	if(!file.Open(filePathAndName,CFile::modeReadWrite))//打开
	{
		//m_errInfo.Format(_T("未能打开文件%s "),filePathAndName);
		return ERROR_FILE_NOT_FOUND;
	}

	//
	int len=file.GetLength();//文件长度
	//用来临时存放所有数据，把所有的数据一次读到内存，速度是否快点,但是否占用内存.???????????????**可能修改**
	BYTE* fileData=new BYTE[len];
	/////
	file.Read(fileData,len);//读取所有信息
	file.Close();//关闭

	//
	if(parseData(fileData,len))//解析2011.12.01
	{
		//setReturnData(testResultInfo);
		convertToReturnData(sorData);
	}
	else
	{
		delete[] fileData;//删除临时变量
		deleteDataOnError();//当加载出错时，删除数据 wcq2012.08.24
		init();// wcq2012.08.24
		return ERROR_FORMAT;
	}

	delete[] fileData;//删除临时变量
	return FILE_SUCCESS;
}
//读取时把标准文件里的相关参数转化为显示曲线需要的数据，如一些long integer 类型的数据转化为float型
//被上面函数调用

//读取内存sorFile内容，lzy2014.11.11
INT CSorFileBase::readSorFile(BYTE *pData, int nLength, SORDATA &sorData)
{
	if(parseData(pData,nLength))//解析2011.12.01
	{
		//setReturnData(testResultInfo);
		convertToReturnData(sorData);
	}
	else
	{
		deleteDataOnError();//当加载出错时，删除数据 wcq2012.08.24
		init();// wcq2012.08.24
		return ERROR_FORMAT;
	}

	return FILE_SUCCESS;

}
BOOL CSorFileBase::convertToReturnData(SORDATA& sorData)
{
	//数据点处理
	//sorData.vAllData = m_dataPoints.vAllData;
	int nTemp = m_dataPoints.vAllData.size();
	Data data;
	sorData.vAllData.clear();
	for (int i=0;i< m_dataPoints.vAllData.size(); i++)
	{
		data.nCount = m_dataPoints.vAllData[i].count;
		data.scaleFactor = m_dataPoints.vAllData[i].scaleFactor;
		data.vData.assign(m_dataPoints.vAllData[i].vData.begin(), m_dataPoints.vAllData[i].vData.end());
		sorData.vAllData.push_back(data);
	}//efor
	//

	//以下计算只考虑只使用一个脉宽的情形，如果遇到有多个脉宽的情形，则需要修改 wcq2012.07.23 ??????????????????????????????????????
	//计算测量距离
	float fC=299792458.0;
	float fReflect=m_fixedParas.groupIndex /100000.0;//折射率
	//float numEachTime=m_fixedParas.vDataSpacing[0]/100000000000000.0;//两点间时间//wcq2011.11.25//改为下面的 cwcq2012.08.24
	float numEachTime =0;//wcq2012.08.24
	if (m_fixedParas.vDataSpacing.size() > 0)//原来没有加判断 cwcq2012.08.24
	{
		numEachTime=m_fixedParas.vDataSpacing[0]/100000000000000.0;//两点间时间//wcq2011.11.25
	}//eif
	numEachTime = C_LIGHT_SPEED * (numEachTime) / fReflect;//每点距离 =两点间时间 * 光在光纤速度//wcq2011.11.25
	//sorData.Length = numEachTime * m_fixedParas.vDataPointsEach[0];//每点距离 * 点数//wcq2011.11.25//改为下面的 cwcq2012.08.24
	if (m_fixedParas.vDataPointsEach.size() > 0)//原来没有加判断 cwcq2012.08.24
	{
		sorData.Length = numEachTime * m_fixedParas.vDataPointsEach[0];//每点距离 * 点数////wcq2012.08.24
	}//eif
	//折射率 两点间距离
	sorData.Refraction = fReflect;
	sorData.DataSpacing = numEachTime;//
	//波长
	sorData.WaveLength = m_fixedParas.actualWavelength/10;//wcq2012.02.07
	//脉宽
	//sorData.PulseWidth = m_fixedParas.vPulseWidth[0];//改为下面的 cwcq2012.08.24
	if (m_fixedParas.vPulseWidth.size() > 0)//原来没有加判断 cwcq2012.08.24
	{
		sorData.PulseWidth = m_fixedParas.vPulseWidth[0];//wcq2012.08.24
	}//eif
	//采样频率
	if (m_fixedParas.vDataSpacing.size() > 0)//原来没有加判断 cwcq2012.08.24
	{
		sorData.Frequency = 100000000.0 / (m_fixedParas.vDataSpacing[0] * 2);//wcq2012.06.12 cwcq2012.08.24
	}//eif

	//事件列表 
	//sorData.vEvent = m_keyEvent.vEvent;
	Event event;
	nTemp = m_keyEvent.vEvent.size();
	sorData.vEvent.clear();
	for (int i = 0;i < nTemp; i++)
	{
		event.propagationTime = m_keyEvent.vEvent[i].propagationTime;
		event.attenCoefficent = m_keyEvent.vEvent[i].attenCoefficent;
		event.eventLoss = m_keyEvent.vEvent[i].eventLoss;
		event.reflectance = m_keyEvent.vEvent[i].reflectance;
		event.code.assign(m_keyEvent.vEvent[i].code.begin(),m_keyEvent.vEvent[i].code.end());
		event.lossMeasurement.assign(m_keyEvent.vEvent[i].lossMeasurement.begin(),m_keyEvent.vEvent[i].lossMeasurement.end());
		event.comment.assign(m_keyEvent.vEvent[i].comment.begin(),m_keyEvent.vEvent[i].comment.end());
		sorData.vEvent.push_back(event);
	}//efor

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//后面添加 wcq2012.10.30
	sorData.BackscatterCoefficient = m_fixedParas.backscatter / 10.0;//散射系数 800 = 80.0 wcq2012.10.30
	sorData.LossThreshold = m_fixedParas.lossThreshold / 1000.0;//衰减门限 200 = 0.200dB wcq2012.10.30
	sorData.ReflectanceThreshold = m_fixedParas.reflectanceThreshod /(- 1000.0);//反射门限 40000 = -40.000 dB wcq2012.10.30
	sorData.EndOfFiberThreshold = m_fixedParas.eofThreshold / 1000.0;//光纤末端门限 3000 = 3.000dB wcq2012.10.30
	sorData.Range = m_fixedParas.ARDistance * 1000 / 10;//km --> m wcq2012.10.30 80.544 = 805
	sorData.TestTimestamp = m_fixedParas.timestamp;   //测量的时刻，lzy2014.8.8
	sorData.AverageCount = m_fixedParas.avgNum; //平均次数，lzy2014.8.8
	sorData.GroupIndex = m_fixedParas.groupIndex; //折射率 2014.8.13
	
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//返回厂商名称等 wcq2013.07.08
	std::string stdTemp;
	vchar2string(stdTemp, m_suppliersParas.name);
	CString temp;//
	temp.Format(_T("name=%s"), CString(stdTemp.c_str()));
	sorData.vParams.push_back(temp);

	return TRUE;
}
//当加载出错时，删除数据,主要是清空数据点 wcq2012.08.24
void CSorFileBase::deleteDataOnError()
{
	if (m_dataPoints.vAllData.size() > 0)
	{
		for (int i=0; i<m_dataPoints.vAllData.size(); i++)
		{
			m_dataPoints.vAllData[i].vData.clear();
		}//eif
	}//eif
}


//保存sor文件
//INT CSorFileBase::saveSorFile(CString filePathAndName,TestResultInfo testResultInfo)//cwcq2012.07.23
INT CSorFileBase::saveSorFile(CString filePathAndName,SORDATA sorData)//wcq2012.07.23
{
	if(""==filePathAndName)//
	{
		//m_errInfo=_T("文件名为空");
		return ERROR_EMPTY_NAME;
	}//eif

	//
	CFile file;
	if(!file.Open(filePathAndName,CFile::modeWrite | CFile::modeCreate))//创建文件，如果已存在，则删除
	{
		//m_errInfo.Format(_T("未能创建文件%s "),filePathAndName);
		return ERROR_CREATE_FILE;
	}

	//
	if (0==m_mapBlock.vBlockInfo.size())//未初始化，即没有经过加载文件再保存，而是直接保存测试曲线
	{
		init();
	}
	//dealTestInfo(testResultInfo);//将传进来的参数转为相应的类成员
	convertDataToSor(sorData);//将传进来的参数转为相应的类成员
	
	vector<BYTE> vData;
	writeData(vData);//将各个成员变量写到vData变量

	//
	file.Write(&vData[0],vData.size());//写入
	file.Close();//关闭

	return TRUE;
}
////保存数据时，将传进来的参数转为类成员//wcq2012.07.23
void CSorFileBase::convertDataToSor(SORDATA sorData)
{
	int nTemp=0;
	DataPoints::DataUseScaleFactor dus;
	m_dataPoints.vAllData.clear();// = sorData.vAllData;
	for (int i=0;i< sorData.vAllData.size(); i++)
	{
		dus.count = sorData.vAllData[i].nCount;
		dus.scaleFactor = sorData.vAllData[i].scaleFactor;
		dus.vData.assign(sorData.vAllData[i].vData.begin(),sorData.vAllData[i].vData.end());
		m_dataPoints.vAllData.push_back(dus);
		nTemp += sorData.vAllData[i].nCount;
	}//efor
	m_dataPoints.dataPointNum = nTemp;//数据点个数
	
	//折射率
	m_fixedParas.groupIndex = sorData.Refraction * 100000;

	CTime ct;//(st);
	time_t lt= ct.GetCurrentTime().GetTime(); 
	m_fixedParas.timestamp = lt;//wcq2012.02.10
	m_fixedParas.actualWavelength = sorData.WaveLength * 10;//wcq2012.02.07
	m_generalParas.nominalWaveLength = sorData.WaveLength;//wcq2012.02.10
	
	//假设只有脉宽，如果有多个，会不准确?????????????????????????????????????????????????????????????
	if (m_fixedParas.vDataSpacing.size() > 0)//原来没有加判断 cwcq2012.08.24
	{
		m_fixedParas.vDataSpacing[0] = 100000000/(2*sorData.Frequency);//wcq2012.06.12
	}//eif
	if (m_fixedParas.vPulseWidth.size() > 0)//原来没有加判断 cwcq2012.08.24
	{
		m_fixedParas.vPulseWidth[0] = sorData.PulseWidth;
	}//eif
	if (m_fixedParas.vDataPointsEach.size() > 0)//原来没有加判断 cwcq2012.08.24
	{
		m_fixedParas.vDataPointsEach[0]= nTemp;
	}//eif

	//事件列表wcq2012.06.11
	m_keyEvent.keyEventNum = sorData.vEvent.size();

	KeyEvent::Event event;
	nTemp = sorData.vEvent.size();
	m_keyEvent.vEvent.clear();
	for (int i=0; i< nTemp;i++)
	{
		event.eventNum = i;
		event.propagationTime = sorData.vEvent[i].propagationTime;
		event.attenCoefficent = sorData.vEvent[i].attenCoefficent;
		event.eventLoss = sorData.vEvent[i].eventLoss;
		event.reflectance = sorData.vEvent[i].reflectance;
		event.code.assign(sorData.vEvent[i].code.begin(),sorData.vEvent[i].code.end());
		event.lossMeasurement.assign(sorData.vEvent[i].lossMeasurement.begin(),sorData.vEvent[i].lossMeasurement.end());
		event.comment.assign(sorData.vEvent[i].comment.begin(),sorData.vEvent[i].comment.end());
		for(int j=0;j<5;j++)
		{
			event.markerLocations[j]  = 0;
		}//efor
		m_keyEvent.vEvent.push_back(event);
	}//efor

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//后面添加 wcq2012.10.30
	m_fixedParas.backscatter = sorData.BackscatterCoefficient * 10;
	m_fixedParas.lossThreshold = sorData.LossThreshold * 1000;
	m_fixedParas.reflectanceThreshod = sorData.ReflectanceThreshold * (-1000);
	m_fixedParas.eofThreshold = sorData.EndOfFiberThreshold * 1000;
	m_fixedParas.acquisitionRange = 0;//和acquisitionRange distance都表示距离,我们使用acquisitionRange distance
	m_fixedParas.ARDistance = (sorData.Range + 50) * 10 / 1000;//我们的文件都使用km,这里转化下 80.544 = 805
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}


//void CSorFileBase::dealTestInfo(TestResultInfo testResultInfo)//cwcq2012.07.23
//{
//	m_dataPoints.dataPointNum = testResultInfo.DataPointsNum;//数据点个数
//	//清空现有数据
//	for (int i=0;i<m_dataPoints.totalScaleFactors;i++)
//	{
//		m_dataPoints.vAllData[i].vData.clear();
//	}
//	//"测试"数据
//	m_dataPoints.vAllData.clear();
//	m_dataPoints.totalScaleFactors = 1;
//	DataPoints::DataUseScaleFactor dus;
//	dus.count = testResultInfo.DataPointsNum;
//	dus.scaleFactor =testResultInfo.ScaleFactor;//缩放系数为1
//	dus.vData.assign(testResultInfo.pDataPoints,testResultInfo.pDataPoints + testResultInfo.DataPointsNum);
//	//dus.vData.resize(testResultInfo.DataPointsNum);
//	//memcpy(&(dus.vData[0]),testResultInfo.pDataPoints,testResultInfo.DataPointsNum);
//	m_dataPoints.vAllData.push_back(dus);
//
//	//
//	//short fRefraction = testResultInfo.Refraction * 100000;
//	m_fixedParas.groupIndex = testResultInfo.Refraction * 100000;
//	//for (int i=0;i<m_dataPoints.vAllData.size();i++)
//	//{
//	//	m_dataPoints.vAllData[i].scaleFactor = fRefraction;
//	//}
//	//m_fixedParas.actualWavelength = testResultInfo.WaveLength;
//	CTime ct;//(st);
//	time_t lt= ct.GetCurrentTime().GetTime(); 
//	m_fixedParas.timestamp = lt;//wcq2012.02.10
//	m_fixedParas.actualWavelength = testResultInfo.WaveLength * 10;//wcq2012.02.07
//	m_generalParas.nominalWaveLength = testResultInfo.WaveLength;//wcq2012.02.10
//	//m_fixedParas.vDataSpacing[0] = testResultInfo.DataSpacing;//wcq2011.12.01testResultInfo.DataSpacing这个是以m为单位
//	//m_fixedParas.vDataSpacing[0] =testResultInfo.DataSpacing * (m_fixedParas.groupIndex /C_LIGHT_SPEED )* 100000000000000.0;//wcq2011.12.01
//	//m_fixedParas.vDataSpacing[0] =testResultInfo.DataSpacing * testResultInfo.Refraction * ( 100000000000000.0/C_LIGHT_SPEED );//wcq2011.12.01
//	m_fixedParas.vDataSpacing[0] = 100000000/(2*testResultInfo.Frequency);//wcq2012.06.12
//	m_fixedParas.vPulseWidth[0] = testResultInfo.PulseWidth;
//	m_fixedParas.vDataPointsEach[0]= testResultInfo.DataPointsNum;
//
//	//事件列表wcq2012.06.11
//	//清空
//	m_keyEvent.clearEvent();
//	m_keyEvent.keyEventNum = testResultInfo.EventList.vEventInfo.size();//事件个数
//	string pos="9999";
//	//float fPTimeCoeff = testResultInfo.Refraction * pow(10.0,-10) /C_LIGHT_SPEED
//	//int eventType = 0;//wcq2012.07.07
//	for(int i=0; i<m_keyEvent.keyEventNum; i++)
//	{
//		KeyEvent::Event event;		
//		event.eventNum = i;////事件编号
//		event.propagationTime = (testResultInfo.EventList.vEventInfo[i].index /(2 *testResultInfo.Frequency))*10000;//距离/光速(km/ps) * 折射率 位置
//		//event.propagationTime = (testResultInfo.EventList.vEventInfo[i].fLocation * pow(10.0,-10)/C_LIGHT_SPEED ) * testResultInfo.Refraction;//距离/光速(km/ps) * 折射率 位置
//		event.attenCoefficent = testResultInfo.EventList.vEventInfo[i].fLineK * 1000;//斜率/衰减因子 dB/km 350=0.35
//		event.eventLoss = testResultInfo.EventList.vEventInfo[i].fEventloss * 1000;////损耗
//		event.reflectance = testResultInfo.EventList.vEventInfo[i].fEventHigh * 1000;//反射
//		//event.code //事件类型????????????
//		//event.code.push_back(testResultInfo.EventList.vEventInfo[i].nEventType + '0');//我们的只有非反射和反射事件,饱和的没有//改为下面的
//		////if (0 == testResultInfo.EventList.vEventInfo[i].nEventType)//cwcq2012.07.09
//		////{
//		////	event.code.push_back('0');
//		////}
//		////else//1或其它
//		////{
//		////	event.code.push_back('1');
//		////}//eif
//		event.code.push_back(testResultInfo.EventList.vEventInfo[i].nEventType + '0');//wcq2012.07.09
//		event.code.push_back(testResultInfo.EventList.vEventInfo[i].cEventType);//
//		event.code.insert(event.code.end(), pos.begin(), pos.end());//不使用 landmark
//		vchar2string(event.lossMeasurement,string("LS"));//几点法 我们用的是4点法
//		///event.markerLocations//算法每点位置，不管
//		///event.comment.push_back('\0');//注释，没有不能添加，添加会出错wcq2012.06.12
//		m_keyEvent.vEvent.push_back(event);
//	}//efor
//}
//
