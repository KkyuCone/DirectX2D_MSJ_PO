#pragma once

// 해당 클래스와 상속이 되어있는지 알려주는 템플릿 클래스
template<typename Parent, typename Child>
class Compile_Dynamic_Cast
{
private:
	static Child* m_CheckChild;

public:
	static const bool IsParent;

private:
	static bool Check(...) {};					// 상속이 안된경우 이 함수로..
	static short Check(Parent* _P) {};			// 상속인 경우 이 함수로..
};

template<typename Parent, typename Child>
Child* Compile_Dynamic_Cast<Parent, Child>::m_CheckChild = nullptr;


// 상속이 되어있는지 체크
template<typename Parent, typename Child>
const bool Compile_Dynamic_Cast<Parent, Child>::IsParent = (bool)(sizeof(Compile_Dynamic_Cast<Parent, Child>::Check(m_CheckChild)) - 1);


class ReferenceBase
{
public:
	template<typename T>
	friend class My_PTR;

	// 디버그 모드
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


	// 타입바꾸기 ( 부모->자식 ) 다운캐스팅 +  업캐스팅도 되게 하기
	template<typename ChangeType>
	operator My_PTR<ChangeType>() const
	{
		if (false == Compile_Dynamic_Cast<T, ChangeType>::IsParent			// 다운캐스팅 여부
			&& false == Compile_Dynamic_Cast<ChangeType, T>::IsParent)		// 업캐스팅 여부
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

	// 바꿀수 있는 타입인지 체크 후 변환..인데 강제형변환 ㄱㄱ
	template<typename ChangeType>
	ChangeType* IsConvert() const
	{
		// 강제 형변환 ㅇㅅㅇ
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


 //스마트 포인터  -> 댕글링 포인터 예방
 //: 포인터처럼 동작하는 클래스 템플릿, 사용이 끝난 후 메모리를 자동으로 해제해줌.

 //스마트 포인터의 종류 ( memory 헤더 파일에 정의되어 있음 )
 //1. unique_ptr ( 하나의 스마트 포인터만 특정 객체를 소유할 수 있음, 객체에 소유권 개념을 도입한 스마트 포인터 )
 //2. shared_ptr ( 하나의 특정 객체를 참조하는 스마트 포인터가 총 몇개인지 참조하는 스마트 포인터, 참조 횟수 ) 
 //3. weak_ptr ( 하나 이상의 shared_ptr 인스턴스가 소유하는 객체에 대한 접근을 제공, 소유자의 수에는 포함 X )

 //레퍼런스 카운팅 ( 참조 횟수 )
 //: 스마트 포인터에서 참조횟수를 센다. (사용하지 않을 경우(지울경우) 차감)
 //  단 순환 참조일 경우 제대로 삭제가 되지 않는다. ( 메모리 해제가 되지 않음 ) 

 //순환 참조
 //: 서로가 상대방을 가리키는 shared_ptr를 가지고 있으면 참조 횟수가 절대로 0이 되지 않기 때문에 메모리가 영원히 해제되지않음.
 //weak_ptr은 이 shared_ptr 인스턴스 사이의 순환참조를 제거하기 위해 사용.