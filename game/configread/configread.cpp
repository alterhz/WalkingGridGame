#include "tinyxml2.h"

#include "configread.h"
#include "debug.h"
#include "tool.h"
#include "file.h"




template<typename T>
CConfigRead<T>::CConfigRead()
	: m_strRecordNodeName("RECORD")
	, m_strKeyName("SN")
{
}

template<typename T>
CConfigRead<T>::~CConfigRead()
{
	for (auto it = m_mapRecord.begin(); it != m_mapRecord.end(); ++it)
	{
		IXmlData *pDelXmlData = it->second;
		delete pDelXmlData;
	}

	m_mapRecord.clear();
}

template<typename T>
bool CConfigRead<T>::LoadConfigData(std::string strFileName)
{
	tinyxml2::XMLDocument doc;
	if (tinyxml2::XML_SUCCESS != doc.LoadFile(strFileName.c_str()) )
	{
		LOGError("载入xml[" + strFileName + "]配置文件失败。");
		return false;
	}

	const tinyxml2::XMLElement *pRoot = doc.RootElement();
	if (nullptr == pRoot)
	{
		LOGError("nullptr == pRoot");
		return false;
	}

	const tinyxml2::XMLElement *pEleRecord = pRoot->FirstChildElement(m_strRecordNodeName.c_str());
	for (; nullptr != pEleRecord; pEleRecord = pEleRecord->NextSiblingElement(m_strRecordNodeName.c_str()))
	{
		const tinyxml2::XMLElement *pEleId = pEleRecord->FirstChildElement(m_strKeyName.c_str());
		if (nullptr == pEleId)
		{
			LOGError("配置文件[" + strFileName + "]没有主键:" + m_strKeyName);
			continue;
		}

		int nSN = NS_IO::String2Int(pEleId->GetText());

		T *pXmlData = new T();
		if (nullptr == pXmlData)
		{
			LOGError("new T() == nullptr.");
			continue;
		}

		pXmlData->SetSN(nSN);
		pXmlData->OnRead(nSN, pEleRecord);

		m_mapRecord.insert(std::make_pair(pXmlData->GetSN(), pXmlData));
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////
CConfigReadManager::CConfigReadManager()
	: m_nGlobalConfigId(1)
{
}

CConfigReadManager::~CConfigReadManager()
{
}

bool CConfigReadManager::LoadConfigData()
{
	std::string strRootPath = NS_IO::GetExeRootPath();

	if (!xdConfig.LoadConfigData(strRootPath + "//..//config//config.xml"))
	{
		return false;
	}

	if (!xdGround.LoadConfigData(strRootPath + "//..//config//ground.xml"))
	{
		return false;
	}

	if (!xdStill.LoadConfigData(strRootPath + "//..//config//still.xml"))
	{
		return false;
	}

	if (!xdWalkable.LoadConfigData(strRootPath + "//..//config//walkable.xml"))
	{
		return false;
	}

	if (!xdSkill.LoadConfigData(strRootPath + "//..//config//skill.xml"))
	{
		return false;
	}

	return true;
}