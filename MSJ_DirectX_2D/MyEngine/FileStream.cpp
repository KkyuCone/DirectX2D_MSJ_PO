#include "FileStream.h"


///////////////////////////////////////////////////
FileStream::FileStream(const wchar_t* _FileName, const wchar_t* _Mode) : m_pFile(nullptr)
{
	_wfopen_s(&m_pFile, _FileName, _Mode);
}


FileStream::~FileStream()
{
	fclose(m_pFile);
}

///////////////////////////////////////////////////

void FileStream::File_Write(void * _Buffer, int _ByteSize)
{
	if (nullptr != m_pFile)
	{
		fwrite(_Buffer, _ByteSize, 1, m_pFile);
	}
}

void FileStream::File_Read(void * _Buffer, int _ByteSize, int _ReadByteSize)
{
	if (nullptr != m_pFile)
	{
		if (_ByteSize <= _ReadByteSize)
		{
			_ReadByteSize = _ByteSize;
		}

		fread_s(_Buffer, _ByteSize, _ReadByteSize, 1, m_pFile);
	}
}


void FileStream::File_Seek(long _Size, int _Mode)
{
	if (nullptr != m_pFile)
	{
		fseek(m_pFile, _Size, _Mode);
	}
}
