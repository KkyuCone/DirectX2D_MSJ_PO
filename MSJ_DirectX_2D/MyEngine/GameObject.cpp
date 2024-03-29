#include "GameObject.h"
#include "Scene.h"
#include "Component.h"
#include "Transform.h"



GameObject::GameObject() : m_Transform(nullptr), m_Order(0)
{
}


GameObject::~GameObject()
{
}


// 背雁 神崎詮闘税 樟聖 Set背層陥.
// SetScene拭 企背.. -さ- 岨何食辞 戚係惟 幻級醸製
// 訊劃檎 益撹 SetScene幻 拝井酔 鎧襖幻 樟聖 竺舛背爽壱 切縦級廃砺澗 竺舛聖 公背爽奄凶庚戚陥.
// 'さ' 切縦級拭惟亀 背雁 樟戚 杭走 硝形爽奄 是背辞 宜鍵陥~
void GameObject::GameObject_SetScene(Scene* _pScene)
{
	SetScene(_pScene);		// 析舘 樟 隔嬢 爽因~

	// 切縦級拭 企背辞亀 樟聖 竺舛背操醤敗!!
	std::list<My_PTR<GameObject>>::iterator MyChild_StartIter = m_ChildList.begin();
	std::list<My_PTR<GameObject>>::iterator MyChild_EndIter = m_ChildList.end();

	for (; MyChild_StartIter != MyChild_EndIter; ++MyChild_StartIter)
	{
		(*MyChild_StartIter)->GameObject_SetScene(_pScene);
	}
}

//

void GameObject::Death()
{
	// 紫諺
	std::list<My_PTR<Component>>::iterator Component_StartIter = m_ComponentList.begin();
	std::list<My_PTR<Component>>::iterator Component_EndIter = m_ComponentList.end();

	// 背雁 陳匂獲闘級 背薦
	for (; Component_StartIter != Component_EndIter; ++Component_StartIter)
	{
		(*Component_StartIter)->SetGameObject(nullptr);
		(*Component_StartIter)->Death();
	}

	// 切縦級 走酔奄
	std::list<My_PTR<GameObject>>::iterator Child_StartIter = m_ChildList.begin();
	std::list<My_PTR<GameObject>>::iterator Child_EndIter = m_ChildList.end();

	for (; Child_StartIter != Child_EndIter; ++Child_StartIter)
	{
		(*Child_StartIter)->m_pParent = nullptr;			// 切縦税 採乞 走酔奄(蟹研 走酔澗依)
		(*Child_StartIter)->Death();						// 切縦 紫諺 ( 切縦戚 鎧採拭辞 切奄税 切縦, 陳匂獲闘 背薦敗)
	}

	UpdateBase::Death();			// 戚薦 沙昔(惟績神崎詮闘)走酔奄
}

void GameObject::PrevUpdate()
{
	Component_StartIter = m_ComponentList.begin();
	Component_EndIter = m_ComponentList.end();

	for (; Component_StartIter != Component_EndIter; ++Component_StartIter)
	{
		if (true == (*Component_StartIter)->IsUpdate())
		{
			(*Component_StartIter)->PrevUpdate();
		}
	}


	// 切縦 軒什闘 伊紫
	Child_StartIter = m_ChildList.begin();
	Child_EndIter = m_ChildList.end();


	for (; Child_StartIter != Child_EndIter; ++Child_StartIter)
	{
		if (true == (*Child_StartIter)->IsUpdate())
		{
			const wchar_t* GenName = (*Child_StartIter)->GetName();
			// 切縦戚 赤陥檎 穣汽戚闘 叔楳
			(*Child_StartIter)->PrevUpdate();
		}
	}
}

void GameObject::Update()
{
	Component_StartIter = m_ComponentList.begin();
	Component_EndIter = m_ComponentList.end();

	for (; Component_StartIter != Component_EndIter; ++Component_StartIter)
	{
		if (true == (*Component_StartIter)->IsUpdate())
		{
			(*Component_StartIter)->Update();
		}
	}

	// 切縦 軒什闘 伊紫
	Child_StartIter = m_ChildList.begin();
	Child_EndIter = m_ChildList.end();

	for (; Child_StartIter != Child_EndIter; ++Child_StartIter)
	{
		if (true == (*Child_StartIter)->IsUpdate())
		{
			// 切縦戚 赤陥檎 穣汽戚闘 叔楳
			(*Child_StartIter)->Update();
		}
	}
}

void GameObject::LateUpdate()
{
	Component_StartIter = m_ComponentList.begin();
	Component_EndIter = m_ComponentList.end();

	for (; Component_StartIter != Component_EndIter; ++Component_StartIter)
	{
		if (true == (*Component_StartIter)->IsUpdate())
		{
			(*Component_StartIter)->LateUpdate();
		}
	}

	// 切縦 軒什闘 伊紫
	Child_StartIter = m_ChildList.begin();
	Child_EndIter = m_ChildList.end();

	for (; Child_StartIter != Child_EndIter; ++Child_StartIter)
	{
		if (true == (*Child_StartIter)->IsUpdate())
		{
			// 切縦戚 赤陥檎 穣汽戚闘 叔楳
			(*Child_StartIter)->LateUpdate();
		}
	}
}

