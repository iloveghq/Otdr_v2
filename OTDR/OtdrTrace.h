#pragma once
#include <math.h>
#include "EndGabor.h"
////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
//OTDR曲线绘制类
//wcq2013.02.04
//说明：
//plot:绘图
//Cur:Current当前
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
#define OTDR_TRACE_MAX 1
#define OTDR_TRACE_MIN 0
//#define POINT2FLOATX(x) m_fRangeX[OTDR_TRACE_MIN] + ( (x - m_rectCurve.left)   * m_fResolutionX)
#define POINT2FLOATX(x) m_fRangeX[OTDR_TRACE_MIN] + ( ((x - m_rectCurve.left)   * (m_fRangeX[OTDR_TRACE_MAX] - m_fRangeX[OTDR_TRACE_MIN])) / m_rectCurve.Width())
#define POINT2FLOATY(y) m_fRangeY[OTDR_TRACE_MIN] + ( (m_rectCurve.bottom - y-HIKE_UP_HEIGHT+10) * m_fResolutionY)//zll 2013.6.25 -HIKE_UP_HEIGHT+10
#define FLOAT2POINTX(x) m_rectCurve.left + (x - m_fRangeX[OTDR_TRACE_MIN]) * m_rectCurve.Width() / (m_fRangeX[OTDR_TRACE_MAX] - m_fRangeX[OTDR_TRACE_MIN])//根据距离得到X坐标
#define FLOAT2RATIO(x) (x-m_fRangeX[OTDR_TRACE_MIN]) / (m_fRangeX[OTDR_TRACE_MAX] - m_fRangeX[OTDR_TRACE_MIN])//xjf 2014.03.03  获取比例系数
#define OTDR_TRACE_Y_MAX 25 //lzy2014.6.17,原为45 
#define OTDR_TRACE_Y_RATE 1.2     //y轴最大值是所有数据中最大值再乘以此系数,lzy2014.06.24
#define OTDR_TRACE_Y_MIN 0
#define OTDR_TRACE_DEFAULT_ZOOM_PERCENT 0.02//2%默认缩放比例 zll 2013.5.11
#define OTDR_TRACE_Y_MIN_CHANGE 0.01//当竖直方向放大后只有0.1db时不再缩放
#define OTDR_TRACE_CURSOR_AB_MARGIN_TOP 30//光标AB竖直方向离顶部多远
#define OTDR_TRACE_CURSOR_AB_VALID_SELE 6//多少象素之内认为选中了A/B标
#define OTDR_TRACE_CURSOR_AB_COLOR_SELE g_COLORREF[11]//A/B标选中颜色
#define OTDR_TRACE_VALID_NUM_LOWEST 10//绘图时最少几个点以下则不缩放
#define OTDR_TRACE_Y_FACTOR 1000//y轴滚动范围为整数型，当放大到一定倍数后，nPage为0，所以需要放大一定倍数，这里设为1000//zll 2013.4.24
#define ZOOM_VERTICAL_STANDARD 30*1000//wcq2011.09.26用于竖直方向缩放时确定最大量程，大于该值时为-65~30,小于时为-55～40
#define CursorAB_Coefficient 1000.0//系数 zll 2013.3.15
#define  COUNT_GRIDLINE_HORIZONE 10//绘图用。水平方向几列//wcq2011.12.19
#define  COUNT_GRIDLINE_VERTICAL 9//绘图用。竖直方向几列//wcq2011.12.19
#define  Divisor_toKm 1000;//zll 2013.5.11
#define OTDR_TRACE_COLOR_SELECT RGB(255, 255, 0)//zll 2013.5.11
#define OTDR_SQUARE_FRAME_COLOR RGB(255, 0, 0)//zll 2013.10.29,方框为红色，与默认选中曲线为黄色区分
#define HIKE_UP_HEIGHT 50//zll 2013.6.25
extern COLORREF g_COLORREF[12];
extern int g_nTraceYmax;     //lzy 2014.06.24

