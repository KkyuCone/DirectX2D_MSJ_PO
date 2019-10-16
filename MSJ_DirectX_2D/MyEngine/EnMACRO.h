#pragma once
#include <assert.h>

#define EAssert(_Value) assert(!(_Value))

#define SAFE_RELEASE(P) if(nullptr != P) { P->Release(); P = nullptr; }


#define DEFINITION_STATIC_CONSTRUCT(CLASSTYPE, FUNCNAME) \
class StaticConstruct \
{ \
public: \
	StaticConstruct() \
	{ \
		CLASSTYPE##::##FUNCNAME(); \
	} \
}; \
static StaticConstruct StaticConstructor;


#define STATIC_CONSTRUCT(CLASSTYPE) CLASSTYPE##::StaticConstruct CLASSTYPE##::StaticConstructor;


#define DEFINITION_STATIC_DESTRUCTOR(CLASSTYPE, FUNCNAME) \
class StaticDestruct \
{ \
public: \
	~StaticDestruct() \
	{ \
		CLASSTYPE##::##FUNCNAME##(); \
	} \
}; \
static StaticDestruct StaticDestructor;


#define STATIC_DESTRUCT(CLASSTYPE) CLASSTYPE##::StaticDestruct CLASSTYPE##::StaticDestructor;

#define FORSTLLIST(MAP, FUNC)

#define DEBUGBREAK if (true == IsDebugMode()) \
	{	\
		int a = 0; \
	}