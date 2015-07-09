//----------------------------------------
// 创建时间 : 2014-7-3
// 作者 : hanzhe
// 说明 : 实用工具集

#ifndef _UTILITYINC_
#define _UTILITYINC_

#define MAX_SENDBUFFER_LENGTH		(0x2000)

template <typename T>
class Singleton
{
public:
	Singleton() {}
	virtual ~Singleton() {}
	static void delMe(void)
	{//可以在子类的destoryMe中被调用
		if (m_pSingleTon)
		{
			delete m_pSingleTon;
			m_pSingleTon = 0;
		}
	}

	static T* instance( void )
	{
		if (!m_pSingleTon)
		{
			m_pSingleTon = new T;
		}

		return m_pSingleTon;
	}

	static T& getMe(void)
	{
		return *instance();
	}

protected:
	static T* m_pSingleTon;

private:
	Singleton(const Singleton&);
	Singleton & operator= (const Singleton &);
};
template <typename T> T* Singleton<T>::m_pSingleTon = nullptr;

#endif