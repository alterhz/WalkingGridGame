#include "battleground.h"

#include "debug.h"

#include "app.h"
#include "country.h"
#include "gobject.h"
#include "ground.h"
#include "mymath.h"


IBattleGround::IBattleGround()
	: m_nIndexId(0)
	, m_eStatus(EStatus_Waiting)
{
	static int g_nIndexId = 0;
	m_nIndexId = (++g_nIndexId);
}

IBattleGround::~IBattleGround()
{
}

bool IBattleGround::Init()
{
	if (OnInit())
	{
		ChangeStatus(EStatus_Run);
	

		return true;
	}
	else
	{
		return false;
	}
}

bool IBattleGround::DoTick()
{
	return OnTick();
}

bool IBattleGround::ChangeStatus(EStatus eStatus)
{
	switch (m_eStatus)
	{
	case IBattleGround::EStatus_None:
		break;
	case IBattleGround::EStatus_Waiting:
		{
			if (EStatus_Run == eStatus)
			{
				OnGoRun();

				m_eStatus = EStatus_Run;
				return true;
			}
		}
		break;
	case IBattleGround::EStatus_Run:
		{
			if (EStatus_Finish == eStatus)
			{
				OnGoFinish();

				m_eStatus = EStatus_Finish;
				return true;
			}
		}
		break;
	case IBattleGround::EStatus_Pause:
		break;
	case IBattleGround::EStatus_Finish:
		break;
	default:
		break;
	}

	return false;
}

void IBattleGround::GetGroundInfo(ICountry *pCountry)
{
	if (nullptr == pCountry)
	{
		LOGError("nullptr == pCountry");
		return ;
	}

	pCountry->SendGetGroundInfo(m_nWGCount, m_nHGCount, m_mapGrid, m_mapGObject);
}

ICountry * IBattleGround::FindCountry(int nIndexId)
{
	MapCountry::iterator itCountry = m_mapCountry.find(nIndexId);
	if (itCountry != m_mapCountry.end())
	{
		return itCountry->second;
	}

	return nullptr;
}

ICountry * IBattleGround::FindCountry(int nIndexId) const
{
	MapCountry::const_iterator itCountry = m_mapCountry.find(nIndexId);
	if (itCountry != m_mapCountry.end())
	{
		return itCountry->second;
	}

	return nullptr;
}

bool IBattleGround::Enter(ICountry *pCountry)
{
	if (nullptr == pCountry)
	{
		LOGError("nullptr == pCountry");
		return false;
	}

	if (!FindCountry(pCountry->GetIndexId()))
	{
		// 进入战场完毕
		m_mapCountry.insert(std::make_pair(pCountry->GetIndexId(), pCountry));
		pCountry->SetBattleGround(this);
	}

	return OnEnter(pCountry);
}

bool IBattleGround::Leave(ICountry *pCountry)
{
	if (nullptr == pCountry)
	{
		LOGError("nullptr == pCountry");
		return false;
	}

	int nCountryIndexId = pCountry->GetIndexId();

	if (FindCountry(nCountryIndexId))
	{
		pCountry->SetBattleGround(nullptr);

		m_mapCountry.erase(nCountryIndexId);

		OnLeave(pCountry);
	}

	return true;
}

bool IBattleGround::GObjectEnter(IGObject *pGObject, int x, int y)
{
	if (nullptr == pGObject)
	{
		LOGError("nullptr == pGObject");
		return false;
	}

	int nGObjectIndexId = pGObject->GetIndexId();

	if (FindGObject(nGObjectIndexId))
	{
		LOGError("场景中已经存在当前GObject[" + nGObjectIndexId + "]。");
		return false;
	}

	IGrid *pGrid = GetGrid(x, y);
	if (nullptr == pGrid)
	{
		LOGError("nullptr == pGrid");
		return false;
	}

	if (!pGrid->IsWalkable(EToWard_Both))
	{
		LOGDebug("不能行走，所以不能直接添加到场景中。");
		return false;
	}

	// 格子绑定GObject
	pGrid->AddGObject(pGObject);

	pGObject->SetBattleGround(this);
	pGObject->SetXY(x, y);

	m_mapGObject.insert(std::make_pair(pGObject->GetIndexId(), pGObject));

	OnGObjectEnter(pGObject);

	return true;
}

