//-------------------------------------------
// author : alterhz
// date : 6/18/2015
// description : 

#ifndef _RINGBUFFER_H_
#define _RINGBUFFER_H_

#include <memory.h>
//#include <time.h>
#include <ctime>

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
	// 自动释放
	void DoAutoRelease();

private:
	// 扩容
	bool ReAlloc(int nLength);
	// 获取使用数据长度
	int GetUsedLength();

private:
	char *m_pData;
	int m_nLength;

	int m_nWritePos;
	int m_nReadPos;

	// 自动扩容
	bool m_bAutoIncrease;

	// 初始的大小
	int m_nInitLength;
	// 当前时间间隔内占用最大字节量
	int m_nIntervalUsedBytes;
	// 上次开始记录的时间
	std::clock_t m_tRecordClock;
};

#endif