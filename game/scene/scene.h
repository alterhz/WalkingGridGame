//-------------------------------------------
// author : alterhz
// date : 8/15/2015
// description : 

#ifndef _SCENE_H_
#define _SCENE_H_

#include <map>

class ISprite;
class CZoneManager;

class IScene
{
	friend class CSceneManager;

	typedef std::map<int, ISprite *> MapSprite;

public:
	enum ESceneType
	{
		ESceneType_Null = 0,
		ESceneType_LandScene = 1,	//普通场景
		ESceneType_Instance = 2,	//副本
	};

public:
	virtual ~IScene();

public:	//base
	int GetId() const { return m_nId; }
	bool GetDead() const { return m_bDead; }
	void SetDead() { m_bDead = true; }

public:	//功能逻辑
	bool SpriteEnter(ISprite *pSprite);
	bool SpriteLeave(ISprite *pSprite);

protected:
	IScene(ESceneType eSceneType);
	void SetId(int nId) { m_nId = nId; }

	bool Init();
	bool DoTick();


protected:
	virtual bool OnInit();
	virtual bool OnTick();
	virtual bool OnSpriteEnter(ISprite *pSprite);
	virtual bool OnSpriteLeave(ISprite *pSprite);

private:
	ESceneType m_eSceneType;
	int m_nId;
	CZoneManager *m_pZoneManager;

	MapSprite m_mapSprite;

protected:
	bool m_bDead;
};

#endif