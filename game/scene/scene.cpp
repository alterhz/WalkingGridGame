#include "scene.h"


IScene::IScene(ESceneType eSceneType)
	: m_eSceneType(eSceneType)
	, m_nId(0)
	, m_bDead(false)
{
}

IScene::~IScene()
{
}


void IScene::DoTick()
{
	OnTick();
}


bool IScene::OnTick()
{
	return true;
}
