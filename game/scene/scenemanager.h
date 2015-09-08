//-------------------------------------------
// author : alterhz
// date : 9/6/2015
// description : 

#ifndef _SCENEMANAGER_H_
#define _SCENEMANAGER_H_

#include <map>

class IScene;
class CLandScene;
class CInstance;

class CSceneManager
{
	typedef std::map<int, IScene *> MapScene;
public:
	CSceneManager();
	~CSceneManager();

public:
	IScene * GetScene(int nSceneId);

	CLandScene * CreateLandScene(int nLandSceneSN);
	CInstance * CreateInstance(int nInstanceSN);

protected:
	void DoTick();

private:
	bool AddScene(IScene *pScene);

	const IScene * GetScene(int nSceneId) const;
	

private:
	int m_nSceneIdAlloc;
	MapScene m_mapScene;
};

#endif