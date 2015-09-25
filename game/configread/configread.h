//-------------------------------------------
// author : alterhz
// date : 8/19/2015
// description : 

#ifndef _CONFIGREAD_H_
#define _CONFIGREAD_H_

#include <string>
#include <map>
#include "xmldata.h"
#include "utilityinc.h"

namespace tinyxml2
{
	class XMLElement;
}

template<typename T>
class CConfigRead
{
	typedef std::map<int, T *> MapT;

public:
	CConfigRead();
	~CConfigRead();

public:
	bool LoadConfigData(std::string strFileName);
	// 查找数据
	const T * GetRecord(int nId) const;

protected:
	// 单条记录载入
	void OnRead(const tinyxml2::XMLElement *pEleRecord);

private:
	// xml记录节点名称
	std::string m_strRecordNodeName;
	// 主键名称
	std::string m_strKeyName;
	// 数据集
	MapT m_mapRecord;
};

template<typename T>
const T * CConfigRead<T>::GetRecord(int nId) const
{
	auto it = m_mapRecord.find(nId);
	if (it != m_mapRecord.end())
	{
		return it->second;
	}

	return nullptr;
}

class CConfigReadManager : public Singleton<CConfigReadManager>
{
public:
	CConfigReadManager();
	~CConfigReadManager();

	// 载入数据
	bool LoadConfigData();

	// 全局配置ID
	int GetGlobalConfigId() const { return m_nGlobalConfigId; }

public:
	int m_nGlobalConfigId;
	// 全局配置
	CConfigRead<CXmlData_Config> xdConfig;
};


#endif