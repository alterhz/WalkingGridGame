#include "scenemanager.h"
#include "scene.h"

#include "debug.h"
using namespace NS_IO;

#include "landscene.h"
#include "instance.h"

CSceneManager::CSceneManager()
	: m_nSceneIdAlloc(0)
{
}

CSceneManager::~CSceneManager()
{
}

const IScene * CSceneManager::GetScene(int nSceneId) const
{
	MapScene::const_iterator it = m_mapScene.find(nSceneId);
	if (it != m_mapScene.end())
	{
		return it->second;
	}

	return nullptr;
}

IScene * CSceneManager::GetScene(int nSceneId)
{
	MapScene::iterator it = m_mapScene.find(nSceneId);
	if (it != m_mapScene.end())
	{
		return it->second;
	}

	return nullptr;
}

bool CSceneManager::AddScene(IScene *pScene)
{
	if (nullptr == pScene)
	{
		LOGError("nullptr == pScene");
		return false;
	}

	int nSceneId = pScene->GetId();
	const IScene *pFindScene = GetScene(nSceneId);
	if (pFindScene)
	{
		LOGError("Ìí¼Ó³¡¾°Id[" + nSceneId + "]ÖØ¸´¡£");
		return false;
	}

	m_mapScene.insert(std::make_pair(nSceneId, pScene));

	return true;
}

void CSceneManager::DoTick()
{
	MapScene::iterator it = m_mapScene.begin();
	for (; it!=m_mapScene.end();)
	{
		IScene *pScene = it->second;
		if (nullptr == pScene)
		{
			LOGError("nullptr == pScene");

			it = m_mapScene.erase(it);
			continue;
		}

		pScene->DoTick();

		if (pScene->GetDead())
		{
			it = m_mapScene.erase(it);
			continue;
		}

		++it;
	}
}

CLandScene * CSceneManager::CreateLandScene(int nLandSceneSN)
{
	CLandScene *pNewLandScene = new CLandScene();
	if (nullptr == pNewLandScene)
	{
		LOGError("nullptr == pLandScene");
		return nullptr;
	}

	int nNewSceneId = (++m_nSceneIdAlloc);
	pNewLandScene->SetId(nNewSceneId);

	if (!AddScene(pNewLandScene))
	{
		return nullptr;
	}

	return pNewLandScene;
}

CInstance * CSceneManager::CreateInstance(int nInstanceSN)
{
	return nullptr;
}


