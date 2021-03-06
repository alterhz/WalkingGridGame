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
	virtual bool GObjectEnter(IGObject *pGObject, int x, int y);
	virtual bool GObjectLeave(IGObject *pGObject);
	// 准备完毕
	virtual void PrepareFinish(ICountry *pCountry);
	// 当前回合战斗结束
	virtual void BattleBoutFinish(ICountry *pCountry);
	// GObject移动
	virtual bool GObjectMove(IGObject *pGObject, const VtCoor2 &vtCoor2);
	// GObject释放技能
	virtual bool GObjectUseSkill(IGObject *pGObject, int nSkillSN, int nTargetGObjectIndexId);
	// 战场结束(将领死亡)参数是失败的阵营ID
	virtual void BattleFinish(int nCampId);

protected:
	virtual bool OnInit() { return true; }
	virtual bool OnTick() { return true; }
	virtual bool OnGoRun() { return true; }
	virtual bool OnGoFinish() { return true; }

	virtual bool OnEnter(ICountry *pCountry);
	virtual bool OnLeave(ICountry *pCountry) { return true; }
	virtual bool OnGObjectEnter(IGObject *pGObject);
	virtual bool OnGObjectLeave(IGObject *pGObject) { return true; }

public:
	ICountry * FindCountry(int nIndexId);
	ICountry * FindCountry(int nIndexId) const;
	IGObject * FindGObject(int nIndexId);
	IGObject * FindGObject(int nIndexId) const;

protected:
	// 路径是否畅通
	bool PathIsOK(const VtCoor2 &vtCoor2);

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
	enum EBattleStatus
	{
		EBattleStatus_Waiting = 1,	//等待角色进入
		EBattleStatus_Prepare,	//准备
		EBattleStatus_Battle,	//战斗
		EBattleStatus_Reward,	//结束
	};

public:
	CFrontBattleGround();
	~CFrontBattleGround();

	void ChangeBattleStatus(EBattleStatus eBattleStatus);
	EBattleStatus GetCurrentBattleStatus() const { return m_eBattleStatus; }

protected:
	// 初始化
	virtual bool OnInit();
	virtual bool OnTick();
	// 角色进入场景事件
	virtual bool OnEnter(ICountry *pCountry);
	virtual bool OnLeave(ICountry *pCountry);
	// 准备完毕
	virtual void PrepareFinish(ICountry *pCountry);
	// 当前回合战斗结束
	virtual void BattleBoutFinish(ICountry *pCountry);
	// GObject移动
	virtual bool GObjectMove(IGObject *pGObject, const VtCoor2 &vtCoor2);
	// GObject释放技能
	virtual bool GObjectUseSkill(IGObject *pGObject, int nSkillSN, int nTargetGObjectIndexId);
	// 战场结束(将领死亡)参数是失败的阵营ID
	virtual void BattleFinish(int nCampId);

	void OnGoPrepare();
	void OnGoBattle();
	void OnGoReward();

private:
	bool HaveMoveOrFight(int nGObjectIndexId) const;
	bool HaveFight(int nGObjectIndexId) const;

private:
	EBattleStatus m_eBattleStatus;
	VtInt m_vtPrepareFinishCountryIndexId;
	VtInt m_vtSurviveCampId;
	int m_nBoutIndex;	//回合轮流顺序
	int m_nBoutCountryIndexId;	//当前回合的CountryIndexId
	VtInt m_vtBoutMoveGObjectIndexId;	//当前回合移动过的GObjectIndexId[可能存在重复]
	VtInt m_vtBoutFightGObjectIndexId;	//当前回合战斗过的GObjectIndexId[可能存在重复]
	// 胜利的阵营
	int m_nWinCampId;
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