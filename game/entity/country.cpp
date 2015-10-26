#include "country.h"

#include "debug.h"

#include "app.h"
#include "battleground.h"
#include "client.h"


ICountry::ICountry()
	: m_nIndexId(0)
	, m_tDisconnect(0)
{
	static int g_nIndexId = 0;
	m_nIndexId = (++g_nIndexId);
}

bool ICountry::SendMessage(unsigned short wProtocolId, google::protobuf::Message *pMessage)
{
	if (nullptr == m_pClient)
	{
		LOGError("nullptr == m_pClient");
		return false;
	}

	return m_pClient->SendMessage(wProtocolId, pMessage);
}

//bool ICountry::SendPrepare(bool bOk)
//{
//	gproto::MSG_G2C_Prepare msg;
//	if (bOk)
//	{
//		msg.set_ret(gproto::MSG_G2C_Prepare_EResult_OK);
//	}
//	else
//	{
//		msg.set_ret(gproto::MSG_G2C_Prepare_EResult_ERR);
//	}
//
//	return SendMessage(gproto::CSID_G2C_Prepare, &msg);
//}

void ICountry::OnConnected(CClient *pClient)
{
	m_pClient = pClient;
}

void ICountry::OnDisconnect()
{
	m_pClient = nullptr;

}

bool ICountry::AddFightGObject(IFightGObject *pFightGObject)
{
	if (nullptr == pFightGObject)
	{
		LOGError("nullptr == pFightGObject");
		return false;
	}

	m_vtFightGObject.push_back(pFightGObject);

	return true;
}

bool ICountry::AddFightGObject(const VtFightGObject &vtFightGObject)
{
	for (IFightGObject *pFightGObject : vtFightGObject)
	{
		m_vtFightGObject.push_back(pFightGObject);
	}

	return true;
}



// CCountry
CCountry::CCountry()
{
}

CCountry::~CCountry()
{

}

// CCountryMananger
CCountryManager::CCountryManager()
	: m_nTimerId(0)
{
	m_nTimerId = CApp::getMe().SetTimer(this, 1000);
}

CCountryManager::~CCountryManager()
{
	CApp::getMe().KillTimer(m_nTimerId);
	m_nTimerId = 0;
}

ICountry * CCountryManager::CreateCountry()
{
	ICountry *pNetCountry = new CCountry();
	if (nullptr == pNetCountry)
	{
		LOGError("nullptr == pNetCountry");
		return false;
	}

	m_mapCountry.insert(std::make_pair(pNetCountry->GetIndexId(), pNetCountry));

	return pNetCountry;
}

bool CCountryManager::PushPrepareList(ICountry *pCountry)
{
	if (nullptr == pCountry)
	{
		LOGError("nullptr == pCountry");
		return false;
	}

	for (ICountry *pTmpCountry : m_listCountry)
	{
		if (pTmpCountry == pCountry)
		{
			// 已经存在
			return false;
		}
	}

	m_listCountry.push_back(pCountry);

	return true;
}

bool CCountryManager::OnTimerEvent(int nTimerId)
{
	while (m_listCountry.size() >= 2)
	{
		ICountry *pCountryA = m_listCountry.front();
		m_listCountry.pop_front();
		ICountry *pCountryB = m_listCountry.front();
		m_listCountry.pop_front();

		OnMatched(pCountryA, pCountryB);
	}

	return true;
}

void CCountryManager::OnMatched(ICountry *pCountryA, ICountry *pCountryB)
{
	LOGDebug("两个国家(部队)匹配成功。");

	if (nullptr == pCountryA || nullptr == pCountryB)
	{
		LOGError("nullptr == pCountryA || nullptr == pCountryB");
		return ;
	}

	// 创建场景
	CFrontBattleGround *pNewFrontBattleGround = CBattleGroundManager::getMe().CreateFrontBattleGround();
	if (nullptr == pNewFrontBattleGround)
	{
		LOGError("nullptr == pNewBattleGround");
		return ;
	}

	// 添加两个国家到场景中
	pNewFrontBattleGround->InitTwoCountry(pCountryA, pCountryB);
}


