
// DUITest.cpp : ����Ӧ�ó��������Ϊ��
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
				::MessageBox(NULL, _T("���ǰ�ť"), _T("����˰�ť"), NULL);
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
			CControlUI* pRoot = builder.Create(_T("duilib.xml"), (UINT)0, NULL, &m_PaintManager);   // duilib.xml��Ҫ�ŵ�exeĿ¼��
			ASSERT(pRoot && "Failed to parse XML");

			m_PaintManager.AttachDialog(pRoot);
			m_PaintManager.AddNotifier(this);   // ��ӿؼ�����Ϣ��Ӧ��������Ϣ�ͻᴫ�ﵽduilib����Ϣѭ�������ǿ�����Notify����������Ϣ����
			return lRes;
		}
		// ����3����ϢWM_NCACTIVATE��WM_NCCALCSIZE��WM_NCPAINT��������ϵͳ������
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
		isMouseDown = true;//��¼��갴��

						   //��갴��ʱ������
		m_startPoint.x = GET_X_LPARAM(lParam);
		m_startPoint.y = GET_Y_LPARAM(lParam);

		//��갴��ʱ���ڵ�λ��
		GetWindowRect(this->GetHWND(), &startRect);

		return 0;
	}
	LRESULT OnLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		//������𣬸ı����״̬
		isMouseDown = false;
		return 0;
	}
	LRESULT OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		if (isMouseDown == true)
		{

			POINT point;

			//��ȡ��ǰ����λ��

			//��һ�ֻ�ȡλ�÷���
			//point.x = GET_X_LPARAM(lParam); 
			//point.y = GET_Y_LPARAM(lParam);

			//�ڶ��ֻ�ȡλ�÷���
			::GetCursorPos(&point);

			::ScreenToClient(m_PaintManager.GetPaintWindow(), &point);


			int Dx = point.x - startPoint.x;
			int Dy = point.y - startPoint.y;
			startRect.left += Dx;
			startRect.right += Dx;
			startRect.top += Dy;
			startRect.bottom += Dy;             //��ȡ�µ�λ��  
			SetWindowPos(this->GetHWND(), NULL, startRect.left, startRect.top, 0, 0, SWP_NOSIZE);     //�������Ƶ��µ�λ��  
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


// CDUITestApp ����

CDUITestApp::CDUITestApp()
{
	// ֧����������������
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CDUITestApp ����

CDUITestApp theApp;


// CDUITestApp ��ʼ��

BOOL CDUITestApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()��  ���򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	AfxEnableControlContainer();

	// ���� shell ���������Է��Ի������
	// �κ� shell ����ͼ�ؼ��� shell �б���ͼ�ؼ���
	CShellManager *pShellManager = new CShellManager;

	// ���Windows Native���Ӿ����������Ա��� MFC �ؼ�����������
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));
// 
// 	// ��׼��ʼ��
// 	// ���δʹ����Щ���ܲ�ϣ����С
// 	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
// 	// ����Ҫ���ض���ʼ������
// 	// �������ڴ洢���õ�ע�����
// 	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
// 	// �����޸�Ϊ��˾����֯��
// 	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));
// 
// 	CDUITestDlg dlg;
// 	m_pMainWnd = &dlg;
// 	INT_PTR nResponse = dlg.DoModal();
// 	if (nResponse == IDOK)
// 	{
// 		// TODO: �ڴ˷��ô����ʱ��
// 		//  ��ȷ�������رնԻ���Ĵ���
// 	}
// 	else if (nResponse == IDCANCEL)
// 	{
// 		// TODO: �ڴ˷��ô����ʱ��
// 		//  ��ȡ�������رնԻ���Ĵ���
// 	}
// 	else if (nResponse == -1)
// 	{
// 		TRACE(traceAppMsg, 0, "����: �Ի��򴴽�ʧ�ܣ�Ӧ�ó���������ֹ��\n");
// 		TRACE(traceAppMsg, 0, "����: ������ڶԻ�����ʹ�� MFC �ؼ������޷� #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS��\n");
// 	}


	CPaintManagerUI::SetInstance(AfxGetInstanceHandle());

	CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath());   // ������Դ��Ĭ��·�����˴�����Ϊ��exe��ͬһĿ¼��

	CDuiFrameWnd duiFrame;
	duiFrame.Create(NULL, _T("DUIWnd"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE);
	duiFrame.CenterWindow();
	duiFrame.ShowModal();





	// ɾ�����洴���� shell ��������
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}

