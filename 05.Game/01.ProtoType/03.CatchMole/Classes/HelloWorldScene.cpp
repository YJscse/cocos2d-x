#include "HelloWorldScene.h"
#include "cstdio"
#include "cstdlib"
#include "ctime"

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

    if ( !Layer::init() )
    {
        return false;
    }

	srand((int)time(NULL));

	auto BackgroundUp = Sprite::create("Images/grass_upper.png");
	auto BackgroundDown = Sprite::create("Images/grass_lower.png");
	Digda1 = Sprite::create("Images/mole_1.png");
	Digda2 = Sprite::create("Images/mole_1.png");
	Digda3 = Sprite::create("Images/mole_1.png");

	BackgroundUp->setPosition(Vec2(240, 240));
	BackgroundDown->setPosition(Vec2(240, 80));
	Digda1->setPosition(Vec2(240, 100));
	Digda2->setPosition(Vec2(95, 100));
	Digda3->setPosition(Vec2(385, 100));

	this->addChild(BackgroundUp);
	this->addChild(BackgroundDown, 2);
	this->addChild(Digda1);
	this->addChild(Digda2);
	this->addChild(Digda3);
	this->tick(this);
	//this->ActionRepeatForever(this);


    return true;
}

void HelloWorld::tick(Ref* pSender)
{
	this->schedule(schedule_selector(HelloWorld::Digda1Action), 1.0f);
	this->schedule(schedule_selector(HelloWorld::Digda2Action), 1.0f);
	this->schedule(schedule_selector(HelloWorld::Digda3Action), 1.0f);

}

void HelloWorld::Digda1Action(float f) 
{
	auto DActionUp1 = MoveBy::create(0.5, Vec2(0, 80));
	auto DActionDown1 = DActionUp1->reverse();
	auto DAction1 = Sequence::create(DActionUp1, DelayTime::create(0.5), DActionDown1, nullptr);
	auto DAction11 = Repeat::create(DAction1, 1);

	dig1 = rand() % 2;

	if(dig1 != 1)
	{
		Digda1->runAction(DAction11);
	}
}

void HelloWorld::Digda2Action(float f)
{
	auto DActionUp2 = MoveBy::create(0.5, Vec2(0, 80));
	auto DActionDown2 = DActionUp2->reverse();
	auto DAction2 = Sequence::create(DActionUp2, DelayTime::create(0.5), DActionDown2, nullptr);
	auto DAction22 = Repeat::create(DAction2, 1);

	dig2 = rand() % 2;

	if (dig2 != 1)
	{
		Digda2->runAction(DAction22);
	}
}

void HelloWorld::Digda3Action(float f)
{
	auto DActionUp3 = MoveBy::create(0.5, Vec2(0, 80));
	auto DActionDown3 = DActionUp3->reverse();
	auto DAction3 = Sequence::create(DActionUp3, DelayTime::create(0.5), DActionDown3, nullptr);
	auto DAction33 = Repeat::create(DAction3, 1);

	dig3 = rand() % 3;

	if (dig1 != 0 || dig2 != 0)
	{
		Digda3->runAction(DAction33);
	}
}