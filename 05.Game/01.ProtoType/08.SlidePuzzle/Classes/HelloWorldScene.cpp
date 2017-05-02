#include "HelloWorldScene.h"
#include "ctime"
#include "cstdio"
#include "cstdlib"

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

    if ( !Layer::init())
    {
        return false;
    }


	
	this->createpuzzle();

	srand((int)time(NULL));
    return true;
}

void HelloWorld::createpuzzle()
{
	
	Texture2D* texture = Director::getInstance()->getTextureCache()->addImage("Imges/class.png");
	
	for (int i = 0; i < 9; i++)
	{
		int column = i % 3;
		int row = i / 3;

		auto sprite = Sprite::create("Images/class.png");
		sprite->setAnchorPoint(Vec2(0, 1));
		sprite->setTextureRect(Rect(column * 105, row * 105, 100, 100));

		puzzle.pushBack(sprite);

	}

	puzzle.at(0)->setPosition(Vec2(0, 320));
	puzzle.at(1)->setPosition(Vec2(105, 320));
	puzzle.at(2)->setPosition(Vec2(210, 320));
	puzzle.at(3)->setPosition(Vec2(0, 215));
	puzzle.at(4)->setPosition(Vec2(105, 215));
	puzzle.at(5)->setPosition(Vec2(210, 215));
	puzzle.at(6)->setPosition(Vec2(0, 110));
	puzzle.at(7)->setPosition(Vec2(105, 110));
	puzzle.at(8)->setPosition(Vec2(210, 110));

	for (int i = 0; i < 5; i++)
	{
		int r1 = rand() % 8;
		int r2 = rand() % 8;

		Vec2 rPos1 = puzzle.at(r1)->getPosition();
		Vec2 rPos2 = puzzle.at(r2)->getPosition();

		puzzle.at(r1)->setPosition(rPos2);
		puzzle.at(r2)->setPosition(rPos1);
	}

	for (int i = 0; i < 8; i++)
	{
		puzzle.at(i);
	}
}


void HelloWorld::onEnter()
{
	Layer::onEnter();

	auto listener = EventListenerTouchOneByOne::create();

	listener->setSwallowTouches(true);

	listener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void HelloWorld::onExit()
{
	Layer::onExit();
}

bool HelloWorld::onTouchBegan(Touch *touch, Event *event)
{
	auto touchPoint = touch->getLocation();

	for (int i = 0; i < 8; i++)
	{
		bool bTouch = puzzle.at(i)->getBoundingBox().containsPoint(touchPoint);

		if (bTouch)
		{
			log("%d 번째 그림 클릭", i);
		}
	}
	return true;
}







