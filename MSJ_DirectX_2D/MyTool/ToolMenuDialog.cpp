// ToolMenuDialog.cpp : 姥薄 督析脊艦陥.
//

#include "stdafx.h"
#include "MyTool.h"
#include "ToolMenuDialog.h"
#include "afxdialogex.h"
#include <Scene.h>
#include "MapDialog.h"
#include "AnimationDialog.h"
#include "SpriteDialog.h"


// ToolMenuDialog 企鉢 雌切脊艦陥.

IMPLEMENT_DYNAMIC(ToolMenuDialog, CDialogEx)

ToolMenuDialog::ToolMenuDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_TOOLMENUDIALOG, pParent)
{

}

ToolMenuDialog::~ToolMenuDialog()
{
	for (size_t i = 0; i < m_TabInfo.size(); ++i)
	{
		if (nullptr != m_TabInfo[i].pDlg)
		{
			delete m_TabInfo[i].pDlg;		// 陥戚杖稽益 舛左陥 赤陥檎 肢薦
		}
	}
}

void ToolMenuDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MENUTAB, m_Tab);

#pragma region DDX_Control 竺誤
	//軒社什 拭巨斗稽 食君 曽嫌税 企鉢 雌切研 幻級嬢兜壱 益依聖 紫遂馬形檎 益 切据級聖 痕呪人 尻衣聖 背操醤廃陥. しさし
	//戚杏 走舛背爽澗 牌鯉戚 DoDataExchange()昔汽 戚 敗呪 照拭 DDX_域伸税 敗呪研 紫遂背辞 元滴獣佃操醤廃陥.
#pragma endregion

}


BEGIN_MESSAGE_MAP(ToolMenuDialog, CDialogEx)
	ON_BN_CLICKED(IDOK, &ToolMenuDialog::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &ToolMenuDialog::OnBnClickedCancel)
	ON_NOTIFY(TCN_SELCHANGE, IDC_MENUTAB, &ToolMenuDialog::OnTcnSelchangeMenutab)
END_MESSAGE_MAP()


// ToolMenuDialog 五獣走 坦軒奄脊艦陥.


void ToolMenuDialog::OnBnClickedOk()
{
	// TODO: 食奄拭 珍闘継 硝顕 坦軒奄 坪球研 蓄亜杯艦陥.
	//CDialogEx::OnOK();
}


void ToolMenuDialog::OnBnClickedCancel()
{
	// TODO: 食奄拭 珍闘継 硝顕 坦軒奄 坪球研 蓄亜杯艦陥.
	//CDialogEx::OnCancel();
}


void ToolMenuDialog::ChangeTab(const wchar_t* _Name)
{
	// 戚硯拭魚献 吐 郊蚊爽奄
	for (int i = 0; i < (int)(m_TabInfo.size()); ++i)
	{
		if (m_TabInfo[i].Name == _Name)
		{
			m_Tab.SetCurSel(i);
			if (true == m_TabInfo[i].m_bScene)
			{
				// 樟亀.. 限惟 郊蚊爽奄 
				Scene::Scene_Change(m_TabInfo[i].Name.GetString());

				// 陥戚杖稽益亜 赤陥檎
				if (nullptr != m_TabInfo[i].pDlg)
				{
					RECT RC;

					m_Tab.GetWindowRect(&RC);		// 紫唖莫亜閃神奄(滴奄)
					m_Tab.ScreenToClient(&RC);			// 適虞戚情闘 鉢檎滴奄..(?)亜閃神奄

					AllDialogHide();

					m_TabInfo[i].pDlg->SetWindowPos(nullptr, 2, 22, RC.right - (2 * 2), RC.bottom - (26), SWP_NOZORDER);
					//m_TabInfo[_Index].pDlg->SetWindowPos(nullptr, 100, 100, RC.right - (10 * 10), RC.bottom - (26), SWP_NOZORDER);
					m_TabInfo[i].pDlg->ShowWindow(SW_SHOWDEFAULT);
				}
				else
				{
					AllDialogHide();
				}
			}
		}
	}
}

void ToolMenuDialog::ChangeTab(int _Index)
{
	// 昔畿什拭 魚献 吐 郊蚊爽奄
	m_Tab.SetCurSel(_Index);

	if (true == m_TabInfo[_Index].m_bScene)
	{
		// 樟戚 赤陥檎~ 樟端昔走~
		Scene::Scene_Change(m_TabInfo[_Index].Name.GetString());
	}

	// 陥戚杖稽益亜 赤陥檎
	if (nullptr != m_TabInfo[_Index].pDlg)
	{
		RECT RC;

		m_Tab.GetWindowRect(&RC);		// 紫唖莫亜閃神奄(滴奄)
		m_Tab.ScreenToClient(&RC);			// 適虞戚情闘 鉢檎滴奄..(?)亜閃神奄

		AllDialogHide();

		m_TabInfo[_Index].pDlg->SetWindowPos(nullptr, 2, 22, RC.right - (2 * 2), RC.bottom - (26), SWP_NOZORDER);
		//m_TabInfo[_Index].pDlg->SetWindowPos(nullptr, 100, 100, RC.right - (10 * 10), RC.bottom - (26), SWP_NOZORDER);
		m_TabInfo[_Index].pDlg->ShowWindow(SW_SHOWDEFAULT);
	}
	else
	{
		AllDialogHide();
	}
}

void ToolMenuDialog::AllDialogHide()
{
	// 陥戚杖稽益 穿採 亜軒奄(照左戚惟馬奄)
	for (int i = 0; i < (int)(m_TabInfo.size()); ++i)
	{
		if (nullptr != m_TabInfo[i].pDlg)
		{
			m_TabInfo[i].pDlg->ShowWindow(SW_HIDE);
		}
	}
}


//void ToolMenuDialog::CreateTab(const wchar_t* _Name)
//{
//	// 吐 幻級奄
//	m_Tab.InsertItem(0, _Name);
//	Scene::Scene_Create(_Name);
//
//	DialogInfo NewInfo;
//	NewInfo.Name = _Name;
//
//	m_TabInfo.push_back(NewInfo);
//}


BOOL ToolMenuDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  食奄拭 蓄亜 段奄鉢 拙穣聖 蓄亜杯艦陥.

	m_TabInfo.reserve(3);			// 樟 3鯵 背匝暗艦猿.. 耕軒 拝雁


									// 唖 吐原陥 眼雁 樟聖 竺舛背匝 依績.
									// 眼雁樟戚 蒸嬢亀 鞠惟 拝暗 しさし..
	CreateTab<MapDialog>(L"Map", IDD_MAPDLG);
	CreateTab<AnimationDialog>(L"Animation", IDD_ANIDLG);
	CreateTab<SpriteDialog>(L"Sprite", IDD_SPRITEDLG);

	ChangeTab(0);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 森須: OCX 紗失 凪戚走澗 FALSE研 鋼発背醤 杯艦陥.
}

// 吐 喚袈聖凶 しさし
void ToolMenuDialog::OnTcnSelchangeMenutab(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 食奄拭 珍闘継 硝顕 坦軒奄 坪球研 蓄亜杯艦陥.
	*pResult = 0;

	// 識澱吉惟 蒸生檎 -1聖 軒渡,.. (剰澗 適遣獣 背雁 吐腰硲研 軒渡背捜)
	int SelectIndex = m_Tab.GetCurSel();		

	if (SelectIndex == -1)
	{
		return;
	}

	ChangeTab(SelectIndex);
}