bool IBattleGround::GObjectLeave(IGObject *pGObject)
{
	if (nullptr == pGObject)
	{
		LOGError("nullptr == pGObject");
		return false;
	}

	int nGObjectIndexId = pGObject->GetIndexId();

	if (!FindGObject(nGObjectIndexId))
	{
		LOGError("场景中不存在当前GObject[" + nGObjectIndexId + "]。");
		return false;
	}

	OnGObjectLeave(pGObject);

	m_mapGObject.erase(nGObjectIndexId);

	pGObject->SetBattleGround(nullptr);

	int nX = pGObject->GetX();
	int nY = pGObject->GetY();

	IGrid *pGrid = GetGrid(nX, nY);
	if (pGrid)
	{
		pGrid->DelGObject(pGObject);
	}

	return true;
}

void IBattleGround::PrepareFinish(ICountry *pCountry)
{
	LOGDebug("准备完毕");
}

IGObject * IBattleGround::FindGObject(int nIndexId)
{
	MapGObject::iterator itGObject = m_mapGObject.find(nIndexId);
	if (itGObject != m_mapGObject.end())
	{
		return itGObject->second;
	}

	return nullptr;
}

IGObject * IBattleGround::FindGObject(int nIndexId) const
{
	MapGObject::const_iterator itGObject = m_mapGObject.find(nIndexId);
	if (itGObject != m_mapGObject.end())
	{
		return itGObject->second;
	}

	return nullptr;
}

bool IBattleGround::OnEnter(ICountry *pCountry)
{
	if (nullptr == pCountry)
	{
		LOGError("nullptr == pCountry");
		return false;
	}

	pCountry->SendGetGroundInfo(m_nWGCount, m_nHGCount, m_mapGrid, m_mapGObject);

	return true;
}

bool IBattleGround::OnGObjectEnter(IGObject *pGObject)
{
	if (nullptr == pGObject)
	{
		LOGError("nullptr == pGObject");
		return false;
	}

	// 通知场景中的国家，添加了一个GObject
	auto itCountry = m_mapCountry.begin();
	for (; itCountry!= m_mapCountry.end(); ++itCountry)
	{
		ICountry *pCountry = itCountry->second;

		if (pCountry)
		{
			pCountry->SendGObjectEnterGround(pGObject->GetType(), pGObject->GetIndexId(), 
				pGObject->GetSN(), pGObject->GetX(), pGObject->GetY(), pGObject->GetHP(), 
				pGObject->GetMaxHP(), pGObject->GetSP(), pGObject->GetLevel(), pGObject->GetCampId());
		}
	}

	return true;
}

void IBattleGround::BattleBoutFinish(ICountry *pCountry)
{
	LOGDebug("[" + pCountry->GetIndexId() + "]战斗回合结束。");
}

bool IBattleGround::PathIsOK(const VtCoor2 &vtCoor2)
{
	// 验证路径点是否都是相邻格子
	if (vtCoor2.size() < 2)
	{
		// 只有一个点无法行走
		return false;
	}

	COOR2 coor2Prev = vtCoor2[0];

	for (int i=1; i<static_cast<int>(vtCoor2.size()); ++i)
	{
		COOR2 coor2 = vtCoor2[i];

		// 路径点是否都可以移动
		IGrid *pGrid = GetGrid(coor2.x, coor2.y);
		if (nullptr == pGrid)
		{
			LOGError("nullptr == pGrid");
			return false;
		}

		if (!pGrid->IsWalkable(EToWard_Both))
		{
			// 路径点，存在不可以行走点
			return false;
		}

		if (COOR2::Length(coor2, coor2Prev) != 1)
		{
			// 相邻路径点距离有误
			return false;
		}

		coor2Prev = coor2;
	}

	return true;
}

