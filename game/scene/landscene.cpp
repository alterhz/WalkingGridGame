#include "landscene.h"

CLandScene::CLandScene()
	: IScene(IScene::ESceneType_LandScene)
{
}

CLandScene::~CLandScene()
{

}

bool CLandScene::OnTick()
{
	return IScene::OnTick();
}
