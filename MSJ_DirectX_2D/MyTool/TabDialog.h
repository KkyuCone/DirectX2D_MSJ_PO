#pragma once
#include <ReferenceBase.h>
#include <Scene.h>


// TabDialog ��ȭ �����Դϴ�.

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
