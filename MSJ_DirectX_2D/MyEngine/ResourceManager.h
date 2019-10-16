#pragma once
#include "ReferenceBase.h"
#include "STLHelper.h"
#include "Path.h"

#include <string>
#include <unordered_map>

//#define DEFCreater friend ResourceManager<>
//#define DEFLoader

//definition

#pragma region Create_Definition
#define DEFINITION_CREATOR(RESOURCETYPE, KEYTYPE) \
public: \
	class Creator \
	{ \
	public: \
		typedef RESOURCETYPE ResourceType; \
		typedef KEYTYPE KeyType; \

#define DEFINITION_CREATOR_FUCTION(...) \
My_PTR<ResourceType> operator() (const KeyType& _Key, __VA_ARGS__) \
{ \
	 My_PTR<ResourceType> Find_PTR = ResourceManager<ResourceType, KeyType>::Find(_Key); \
	if (nullptr != Find_PTR) \
	{ \
		return Find_PTR; \
	} \
	ResourceType* NewResource = new ResourceType(); \
	NewResource->SetPath(_Key);


#define DEFINITION_CREATOR_FUNCTION_PARAMETER(...) \
	if (false == NewResource->Create( ## __VA_ARGS__ ## )) \
	{ \
		EAssert(true);	\
		delete NewResource;	\
		return nullptr;	\
	} \
	ResourceManager< ResourceType, KeyType >::m_ResourceMap.insert(std::unordered_map<KeyType, My_PTR<ResourceType>>::value_type(_Key, NewResource)); \
	return NewResource; \
	}

#define DEFINITION_CREATOR_END }; \
friend Creator;
#pragma endregion


#pragma region Loder_Definition
#define DEFINITION_LOADER(RESOURCETYPE, KEYTYPE) \
public: \
class Loader \
{ \
public: \
	typedef RESOURCETYPE ResourceType; \
	typedef KEYTYPE KeyType; 

#define DEFINITION_KEY_LOAD_FUNCTION(...) \
My_PTR<ResourceType> operator() (const KeyType& _Key, const std::wstring& _Path, __VA_ARGS__) \
{ \
	My_PTR<ResourceType> Find_PTR = ResourceManager<ResourceType, KeyType>::Find(_Key); \
	if (nullptr != Find_PTR) \
	{ \
		return Find_PTR; \
	} \
	ResourceType* NewResource = new ResourceType(); \
	NewResource->SetPath(_Path);

#define DEFINITION_LOADER_FUNTION_PARAMETER(...) \
	if (false == NewResource->Load( ## __VA_ARGS__ ## )) \
	{ \
		EAssert(true);	\
		delete NewResource;	\
		return nullptr;	\
	} \
	ResourceManager< ResourceType, KeyType >::m_ResourceMap.insert(std::unordered_map<KeyType, My_PTR<ResourceType>>::value_type(_Key, NewResource)); \
	return NewResource; \
}

#define DEFINITION_PATH_LOADER_FUNCTION(...) \
My_PTR<ResourceType> operator() (const std::wstring& _Path, __VA_ARGS__) \
{ \
	My_PTR<ResourceType> Find_PTR = ResourceManager<ResourceType, KeyType>::Find(Path::PathToFullFileName(_Path)); \
	if (nullptr != Find_PTR) \
	{ \
		return Find_PTR; \
	} \
	ResourceType* NewResource = new ResourceType(); \
	NewResource->SetPath(_Path); 

#define DEFINITION_PATH_LOADER_FUNCTION_PARAMETER(...) \
	if (false == NewResource->Load(## __VA_ARGS__ ## )) \
	{ \
		EAssert(true); \
		delete NewResource; \
		return nullptr; \
	} \
	ResourceManager<ResourceType, KeyType>::m_ResourceMap.insert(std::unordered_map<KeyType, My_PTR<ResourceType>>::value_type(NewResource->Full_FileName(), NewResource)); \
	return NewResource; \
	}


#define DEFINITION_LOADER_END }; \
friend Loader;
#pragma endregion



template<typename RESOURCE, typename KEY = std::wstring>
class ResourceManager
{
private:
	static std::unordered_map<KEY, My_PTR<RESOURCE>> m_ResourceMap;


public:
	static My_PTR<RESOURCE> Find(const KEY& _Key)
	{
		My_PTR<RESOURCE> aaa = Map_Find_PTR<My_PTR<RESOURCE>>(m_ResourceMap, _Key);
		return Map_Find_PTR<My_PTR<RESOURCE>>(m_ResourceMap, _Key);
	}

public:
	friend class RESOURCE::Loader;
	friend class RESOURCE::Creator;
	static typename RESOURCE::Loader Load;
	static typename RESOURCE::Creator Create;

public:
	ResourceManager() {}
	~ResourceManager() {}
};

template<typename RESOURCE, typename KEY = std::wstring>
std::unordered_map<KEY, My_PTR<RESOURCE>> ResourceManager<RESOURCE, KEY>::m_ResourceMap;

template<typename RESOURCE, typename KEY = std::wstring>
typename RESOURCE::Loader ResourceManager<RESOURCE, KEY>::Load;

template<typename RESOURCE, typename KEY = std::wstring>
typename RESOURCE::Creator ResourceManager<RESOURCE, KEY>::Create;


template<typename RESOURCE, typename KEY = std::wstring, typename MULTIKEY = std::wstring>
class MULTIResourceManager
{
	static std::unordered_map<MULTIKEY, std::unordered_map<KEY, My_PTR<RESOURCE>>> m_ResourceMultiMap;
};