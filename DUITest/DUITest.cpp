
// DUITest.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "DUITest.h"
#include "DUITestDlg.h"

#include <UIlib.h>
using namespace DuiLib;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#ifdef _DEBUG
#   ifdef _UNICODE
#       pragma comment(lib, "DuiLib_ud.lib")
#   else
#       pragma comment(lib, "DuiLib_d.lib")
#   endif
#else
#   ifdef _UNICODE
#       pragma comment(lib, "DuiLib_u.lib")
#   else
#       pragma comment(lib, "DuiLib.lib")
#   endif
#endif
class CDuiFrameWnd : public CWindowWnd, public INotifyUI
{
public:
	virtual LPCTSTR GetWindowClassName() const { return _T("DUIMainFrame"); }
	virtual void    Notify(TNotifyUI& msg)
	{
		if (msg.sType == _T("click"))
		{
			if (msg.pSender->GetName() == _T("btnHello"))
			{
				::MessageBox(NULL, _T("我是按钮"), _T("点击了按钮"), NULL);
			}
		}
	}

	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		LRESULT lRes = 0;

		if (uMsg == WM_CREATE)
		{
			m_PaintManager.Init(m_hWnd);

			CDialogBuilder builder;
			CControlUI* pRoot = builder.Create(_T("duilib.xml"), (UINT)0, NULL, &m_PaintManager);   // duilib.xml需要放到exe目录下
			ASSERT(pRoot && "Failed to parse XML");

			m_PaintManager.AttachDialog(pRoot);
			m_PaintManager.AddNotifier(this);   // 添加控件等消息响应，这样消息就会传达到duilib的消息循环，我们可以在Notify函数里做消息处理
			return lRes;
		}
		// 以下3个消息WM_NCACTIVATE、WM_NCCALCSIZE、WM_NCPAINT用于屏蔽系统标题栏
// 		else if (uMsg == WM_NCACTIVATE)
// 		{
// 			if (!::IsIconic(m_hWnd))
// 			{
// 				return (wParam == 0) ? TRUE : FALSE;
// 			}
// 		}
// 		else if (uMsg == WM_NCCALCSIZE)
// 		{
// 			return 0;
// 		}
// 		else if (uMsg == WM_NCPAINT)
// 		{
// 			return 0;
// 		}

		if (m_PaintManager.MessageHandler(uMsg, wParam, lParam, lRes))
		{
			return lRes;
		}

		return __super::HandleMessage(uMsg, wParam, lParam);
	}
	bool isMouseDown;
	CPoint m_startPoint;
	CPoint startPoint;
	CRect startRect;
	LRESULT OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		isMouseDown = true;//记录鼠标按下

						   //鼠标按下时的坐标
		m_startPoint.x = GET_X_LPARAM(lParam);
		m_startPoint.y = GET_Y_LPARAM(lParam);

		//鼠标按下时窗口的位置
		GetWindowRect(this->GetHWND(), &startRect);

		return 0;
	}
	LRESULT OnLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		//左键弹起，改变鼠标状态
		isMouseDown = false;
		return 0;
	}
	LRESULT OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		if (isMouseDown == true)
		{

			POINT point;

			//获取当前鼠标的位置

			//第一种获取位置方法
			//point.x = GET_X_LPARAM(lParam); 
			//point.y = GET_Y_LPARAM(lParam);

			//第二种获取位置方法
			::GetCursorPos(&point);

			::ScreenToClient(m_PaintManager.GetPaintWindow(), &point);


			int Dx = point.x - startPoint.x;
			int Dy = point.y - startPoint.y;
			startRect.left += Dx;
			startRect.right += Dx;
			startRect.top += Dy;
			startRect.bottom += Dy;             //获取新的位置  
			SetWindowPos(this->GetHWND(), NULL, startRect.left, startRect.top, 0, 0, SWP_NOSIZE);     //将窗口移到新的位置  
		}

		return 0;
	}
protected:
	CPaintManagerUI m_PaintManager;
};


// int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
// {
// 	CPaintManagerUI::SetInstance(hInstance);
// 
// 	CDuiFrameWnd duiFrame;
// 	duiFrame.Create(NULL, _T("DUIWnd"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE);
// 	duiFrame.ShowModal();
// 	return 0;
// }








// CDUITestApp

BEGIN_MESSAGE_MAP(CDUITestApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CDUITestApp 构造

CDUITestApp::CDUITestApp()
{
	// 支持重新启动管理器
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CDUITestApp 对象

CDUITestApp theApp;


// CDUITestApp 初始化

BOOL CDUITestApp::InitInstance()
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。  否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	AfxEnableControlContainer();

	// 创建 shell 管理器，以防对话框包含
	// 任何 shell 树视图控件或 shell 列表视图控件。
	CShellManager *pShellManager = new CShellManager;

	// 激活“Windows Native”视觉管理器，以便在 MFC 控件中启用主题
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));
// 
// 	// 标准初始化
// 	// 如果未使用这些功能并希望减小
// 	// 最终可执行文件的大小，则应移除下列
// 	// 不需要的特定初始化例程
// 	// 更改用于存储设置的注册表项
// 	// TODO: 应适当修改该字符串，
// 	// 例如修改为公司或组织名
// 	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));
// 
// 	CDUITestDlg dlg;
// 	m_pMainWnd = &dlg;
// 	INT_PTR nResponse = dlg.DoModal();
// 	if (nResponse == IDOK)
// 	{
// 		// TODO: 在此放置处理何时用
// 		//  “确定”来关闭对话框的代码
// 	}
// 	else if (nResponse == IDCANCEL)
// 	{
// 		// TODO: 在此放置处理何时用
// 		//  “取消”来关闭对话框的代码
// 	}
// 	else if (nResponse == -1)
// 	{
// 		TRACE(traceAppMsg, 0, "警告: 对话框创建失败，应用程序将意外终止。\n");
// 		TRACE(traceAppMsg, 0, "警告: 如果您在对话框上使用 MFC 控件，则无法 #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS。\n");
// 	}


	CPaintManagerUI::SetInstance(AfxGetInstanceHandle());

	CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath());   // 设置资源的默认路径（此处设置为和exe在同一目录）

	CDuiFrameWnd duiFrame;
	duiFrame.Create(NULL, _T("DUIWnd"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE);
	duiFrame.CenterWindow();
	duiFrame.ShowModal();





	// 删除上面创建的 shell 管理器。
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	// 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，
	//  而不是启动应用程序的消息泵。
	return FALSE;
}

