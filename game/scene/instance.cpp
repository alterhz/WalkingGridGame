#include "instance.h"

CInstance::CInstance()
	: IScene(IScene::ESceneType_Instance)
{
}

CInstance::~CInstance()
{

}

bool CInstance::OnTick()
{
	return IScene::OnTick();
}
