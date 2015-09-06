#include "ringbuffer.h"

CRingBuffer::CRingBuffer(int nLength)
	: m_pData(nullptr)
	, m_nLength(0)
	, m_nWritePos(0)
	, m_nReadPos(0)
	, m_bAutoIncrease(true)
	, m_nIntervalUsedBytes(0)
	, m_tRecordClock(0)
	, m_nInitLength(nLength)
{
	if (nLength > 0)
	{
		m_pData = new char[nLength];
		m_nLength = nLength;
	}

	m_tRecordClock = std::clock();
}

CRingBuffer::~CRingBuffer()
{
	delete m_pData;
	m_pData = nullptr;
}

bool CRingBuffer::Write(const char *pData, int nLength)
{
	if (m_nWritePos >= m_nReadPos)
	{
		int nFreeLength = m_nLength - m_nWritePos + m_nReadPos;
		if (nLength > nFreeLength)
		{
			// 空闲空间不足
			if (m_bAutoIncrease)
			{
				if (!ReAlloc(m_nLength * 2))
				{
					return false;
				}

				Write(pData, nLength);

				return true;
			}

			return false;
		}

		int nWriteToEnd = m_nLength - m_nWritePos;
		if (nLength > nWriteToEnd)
		{
			memcpy(m_pData + m_nWritePos, pData, nWriteToEnd);
			memcpy(m_pData, pData + nWriteToEnd, nLength - nWriteToEnd);
			m_nWritePos = nLength - nWriteToEnd;
		}
		else
		{
			memcpy(m_pData + m_nWritePos, pData, nLength);
			m_nWritePos += nLength;
		}
	}
	else
	{
		int nFreeLength = m_nReadPos - m_nWritePos;
		if (nLength > nFreeLength)
		{
			// 空闲空间不足
			if (m_bAutoIncrease)
			{
				if (!ReAlloc(m_nLength * 2))
				{
					return false;
				}

				Write(pData, nLength);

				return true;
			}

			return false;
		}

		memcpy(m_pData + m_nWritePos, pData, nLength);
		m_nWritePos += nLength;
	}

	return true;
}

bool CRingBuffer::Read(char *pData, int &nLength)
{
	if (m_nWritePos >= m_nReadPos)
	{
		int nUsedLength = m_nWritePos - m_nReadPos;

		if (nLength >= nUsedLength)
		{
			memcpy(pData, m_pData + m_nReadPos, nUsedLength);
			m_nReadPos += nUsedLength;
			nLength = nUsedLength;
		}
		else
		{
			memcpy(pData, m_pData + m_nReadPos, nLength);
			m_nReadPos += nLength;
			nLength = nLength;
		}
	}
	else
	{
		int nReadToEnd = m_nLength - m_nReadPos;

		if (nLength > nReadToEnd)
		{
			int nUsedLength = m_nLength - m_nReadPos + m_nWritePos;

			if (nLength > nUsedLength)
			{
				// 数据全部读取完
				memcpy(pData, m_pData + m_nReadPos, nReadToEnd);
				memcpy(pData + nReadToEnd, m_pData, nUsedLength - nReadToEnd);
				m_nReadPos = m_nWritePos;
				nLength = nUsedLength;
			}
			else
			{
				// 空间不足，没有读取完
				memcpy(pData, m_pData + m_nReadPos, nReadToEnd);
				memcpy(pData + nReadToEnd, m_pData, nLength - nReadToEnd);
				m_nReadPos = nLength - nReadToEnd;
				nLength = nLength;
			}
		}
		else
		{
			memcpy(pData, m_pData + m_nReadPos, nLength);
			m_nReadPos += nLength;
			nLength = nLength;
		}
	}

	return true;
}

bool CRingBuffer::ReAlloc(int nLength)
{
	char *pNewData = new char[nLength];
	if (nullptr == pNewData)
	{
		return false;
	}

	int nUsedLength = GetUsedLength();

	if (m_nLength <= nUsedLength)
	{
		return false;
	}

	int nReadLength = nLength;
	if (!Read(pNewData, nReadLength))
	{
		return false;
	}

	// 删除已有数据
	delete m_pData;
	m_pData = nullptr;

	// 用新空间替换
	m_pData = pNewData;
	m_nLength = nLength;

	m_nWritePos = nReadLength;
	m_nReadPos = 0;

	return true;
}

int CRingBuffer::GetUsedLength()
{
	if (m_nWritePos >= m_nReadPos)
	{
		int nUsedLength = m_nWritePos - m_nReadPos;
		return nUsedLength;
	}
	else
	{
		int nUsedLength = m_nLength - m_nReadPos + m_nWritePos;
		return nUsedLength;
	}
}

void CRingBuffer::DoAutoRelease()
{
	if (m_nInitLength * 2 > m_nLength)
	{
		// 不需要处理
		return ;
	}

	int nUsedLength = GetUsedLength();

	if (nUsedLength > m_nIntervalUsedBytes)
	{
		m_nIntervalUsedBytes = nUsedLength;
	}
	
	std::clock_t tNow = std::clock();
	double dDuration = (tNow - m_tRecordClock) / CLOCKS_PER_SEC;
	if (dDuration > 3)
	{
		// 超过3秒处理一次释放逻辑
		m_tRecordClock = tNow;

		if (m_nIntervalUsedBytes * 2 < m_nLength)
		{
			ReAlloc(m_nLength / 2);
		}

		m_nIntervalUsedBytes = 0;
	}
}
