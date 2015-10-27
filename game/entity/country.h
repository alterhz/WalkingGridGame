// 国家（部队）

#ifndef _COUNTRY_H_
#define _COUNTRY_H_

#include <map>
#include <vector>
#include <list>

#include "event.h"
#include "msginc.h"
#include "macrosdef.h"
#include "utilityinc.h"

class CClient;
class IBattleGround;

class ICountry
{
public:
	virtual ~ICountry() {}

protected:
	ICountry();

public:
	int GetIndexId() const { return m_nIndexId; }
	CClient * GetClient() const { return m_pClient; }

	void SetBattleGround(IBattleGround *pBattleGround) { m_pBattleGround = pBattleGround; }
	IBattleGround * GetBattleGround() { return m_pBattleGround; }

	void PrepareGround(int nBattleGroundIndexId);
	void ClearPrepareGround() { m_nPrepareBattleGroundIndexId = 0; }

public:
	// 进入场景
	void EnterGround();
	// 离开场景
	void LeaveGround();

public:
	// 添加战斗部队
	bool AddFightGObject(IGObject *pFightGObject);
	bool AddFightGObject(const VtGObject &vtFightGObject);
	// 清空战斗部队
	void ClearFightGObject() { m_vtFightGObject.clear(); }

	// 获取战斗部队
	const VtGObject & GetFightGObject() const { return m_vtFightGObject; }

public:
	void OnConnected(CClient *pClient);
	void OnDisconnect();

public:	// 消息发送
	// 准备场景
	bool SendPrepareGround();
	// 场景信息获取
	bool SendGetGroundInfo(int nWGCount, int nHGCount, const MapGrid &mapGrid, const MapGObject &mapGObject);
	// 进入场景应答
	bool SendEnterGround(bool bOk);
	// 离开场景应答
	bool SendLeaveGround(bool bOk);
	// GObject进入场景
	bool SendGObjectEnterGround(GObjectType gObjectType, int nIndexId, int nSN, int x, int y,
		int nHP, int nMaxHP, int nSP, int nLevel, int nCampId);

private:
	bool SendMessage(unsigned short wProtocolId, google::protobuf::Message *pMessage);

private:
	int m_nIndexId;
	// 通信对象
	CClient *m_pClient;
	// 断开时间
	time_t m_tDisconnect;

	// 部队
	VtGObject m_vtFightGObject;

	// 待进入场景
	int m_nPrepareBattleGroundIndexId;
	// 场景信息
	IBattleGround *m_pBattleGround;
	
};

class CCountry : public ICountry
{
public:
	CCountry();
	~CCountry();
	
public:
	
};

// 国家管理
class CCountryManager : public Singleton<CCountryManager>, public NS_IO::ITimerEvent
{
	typedef std::map<int, ICountry *> MapCountry;
	typedef std::vector<ICountry *> VtCountry;
	typedef std::list<ICountry *> ListCountry;

public:
	CCountryManager();
	~CCountryManager();

public:
	// 创建国家部队
	ICountry * CreateCountry();
	// 准备
	bool PushPrepareList(ICountry *pCountry);


protected:
	virtual bool OnTimerEvent(int nTimerId);
	// 匹配
	void OnMatched(ICountry *pCountryA, ICountry *pCountryB);

private:
	int m_nTimerId;
	MapCountry m_mapCountry;
	ListCountry m_listCountry;
};

#endif