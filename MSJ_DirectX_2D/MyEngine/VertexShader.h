#pragma once
#include "Shader.h"




class VertexShader : public Shader
{
private:
	ID3D11VertexShader* m_pVertexShader;

	DEFINITION_LOADER(VertexShader, std::wstring)
		DEFINITION_KEY_LOAD_FUNCTION(UINT _VersionFirst, UINT _VersionSecond, const char* _FunctionName)
		DEFINITION_LOADER_FUNTION_PARAMETER(_VersionFirst, _VersionSecond, _FunctionName)
		DEFINITION_LOADER_END


	// 위에있는 매크로들..
//	#define DEFINITION_LOADER(RESOURCETYPE, KEYTYPE) 
//  RESOURCETYPE -> VertexShader, KEYTYPE -> std::wstring
//
//	public: 
//	class Loader 
//	{ 
//	public: 
//		typedef VertexShader ResourceType; 
//		typedef std::wstring KeyType; 
//	
//	My_PTR<ResourceType> operator() (const KeyType& _Key, const std::wstring& _Path, __VA_ARGS__) 
//	{ 
//		My_PTR<ResourceType> Find_PTR = ResourceManager<ResourceType, KeyType>::Find(_Key); 
//		if (nullptr != Find_PTR) 
//		{ 
//			return Find_PTR; 
//		} 
//		ResourceType* NewResource = new ResourceType(); 
//		NewResource->SetPath(_Path);
//	
//		if (false == NewResource->Load( ## __VA_ARGS__ ## )) 
//		{ 
//			EAssert(true);	
//			delete NewResource;	
//			return nullptr;	
//		} 
//		ResourceManager< ResourceType, KeyType >::m_ResourceMap.insert(std::unordered_map<KeyType, My_PTR<ResourceType>>::value_type(_Key, NewResource)); 
//		return NewResource; 
//	}
//	
//	friend Loader;

public:
	bool Load(UINT _VersionFirst, UINT _VersionSecond, const char* _FunctionName);
	void Update() override;

	template<typename T>
	void UpdateConstBuffer(const wchar_t* _Name, const T& _Value)
	{
		My_PTR<ConstBuffer> Find_CB = Find_ConstBuffer(_Name);

		if (nullptr == Find_CB)
		{
			EAssert(true);
			return;
		}

		if (false == Find_CB->ClassType_Check<T>())
		{
			EAssert(true);
			return;
		}

		Find_CB->Update<T>(_Value);
		Find_CB->Vertex_Set();
		return;
	}


public:
	VertexShader();
	~VertexShader();
};