bool IBattleGround::GObjectMove(IGObject *pGObject, const VtCoor2 &vtCoor2)
{
	LOGDebug("GObjectMove");
	return true;
}

bool IBattleGround::GObjectUseSkill(IGObject *pGObject, int nSkillSN, int nTargetGObjectIndexId)
{
	LOGDebug("目标释放技能");
	return true;
}

const int G_nDemoWidthCount = 20;
const int G_nDemoHeigthCount = 30;

CFrontBattleGround::CFrontBattleGround()
	: m_eBattleStatus(EBattleStatus_Waiting)
	, m_nBoutIndex(0)
	, m_nBoutCountryIndexId(0)
	, m_nWinCountryIndexId(0)
{
}

CFrontBattleGround::~CFrontBattleGround()
{
}

bool CFrontBattleGround::OnInit()
{
	int nHGCount = G_nDemoHeigthCount;
	int nWGCount = G_nDemoWidthCount;

	// 初始化场景地图
	InitGroundSize(nWGCount, nHGCount);

	// 初始化地形数据
	for (int nY=1; nY<=nHGCount; ++nY)
	{
		for (int nX=1; nX<=nWGCount; ++nX)
		{
			IGrid *pNewGrid = new CGrid(nX, nY);
			if (nullptr == pNewGrid)
			{
				LOGError("nullptr == pNewGrid");
				continue;
			}

			int nSN = 1;

			if (!pNewGrid->Init(nSN))
			{
				LOGError("初始化地图格子[SN:" + nSN + "]失败。");
				continue;
			}

			SetGrid(nX, nY, pNewGrid);
		}
	}

	// 加载场景（固定）单位，如桥梁，树木，城墙，大炮

	// 地图中心放入3个桥梁
	{
		CStillObject *pNewGBridge = new CStillObject();
		if (nullptr == pNewGBridge)
		{
			LOGError("nullptr == pNewGBridge");
			return false;
		}

		pNewGBridge->Init(1);
		pNewGBridge->SetCampId(0);
		pNewGBridge->SetLevel(1);

		pNewGBridge->EnterGround(10, 15, this);
	}

	{
		CStillObject *pNewGBridge = new CStillObject();
		if (nullptr == pNewGBridge)
		{
			LOGError("nullptr == pNewGBridge");
			return false;
		}

		pNewGBridge->Init(2);
		pNewGBridge->SetCampId(0);
		pNewGBridge->SetLevel(1);

		pNewGBridge->EnterGround(8, 15, this);
	}

	{
		CStillObject *pNewGBridge = new CStillObject();
		if (nullptr == pNewGBridge)
		{
			LOGError("nullptr == pNewGBridge");
			return false;
		}

		pNewGBridge->Init(2);
		pNewGBridge->SetCampId(0);
		pNewGBridge->SetLevel(1);

		pNewGBridge->EnterGround(12, 15, this);
	}


	// 加载场景（移动）中立单位
	{
		CWalkableObject *pNewDogFace = new CWalkableObject();
		if (nullptr == pNewDogFace)
		{
			LOGError("nullptr == pNewDogFace");
			return false;
		}

		pNewDogFace->Init(1);
		pNewDogFace->SetCampId(0);
		pNewDogFace->SetLevel(1);

		pNewDogFace->EnterGround(1, 15, this);
	}

	{
		CWalkableObject *pNewDogFace = new CWalkableObject();
		if (nullptr == pNewDogFace)
		{
			LOGError("nullptr == pNewDogFace");
			return false;
		}

		pNewDogFace->Init(1);
		pNewDogFace->SetCampId(0);
		pNewDogFace->SetLevel(1);

		pNewDogFace->EnterGround(18, 15, this);
	}

	return true;
}

bool CFrontBattleGround::OnTick()
{
	//LOGDebug("阵地战DoTick");
	return true;
}

bool CFrontBattleGround::OnEnter(ICountry *pCountry)
{
	IBattleGround::OnEnter(pCountry);

	// 场景中有两个国家，则开始
	if (m_mapCountry.size() >= 2)
	{
		// 切换场景状态
		ChangeBattleStatus(EBattleStatus_Prepare);
	}

	return true;
}

