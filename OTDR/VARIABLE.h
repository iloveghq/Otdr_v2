//存放define变量及一些全局常量
//说明：正式使用时删除MY_DEBUG_TRACE定义??????????????????????????

//??????????????????????????????????????????????
//#define MY_DEBUG_TRACE 1//调试时用来TRACEG一些变量等，正式使用时删除/注释?wcq101130???????????????????????????	 
///#define MY_DEBUG_TO_LOG 1//是否通过日记来记录程序出错时的状态。若不需要则注释。wcq20110801???????????????????????????	 

//信息提示框
#define MESSAGE(info) MessageBox(info,_T("OTDR Viewer"),MB_OK|MB_ICONEXCLAMATION)//提示对话框
#define MESSAGEYN(info) MessageBox(info,_T("OTDR Viewer"),MB_YESNO|MB_ICONQUESTION)//提示对话框,上面有是否IDYES/IDNO
#define MESSAGEOC(info) MessageBox(info,_T("OTDR Viewer"),MB_OKCANCEL|MB_ICONQUESTION)//提示对话框,上面有确定取消IDOK/IDCANCEL
#define SET_CHECK(ID,bCheck) ((CButton*)this->GetDlgItem(ID))->SetCheck(bCheck)//check/radio选中
#define GET_CHECK(ID) ((CButton*)this->GetDlgItem(ID))->GetCheck()//check/radio选中
#define SET_TEXT(ID,STR) this->GetDlgItem(ID)->SetWindowTextW(STR)//check/radio选中
#define GET_TEXT(ID,STR) this->GetDlgItem(ID)->GetWindowTextW(STR)//check/radio选中
#define ENABLE_ID(ID,bEn) (this->GetDlgItem(ID)->EnableWindow(bEn))//check/radio选中
#define SHOW_ID(ID) (this->GetDlgItem(ID)->ShowWindow(SW_SHOW))//显示
#define HIDE_ID(ID) (this->GetDlgItem(ID)->ShowWindow(SW_HIDE))//隐藏

//
#define MAIN ((CMainFrame *)AfxGetMainWnd())//隐藏
//
#define STRING_ROW _T("Low")//
//
#define DEFAULT_WAVEFORM_HEIGHT 490//曲线窗口默认高度
#define DEFAULT_EVENTLIST_HEIGHT_BOTTOM 62//
#define DEFAULT_STATUSBAR_HEIGHT 19 //状态栏高度
#define DEFAULT_EVENTLIST_DIST 62 //事件列表距离窗口底部高度
#define DEFAULT_MAIN_SETTING_WIDTH 250 //主设置窗口宽度

#define COOR_SPLIT_COUNT_H 10//横坐标10等分//zll 2013.5.28
#define COOR_SPLIT_COUNT_V 8//纵坐标9等分//zll 2013.5.28 由10等分改为9等分
#define MAX_CURVE_NUM 500//最多加载8条曲线数据 //zll 2012.9.24 暂修改 可加载多条曲线
#define NUM_VALIDE_LEAST 100//当TestResultInfo中的数据个数<该值时，忽略，不进行绘wcq2011.09.13

//报表尺寸
#define REPORT_HEADER_HEIGHT 45
#define REPORT_WIZARD_WIDTH 489
#define REPORT_WIZARD_HEIGHT 650//589 //zll 2012.10.26修改
#define REPORT_WIZARD_BOTTOM_HEIGHT -10
#define REPORT_WIZARD_LEFT_OFFSET -200

//与驱动通讯///////////////////////////////////////////////////////////////////////////////////////
// 开放给运用软件
#define IOCTL_FRM_READ_VER		                0x04001060
#define IOCTL_FRM_READ_SERIAL			        0x04001061
#define IOCTL_FRM_TEST                          0x04001062
//生产测试时用
#define IOCTL_FRM_WRITE_SERIAL			        0x08001061

///////////////////////////////////////////////////////////////////////////////
//消息
///////////////////////////////////////////////////////////////////////////////
#define OTDR_DATA_READY (WM_USER+1000)//驱动已把数据准备好
#define WAVEFROM_LEFT_LIST_FRESH (WM_USER+1001)//刷新界面列表

#define SOCK_ONRECEIVE (WM_USER+1002)//接收Socket消息
#define SOCK_ONCLOSE (WM_USER+1003)//结束Socket消息

#define SOCK_ONRECEIVE_DATA_READY (WM_USER+1004)//接收Socket数据

//定义一些变量
#define COLOR_WHITE RGB(255,255,255)//白色
///#define COLOR_DARKGREEN RGB(192,192,192)//银白色
#define COLOR_BLACK RGB(0,0,0)//黑色
#define COLOR_RED RGB(255,0,0)//红色
#define COLOR_PURPLE RGB(128,0,128)//紫色
#define COLOR_YELLOW RGB(255,255,0)//黄色，按钮文本颜色
#define COLOR_GOLD RGB(255,215,0)//金色,选中条目颜色
#define COLOR_BLUEVIOLET RGB(138,43,266)//深紫罗兰的蓝色
#define COLOR_SLATEBLUE RGB(106,90,205)//板岩暗蓝灰色
#define COLOR_DARKGREEN RGB(0,100,0)//深绿色
#define COLOR_LOWNEEN RGB(124,252,0)//草坪绿,选中条目颜色

#define COLOR_CURVE COLOR_DARKGREEN//曲线颜色
/*******************************************************************************/
/* Socket设置                                                                     */
/*****************************************************************************/
#define NET_TYPE_SYSTEM _T("1")//网络通讯代表系统设置 wcq2012.05.18
#define NET_TYPE_SETUP _T("2")//网络通讯代表测试设置 wcq2012.05.18
#define NET_TYPE_SYSTEM_GET _T("get1")//网络通讯代表获取下位机系统设置 wcq2012.05.18
#define NET_TYPE_SETUP_GET _T("get2")//网络通讯代表获取下位机测试设置 wcq2012.05.18

/*******************************************************************************/
/* 单位相关设置                                                                     */
/*****************************************************************************/
#define UNIT_SHOW_KM _T("m")//AB标位置当选择了"Km"时显示为"m"
#define UNIT_SHOW_KFEET _T("ft")//AB标位置当选择了"kfeet"时显示为"m"
#define UNIT_SHOW_MILE _T("mi")//AB标位置当选择了"miles"时显示为"m"

//报表设计相关
#define REPORT_RPT_PATH_REG _T("filepathrpt")//报表位置注册表名称
#define REPORT_RPT_DEFAULT_PATH _T("\\default\\curve\\")//默认报表文件路径