//增加A\B曲线绘制数据
struct CursorInfo
{
	BOOL bDrawAB; //是否绘制
	FLOAT fRatioA; //A光标在坐标中的比例系数	
	CString strDistanceA;
	CString strDBA;
	CString strA;
	

	FLOAT fRatioB;//B光标在坐标中的比例系数
	CString strDistanceB;
	CString strDBB;
	CString strB;
};