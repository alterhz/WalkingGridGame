#include "sprite.h"
#include "debug.h"
using namespace NS_IO;

int g_nSpriteIdAlloc = 0;

ISprite::ISprite()
	: m_nId(++g_nSpriteIdAlloc)
	, m_pCurrentAction(nullptr)
{
	m_attr.SetAttr(EAttrType_Speed, 4.0f);
}

ISprite::~ISprite()
{
	DestroyAction(m_pCurrentAction);
	m_pCurrentAction = nullptr;
}

bool ISprite::DoTick(MSTIME msNow)
{
	// 自动切换状态
	OnActionAutoSwitch(msNow);

	return OnTick(msNow);
}

Vector2 ISprite::GetCurrentPosition( MSTIME msNow ) const
{
	if (m_pCurrentAction)
	{
		return m_pCurrentAction->GetCurrentPosition(msNow);
	}

	return Vector2();
}

bool ISprite::MoveTo( Vector2 &v2 )
{
	CStandAction *pStandAction = new CStandAction(v2);
	if (nullptr == pStandAction)
	{
		return false;
	}

	if (ChangeAction(pStandAction))
	{
		return true;
	}

	DestroyAction(pStandAction);
	return false;
}

bool ISprite::RunTo( VtPath &vtPath, MSTIME msNow)
{
	float fSpeed = m_attr.GetAttr(EAttrType_Speed);

	CRunAction *pRunAction = new CRunAction(vtPath, fSpeed, msNow);

	if (nullptr == pRunAction)
	{
		return false;
	}

	if (ChangeAction(pRunAction))
	{
		return true;
	}

	DestroyAction(pRunAction);
	return false;
}

bool ISprite::OnEnterScene(IScene *pScene)
{
	return true;
}

bool ISprite::OnLeaveScene(IScene *pScene)
{
	return true;
}

bool ISprite::ChangeAction( IAction *pAction )
{
	if (nullptr == pAction)
	{
		LOGError("nullptr == pAction");
		return false;
	}

	if (nullptr == m_pCurrentAction)
	{
		// 首次状态赋值，只能是Stand状态

		if (EActionType_Stand != pAction->GetCurrentActionType())
		{
			LOGError("首次初始化状态[" + pAction->GetCurrentActionType() + "]错误。");
			return false;
		}

		m_pCurrentAction = pAction;

		return true;
	}
	else
	{
		switch (m_pCurrentAction->GetCurrentActionType())
		{
		case EActionType_Null:
			break;
		case EActionType_Stand:
			{
				// 站立状态可以切换到任何状态
				DestroyAction(m_pCurrentAction);
				m_pCurrentAction = nullptr;

				// 设置新状态
				m_pCurrentAction = pAction;

				return true;
			}
			break;
		case EActionType_Run:
			{
				// 行走状态下可以切换回站立状态
				if (EActionType_Stand == pAction->GetCurrentActionType())
				{
					// 站立状态可以切换到任何状态
					DestroyAction(m_pCurrentAction);
					m_pCurrentAction = nullptr;

					// 设置新状态
					m_pCurrentAction = pAction;

					return true;
				}
				else if (EActionType_Run == pAction->GetCurrentActionType())
				{
					// 站立状态可以切换到任何状态
					DestroyAction(m_pCurrentAction);
					m_pCurrentAction = nullptr;

					// 设置新状态
					m_pCurrentAction = pAction;

					return true;
				}
				else if (EActionType_Dead == pAction->GetCurrentActionType())
				{
					// 站立状态可以切换到任何状态
					DestroyAction(m_pCurrentAction);
					m_pCurrentAction = nullptr;

					// 设置新状态
					m_pCurrentAction = pAction;

					return true;
				}
			}
			break;
		case EActionType_Dead:
			{
				// 死亡状态只能切换回站立状态
				if (EActionType_Stand == pAction->GetCurrentActionType())
				{
					// 站立状态可以切换到任何状态
					DestroyAction(m_pCurrentAction);
					m_pCurrentAction = nullptr;

					// 设置新状态
					m_pCurrentAction = pAction;

					return true;
				}
			}
			break;
		default:
			break;
		}
	}

	return false;
}

void ISprite::DestroyAction( IAction *pAction )
{
	delete pAction;
	pAction = nullptr;
}

void ISprite::SetHP(int nHP)
{
	if (nHP > m_attr.GetAttr(EAttrType_MaxHP))
	{
		m_attr.SetAttr(EAttrType_HP, m_attr.GetAttr(EAttrType_MaxHP));
	}
	else
	{
		m_attr.SetAttr(EAttrType_HP, nHP);
	}
}

void ISprite::SetMaxHP(int nMaxHP)
{
	if (m_attr.GetAttr(EAttrType_HP) > nMaxHP)
	{
		m_attr.SetAttr(EAttrType_HP, nMaxHP);
	}

	m_attr.SetAttr(EAttrType_MaxHP, nMaxHP);
}

bool ISprite::DoEnterScene()
{


	return true;
}

bool ISprite::DoLeaveScene()
{
	return true;
}

//////////////////////////////////////////////////////////////////////////
// 事件处理函数
//////////////////////////////////////////////////////////////////////////

bool ISprite::OnTick(MSTIME msNow)
{
	// 逻辑循环
	return true;
}

void ISprite::OnActionAutoSwitch(MSTIME msNow)
{
	if (m_pCurrentAction)
	{
		if (m_pCurrentAction->NeedSwitch(msNow))
		{
			Vector2 v2 = m_pCurrentAction->GetCurrentPosition(msNow);
			CStandAction *pStandAction = new CStandAction(v2);
			ChangeAction(pStandAction);
		}
	}
}

