// 基础战场

#ifndef _BATTLEGROUND_H_
#define _BATTLEGROUND_H_

#include <map>

#include "event.h"
#include "utilityinc.h"

class ICountry;

class IBattleGround
{
	enum EStatus
	{
		EStatus_None = 0,
		EStatus_Waiting,
		EStatus_Run,
		EStatus_Pause,
		EStatus_Finish,
	};

public:
	IBattleGround();
	virtual ~IBattleGround();

	int GetIndexId() const { return m_nIndexId; }

	EStatus GetCurrentStatus() const { return m_eStatus; }
	bool ChangeStatus(EStatus eStatus);

public:
	bool Init();
	bool DoTick();

public:
	// 获取场景数据
	virtual void GetGroundInfo(ICountry *pCountry);


protected:
	virtual bool OnInit() { return true; }
	virtual bool OnTick() { return true; }
	virtual bool OnGoRun() { return true; }
	virtual bool OnGoFinish() { return true; }

protected:
	EStatus m_eStatus;

private:
	int m_nIndexId;
};

class ICountry;
class IGround;

// 战场(阵地战)
class CFrontBattleGround : public IBattleGround
{
	typedef std::map<int, ICountry *> MapCountry;

public:
	CFrontBattleGround();
	~CFrontBattleGround();

public:
	// 添加对战双方
	bool InitTwoCountry(ICountry *pCountryA, ICountry *pCountryB);

public:
	// 获取场景数据
	virtual void GetGroundInfo(ICountry *pCountry);

protected:
	// 初始化
	virtual bool OnInit();
	virtual bool OnTick();

private:
	// 地形
	IGround * m_pGround;
	// 国家（部队）列表
	MapCountry m_mapCountry;
};

class CBattleGroundManager : public Singleton<CBattleGroundManager>, public NS_IO::ITimerEvent
{
	typedef std::map<int, IBattleGround *> MapBattleGround;
public:
	CBattleGroundManager();
	~CBattleGroundManager();

public:
	bool Init();

	// 分配一个阵地战场景
	CFrontBattleGround* CreateFrontBattleGround();

protected:
	// 返回true:继续；返回false:终止
	virtual bool OnTimerEvent(int nTimerId);


private:
	int m_nDoTickTimerId;
	MapBattleGround m_mapBattleGround;
};

#endif