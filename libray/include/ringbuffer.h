//-------------------------------------------
// author : alterhz
// date : 6/18/2015
// description : 

#ifndef _RINGBUFFER_H_
#define _RINGBUFFER_H_

#include <memory.h>

class CRingBuffer
{
public:
	CRingBuffer(int nLength = 0x8000);
	~CRingBuffer();

public:
	// 写入
	bool Write(const char *pData, int nLength);
	// 读取
	bool Read(char *pData, int &nLength);
	// 设置自动增长
	void SetAutoIncrease() { m_bAutoIncrease = true; }

private:
	// 扩容
	bool ReAlloc(int nLength);

private:
	char *m_pData;
	int m_nLength;

	int m_nWritePos;
	int m_nReadPos;

	// 自动扩容
	bool m_bAutoIncrease;
};

#endif