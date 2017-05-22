#include "GameMain.h"
#include "Stage1.h"

Scene* Stage1::createScene()
{
	auto scene = Scene::create();
	auto layer = Stage1::create();
	scene->addChild(layer);
	return scene;
}

bool Stage1::init()
{
	if (!Layer::init())
	{
		return false;
	}

	winSize = Director::getInstance()->getWinSize();

	return true;
}
