#include "Animation2D.h"
#include "Time.h"



// �̳� Ŭ���� (������ ���)
void Animation2D::AnimationFrame::Init()
{
	// �ʱ�ȭ(����)
	m_CurFrame = m_StartFrame;			// ���� ������
	m_CurTime = m_FrameTime;			// ����� �������� ������
}

unsigned int Animation2D::AnimationFrame::Update()
{
	if (m_CurFrame == m_EndFrame
		&& false == m_Loop)
	{
		// �ִϸ��̼��� �ݺ� ��� X  + �ִϸ��̼� �������κ��̶��
		m_bEnd = true;
		return m_EndFrame;
	}

	m_bEnd = false;
	m_CurTime -= Time::DeltaTime();

	if (0.0f >= m_CurTime)
	{
		// ���(m_FrameTime) 
		// ���� m_CurTime�� ���ְ� �ֱ⋚���� 0.0f���� ������ �ٽ� �������·� ����
		++m_CurFrame;
		m_CurTime = m_FrameTime;
	}

	if (m_CurFrame > m_EndFrame)
	{
		m_bEnd = true;

		if (true == m_Loop)
		{
			m_bEnd = true;
			m_CurFrame = m_StartFrame;
			m_CurTime = m_FrameTime;
		}
	}

	return m_CurFrame;
}


Animation2D::Animation2D()
{
}


Animation2D::~Animation2D()
{
}

//

bool Animation2D::Init()
{
	// �ִϸ��̼� �ʱ�ȭ
	m_SpriteRenderer = GetComponent<SpriteRenderer2D>();

	if (nullptr == m_SpriteRenderer)
	{
		EAssert(true);
		return false;
	}

	return true;
}

void Animation2D::PrevUpdate()
{
	if (nullptr != m_CurAnimation)
	{
		m_SpriteRenderer->Index(m_CurAnimation->Update());
	}

}



My_PTR<Animation2D::AnimationFrame> Animation2D::Animation_Find(const wchar_t* _AnimationName)
{
	return Map_Find_PTR<My_PTR<AnimationFrame>>(m_AnimationMap, _AnimationName);
}

void Animation2D::Animation_Create(const wchar_t* _AnimationName
	, unsigned int _StartFrame, unsigned int _EndFrame
	, float _FrameTime/* = 0.1f*/, bool _bLoop /*= true*/)
{
	if (nullptr != Animation_Find(_AnimationName))
	{
		EAssert(true);
		return;
	}

	My_PTR<AnimationFrame> NewAnimation = new AnimationFrame();
	NewAnimation->SetName(_AnimationName);
	NewAnimation->m_Loop = _bLoop;
	NewAnimation->m_StartFrame = _StartFrame;
	NewAnimation->m_EndFrame = _EndFrame;
	NewAnimation->m_FrameTime = _FrameTime;
	m_AnimationMap.insert(std::unordered_map<std::wstring, My_PTR<AnimationFrame>>::value_type(NewAnimation->GetName(), NewAnimation));
}

void Animation2D::Animation_Create(const wchar_t* _AnimationName, unsigned int _StartFrame, unsigned int _EndFrame, bool _bLoop, float _FrameTime /*= 0.1f*/)
{
	Animation_Create(_AnimationName, _StartFrame, _EndFrame, _FrameTime, _bLoop);
}


void Animation2D::Animation_Change(const wchar_t* _AnimationName, unsigned int _StartFrame /*= 0*/)
{
	m_CurAnimation = Animation_Find(_AnimationName);

	if (0 == m_CurAnimation->m_CurFrame)
	{
		m_CurAnimation->m_CurFrame += m_CurAnimation->m_StartFrame;
	}
}
