#include "HelloWorldScene.h"
#include "GameMain.h"
#include "soundManager.h"

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
	logo->setScale(2.0f);
	logo->setOpacity(0);
	this->addChild(logo);

	auto effect = FadeIn::create(1.5f);
	logo->runAction(effect);

//	auto pScene = GameMain::createScene();
//	Director::getInstance()->replaceScene(TransitionFade::create(0.5f, pScene));
	
	this->schedule(schedule_selector(HelloWorld::SceneTrans), 2.0f);

	UserDefault::getInstance()->setBoolForKey("bgm", true);
	UserDefault::getInstance()->setBoolForKey("effect", true);
	//UserDefault::getInstance()->setIntegerForKey("stage", 1);

	CSoundManager::getInstance()->preloadAudioFile("sounds/Game_Play_BGM.ogg");
	CSoundManager::getInstance()->preloadAudioFile("sounds/Coin1.ogg");
	CSoundManager::getInstance()->preloadAudioFile("sounds/Game_Over.ogg");
	CSoundManager::getInstance()->preloadAudioFile("sounds/dead.ogg");
	CSoundManager::getInstance()->preloadAudioFile("sounds/Jump.ogg");
	CSoundManager::getInstance()->preloadAudioFile("sounds/Item.ogg");
	CSoundManager::getInstance()->preloadAudioFile("sounds/Shield.ogg");
	CSoundManager::getInstance()->preloadAudioFile("sounds/Stage_Clear.ogg");

	return true;
}

void HelloWorld::SceneTrans(float dt)
{
	auto pScene = GameMain::createScene();
	Director::getInstance()->replaceScene(TransitionFade::create(0.5f, pScene));
}