void GameObject::EndUpdate()
{
	Component_StartIter = m_ComponentList.begin();
	Component_EndIter = m_ComponentList.end();

	for (; Component_StartIter != Component_EndIter; ++Component_StartIter)
	{
		if (true == (*Component_StartIter)->IsUpdate())
		{
			(*Component_StartIter)->EndUpdate();
		}
	}

	// 切縦 軒什闘 伊紫 ( 切縦 穣汽戚闘 宜形爽澗暗績 しさし)
	Child_StartIter = m_ChildList.begin();
	Child_EndIter = m_ChildList.end();

	for (; Child_StartIter != Child_EndIter; ++Child_StartIter)
	{
		if (true == (*Child_StartIter)->IsUpdate())
		{
			// 切縦戚 赤陥檎 穣汽戚闘 叔楳
			(*Child_StartIter)->EndUpdate();
		}
	}
}

void GameObject::FinalUpdate()
{
	Component_StartIter = m_ComponentList.begin();
	Component_EndIter = m_ComponentList.end();

	for (; Component_StartIter != Component_EndIter; ++Component_StartIter)
	{
		if (true == (*Component_StartIter)->IsUpdate())
		{
			(*Component_StartIter)->FinalUpdate();
		}
	}

	// 切縦 軒什闘 伊紫 ( 切縦 穣汽戚闘 宜形爽澗暗績 しさし)
	Child_StartIter = m_ChildList.begin();
	Child_EndIter = m_ChildList.end();

	for (; Child_StartIter != Child_EndIter; ++Child_StartIter)
	{
		if (true == (*Child_StartIter)->IsUpdate())
		{
			// 切縦戚 赤陥檎 穣汽戚闘 叔楳
			(*Child_StartIter)->FinalUpdate();
		}
	}
}

void GameObject::SetParent(My_PTR<GameObject> _ParentObj)
{
	// 鎧亜 採乞稽 背匝 神崎詮闘拭惟 
	// 鎧亜 革 切縦戚鞘!!
	// 馬壱 硝形捜 しさし
	//_ParentObj->m_ChildList.push_back(this);

	// 旭精 採乞研 砧腰 隔嬢爽奄 省奄 是背 達奄獣拙~
	std::set<My_PTR<GameObject>>::iterator Find_Iter = _ParentObj->m_WaitChildGroup.find(this);

	if (Find_Iter != _ParentObj->m_WaitChildGroup.end())
	{
		// 背雁 採乞亜 戚耕 Set鞠嬢赤陥檎 return.
		// or 蒸陥檎..
		return;
	}

	m_pParent = _ParentObj;			// 採乞竺舛

	if (nullptr != GetTransform())
	{
		// 杉球 滴奄 域至
		GetTransform()->CalWScale();
	}

	// 切縦神崎詮闘拭 採乞亜 刊浦走 胡煽 竺舛背層板拭
	// 採乞 神崎詮闘拭辞 切縦 企奄益血拭 切縦戚吃 神崎詮闘研 隔嬢爽壱
	// 切縦 神崎詮闘軒什闘拭 馬蟹梢 切縦級聖 隔嬢層陥.
	_ParentObj->m_WaitChildGroup.insert(this);
}


void GameObject::ChildWaitCheck()
{
	// 森搾切縦級戚 馬蟹亀 蒸陥檎 しさし
	if (true == m_WaitChildGroup.empty())
	{
		return;
	}

	std::set<My_PTR<GameObject>>::iterator Start_Iter = m_WaitChildGroup.begin();
	std::set<My_PTR<GameObject>>::iterator End_Iter = m_WaitChildGroup.end();

	for (; Start_Iter != End_Iter; )
	{
		// 森搾 切縦戚 赤陥檎
		// 益 切縦拭惟 採乞研 蟹稽 走舛
		(*Start_Iter)->m_pParent = this;

		(*Start_Iter)->GetTransform()->m_bWorldUpdate = true;			// 杉球 穣汽戚闘(域至)聖 是背辞 true稽 竺舛(採乞域至亀 杯背辞..)
		(*Start_Iter)->GetTransform()->m_UpdateGeometry = -1;

		m_ChildList.push_back(*Start_Iter);
		Start_Iter = m_WaitChildGroup.erase(Start_Iter);		// 森搾切縦拭惟 採乞研 竺舛背早生糠稽 森搾切縦益血拭辞澗 蟹亜醤敗
	}
}

// 戚 敗呪研 紫遂馬澗 神崎詮闘澗
// 樟聖 角蟹球澗 神崎詮闘稽 竺舛背匝依績.
// OverOn() -> 樟聖 角蟹球澗 神崎詮闘稽 竺舛背爽畏陥澗 敗呪
void GameObject::OverOn()
{
	Scene::OverGameObjectInfo_Create(this);
	m_bOver = true; 
}

void GameObject::PushOverScene(const wchar_t* _SceneName)
{
	Scene::PushOverInfo(this, _SceneName);
}
