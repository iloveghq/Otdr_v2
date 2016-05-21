//该类主要用来读写sor文件
//MM:May Modify缩写,可能修改
//Paras:Parameters缩写
//Off:offset缩写
#pragma once
#include <map>
#include <vector>
#include "SorFile.h"
using namespace std;
#define FOURINT(DATA,IT) ((DATA[IT+3]<<(8*3))+(DATA[IT+2]<<(8*2))+(DATA[IT+1]<<(8))+DATA[IT])
#define TWOINT(DATA,IT) ((DATA[IT+1]<<(8))+DATA[IT])//DATA:包含所有数据,IT：起始位置
#define SIZEOFWCHAR_T 2//wcq2011.10.8
#define MAX_STRING_LEN 1000//读取Block中字符串时，以该值作为最大长度，超过该长度认为出错2011.10.10
#define REVISION_NUM 200//各个block的修订版本号
#define C_LIGHT_SPEED 299792458.0//光速//wcq2011.10.19
//#define B_INCLUDE_LNKPARAMS 1//创建sor文件时是否包含LnkParams
class CSorFileBase
{
public:
	CSorFileBase(void);
	~CSorFileBase(void);
public:
	struct MapBlock
	{
		INT getsize();//获取长度
		struct blockInfo
		{
			//CString ID;//名称
			vector<char> ID;//名称
			short RevisionNo;//修订版本2
			INT Size;//大小4
		};
		blockInfo mapBlock;//Map 本身包含信息
		short blockCount;//block个数
		vector<blockInfo> vBlockInfo;//block基本情况
	}m_mapBlock;
	struct GeneralParas//
	{
		void write(vector<BYTE>& vData);
		INT getsize();//获取长度
		//CString ID;//名称
		vector<char> ID;//名称
		//CString languageCode;//语言编码
		//CString cableID;//电缆编号
		//CString fiberID;//光纤编号
		//short fiberType;//光纤类型
		//short nominalWaveLength;//额定波长????如：1310
		//CString OLocation;//Originating Location起始位置?????????
		//CString TLocation;//Terminating Location结束位置?????????
		//CString cableCode;//电缆编码?????
		//CString currentDataFlag;//条件
		vector<char> languageCode;//语言编码 固定2字节
		vector<char> cableID;//电缆编号
		vector<char> fiberID;//光纤编号
		short fiberType;//光纤类型
		short nominalWaveLength;//额定波长????如：1310
		vector<char> OLocation;//Originating Location起始位置?????????
		vector<char> TLocation;//Terminating Location结束位置?????????
		vector<char> cableCode;//电缆编码?????
		vector<char> currentDataFlag;//条件固定2字节
		INT userOffset;//???
		INT uODistance;//user Offset Distance
		//CString operater;//operator操作人员
		vector<char> operater;//名称
		//CString comment;//注释
		vector<char> comment;//注释
	}m_generalParas;
	struct SuppliersParas//
	{
		INT getsize();//获取长度
		void write(vector<BYTE>& vData);
		//CString ID;//名称
		vector<char> ID;//名称
		//CString name;//厂商名称
		//CString mainframeID;//主面板型号
		//CString mainframeSn;//主面板序列号
		//CString moduleID;//插件模板号
		//CString moduleSn;//插件（plug-in）序列号
		//CString softVersion;//软硬件版本号
		//CString other;//其它信息
		vector<char> name;//厂商名称
		vector<char> mainframeID;//主面板型号
		vector<char> mainframeSn;//主面板序列号
		vector<char> moduleID;//插件模板号
		vector<char> moduleSn;//插件（plug-in）序列号
		vector<char> softVersion;//软硬件版本号
		vector<char> other;//其它信息
	}m_suppliersParas;
	struct FixedParas//
	{
		INT getsize();//获取长度
		void write(vector<BYTE>& vData);
		//CString ID;//名称
		vector<char> ID;//名称
		UINT timestamp;//时间
		//CString distanceUnit;//单位 km,mt,ft,kf,mi 固定2字节
		vector<char> distanceUnit;//单位 km,mt,ft,kf,mi 固定2字节
		short actualWavelength;//实际测量波长
		INT acquisitionOffset;//???
		INT AODistance;//acquisition Offset Distance
		short pulseWidthNum;//Total Number of Pulse Widths Used脉宽个数
		vector<short> vPulseWidth;//Pulse Widths Used 使用的脉宽
		vector<INT> vDataSpacing;//不同脉宽，10000个数据点时间
		vector<INT> vDataPointsEach;//每个脉宽数据点个数,采样数wcq
		INT groupIndex;//自己测试得到是折射率wcq2011.10.18
		short backscatter;
		INT avgNum;//average number
		unsigned short avgTime;//average time
		INT acquisitionRange;
		INT ARDistance;//acquisitionRange distance
		INT frontPanelOff;
		unsigned short noiseFloorLevel;
		short noiseFloorFactor;//噪声级别系数
		unsigned short powerOff;//power offset first point
		unsigned short lossThreshold;
		unsigned short reflectanceThreshod;
		unsigned short eofThreshold;//end-of-fiber threshold
		//CString traceType;
		vector<char> traceType;//标准上说是string，实际上是string 固定2字节，后面没\0
		INT windowCoor1;//window coordinates左上右下坐标
		INT windowCoor2;//window coordinates
		INT windowCoor3;//window coordinates
		INT windowCoor4;//window coordinates
	}m_fixedParas;
	struct KeyEvent//
	{
		INT getsize();//获取长度
		void write(vector<BYTE>& vData);
		void clearEvent();//清空事件列表wcq2012.06.11
		//CString ID;//名称
		vector<char> ID;//名称
		short keyEventNum;//事件个数 
		struct Event
		{
			short eventNum;//事件编号
			INT propagationTime;//连接后第一个e事件点时间
			short attenCoefficent;//衰减系数
			short eventLoss;//事件损耗
			INT   reflectance;//事件反射
			//CString code;//事件标志
			//CString lossMeasurement;
			vector<char> code;//事件标志固定6字节6
			vector<char> lossMeasurement;//固定2字节2
			INT markerLocations[5];//2011.10.11
			//	CString comment;//注释
			vector<char> comment;//注释
		};
		vector<Event> vEvent;//所有事件
		INT eteLoss;//End-to-End Loss
		INT eteMarkerStart;
		INT eteMarkerFinish;
		unsigned short returnLoss;//回损
		INT RLMarkerStart;
		INT RLMarkerFinish;
	}m_keyEvent;
	struct LinkParas//
	{
		INT getsize();//获取长度
		void write(vector<BYTE>& vData);
		//CString ID;//名称
		vector<char> ID;//名称
		short totalLandmarks;//lankmarks标志个数,默认和事件列表个数一样
		struct Landmark
		{
			short num;//编号
			//CString code;//编码2字节
			vector<char> code;//编码2字节固定2字节
			INT location;//
			short relatedEventNum;//相应的事件编号
			INT GPSLongitude;//GPS经度
			INT GPSLatitude;//GPS纬度
			short fiberCorrFactor;//
			INT   sheathEntering;
			INT   sheathLeaving;
			//	CString SLUnit;//Units of Sheath Marker Leaving Landmark
			vector<char> SLUnit;//Units of Sheath Marker Leaving Landmark 2字节固定2字节
			short modeFieldDiameter;
			//CString comment;
			vector<char> comment;
		};
		vector<Landmark> vLandmark;//所有事件
	}m_linkParas;
	struct DataPoints//
	{
		INT getsize();//获取长度
		void write(vector<BYTE>& vData);
		//CString ID;//名称
		vector<char> ID;//名称
		INT dataPointNum;//数据点个数
		short totalScaleFactors;//使用的扩展系数个数
		struct DataUseScaleFactor
		{
			INT count;//编号
			short scaleFactor;//编码2字节
			vector<unsigned short> vData;//
		};
		vector<DataUseScaleFactor> vAllData;//所有数据
	}m_dataPoints;
	struct Checksum//
	{
		INT getsize();//获取长度
		void write(vector<BYTE>& vData);
		//CString ID;//名称
		vector<char> ID;//名称
		unsigned short checksum;//校验码
	}m_checksum;
	
