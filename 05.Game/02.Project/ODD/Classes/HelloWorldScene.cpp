#include "HelloWorldScene.h"
#include "GameMain.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
	auto scene = Scene::create();
	auto layer = HelloWorld::create();
	scene->addChild(layer);
	return scene;
}

bool HelloWorld::init()
{
	if (!Layer::init())
	{
		return false;
	}

	winSize = Director::getInstance()->getWinSize();

	auto logo = Sprite::create("Images/Intro_Yasss.png");
	logo->setPosition(Vec2(winSize.width / 2, winSize.height * 2/3));
	logo->setScale(6.0f);
	logo->setOpacity(0);
	this->addChild(logo);

	auto effect = FadeIn::create(1.5f);
	logo->runAction(effect);

	auto pScene = GameMain::createScene();
	Director::getInstance()->replaceScene(TransitionFade::create(0.5f, pScene));
	
	this->schedule(schedule_selector(HelloWorld::SceneTrans), 2.0f);

	return true;
}

void HelloWorld::SceneTrans(float dt)
{
	auto pScene = GameMain::createScene();
	Director::getInstance()->replaceScene(TransitionFade::create(0.5f, pScene));
}

