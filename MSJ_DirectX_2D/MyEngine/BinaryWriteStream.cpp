#include "BinaryWriteStream.h"


/////////////////////////////////////////
BinaryWriteStream::BinaryWriteStream(const wchar_t* _Path) : FileStream(_Path, L"wb")
{
}


BinaryWriteStream::~BinaryWriteStream()
{
}

/////////////////////////////////////////

void BinaryWriteStream::Binary_Wirte(void * _Buffer, int _ByteSize)
{
	FileStream::File_Write(_Buffer, _ByteSize);
}