bool CFrontBattleGround::OnLeave(ICountry *pCountry)
{
	// 一个角色离开场景，另外一个角色获得胜利
	EBattleStatus eBattleStatus = GetCurrentBattleStatus();
	if (EBattleStatus_Prepare == eBattleStatus
		|| EBattleStatus_Battle == eBattleStatus)
	{
		if (m_mapCountry.size() == 1)
		{
			auto itCountry = m_mapCountry.begin();

			if (itCountry->second)
			{
				// 获胜的CountryIndexId
				m_nWinCountryIndexId = itCountry->second->GetIndexId();

				ChangeBattleStatus(EBattleStatus_Reward);
			}
		}
	}

	return false;
}

void CFrontBattleGround::ChangeBattleStatus(EBattleStatus eBattleStatus)
{
	switch (m_eBattleStatus)
	{
	case CFrontBattleGround::EBattleStatus_Waiting:
		{
			if (EBattleStatus_Prepare == eBattleStatus)
			{
				OnGoPrepare();

				m_eBattleStatus = eBattleStatus;
			}
		}
		break;
	case CFrontBattleGround::EBattleStatus_Prepare:
		{
			if (EBattleStatus_Battle == eBattleStatus)
			{
				OnGoBattle();

				m_eBattleStatus = eBattleStatus;
			}
			else if (EBattleStatus_Reward == eBattleStatus)
			{
				OnGoReward();

				m_eBattleStatus = eBattleStatus;
			}
		}
		break;
	case CFrontBattleGround::EBattleStatus_Battle:
		{
			if (EBattleStatus_Reward == eBattleStatus)
			{
				OnGoReward();

				m_eBattleStatus = eBattleStatus;
			}
		}
		break;
	case CFrontBattleGround::EBattleStatus_Reward:
		break;
	default:
		break;
	}
}

// 战场准备
void CFrontBattleGround::OnGoPrepare()
{
	// 测试阶段，客户端的排兵布阵，直接由服务器自动排兵，然后客户端发送准备完成即可。
	{
		if (m_mapCountry.size() == 2)
		{
			auto itCountry = m_mapCountry.begin();

			ICountry *pCountryA = itCountry->second;
			if (pCountryA)
			{
				const VtGObject &vtGObject = pCountryA->GetPrepareGObject();

				int nSetX = 10 - vtGObject.size() / 2;
				for (IGObject *pGObject : vtGObject)
				{
					if (pGObject)
					{
						if (GObjectType_Still == pGObject->GetType())
						{
							CStillObject *pStillObject = pGObject->GetStillObject();
							if (nullptr == pStillObject)
							{
								LOGError("nullptr == pStillObject");
								continue;
							}

							if (pStillObject->IsFlag())
							{
								// 将领
								pStillObject->EnterGround(10, 1, this);
								continue;
							}
						}

						if (!pGObject->EnterGround(nSetX++, 2, this))
						{
							LOGError("GObject[" + pGObject->GetIndexId() + "]进入场景失败。");
						}
					}
				}
			}

			++itCountry;
			ICountry *pCountryB = itCountry->second;
			if (pCountryB)
			{
				const VtGObject &vtGObject = pCountryB->GetPrepareGObject();

				int nSetX = 10 - vtGObject.size() / 2;
				for (IGObject *pGObject : vtGObject)
				{
					if (pGObject)
					{
						if (GObjectType_Still == pGObject->GetType())
						{
							CStillObject *pStillObject = pGObject->GetStillObject();
							if (nullptr == pStillObject)
							{
								LOGError("nullptr == pStillObject");
								continue;
							}

							if (pStillObject->IsFlag())
							{
								// 将领
								pStillObject->EnterGround(10, 30, this);
								continue;
							}
						}

						if (!pGObject->EnterGround(nSetX++, 29, this))
						{
							LOGError("GObject[" + pGObject->GetIndexId() + "]进入场景失败。");
						}
					}
				}
			}

		}
	}

	// 通知客户端准备
	auto itCountry = m_mapCountry.begin();
	for (; itCountry!= m_mapCountry.end(); ++itCountry)
	{
		ICountry *pCountry = itCountry->second;

		if (pCountry)
		{
			pCountry->SendBattlePrepare();
		}
	}
}

