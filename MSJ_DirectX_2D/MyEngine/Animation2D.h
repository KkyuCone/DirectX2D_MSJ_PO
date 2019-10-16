#pragma once
#include "Component.h"
#include "SpriteRenderer2D.h"
#include "STLHelper.h"
#include <unordered_map>

class SpriteRenderer2D;
class Animation2D : public Component
{
private:

	class AnimationFrame : public ReferenceBase, public NameBase
	{
	public:
		bool			m_Loop;			// �ִϸ��̼� �ݺ���� ����
		unsigned int	m_StartFrame;	// �ִϸ��̼� ��������
		unsigned int	m_EndFrame;		// �ִϸ��̼� ������
		float			m_FrameTime;	// Ÿ��
		unsigned int	m_CurFrame;		// ���� �ִϸ��̼� ������
		float			m_CurTime;	
		
		bool			m_bEnd;			// �ִϸ��̼��� ������ (��������..) ����

	public:
		void Init();
		unsigned int Update();
	};


private:
	My_PTR<SpriteRenderer2D> m_SpriteRenderer;
	std::unordered_map<std::wstring, My_PTR<AnimationFrame>> m_AnimationMap;
	My_PTR<AnimationFrame> m_CurAnimation;

public:
	My_PTR<AnimationFrame> Animation_Find(const wchar_t* _AnimationName);
	void Animation_Create(const wchar_t* _AnimationName, unsigned int _StartFrame, unsigned int _EndFrame, float _FrameTime = 0.2f, bool _bLoop = true);
	void Animation_Create(const wchar_t* _AnimationName, unsigned int _StartFrame, unsigned int _EndFrame, bool _bLoop, float _FrameTime = 0.2f);

public:
	void Animation_Change(const wchar_t* _AnimationName, unsigned int _StartFrame = 0);		// �ִϸ��̼� ������������ �ִϸ��̼� �ٲ���..

public:
	bool Init();
	void PrevUpdate() override;

	bool IsEndAnimation()
	{
		return m_CurAnimation->m_bEnd;
	}

	bool IsCurAnimation(std::wstring _AnimationName)
	{
		return (_AnimationName == m_CurAnimation->GetName());
	}

	std::wstring CurAnimationName()
	{
		return m_CurAnimation->GetName();
	}

	void SetFrame(int _Frame) 
	{
		m_CurAnimation->m_CurFrame = _Frame;
	}

	void SetCurTime(float _Time)
	{
		m_CurAnimation->m_CurTime = _Time;
	}

	int CurAnimationFram()
	{
		return (int)(m_CurAnimation->m_CurFrame);
	}

public:
	Animation2D();
	~Animation2D();
};

