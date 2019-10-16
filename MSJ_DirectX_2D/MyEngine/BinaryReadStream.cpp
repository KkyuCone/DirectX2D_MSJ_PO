#include "BinaryReadStream.h"

////////////////////////////////////////////////////

BinaryReadStream::BinaryReadStream(const wchar_t* _Path) : FileStream(_Path, L"rb")
{
}


BinaryReadStream::~BinaryReadStream()
{
}

////////////////////////////////////////////////////

void BinaryReadStream::Binary_Read(void * _Buffer, int _ByteSize, int _ReadByteSize)
{
	FileStream::File_Read(_Buffer, _ByteSize, _ReadByteSize);
}

void BinaryReadStream::Binary_Seek(long _Size, int _Mode)
{
	FileStream::File_Seek(_Size, _Mode);
}