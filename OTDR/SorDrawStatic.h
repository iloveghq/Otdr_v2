// OTDRDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "EndGabor.h"
#include "Util.h"//wcq2011.09.14
#include <vector>
#include "DATASTRUCT.h"
//#include "20120801.h"
#include "Waveform.h"
#include <math.h>

//曲线纵坐标起始db,曲线纵坐标最大值//wcq2011.09.26

#define  M_FSTARTDB0 -65.63//-65.53//wcq2011.12.05
#define  M_FMAXDB0   -5.63//-9.23//wcq2011.12.05   zll 2012.10.26 由-9.23改为-15.63   lzy2014.6.17 由-15.63改为-5.63
#define  M_FSTARTDB1 -58.3//当所曲线最小值都 > ZOOM_VERTICAL_STANDARD 30*1000时使用//wcq2011.09.26
#define  M_FMAXDB1   -8.3//wcq2011.09.26  zll 2012.10.26 由-1改为-8.3
#define  ZOOM_VERTICAL_LEAST_RANGE 0.8//竖直方向放大到最大后总共显示多少dB//wcq2011.09.28
#define  ZOOM_VERTICAL_PER_TIME 0.5//竖直方向放大因子//wcq2011.09.28
#define  ZOOM_VERTICAL_COUNT 47//竖直方向最大放大次数//wcq2011.09.28//  zll 2012.12.31修改
#define  ZOOM_VERTICAL_MAX 98//竖直方向放大到最大时有多少dB//wcq2011.11.11上
#define  EVENT_POINT_HEIGHT 30//在曲线绘制事件点时会绘一条竖直线，该值为曲线高度//wcq2011.11.15
#define  EVENT_POINT_NUM_HEIGHT 20//在曲线绘制事件点时会绘一条竖直线，该值为竖直线下方的1,2..竖直方向位置//wcq2011.11.15
#define  COUNT_POINT_MAX_MIN 686*3//绘图用。曲线数据x轴上各象素最大最小值点个数,可能修改//wcq2011.11.23
#define  COUNT_GRIDLINE_HORIZONE 10//绘图用。水平方向几列//wcq2011.12.19
#define  COUNT_GRIDLINE_VERTICAL 8//绘图用。竖直方向几列//wcq2011.12.19

//消息
#define MESSAGE_ZOOMIN (WM_USER + 1001)//曲线缩小消息wcq2011.09.30
#define MESSAGE_ZOOMOUT (WM_USER + 1002)//曲线放大消息wcq2011.09.30
//
#define OTDR_DATA_READY (WM_USER+2000)////wcq2011.08.11 驱动消息，数据已准备好
#define OTDR_LIGHT_CHECK (WM_USER+2001)//驱动对有光检测命令回应//wcq2011.12.02
#define OTDR_PARAMS_CONFIG (WM_USER+2002)//驱动对参数配置命令回应//wcq2011.12.02
//有光检测 参数配置 数据已准备好区别参数//wcq2011.12.05
#define OTDR_MESSAGE_CODE_LIGHT 18//有光检测//wcq2011.12.05
#define OTDR_MESSAGE_CODE_PARAMS 19//参数配置//wcq2011.12.05
#define OTDR_MESSAGE_CODE_DATA 23//数据已准备好//wcq2011.12.05
//平均测试时当前处于哪个阶段：有光检测 参数配置 数据已准备好
#define AVG_STATE_LIGHT 1//有光检测//wcq2011.12.06
#define AVG_STATE_LINK 2//连接状态 包括实时测试下的正在测试//wcq2011.12.07
#define AVG_STATE_PARAMS 3//参数配置//wcq2011.12.06
#define AVG_STATE_TESTING 4//数据读取 正在测试 //wcq2011.12.06

//
#define TIME_SHOW_LINK_PARAMS 1000//参数配置前显示连接状态多长时间//wcq2011.12.07
//
#define COUNT_READ_DATA_OTHER 10//读取数据时除了各点数据外，其它如波长等字段个数.多少由协议决定wcq2011.12.06

//最后一次多少点时算本次缩放，多少点由该系数确定
#define ZOOM_LAST_COEFF 0.6///
#define COUNT_POINT_ONE_PIX 4///当点数很多时取点，取点时以宽度的该数据倍作为是否取点的临界点//wcq2011.09.28

