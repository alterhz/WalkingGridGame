//-------------------------------------------
// author : alterhz
// date : 8/15/2015
// description : 

#ifndef _SCENE_H_
#define _SCENE_H_

class IScene
{
	friend class CSceneManager;

public:
	enum ESceneType
	{
		ESceneType_Null = 0,
		ESceneType_LandScene = 1,	//普通场景
		ESceneType_Instance = 2,	//副本
	};

public:
	virtual ~IScene();

public:
	int GetId() const { return m_nId; }
	bool GetDead() const { return m_bDead; }
	void SetDead() { m_bDead = true; }

protected:
	IScene(ESceneType eSceneType);
	void SetId(int nId) { m_nId = nId; }
	void DoTick();


protected:
	virtual bool OnTick();

private:
	ESceneType m_eSceneType;
	int m_nId;

protected:
	bool m_bDead;
};

#endif