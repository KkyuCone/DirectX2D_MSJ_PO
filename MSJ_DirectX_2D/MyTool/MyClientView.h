#pragma once


class MyClientView : public CView
{
	// DECLARE_DYNCREATE(Ŭ�����̸�) : �������� ����°��� �Ҽ��ְ� ����. �������� ����� ������ ����������Ѵ�.
	DECLARE_DYNCREATE(MyClientView);

protected:
	MyClientView();				// �������� �� ������� ���������� ����ϰԵ�
	virtual ~MyClientView();


public:
	virtual void OnDraw(CDC* _pDC);				// �� �並 �׸��� ���� ������ �ȴ�.

#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef  _WIN32_WCE
	virtual void Dump(CDumpContext& _DC) const;
#endif // ! _WIN32_WCE

#endif // _DEBUG

	
	// ������ �޽��� �� �Լ�
protected:
	DECLARE_MESSAGE_MAP()
};