// 开始战斗
void CFrontBattleGround::OnGoBattle()
{
	auto itCountry = m_mapCountry.begin();
	for (; itCountry!= m_mapCountry.end(); ++itCountry)
	{
		ICountry *pCountry = itCountry->second;

		if (pCountry)
		{
			pCountry->SendBattleStart();
		}
	}

	// 随机分配先开始回合(bout)的角色
	int nRandInt = NS_IO::Random();

	if (m_vtPrepareFinishCountryIndexId.size() >= 2)
	{
		m_nBoutIndex = nRandInt % m_vtPrepareFinishCountryIndexId.size();

		m_nBoutCountryIndexId = m_vtPrepareFinishCountryIndexId[m_nBoutIndex];
	}
	else
	{
		LOGError("战斗开始，场景中角色不足。");
		return ;
	}

	// 通知客户端，当前战斗回合的Country
	auto itCountrySend = m_mapCountry.begin();
	for (; itCountrySend!= m_mapCountry.end(); ++itCountrySend)
	{
		ICountry *pCountry = itCountrySend->second;

		if (pCountry)
		{
			pCountry->SendBattleFight(m_nBoutCountryIndexId);
		}
	}
}

// 战场结束，奖励
void CFrontBattleGround::OnGoReward()
{
	auto itCountry = m_mapCountry.begin();
	for (; itCountry!= m_mapCountry.end(); ++itCountry)
	{
		ICountry *pCountry = itCountry->second;

		if (pCountry)
		{
			pCountry->SendBattleReward(m_nWinCountryIndexId);
		}
	}	
}

void CFrontBattleGround::PrepareFinish(ICountry *pCountry)
{
	if (nullptr == pCountry)
	{
		LOGError("nullptr == pCountry");
		return ;
	}

	if (!FindCountry(pCountry->GetIndexId()))
	{
		LOGError("不是当前场景的Country，不能准备。");
		return ;
	}

	for (int nCountryId : m_vtPrepareFinishCountryIndexId)
	{
		if (pCountry->GetIndexId() == nCountryId)
		{
			return ;
		}
	}

	m_vtPrepareFinishCountryIndexId.push_back(pCountry->GetIndexId());

	if (m_vtPrepareFinishCountryIndexId.size() == m_mapCountry.size())
	{
		// 准备人数与场景人数相等，则代表都准备完成，开始战斗
		ChangeBattleStatus(EBattleStatus_Battle);
	}
}

void CFrontBattleGround::BattleBoutFinish(ICountry *pCountry)
{
	if (nullptr == pCountry)
	{
		LOGError("nullptr == pCountry");
		return ;
	}

	if (pCountry->GetIndexId() != m_nBoutCountryIndexId)
	{
		LOGDebug("不是自己[" + pCountry->GetIndexId() + "]的回合[" + m_nBoutCountryIndexId + "]，不能结束。");
		return ;
	}

	++m_nBoutIndex;

	if (m_vtPrepareFinishCountryIndexId.size() > 0)
	{
		m_nBoutIndex = m_nBoutIndex % m_vtPrepareFinishCountryIndexId.size();

		m_nBoutCountryIndexId = m_vtPrepareFinishCountryIndexId[m_nBoutIndex];
	}
	else
	{
		LOGError("场景中country[" + m_vtPrepareFinishCountryIndexId.size() + "]数量有误！");
		return ;
	}

	// 清理移动战斗列表
	m_vtBoutMoveFightGObjectIndexId.clear();

	// 通知客户端，当前战斗回合的Country
	auto itCountry = m_mapCountry.begin();
	for (; itCountry!= m_mapCountry.end(); ++itCountry)
	{
		ICountry *pCountry = itCountry->second;

		if (pCountry)
		{
			pCountry->SendBattleFight(m_nBoutCountryIndexId);
		}
	}
}

