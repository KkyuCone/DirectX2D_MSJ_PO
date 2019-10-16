#pragma once
#include <ReferenceBase.h>
#include <Scene.h>


// TabDialog 대화 상자입니다.

class TabDialog : public CDialogEx
{
protected:
	My_PTR<Scene> m_TabScene;

public:
	void SetScene(My_PTR<Scene> _Scene)
	{
		m_TabScene = _Scene;
	}

public:
	TabDialog(UINT nIDTemplate, CWnd *pParent = NULL);
	~TabDialog();
};
