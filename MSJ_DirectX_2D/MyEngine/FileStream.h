#pragma once
#include <iostream>

class FileStream
{
public:
	FILE* m_pFile;

public:
	void File_Write(void* _Buffer, int _ByteSize);
	void File_Read(void* _Buffer, int _ByteSize, int _ReadByteSize);
	void File_Seek(long _Size, int _Mode = SEEK_SET);
public:
	FileStream(const wchar_t* _FileName, const wchar_t* _Mode);
	~FileStream();
};