bool CFrontBattleGround::GObjectMove(IGObject *pGObject, const VtCoor2 &vtCoor2)
{
	if (nullptr == pGObject)
	{
		LOGError("nullptr == pGObject");
		return false;
	}

	// 是否已经移动战斗过
	if (IsMoveFight(pGObject->GetIndexId()))
	{
		return false;
	}

	// 验证路径通畅
	if (!PathIsOK(vtCoor2))
	{
		return false;
	}

	IGrid *pOldGrid = pGObject->GetGrid();
	if (pOldGrid)
	{
		pOldGrid->DelGObject(pGObject);
	}

	if (vtCoor2.size() <= 2)
	{
		LOGError("路径点数量[" + vtCoor2.size() + "]不足两个。");
		return false;
	}

	COOR2 coor2MoveTo = vtCoor2[vtCoor2.size() - 1];
	// 新坐标点绑定
	IGrid *pNewGrid = GetGrid(coor2MoveTo.x, coor2MoveTo.y);
	if (pNewGrid)
	{
		pNewGrid->AddGObject(pGObject);
	}

	// 更新位置数据
	pGObject->SetXY(coor2MoveTo.x, coor2MoveTo.y);

	// 行走广播
	auto itCountry = m_mapCountry.begin();
	for (; itCountry!= m_mapCountry.end(); ++itCountry)
	{
		ICountry *pCountry = itCountry->second;
		if (pCountry)
		{
			pCountry->SendMove(pGObject->GetIndexId(), vtCoor2);
		}
	}

	m_vtBoutMoveFightGObjectIndexId.push_back(pGObject->GetIndexId());

	return true;
}

bool CFrontBattleGround::IsMoveFight(int nGObjectIndexId) const
{
	for (const int nHaveMoveFightGObjectIndexId : m_vtBoutMoveFightGObjectIndexId)
	{
		if (nHaveMoveFightGObjectIndexId == nGObjectIndexId)
		{
			return true;
		}
	}

	return false;
}