#define LASER_TIMER 1
#define AVG_TEST_TIMER 2
#define STATE_PARAMS_TIMER 3 //参数配置前先显示一小会连接状态 wcq2011.12.07
#define NUM_VALIDE_LEAST 100//当CurveData中的数据个数<该值时，忽略，不进行绘制wcq2011.09.13

#define CLEAR_DATAPOINTS_NUM 1//重新测试后清空数据时设相应波长的pDataPointsNum值。设为0容易导致除0,表示清空.wcq2011.12.23

#define WIDTH_CURSOR_LEFT 36//光标移动时光标左侧宽度。wcq2011.12.26
#define WIDTH_CURSOR_INVALID 136//光标移动时光标所在位置和文字所在区域宽度。用以确定重绘区域。wcq2011.12.26
#define WIDTH_CURSOR_MOVE_LONG 20//光标长按每次移动距离 wcq2011.12.27
#define WIDTH_CURVE_MOVE_LONG 16//曲线平移 长按每次移动距离 wcq2011.12.27
#define WIDTH_OTDR_BG_MAIN_TOP 5//顶部渐变背景高度wcq2012.02.13
#define INVALID_WAVELENGTH -1//无效（未测试时默认）波长
#define INVALID_MAX_POINT 0//无效（未测试时默认）缩放数据点个数wcq2012.02.20
#define WIDTH_CURVE_SPAN 4//绘图区域和边距间距//wcq2012.02.14
#define WIDTH_CLIENT 700//客户区宽度//wcq2012.02.14
#define HEIGHT_CLIENT (480-28)//客户区宽度//wcq2012.02.14

////#define TRACE_PAINT_TIME 1//仅为了方便调试时输出绘图时间2011.12.15
//#define TRACE_FIND_MEMORY 1////仅为了方便调试时查找内存泄漏相关。2011.12.15
#define  TIMER_TEMP 12//查找内容临时用的

#define  OTDR_EVENT_NUM_MAX 100//事件最大数
typedef struct EventPtStruct
{
	unsigned int nLocation;//事件位置
	int nEventType;//事件类型0,开始事件，1反射事件，2非反射事件，4结束事件
	float fReflectValue;//反射值
	float fInsertloss;//插入损耗
	float fAttenuCoefficient;//衰减系数
	float fCumulativeLoss;//累计损耗*/

}EventPtInfo;

class CMainFrame;

// CSorDrawStatic 对话框
class CSorDrawStatic : public CButton
{
	DECLARE_DYNAMIC(CSorDrawStatic)
	// 构造
public:
	CSorDrawStatic();	// 标准构造函数
	virtual ~CSorDrawStatic();
	void FillCurveDataFromFile(TestResultInfo* pCurve);
	void EventIsSelected(int PointAOrB);
	void AddEvent();
	void RemoveEvent();
	int GetCurTestMode() const;//获取当前测量模式
	void SetCurTestMode(const int iTestMode); //设置当前测量模式wcq2011.12.06
	void SetWaveToDevice(const int iTestMode); //设置当前发送到设备的波长(m_nWaveToDevice:1550/1310) wcq2011.12.19
	//Draw Curve
	void DrawCursor(CDC *pDC);
	void DrawCurve(CDC *pDC, TestResultInfo* pTestResultInfo,int SingleorMulti);//绘制单条曲线
	void DrawCurves(CDC *pDC); //绘制所有曲线
	void CalMaxDistance();//计算所有曲线中最长的距离并赋给所有曲线结构相应变量wcq 2011.09.09

	float Get_XCoord(int xpoint);
	float Get_XCoord(unsigned int xpoint, TestResultInfo* pData);//计算距离
	float Get_XCoord2nf(unsigned int xpoint, TestResultInfo* pData);//计算距离,以c/2nf公式计算
	UINT  Get_PosPoint(float distance,TestResultInfo* pData);//wqc2012.02.21//已知距离反求数组下标
	float GetMaxDistance();////wcq2011.09.06得到所有曲线中最长的距离,以km为单位????????????????
	///float GetTestDistance(const int iPointNumber) const;//wcq2012.03.09
	void DrawCoord(CDC *pDC);
	void CalCulDispRect();
	void ReCalABLabelPos();//每次加载或读到一条新的曲线数据时重新计算A/B标位置，并给变量m_curABWave.wcq 2011.0913
	void ResetCurveData(); //重新计算曲线
	//Load Curve Data

