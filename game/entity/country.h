// 国家（部队）

#ifndef _COUNTRY_H_
#define _COUNTRY_H_

class ICountry
{
public:
	ICountry() {}
	virtual ~ICountry() {}

public:

private:

};

class CClient;

class CCountry : public ICountry
{
public:
	CCountry();
	~CCountry();

	int GetIndexId() const { return m_nIndexId; }

	CClient * GetClient() const { return m_pClient; }
	
public:

public:
	int m_nIndexId;
	// 通信对象
	CClient *m_pClient;
};

#endif