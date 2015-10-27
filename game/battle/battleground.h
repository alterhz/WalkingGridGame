// 基础战场

#ifndef _BATTLEGROUND_H_
#define _BATTLEGROUND_H_

#include <map>

#include "event.h"
#include "ground.h"
#include "utilityinc.h"

class ICountry;

class IBattleGround : public IGround
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
	// 进入战场
	virtual bool Enter(ICountry *pCountry);
	// 离开战场
	virtual bool Leave(ICountry *pCountry);
	// 进入
	virtual bool GObjectEnter(IGObject *pGObject);
	virtual bool GObjectLeave(IGObject *pGObject);


protected:
	virtual bool OnInit() { return true; }
	virtual bool OnTick() { return true; }
	virtual bool OnGoRun() { return true; }
	virtual bool OnGoFinish() { return true; }

	virtual bool OnEnter(ICountry *pCountry) { return true; }
	virtual bool OnLeave(ICountry *pCountry) { return true; }
	virtual bool OnGObjectEnter(IGObject *pGObject);
	virtual bool OnGObjectLeave(IGObject *pGObject) { return true; }

private:
	ICountry * FindCountry(int nIndexId);
	ICountry * FindCountry(int nIndexId) const;
	IGObject * FindGObject(int nIndexId);
	IGObject * FindGObject(int nIndexId) const;

protected:
	EStatus m_eStatus;
	// 国家（部队）列表
	MapCountry m_mapCountry;
	// 场景单位（包括场景物体和角色）
	MapGObject m_mapGObject;

private:
	int m_nIndexId;
};

// 战场(阵地战)
class CFrontBattleGround : public IBattleGround
{
public:
	CFrontBattleGround();
	~CFrontBattleGround();

protected:
	// 初始化
	virtual bool OnInit();
	virtual bool OnTick();

private:

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

	IBattleGround * FindBattleGround(int nBattleGroundIndexId);
	IBattleGround * FindBattleGround(int nBattleGroundIndexId) const;


protected:
	// 返回true:继续；返回false:终止
	virtual bool OnTimerEvent(int nTimerId);


private:
	int m_nDoTickTimerId;
	MapBattleGround m_mapBattleGround;
};

#endif