	void EventList();
	void EventCursor();
	//Cursor Operator
	void CursorMoveLeft();
	void CursorMoveRight();
	//Curve Operate
	void CurveHorizonZoomIn();
	void HorizonZooms();//水平缩放所有曲线wcq 2011.09.13
	void HorizonZoom(TestResultInfo* pData);//缩放一条曲线wcq2011.09.13
	void CurveHorizonZoomOut();
	void CurveVerticalZoomIn();
	void CurveVerticalZoomOut();
	void VerticalZoom();
	void CurveCompare(const CString strFileName);
	void CurvesHorMove(int nRange, float nHorBalFactor);//水平移动曲线
	void CurveHorMove(TestResultInfo* pData, int nRange, float nHorBalFactor);
	void CurvesVerMove(int nDelta);//垂直移动曲线
	void CurveVerMove(TestResultInfo* pData, int nDelta, bool bSpin = false/*是否是通过旋钮调整*/);
	//事件选中切换
	void EventSelectUp();
	void EventSelectDown();
	BOOL IsLineABSeleced(CPoint point);

	virtual void SetPageTitle();
	virtual void SetCurPageText();

	//设置当前曲线缩放模式 0：缩放；1：平移
	void SetCurCurveZoomMode(int nCurveZoomMode);
	void CalDrawCurveParams(int curveWidth,CRect rectCurve,int dataCount);
	TestResultInfo* GetMaxLengthCurveData();//获取具有最长距离的曲线
	int GetZoomCount(int maxDistanceDataCount);////得到曲线缩放次数wcq 2011.09.20
	int GetLastZoomCount(int countPerTime,int dataCount,int zoomCount);////得到曲线缩放最后一次缩放的数据点个数wcq 2011.09.21
	void GetLastZoomCounts();////得到所有曲线缩放最后一次缩放的数据点个数wcq 2011.09.23
	//int GetLastZoomCount(int countPerTime,int dataCount,int zoomCount);////得到第几次缩放wcq 2011.09.21
	void CalFullPointCount();//计算所有曲线未缩放时绘满绘图区域的点数 wcq 2011.09.21
	void ReCalZoomParas();//第一次或有更长距离曲线数据到来，重新计算缩放相关参数。在里面调用GetZoomCount,CalFullPointCount,GetLastZoomCount.wcq2011.09.23
	void SetLowerStandard(TestResultInfo* pData);//设置CCurveData中的IsLowerStandard变量，以方便曲线竖直方向缩放.wcq2011.09.26
	BOOL IsLowerStandardAll();//遍布曲线所有数据，看是否至少有一个IsLowerStandard为真.wcq2011.09.26
	void CalVerticalRange(int zoomNum,float fStandPosDb);//计算竖直方向缩放时最大值最小值.赋给m_fStartDB,m_fMaxDB.wcq2011.09.26
	void MoveEventList(INT nIndex);//选择了事件列表后，重新绘制曲线;wcq2011-11.15
	void InvalidateAll();//刷新绘图区综略图波长脉宽显示区
	void GetDataCurve(TestResultInfo* pData);//给CurveData中的 pDataCurve变量赋值，得到计算过后的绘制曲线数据wcq2011.11.23
	int  GetLEvent(int realPulseWidth);//根据脉宽得到LEvent(事件列表算法的一个参数).2011.12.06
	void ResetYOffset();////从平移到缩放时把竖直方向的平移恢复为0;wcq2011.12.14
	void ClearAllCurveData();//清空所有曲线数据，设置DataPointsNum为0.//wqc2011.12.19
	void OnVKReturn();////按下回车将曲线回到未缩放/平移状态 wcq2012.01.12
	void NewTestInit(TestResultInfo* pData);//按下测试后初始化一些参数，如A/B标位置 wcq2012.02.21
	void StartTest(int iTestMode);//AVG/REAL time测试共同响应事件 wcq2012.02.21
	//绘图相关wcq2011.12.09
	void GetCursorRect(CRect *rect);//光标移动时需要重绘区域。//wcq2011.12.26
	UINT DealData();// wcq2012.02.07
	void DealDataDetail();// wcq2012.02.07

