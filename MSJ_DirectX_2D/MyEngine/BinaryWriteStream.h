#pragma once
#include "FileStream.h"

class BinaryWriteStream : protected FileStream
{
public:
	void Binary_Wirte(void* _Buffer, int _ByteSize);

public:
	template<typename T>
	void Binary_Wirte(T& _Buffer)
	{
		FileStream::File_Write(&_Buffer, sizeof(T));
	}
public:
	BinaryWriteStream(const wchar_t* _Path);
	~BinaryWriteStream();
};