enum CURVE_CURRECT_MODE
{
	CURVE_NONE=0,
	CURVE_GET_DATA,
	CURVE_SELECT,
	CURVE_ZOOM_OUT ,
	CURVE_ZOOM_OUT_V,
	CURVE_ZOOM_OUT_H,
	CURVE_ZOOM_IN,
	CURVE_ZOOM_IN_V,
	CURVE_ZOOM_IN_H,
	CURVE_MOVE,
	CURVE_MOVE_UP,
	CURVE_MOVE_DOWN,
	CURVE_MOVE_LEFT,
	CURVE_MOVE_RIGHT,
	CURVE_RESET
};
enum OTDR_TRACE_AB_SELECT//光标AB选中状态
{
	OTDRTRACE_AB_SELE_NONE=0,
	OTDRTRACE_AB_SELE_A,
	OTDRTRACE_AB_SELE_B,
	OTDRTRACE_AB_SELE_AB
};
enum OTDR_TRACE_AB_TYPE//光标AB模式  
{
	OTDRTRACE_AB_TYPE_SINGLE_A_OR_B = 0,//选一个(A/B)
	OTDRTRACE_AB_SELE_DOUBLE_AB//AB模式
};
///////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////
//曲线
class CGraphElement
{
public:
	CGraphElement()
	{
		bIsPlotAvailable = FALSE;
		m_LineColor = RGB(0,0,0);
		m_bShow = TRUE;
		//m_strName.Format(_T("Element-%d"),id);
		m_pData = NULL;
		m_nOffsetY = 0;
		m_bShowEvent = TRUE;
		bSelected = FALSE;
	}//eif
	struct TraceEvent
	{
		INT m_nIndex;
		FLOAT m_distance;
	};
	INT  DistanceToIndex(FLOAT fDistance);//根据距离转化为对应的数据点下标,一般用来确定每次绘图的起始点及结束点
	void DrawCurve(CDC* pDC, CRect rect,int nIndexCurve);//绘制曲线
	void SetYOffset(INT nOffset=1);//竖直方向平移
	//void DrawEvents(CDC* pDC, INT nStartIndex, INT nEndIndex);//绘制事件
	BOOL m_bShow;//是否显示
	BOOL m_bShowEvent;//是否显示事件列表
	BOOL bSelected;//是否是选中曲线
	BOOL bIsPlotAvailable;//是否可绘
	COLORREF m_LineColor;//线条颜色
	CString m_strName;//名称
	INT m_nPointsCount;//总共有多少个数据点,当前曲线实际点数
	//INT m_nPointsCountVirtual;//虚拟曲线点数。主要是为了多条曲线显示，以最长距离曲线为准计算当前曲线应该有多少个点(比当前实际的点数大)时，得到的长度等于最长距离曲线长度
	INT m_nOffsetY;//y轴偏移量
	FLOAT m_fLength;//长度
	static FLOAT m_fRangeX[2];//保存当前绘图时最小最大距离,相对x轴，针对多条曲线
	static FLOAT m_fRangeY[2];//保存当前绘图时y轴范围，针对多条曲线
	static FLOAT m_fMaxDistance;//最长距离，针对多条曲线
	FLOAT m_fDataSpacing;//理解为两点间距离,曲线两点间距离
	FLOAT* m_pData;//点集合
	FLOAT* m_pDataSmooth;//平滑点集合
	std::vector<TraceEvent> m_vAllEvents;
};

// COtdrTrace