	template<typename To,typename From>
	void vchar2string(To& t,From& f)
	{
		t.assign(f.begin(),f.end());
	}


public:
	vector< vector<BYTE> > m_vUnknownParas;//保存其他OTDR设备厂商自定义的Block.wcq2011.10.11
	CString m_errInfo;//错误提示
	//INT m_parsePos;//解析时数据字节位置,下一记录起始位置

public:
	//
	void writeMapBlock(vector<BYTE>& vData);
	void writeData(vector<BYTE>& vData);
	void init();//初始化一些参数，给一些变量赋默认值，刚测试完的数据保存时需要初始化
	BOOL checkSize(int fileLen);//读取文件时检查各个block大小之和是否大于文件总大小
	BOOL parseData(BYTE data[],int len);//打开文件后解析各个block，会调用下面几个read函数
	//读写
	unsigned short crc16(byte d[], int len);//CRC-CCITT 2011.10.8
	/*INT readSorFile(CString filePathAndName,TestResultInfo& testResultInfo);//cwcq2012.07.23
	BOOL setReturnData(TestResultInfo& testResultInfo);//将解析后的数据赋给传入的参数
	INT saveSorFile(CString filePathAndName,TestResultInfo testResultInfo);*///cwcq2012.07.23
	//void dealTestInfo(TestResultInfo testResultInfo);//保存数据时，将传进来的参数转为类成员//cwcq2012.07.23
	INT readSorFile(CString filePathAndName,SORDATA& sorData);//wcq2012.07.23
	BOOL setReturnData(SORDATA& sorData);//将解析后的数据赋给传入的参数//wcq2012.07.23
	INT saveSorFile(CString filePathAndName,SORDATA sorData);//wcq2012.07.23
	void convertDataToSor(SORDATA sorData);//保存数据时，将传进来的参数转为类成员//wcq2012.07.23
	//
	int readString(BYTE data[],vector<char>& vChar);
	//读取标准MapBlock
	INT readMap(BYTE data[]);//读取MapBlock
	INT readGeneralParas(BYTE data[]);//读取GeneralParas Block
	INT readSuppliersParas(BYTE data[]);//读取SuppliersParas Block
	INT readFixedParas(BYTE data[]);//读取FxdParams Block
	INT readKeyEvents(BYTE data[]);//读取KeyEvent Block
	INT readLinkParas(BYTE data[]);//读取LinkParas 
	INT readDataPts(BYTE data[]);//读取DataPts Block
	INT readCKsum(BYTE data[]);//读取CKsum Block
	//读取未知（非标准的自定义的） Block
	//INT readUnknownParas(BYTE data[],vector<char> nextBlockID,int startPos,int maxLen);//
	//如果需要自定义block，可在此添加

	CString readString(BYTE data[],int startPos);
};