	void SetHorScroll(); //设置水平滚动条
	void SetVerScoll(); //设置竖直滚动条
	//调用dll得到数据后转化为事件列表
	void GaborResultToEvent(TestResultInfo* pCurveData);
	//从文件读取事件列表后转化为使用的GaborResult结构体 wcq2012.06.11
	void EventToGaborResult(TestResultInfo* pCurveData);

public:
	// 实现
protected:

public:

	CRect m_rectCurve; //绘制曲线的矩形区域
	CRect m_rectCoord; //坐标轴以内矩形区域
	CRect m_rectSubInfo; //AB距离显示区域wcq2011.11.16
	long int m_iStartLocation; //起点位置，单位为(m)
	long int m_iMaxLocation; //末点位置，单位(m)
	unsigned int m_nStartPoint;  //曲线横坐标的起点
	unsigned int m_nMaxPoint;    //曲线横坐标点数的最大值
	CPoint *m_ptRecord; //画图坐标点
	float m_fStartDB;    //曲线纵坐标起始db
	float m_fMaxDB;      //曲线纵坐标最大值
	float m_fFrequency;  //频率
	CArray<double,double > m_CArraySampleLoss;//存储网络传输来的采样点损耗
	BOOL m_bReceiveResultOK; //接收数据完毕

	int m_nSelectCursor; //Seleced标志,1选中A，2选中B, 4选中AB
	BOOL m_bEventCursor; //是否为事件光标
	int m_nPointA;//A线的位置点，横坐标 不可设为无符号型,  //始终以最长距离的曲线为准wcq
	int m_nPointB;//B线的位置点，横坐标  //始终以最长距离的曲线为准wcq
	int m_point_minus;//AB线之间的相对距离，zll 2013.1.6
	int m_nPointEvent; //当前事件点， 横坐标
	int m_nSelectIndex; //选中的事件点索引


	int m_iHorizonZoomNum; //水平放大次数
	int m_iVerticalZoomNum; //垂直放大次数

	BOOL m_bReadFromFile; //从文件读取了数据
	unsigned int m_nStartPointFromFile;  //曲线横坐标的起点
	unsigned int m_nMaxPointFromFile;    //曲线横坐标点数的最大值
	///CArray<double,double > m_CArraySampleLossFromFile;//存储文件读取的采样点损耗//wcq2012.03.09
	//	int m_nEventNumFromFile; //从文件读取的事件点数
	CArray<EventPtInfo,EventPtInfo> m_CArrayEventInfoFromFile; //从文件读取的事件信息
	float m_fRefraction;  //折射率
	unsigned long int	m_iEventNum; //事件数目
	CArray<EventPtInfo,EventPtInfo> m_CArrayEventInfoFromNode; //从网络接收的事件数组

	int m_iCurveZoomMode; //曲线缩放模式 0：缩放；1：平移
	int m_iTestModeForFile; //文件曲线的测量模式
	int m_iCtrlTime; //控制进度条时间


