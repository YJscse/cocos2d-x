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
	
	auto pLabel1 = LabelTTF::create("GEARS", "Arial", 100);

	pLabel1->setPosition(Vec2(360, 700));

	pLabel1->setColor(Color3B::GREEN);

	auto action = FadeOut::create(0.5f);
	auto seq = Sequence::create(DelayTime::create(1.0f), action, nullptr);

	this->addChild(pLabel1);
	pLabel1->runAction(seq);

	this->schedule(schedule_selector(HelloWorld::SceneTrans), 1.0f);
	
	return true;
}

void HelloWorld::SceneTrans(float dt)
{
	auto pScene = GameMain::createScene();
	Director::getInstance()->replaceScene(TransitionFade::create(0.5f, pScene));
}
