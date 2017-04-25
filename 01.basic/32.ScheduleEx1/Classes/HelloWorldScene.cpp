#include "HelloWorldScene.h"


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

    if ( !LayerColor::initWithColor(Color4B(255, 255,255,255) ))
    {
        return false;
    }

	nNum = 0;

	this->schedule(schedule_selector(HelloWorld::callEveryFrame));
	this->schedule(schedule_selector(HelloWorld::myTick), 1.0f);
	this->scheduleOnce(schedule_selector(HelloWorld::myTickOnce), 3.0f);

    return true;
}

void HelloWorld::callEveryFrame(float f)
{
	nNum++;
	if (nNum > 60)
	{
		nNum = 1;
	}

	log("fps..%d", nNum);
}

void HelloWorld::myTick(float f)
{
	log("tick");
}

void HelloWorld::myTickOnce(float f)
{
	log("tickOnce after delay");
}