
// MainFrm.cpp : CMainFrame Ŭ������ ����
//

#include "stdafx.h"
#include "MyTool.h"
#include "MyClientView.h"
#include "MyToolView.h"

#include "MainFrm.h"
#include <Core.h>
#include <MiddleStart.h>
#include "ToolCoreBuilder.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
END_MESSAGE_MAP()

// CMainFrame ����/�Ҹ�


CMainFrame::CMainFrame() : m_ClientWindowSize((int)1024, (int)768)
{
	// TODO: ���⿡ ��� �ʱ�ȭ �ڵ带 �߰��մϴ�.
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	//// �������� Ŭ���̾�Ʈ ������ �����ϴ� �並 ����ϴ�.
	//if (!m_wndView.Create(NULL, NULL, AFX_WS_DEFAULT_VIEW,
	//	CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST, NULL))
	//{
	//	TRACE0("�� â�� ������ ���߽��ϴ�.\n");
	//	return -1;
	//}

	MiddleStart::Init();		// �ʱ�ȭ ( ����Ű, ��� )

	m_Sp.Lock();

	m_Sp.CreateStatic(this, 1, 2, WS_CHILD | WS_VISIBLE);

	m_Sp.CreateView(0, 0, RUNTIME_CLASS(MyClientView), SIZE{ m_ClientWindowSize.ix, m_ClientWindowSize.iy }, nullptr);		// â ũ�� �����ֱ� -  Ŭ���̾�Ʈ

	CWnd* ClientToolView = m_Sp.GetPane(0, 0);
	Core::Start<ToolCoreBuilder>(lpCreateStruct->hInstance, L"ToolClientWindow", ClientToolView->m_hWnd);

	m_Sp.CreateView(0, 1, RUNTIME_CLASS(MyToolView), SIZE{ 500, 600 }, nullptr);		// ��

	RECT rc = { 0, 0, m_ClientWindowSize.ix + 800, m_ClientWindowSize.iy };

	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, false);		//������ �������

	SetWindowPos(nullptr, 0, 0, rc.right - rc.left, rc.bottom - rc.top + (m_Sp.BoderSizeY() * 2), SWP_NOMOVE | SWP_NOZORDER);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.
	cs.hMenu = nullptr;					// �޴����� ����
	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;

	cs.lpszClass = AfxRegisterWndClass(0);
	return TRUE;
}

// CMainFrame ����

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG


// CMainFrame �޽��� ó����

void CMainFrame::OnSetFocus(CWnd* /*pOldWnd*/)
{
	// �� â���� ��Ŀ���� �̵��մϴ�.
	//m_wndView.SetFocus();
}

BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// �信�� ù° ũ���� �ش� ��ɿ� ��Ÿ������ �մϴ�.
	//if (m_wndView.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
	//	return TRUE;

	// �׷��� ������ �⺻ ó���մϴ�.
	return CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