bool CFrontBattleGround::GObjectUseSkill(IGObject *pGObject, int nSkillSN, int nTargetGObjectIndexId)
{
	if (nullptr == pGObject)
	{
		LOGError("nullptr == pGObject");
		return false;
	}

	const CXmlData_Skill *pXmlData_Skill = CConfigReadManager::getMe().xdSkill.GetRecord(nSkillSN);
	if (nullptr == pXmlData_Skill)
	{
		LOGError("nullptr == pXmlData_Skill");
		return false;
	}

	if (CXmlData_Skill::ERange_Single == pXmlData_Skill->eRange)
	{
		if (CXmlData_Skill::ETarget_Enemy == pXmlData_Skill->eTarget)
		{
			IGObject *pTargetGObject = FindGObject(nTargetGObjectIndexId);
			if (nullptr == pTargetGObject)
			{
				LOGError("没有找到技能[" + nSkillSN + "]释放目标[" + nTargetGObjectIndexId + "]。");
				return false;
			}

			// 被攻击目标是否参与战斗
			if (!pTargetGObject->IsCanFight())
			{
				LOGDebug("被攻击者不参与战斗，不能释放技能。");
				return false;
			}

			if (pTargetGObject->GetCampId() == pGObject->GetCampId())
			{
				LOGDebug("目标[" + nTargetGObjectIndexId + "]是友方单位或者自己，不能攻击。");
				return false;
			}

			// 计算攻击距离
			const COOR2 &coor2Target = pTargetGObject->GetCoor2();
			const COOR2 &coor2Self = pGObject->GetCoor2();

			int nLength = COOR2::Length(coor2Self, coor2Target);

			if (nLength < pXmlData_Skill->nMinAttackLength
				|| nLength > pXmlData_Skill->nMaxAttackLength)
			{
				LOGDebug("超过攻击范围[nLength:" + nLength + ", minLength:" + pXmlData_Skill->nMinAttackLength 
					+ ", nMaxLength:" + pXmlData_Skill->nMaxAttackLength + "]。");
				return false;
			}

			// 伤害计算
			int nAtt = pGObject->GetAtt();

			int nRand = NS_IO::Random() % 21;

			int nDamageValue = static_cast<int>(nAtt * (90 + nRand) / 100.0f) + pXmlData_Skill->nAddDamage;

			int nLostHP = pTargetGObject->LostHP(nDamageValue);
			int nRemainHP = pTargetGObject->GetHP();

			// 技能伤害广播
			auto itCountry = m_mapCountry.begin();
			for (; itCountry!= m_mapCountry.end(); ++itCountry)
			{
				ICountry *pCountry = itCountry->second;
				if (pCountry)
				{
					pCountry->SendFight(nSkillSN, pGObject->GetIndexId(), pTargetGObject->GetIndexId(), nLostHP, nRemainHP);
				}
			}
		}
		else if (CXmlData_Skill::ETarget_Self == pXmlData_Skill->eTarget)
		{

		}
		else if (CXmlData_Skill::ETarget_Friend == pXmlData_Skill->eTarget)
		{

		}
	}
	else if (CXmlData_Skill::ERange_Multi == pXmlData_Skill->eRange)
	{
		if (CXmlData_Skill::ETarget_Self == pXmlData_Skill->eTarget)
		{
			// 以自身为中心释放技能

		}
		else if (CXmlData_Skill::ETarget_Friend == pXmlData_Skill->eTarget)
		{

		}
		else if (CXmlData_Skill::ETarget_Enemy == pXmlData_Skill->eTarget)
		{

		}
	}
	else if (CXmlData_Skill::ERange_All == pXmlData_Skill->eRange)
	{

	}
	else
	{
		LOGError("技能范围类型[" + pXmlData_Skill->eRange + "]异常。");
	}


	return true;
}



CBattleGroundManager::CBattleGroundManager()
	: m_nDoTickTimerId(0)
{
	m_nDoTickTimerId = CApp::getMe().SetTimer(this, 100);
}

CBattleGroundManager::~CBattleGroundManager()
{
	CApp::getMe().KillTimer(m_nDoTickTimerId);
	m_nDoTickTimerId = 0;
}

bool CBattleGroundManager::Init()
{
	return true;
}

CFrontBattleGround* CBattleGroundManager::CreateFrontBattleGround()
{
	CFrontBattleGround *pNewFrontBattleGround = new CFrontBattleGround();
	if (nullptr == pNewFrontBattleGround)
	{
		LOGError("nullptr == pNewFrontBattleGround");
		return nullptr;
	}

	pNewFrontBattleGround->Init();

	m_mapBattleGround.insert(std::make_pair(pNewFrontBattleGround->GetIndexId(), pNewFrontBattleGround));

	return pNewFrontBattleGround;
}

bool CBattleGroundManager::OnTimerEvent(int nTimerId)
{
	if (m_nDoTickTimerId == nTimerId)
	{
		auto itBattleGround = m_mapBattleGround.begin();
		for (; itBattleGround!=m_mapBattleGround.end(); ++itBattleGround)
		{
			itBattleGround->second->DoTick();
		}
	}

	return true;
}

IBattleGround * CBattleGroundManager::FindBattleGround(int nBattleGroundIndexId)
{
	MapBattleGround::iterator itBattleGround = m_mapBattleGround.find(nBattleGroundIndexId);
	if (itBattleGround != m_mapBattleGround.end())
	{
		return itBattleGround->second;
	}

	return nullptr;
}

IBattleGround * CBattleGroundManager::FindBattleGround(int nBattleGroundIndexId) const
{
	MapBattleGround::const_iterator itBattleGround = m_mapBattleGround.find(nBattleGroundIndexId);
	if (itBattleGround != m_mapBattleGround.end())
	{
		return itBattleGround->second;
	}

	return nullptr;
}
