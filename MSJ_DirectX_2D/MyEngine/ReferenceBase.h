#pragma once

// �ش� Ŭ������ ����� �Ǿ��ִ��� �˷��ִ� ���ø� Ŭ����
template<typename Parent, typename Child>
class Compile_Dynamic_Cast
{
private:
	static Child* m_CheckChild;

public:
	static const bool IsParent;

private:
	static bool Check(...) {};					// ����� �ȵȰ�� �� �Լ���..
	static short Check(Parent* _P) {};			// ����� ��� �� �Լ���..
};

template<typename Parent, typename Child>
Child* Compile_Dynamic_Cast<Parent, Child>::m_CheckChild = nullptr;


// ����� �Ǿ��ִ��� üũ
template<typename Parent, typename Child>
const bool Compile_Dynamic_Cast<Parent, Child>::IsParent = (bool)(sizeof(Compile_Dynamic_Cast<Parent, Child>::Check(m_CheckChild)) - 1);


class ReferenceBase
{
public:
	template<typename T>
	friend class My_PTR;

	// ����� ���
private:
	bool m_bDebug;

public:
	bool IsDebugMode()
	{
		return m_bDebug;
	}

	//void CheckDebug()
	//{
	//	if (true == m_bDebug)
	//	{
	//		int a = 0;
	//	}
	//}

	void DebugMode_On()
	{
		m_bDebug = true;
	}

	void DebugMode_Off()
	{
		m_bDebug = false;
	}

private:
	int m_ref;

private:
	void Add();
	void Dec();

public:
	ReferenceBase();
	virtual ~ReferenceBase() = 0;
};


template<typename T>
class My_PTR
{
private:
	T* PTR;

public:
	bool operator!=(const std::nullptr_t _OtherPTR) const { return PTR != _OtherPTR; }
	bool operator!=(const T* const _OtherPTR) const { return PTR != _OtherPTR; }
	bool operator!=(const My_PTR<T>& _OtherPTR) const { return PTR != _OtherPTR.PTR; }

	bool operator==(const std::nullptr_t _OtherPTR) const { return PTR == _OtherPTR; }
	bool operator==(const T* const _OtherPTR) const { return PTR == _OtherPTR; }
	bool operator==(const My_PTR<T>& _OtherPTR) const { return PTR == _OtherPTR.PTR; }


	// Ÿ�Թٲٱ� ( �θ�->�ڽ� ) �ٿ�ĳ���� +  ��ĳ���õ� �ǰ� �ϱ�
	template<typename ChangeType>
	operator My_PTR<ChangeType>() const
	{
		if (false == Compile_Dynamic_Cast<T, ChangeType>::IsParent			// �ٿ�ĳ���� ����
			&& false == Compile_Dynamic_Cast<ChangeType, T>::IsParent)		// ��ĳ���� ����
		{
			return nullptr;
		}

		return (ChangeType*)PTR;

	}

	operator T*() const
	{
		return PTR;
	}

	template<typename ParentType>
	bool IsParent() const
	{
		return Compile_Dynamic_Cast<ParentType, T>::IsParent;
	}

	template<typename ChildType>
	bool IsChile() const
	{
		return Compile_Dynamic_Cast<T, ChildType>::IsParent;
	}

	// �ٲܼ� �ִ� Ÿ������ üũ �� ��ȯ..�ε� ��������ȯ ����
	template<typename ChangeType>
	ChangeType* IsConvert() const
	{
		// ���� ����ȯ ������
		return dynamic_cast<ChangeType*>(PTR);
	}

	//const T* operator->() const
	//{
	//	return PTR;
	//}

	//T* operator->()
	//{
	//	return PTR;
	//}

	T* operator->() const
	{
		return PTR;
	}

	// My_PTR<T> a = new T(...._OtherPTR)
	My_PTR& operator=(T* const _OtherPTR)
	{
		if (_OtherPTR == PTR)
		{
			return *this;
		}

		Check_Dec();

		PTR = _OtherPTR;

		Check_Add();

		return *this;
	}

	My_PTR<T>& operator=(My_PTR<T>& _OtherPTR)
	{
		if (_OtherPTR.PTR == PTR)
		{
			return *this;
		}

		Check_Dec();

		PTR = _OtherPTR.PTR;

		Check_Add();

		return *this;
	}

private:
	void Check_Add()
	{
		if (nullptr == PTR)
		{
			return;
		}

		PTR->Add();
	}

	void Check_Dec()
	{
		if (nullptr == PTR)
		{
			return;
		}

		PTR->Dec();
	}

public:
	My_PTR() : PTR(nullptr)	{}
	My_PTR(std::nullptr_t _PTR) : PTR(nullptr){}
	My_PTR(T* _PTR) : PTR(_PTR)							 { Check_Add(); }
	My_PTR(const My_PTR& _OtherPTR) : PTR(_OtherPTR.PTR) { Check_Add(); }

	~My_PTR() { Check_Dec(); }

};

template<typename T>
bool operator==(std::nullptr_t _OtherNullPTR, const My_PTR<T>& _OtherPTR) { return _OtherPTR.operator==(_OtherNullPTR); }

template<typename T>
bool operator!=(std::nullptr_t _OtherNullPTR, const My_PTR<T>& _OtherPTR) { return _OtherPTR.operator!=(_OtherNullPTR); }


 //����Ʈ ������  -> ��۸� ������ ����
 //: ������ó�� �����ϴ� Ŭ���� ���ø�, ����� ���� �� �޸𸮸� �ڵ����� ��������.

 //����Ʈ �������� ���� ( memory ��� ���Ͽ� ���ǵǾ� ���� )
 //1. unique_ptr ( �ϳ��� ����Ʈ �����͸� Ư�� ��ü�� ������ �� ����, ��ü�� ������ ������ ������ ����Ʈ ������ )
 //2. shared_ptr ( �ϳ��� Ư�� ��ü�� �����ϴ� ����Ʈ �����Ͱ� �� ����� �����ϴ� ����Ʈ ������, ���� Ƚ�� ) 
 //3. weak_ptr ( �ϳ� �̻��� shared_ptr �ν��Ͻ��� �����ϴ� ��ü�� ���� ������ ����, �������� ������ ���� X )

 //���۷��� ī���� ( ���� Ƚ�� )
 //: ����Ʈ �����Ϳ��� ����Ƚ���� ����. (������� ���� ���(������) ����)
 //  �� ��ȯ ������ ��� ����� ������ ���� �ʴ´�. ( �޸� ������ ���� ���� ) 

 //��ȯ ����
 //: ���ΰ� ������ ����Ű�� shared_ptr�� ������ ������ ���� Ƚ���� ����� 0�� ���� �ʱ� ������ �޸𸮰� ������ ������������.
 //weak_ptr�� �� shared_ptr �ν��Ͻ� ������ ��ȯ������ �����ϱ� ���� ���.