	//dwssstesting
	DWORD m_dwWaveTesting;//当前正在测试的波长 可能为1310或1550 wcq 2011.09.14
	CUtil m_util;//wcq 2011.09.14
	BOOL m_bBusy;//是否收到消息后正在处理数据 wcq 2011.09.15
	DWORD m_dwMaxPointCount;//所有曲线最大点数,方便在绘制曲线时使用 wcq 2011.09.15
	DWORD m_dwWaveSelected;//选中了哪条曲线 wcq 2011.09.16
	int m_nZoomCount;//缩放次数
	//处于测试的哪一阶段,根据该变量绘制提示信息 有光检测 连接状态(中间) 参数配置 正在测试/连接状态 (连接状态在实时测试时会出现两次)
	COLORREF m_clrCurrent; //中间变量，颜色//wcq2011.12.07
	//CRect m_rectMoveThumb; //方便曲线平移时确定平移结束位置//2011.12.14
	int m_nWaveToDevice;//当前测试时要发到设备的波长,双波长平均测试时先测完1550，再测1310.//2011.12.19
	BOOL m_nStopTestNormal;//是否按正常顺序结束测试，双波长时先测完1550，再测1310，期间有可能人重新按下测试导致结束//2011.12.19
	BOOL m_bRedrawByCursor;//重绘原因是由于光标移动.wcq2011.12.23
	int m_nDataLenMsg;//消息中包括的数据个数wcq2012.02.07
	INT m_lastPageType;//当前的页面类型，用于按下“曲线对比”退出后返回到当前界面。wcq2012.02.14
	/////是否需要重新计算A/B位置////////////////////////////////////////////////////////////
	BOOL m_bNeedHorizonZoom;//是否需要进行缩放，由下面三个参数决定 wcq2012.02.17
	INT m_lastWaveLength;//上一次波长 wcq2012.02.20
	INT m_lastDataLength;//上一次测试点数 wcq2012.02.20
	float m_lastRefraction;//上一次测试的折射率 wcq2012.02.20
	/////////////////////////////////////////////////////////////////
	BOOL m_bFirstRead;//单次测试是否第一次收到数据 wcq2012.02.21
	int m_nMaxIndex;

	MEMORYSTATUS buffer;//调试时用来检查内存用的wcq

public:
	struct LastTestInfo//wcq2012.02.21
	{//主要记录下上次测试时的一些参数
		LastTestInfo()
		{
			m_distanceA = 0;
			m_distanceB = 0;
		}
		FLOAT m_distanceA;//A标与原点距离
		FLOAT m_distanceB;//B标与原点距离
	}m_lastTestInfo;
	struct CurTestParams//wcq2012.02.21
	{//当前测试(从按下"start"到按下"stop")时的参数
		INT nSecondTestTime; //测试时间
		INT nTestMode; //测试模式，0为手动，1为自动
		//DWORD dwDataType;//数据类型：5858
		DWORD dwWaveLength;//波长：如波长为1310，表示为整数1310。
		DWORD dwPulseWidth;//脉宽：如脉宽20ns ，表示为整数20，自动配置参数50000。
		DWORD dwDistance;//测试距离：如测试距离300m，表示为300；如测试距离200KM，表示为200000。自动配置参数500000。
		//DWORD dwModel;//测试模式：平均测试2222，实时测试模式5555。配置参数4444，有光检测1111。
		DWORD dwLimit;//末端门限：应用软件中光线末端门限的值。
		FLOAT fReflect1310;//1310折射率。
		FLOAT fReflect1550;//折射率。
	}m_curTestParas;

	int m_tempInt;
	float m_fCoeff; //垂直缩放系数
	
	// actual scroll position
	int m_nScrollPosVer;
	int m_nScrollPosHor;
	// actual dialog height
	int m_nCurHeight;
	int m_nDrawHeight; //绘图区域高度(滚动条高度)
	// actual dialog width
	int m_nCurWidth;
	int m_nDrawWidth; //绘图区域宽度
	BOOL m_bZoomOK; //拖动鼠标左键时如果没有选中AB此值为真，表示对选中区域放大，否则为假，表示没有放大区域
private:
	///CString m_strCurveNameFromFile; //曲线对比，从内存设备中选取的文件//wcq2012.03.09
	//struct TestResultInfo Result;
	///int CountLen;//wcq2012.03.09
	WORD* m_pRecvbuf; //接收数据缓冲区
	///int recvlen; //接收数据长度//wcq2012.03.09
	//开始平均时间
	DWORD m_dwStartTestTime;
	///int m_nCount;//wcq2012.03.09
	POINT m_cursorPoint;
	CPoint m_PLMouseDown;//鼠标左键按下的位置;
	CRect m_RectSelected;//标识选中的放大区域

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg LRESULT OnDataReady(WPARAM wParam,LPARAM lParam);//wcq2011.08.11 收到驱动消息，进行读取

	afx_msg LRESULT OnDataParamsConfig(WPARAM wParam,LPARAM lParam);//wcq2011.12.02 收到驱动参数配置消息

	afx_msg void OnClickListEvent(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
public:
	CEndGabor m_endgabor;//zll 2013.8.19
};