class COtdrTrace : public CButton
{
	DECLARE_DYNAMIC(COtdrTrace)

public:
	COtdrTrace();
	virtual ~COtdrTrace();

public:
	struct CursorAB
	{
		CursorAB()
		{
			m_nPos = 0;
			m_fPos = 0;
			m_colorLine = RGB(0,0,255);//如选中A，则此时B为m_colorLine，选中B，则此时A为m_colorLine
			m_colorLabel = RGB(255,255,255);//A或B坐标旁边的文本，非事件坐标旁边的文本
		}
		INT m_nPos;//位置，针对屏幕，多少象素
		FLOAT m_fPos;//位置，多少km,针对曲线位置
		CString m_strAOrB;//"A"/"B"
		CString m_strDistance;//多少距离，如100km，光标旁显示的字符串
		CString m_strDB;//zll 2013.12.6 增加 AB光标的dB值
		COLORREF m_colorLine;//竖线颜色
		COLORREF m_colorLabel;//竖线上文本，如A 100km颜色
	}m_cursorA,m_cursorB,m_cursorAminusB;//zll 2013.6.25 增加m_cursorAminusB
public:
	void InitPaint(CDC* pDC);//初始化绘图用的笔刷等
	void DoDraw(CDC* pDC);
	void CreateMyFont(CDC* pDC, CFont& font, CString fontName = _T(""));//创建字体
	void DrawMouseSeleRect(CDC* pDc);//绘制鼠标拖动矩形
	void DrawCurves(CDC* pDC,INT m_nCurveIndex);//绘制所有曲线//zll 2013.6.25，增加参数INT m_nCurveIndex
	void DrawLineAB(CDC* pDC);//绘制AB光标
	void DrawLineABDetail(CDC* pDC, CursorAB* pCursorAB);//具体绘制函数.绘制AB光标
	void DrawGrid_old(CDC *pDC);//绘制GridLine//zll 2013.4.22
	void DrawGrid(CDC *pDC);//绘制GridLine//lzy2014.7.16
	void DrawScale(CDC *pDC); //绘制刻度 lzy2014.7.17
	void CalculateRect();//计算绘图区域
	void SetRange(float xmin, float xmax, float ymin, float ymax, BOOL bSetScrollInof = TRUE);//设置绘图界限，需要重绘
	void SetMaxDistance(FLOAT fMaxDistance);//设置最长距离
	FLOAT GetMaxDistance();//得到最长距离
	void ZoomInHorizonForBig(FLOAT fPercent = OTDR_TRACE_DEFAULT_ZOOM_PERCENT);//水平放大 fPercent放大比例，默认2%
	void ZoomOutHorizonForSmall(FLOAT fPercent = OTDR_TRACE_DEFAULT_ZOOM_PERCENT);//水平缩小 fPercent放大比例，默认2%
	void ZoomInVertialForBig(FLOAT fPercent = OTDR_TRACE_DEFAULT_ZOOM_PERCENT);//竖直放大 fPercent放大比例，默认2%
	void ZoomOutVertialForSmall(FLOAT fPercent = OTDR_TRACE_DEFAULT_ZOOM_PERCENT);//竖直缩小 fPercent放大比例，默认2%
	void ZoomReset();//还原，水平竖直缩放为
	void ZoomOutClickForBig(INT nPointX, INT nPointY, FLOAT fPercent = OTDR_TRACE_DEFAULT_ZOOM_PERCENT * 5);//单击放大，默认放大10%
	void CursorABOnMove(CPoint pointMoving);//拖动A/B标时重新计算A/B标位置
	FLOAT  GetCenterOffset();//根据当前选择的A/B标，计算缩放时的中心偏移
	FLOAT GetYDB(FLOAT fPos);//得到某点对应的db值。fPos:某处的位置
	//INT GetABPositionOnMove(CPoint pointMoving);//光标移动时计算AB对应的X轴坐标
	//FLOAT GetABDistanceOnMove(CPoint pointMoving, INT nXAB);//得到AB标上面的距离

	INT GetABSelectState(CPoint pointClick);//按下鼠标时，判断是否选中AB标
	BOOL IsInCurRangeX(FLOAT fDistance);//判断是否在当前显示范围内,针对水平方向
	BOOL IsInCurRangeY(FLOAT fDistance);//判断是否在当前显示范围内,针对竖直方向
	CGraphElement* GetSeleCurve();


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

	void SetVerScoll(); //设置竖直滚动条
	void SetHorScroll();//设置水平滚动条
	void InitScrollInfo();//初始化滚动条//zll 2013.4.24
	void SetMyScrollInfo(float xmin, float xmax, float ymin, float ymax);//设置滚动条范围

	//////////////////////////////////////////////////////////////////////////
	void SetScrollRangeX();//重设水平方向范围//zll2013.4.23
	void AddEvent();
	//调用dll得到数据后转化为事件列表
	void GaborResultToEvent(TestResultInfo* pCurveData);
	float Get_XCoord(unsigned int xpoint, TestResultInfo* pData);//计算距离

