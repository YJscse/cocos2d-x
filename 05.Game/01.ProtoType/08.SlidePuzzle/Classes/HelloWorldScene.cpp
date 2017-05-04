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
	if (!Layer::init())
	{
		return false;
	}

	// 그림을 일정크기로 자른다.
	for (int i = 0; i < 9; i++)
	{
		int column = i % 3; // 0, 2, 1, 0 ...
		int row = i / 3;    // 0, 0, 0, 1, 1, 1, 2 .....

		auto sprite = Sprite::create("Images/class.png");
		sprite->setTextureRect(Rect(column * 106, row * 106, 105, 105));   //(start.x, start.y, x자르는양, y자르는양)

		Puzzle.pushBack(sprite);   // 차례대로 하나씩 Puzlle이라는 Vector에 넣어라.
	}

	Puzzle.at(0)->setPosition(Vec2(54, 270));  // Puzzle.at(0) ←Puzzle이라는 Vector에 있는 0번째 인수의 자리를 54, 270 에 배치
	ClearPos0 = Puzzle.at(0)->getPosition();   // ClearPos0 은 Puzzle이라는 Vector에 있는 0번째 인수의 자리를 가지고온다.

	Puzzle.at(1)->setPosition(Vec2(162, 270));
	ClearPos1 = Puzzle.at(1)->getPosition();

	Puzzle.at(2)->setPosition(Vec2(270, 270));
	ClearPos2 = Puzzle.at(2)->getPosition();

	Puzzle.at(3)->setPosition(Vec2(54, 162));
	ClearPos3 = Puzzle.at(3)->getPosition();

	Puzzle.at(4)->setPosition(Vec2(162, 162));
	ClearPos4 = Puzzle.at(4)->getPosition();

	Puzzle.at(5)->setPosition(Vec2(270, 162));
	ClearPos5 = Puzzle.at(5)->getPosition();

	Puzzle.at(6)->setPosition(Vec2(54, 54));
	ClearPos6 = Puzzle.at(6)->getPosition();

	Puzzle.at(7)->setPosition(Vec2(162, 54));
	ClearPos7 = Puzzle.at(7)->getPosition();

	Puzzle.at(8)->setPosition(Vec2(270, 54));
	Puzzle.at(8)->setVisible(false);     // 마지막 사진은 숨겨라 

	//-------------------------------------------------------
	//                     랜덤
	srand((int)time(NULL));

	for (int i = 0; i < 5; i++)
	{
		int r1 = rand() % 8;
		int r2 = rand() % 8;

		Vec2 rPos1 = Puzzle.at(r1)->getPosition();
		Vec2 rPos2 = Puzzle.at(r2)->getPosition();

		Puzzle.at(r1)->setPosition(rPos2);
		Puzzle.at(r2)->setPosition(rPos1);
	}

	for (int i = 0; i < 9; i++)
	{
		this->addChild(Puzzle.at(i));
	}

	//                     랜덤
	//-------------------------------------------------------

	return true;
}

void HelloWorld::onEnter()
{
	Layer::onEnter();

	auto listener = EventListenerTouchOneByOne::create();

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

	for (int i = 0; i < 9; i++)
	{
		bool bTouch = Puzzle.at(i)->getBoundingBox().containsPoint(touchPoint);

		if (bTouch)
		{
			Change(i);
		}
	}

	return true;
}

void HelloWorld::Change(int num)
{
	auto cPos1 = Puzzle.at(num)->getPosition();
	auto cPos2 = Puzzle.at(8)->getPosition();

	Vec2 cPosAdd = cPos1 - cPos2;

	if (cPosAdd.x == -108 && cPosAdd.y == 0)
	{
		Puzzle.at(num)->setPosition(cPos2);
		Puzzle.at(8)->setPosition(cPos1);
	}
	else if (cPosAdd.x == 108 && cPosAdd.y == 0)
	{
		Puzzle.at(num)->setPosition(cPos2);
		Puzzle.at(8)->setPosition(cPos1);
	}
	else if (cPosAdd.x == 0 && cPosAdd.y == -108)
	{
		Puzzle.at(num)->setPosition(cPos2);
		Puzzle.at(8)->setPosition(cPos1);
	}
	else if (cPosAdd.x == 0 && cPosAdd.y == 108)
	{
		Puzzle.at(num)->setPosition(cPos2);
		Puzzle.at(8)->setPosition(cPos1);
	}

	Clear();
}

void HelloWorld::Clear()
{
	auto Disappear = FadeOut::create(1);
	auto Appear = FadeIn::create(0.8);
	auto Seq = Sequence::create(DelayTime::create(1), Appear, nullptr);

	if (ClearPos0 == Puzzle.at(0)->getPosition() &&
		ClearPos1 == Puzzle.at(1)->getPosition() &&
		ClearPos2 == Puzzle.at(2)->getPosition() &&
		ClearPos3 == Puzzle.at(3)->getPosition() &&
		ClearPos4 == Puzzle.at(4)->getPosition() &&
		ClearPos5 == Puzzle.at(5)->getPosition() &&
		ClearPos6 == Puzzle.at(6)->getPosition() &&
		ClearPos7 == Puzzle.at(7)->getPosition())
	{
		for (int i = 0; i < 8; i++)
		{
			Puzzle.at(i)->runAction(Disappear->clone());
		}

		auto ClearSprite = Sprite::create("Images/class.png");
		ClearSprite->setAnchorPoint(Vec2(0, 0));
		ClearSprite->setPosition(Vec2(0, 0));
		ClearSprite->setOpacity(0);

		this->addChild(ClearSprite);
		ClearSprite->runAction(Seq);
	}

}