	void MoveEventList(INT nIndex);//选择了事件列表后，重新绘制曲线;wcq2011-11.15
	void HorizonZoom(TestResultInfo* pData);//缩放一条曲线wcq2011.09.13
	void ReCalABLabelPos();//每次加载或读到一条新的曲线数据时重新计算A/B标位置，并给变量m_curABWave.wcq 2011.0913
	void VerticalZoom();
	void NewTestInit(TestResultInfo* pData);//按下测试后初始化一些参数，如A/B标位置 wcq2012.02.21

	void CalMaxDistance();//计算所有曲线中最长的距离并赋给所有曲线结构相应变量wcq 2011.09.09
	void ReCalZoomParas();//第一次或有更长距离曲线数据到来，重新计算缩放相关参数。在里面调用GetZoomCount,CalFullPointCount,GetLastZoomCount.wcq2011.09.23
	int GetZoomCount(int maxDistanceDataCount);////得到曲线缩放次数wcq 2011.09.20
	void CalFullPointCount();//计算所有曲线未缩放时绘满绘图区域的点数 wcq 2011.09.21
	void GetLastZoomCounts();////得到所有曲线缩放最后一次缩放的数据点个数wcq 2011.09.23
	int GetLastZoomCount(int countPerTime,int dataCount,int zoomCount);////得到曲线缩放最后一次缩放的数据点个数wcq 2011.09.21
	void CalVerticalRange(int zoomNum,float fStandPosDb);//计算竖直方向缩放时最大值最小值.赋给m_fStartDB,m_fMaxDB
	BOOL IsLowerStandardAll();//遍布曲线所有数据，看是否至少有一个IsLowerStandard为真.wcq2011.09.26
	UINT  Get_PosPoint(float distance,TestResultInfo* pData);//wqc2012.02.21//已知距离反求数组下标

	float Get_XCoord2nf(unsigned int xpoint, TestResultInfo* pData);//计算距离,以c/2nf公式计算
	void SetLowerStandard(TestResultInfo* pData);//设置CCurveData中的IsLowerStandard变量，以方便曲线竖直方向缩放.wcq2011.09.26
	void EventIsSelected(int PointAOrB);
	void GetDataCurve(TestResultInfo* pData);//给CurveData中的 pDataCurve变量赋值，得到计算过后的绘制曲线数据wcq2011.11.23
public:
	CRect m_rectMouseSelect;//鼠标选择区域cwcq
	CRect m_rectClient;//
	CRect m_rectCurve; //曲线绘图区域大小
	BOOL m_bLeftButtonDown;
	BOOL m_bInitPaint;//是否已进行过初始化笔刷等
	INT m_nCurMode;//模式
	CDC m_dcGrid;//绘网格DC
	CDC m_dcCurve;//绘图DC分辨率
	CBitmap m_bmpGrid;
	CBitmap m_bmpCurve;
	FLOAT m_fResolutionY, m_fResolutionX;//(屏幕每一象素占多少个数据点/每一象素对应曲线多长距离)
	FLOAT m_fRangeY[2], m_fRangeX[2];//最大最小值
	FLOAT m_fOldRangeX[2], m_fOldRangeY[2];//原来的最大最小值,用于缩小
	FLOAT m_fMaxDistance;//最长距离，针对多条曲线
	std::vector<CGraphElement> m_vAllData;//所有数据

	BOOL m_bShowLineAB;//是否显示A/B光标
	BOOL m_bDrawGrid;//是否绘制格子线(GridLine)//zll 2013.4.22
	INT  m_nCurABSele;//当前是否选中AB标，如果选中，选中是A还是B
	INT  m_nABSele;//当前A还是B作为缩放中心
	INT  m_nCurveIndex;//当前选中曲线下标
	INT  m_nABType;//光标选择模式，当前只选一个光标(A/B)或同时选两个(AB,拖动一条另一条也会动)
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//滚动条
	int m_nHorzPos;
	FLOAT m_nVertPos;
	int m_nHorzMaxSize;
	FLOAT m_nVertMaxSize;
	int m_nHorzPageSize;
	FLOAT m_nVertPageSize;
	int m_nHorzOneStep;
	FLOAT m_nVertOneStep;
	int m_nOneStep;

	int m_nSelectCursor; //Seleced标志,1选中A，2选中B, 4选中AB
	int m_nPointA;//A线的位置点，横坐标 不可设为无符号型,  //始终以最长距离的曲线为准wcq
	int m_nPointB;//B线的位置点，横坐标  //始终以最长距离的曲线为准wcq

	int m_iHorizonZoomNum; //水平放大次数
	int m_nDrawWidth; //绘图区域宽度
	int m_nZoomCount;//缩放次数
	unsigned int m_nStartPoint;  //曲线横坐标的起点
	unsigned int m_nMaxPoint;    //曲线横坐标点数的最大值
	int m_nMaxIndex;
	CRect m_rectCoord; //坐标轴以内矩形区域
	int m_iVerticalZoomNum; //垂直放大次数
	float m_fCoeff; //垂直缩放系数
	int m_nDrawHeight; //绘图区域高度(滚动条高度)
	float m_fStartDB;    //曲线纵坐标起始db
	float m_fMaxDB;      //曲线纵坐标最大值

	int m_nDataLenMsg;//消息中包括的数据个数wcq2012.02.07
	WORD* m_pRecvbuf; //接收数据缓冲区
	DWORD m_dwMaxPointCount;//所有曲线最大点数,方便在绘制曲线时使用 wcq 2011.09.15
	BOOL m_bFirstRead;//单次测试是否第一次收到数据 wcq2012.02.21
	BOOL m_bBusy;//是否收到消息后正在处理数据 wcq 2011.09.15
	BOOL m_bNeedHorizonZoom;//是否需要进行缩放，由下面三个参数决定 wcq2012.02.17
	BOOL m_bEventCursor; //是否为事件光标
	int m_nSelectIndex; //选中的事件点索引
	DWORD m_dwWaveSelected;//选中了哪条曲线 wcq 2011.09.16
	int m_nPointEvent; //当前事件点， 横坐标
	CRect m_rectSubInfo; //AB距离显示区域wcq2011.11.16	
	BOOL m_bShowScale;  //显示坐标lzy2014.9.9
	CPoint m_ptMouse;  //保存鼠标的位置lzy2014.9.9
	BOOL m_bShowEventList;  //显示坐标wcq2015.05.06

	WORD m_wdGridH;  //水平轴分割数
	WORD m_wdGridV;  //垂直轴分割数
public:
	CFont m_font;
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
protected:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
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
public:
	TestResultInfo* GetMaxLengthCurveData();//获取具有最长距离的曲线
public:
	INT GetCurrentCursorIndex();//得到当前光标对应当前曲线下标	
	void OnShowAB();  //菜单的响应，AB表的显示, lzy2014.9.9
	void OnShowScale(); //显示坐标，lzy2014.9.9
	void OnShowEventList(); //显示坐标，wqc2015.05.06
//zll 2013.5.9
	public:
		void CalCulDispRect();
		BOOL IsCurrentTypeAB();//当前是否选中AB标，同时移动AB标
		void SetCurrentTypeAB(INT nABType);//OTDRTRACE_AB_TYPE_SINGLE_A_OR_B或OTDRTRACE_AB_SELE_DOUBLE_AB
		void TraceCountChanged();//新增了曲线或删除了曲线，计算下最长距离等
		void ReCalMaxDistance();//从新计算下最长距离，主要是针对删除一条曲线
		void SetCurrentSeleTrace(INT nIndex);//设为当前选中曲线//zll 2013.5.11
		void SetShowCurve(int iNum,BOOL boolShow);
		void SetCurveCursorData(); //设置曲线光标数据 //xjf 2014.3.3		
		void ShowCatch(CDC *pDC,CPoint pt);  //显示抓取AB标的小图形,lzy2014.9.9
public:
	CEndGabor m_endgabor;//zll 2013.8.19
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	void OnInitAB(void);
};


