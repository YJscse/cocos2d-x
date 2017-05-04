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

    if ( !Layer::init())
    {
        return false;
    }

	for (int i = 0; i < 20; i++)
	{
		int column = i % 4;  
		int row = i / 4; 

		sprite1 = Sprite::create("Images/box_normal.png");
		sprite1->setAnchorPoint(Vec2(0, 0));
		sprite1->setPosition(Vec2(64 * column, 64 * row));
		sprite1->setScale(0.8f);
		
		log("column : %d row : %d", column, row);

		pair.pushBack(sprite1);
	}

	for (int i = 0; i < 20; i++)
	{
		this->addChild(pair.at(i), 2);
	}
	
	//this->createPair();

	pair1 = Sprite::create("Images/card01.png");
	pair2 = Sprite::create("Images/card02.png");
	pair3 = Sprite::create("Images/card03.png");
	pair4 = Sprite::create("Images/card04.png");
	pair5 = Sprite::create("Images/card05.png");
	pair6 = Sprite::create("Images/card06.png");
	pair7 = Sprite::create("Images/card07.png");
	pair8 = Sprite::create("Images/card08.png");
	pair9 = Sprite::create("Images/card09.png");
	pair10 = Sprite::create("Images/card10.png");
	pair11 = Sprite::create("Images/card01.png");
	pair12 = Sprite::create("Images/card02.png");
	pair13 = Sprite::create("Images/card03.png");
	pair14 = Sprite::create("Images/card04.png");
	pair15 = Sprite::create("Images/card05.png");
	pair16 = Sprite::create("Images/card06.png");
	pair17 = Sprite::create("Images/card07.png");
	pair18 = Sprite::create("Images/card08.png");
	pair19 = Sprite::create("Images/card09.png");
	pair20 = Sprite::create("Images/card10.png");

	//pair1->setTag(1);
	//pair2->setTag(2);
	//pair3->setTag(3);
	//pair4->setTag(4);
	//pair5->setTag(5);
	//pair6->setTag(6);
	//pair7->setTag(7);
	//pair8->setTag(8);
	//pair9->setTag(9);
	//pair10->setTag(10);
	//pair11->setTag(1);
	//pair12->setTag(2);
	//pair13->setTag(3);
	//pair14->setTag(4);
	//pair15->setTag(5);
	//pair16->setTag(6);
	//pair17->setTag(7);
	//pair18->setTag(8);
	//pair19->setTag(9);
	//pair20->setTag(10);

	card1.pushBack(pair1);
	card1.pushBack(pair2);
	card1.pushBack(pair3);
	card1.pushBack(pair4);
	card1.pushBack(pair5);
	card1.pushBack(pair6);
	card1.pushBack(pair7);
	card1.pushBack(pair8);
	card1.pushBack(pair9);
	card1.pushBack(pair10);
	card1.pushBack(pair11);
	card1.pushBack(pair12);
	card1.pushBack(pair13);
	card1.pushBack(pair14);
	card1.pushBack(pair15);
	card1.pushBack(pair16);
	card1.pushBack(pair17);
	card1.pushBack(pair18);
	card1.pushBack(pair19);
	card1.pushBack(pair20);

	Vector<Sprite*>::iterator it = card1.begin();

	for (int i = 0; it != card1.end(); ++it)
	{
		int column = i % 4;
		int row = i / 4;

		obj = (Sprite*)(*it);
		obj->setAnchorPoint(Vec2(0, 0));
		obj->setScale(0.8f);
		obj->setPosition(Vec2(64 * column, 64 * row));
		obj->setTag(i % 10);
		//this->addChild(obj);
		i++;
	}

	//------------------------------------------------------
	//                        랜덤
	srand((int)time(NULL));

	for (int i = 0; i < 5; i++)
	{
		int r1 = rand() % 20;
		int r2 = rand() % 20;

		Vec2 rPos1 = card1.at(r1)->getPosition();
		Vec2 rPos2 = card1.at(r2)->getPosition();

		card1.at(r1)->setPosition(rPos2);
		card1.at(r2)->setPosition(rPos1);
	}

	for (int i = 0; i < 20; i++)
	{
		this->addChild(card1.at(i));
	}
	//                         랜덤
	//--------------------------------------------------------
    return true;
}

//void HelloWorld::createPair()
//{
//
//
//}
void HelloWorld::onEnter()
{
	Layer::onEnter();

	auto listener = EventListenerTouchOneByOne::create();
	//listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
	listener->onTouchEnded = CC_CALLBACK_2(HelloWorld::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}


void HelloWorld::onExit()
{
	Layer::onExit();
}

bool HelloWorld::onTouchBegan(Touch *touch, Event *event)
{
	int Pair = 
	if (count == 2)
	{
		count = 0;
		if (getTag(1))
		{

		}
	}
	return true;
}

void HelloWorld::onTouchEnded(Touch *touch, Event *event)
{
	touchPoint = touch->getLocation();
	//touchPoint = this->convertToNodeSpace(touchPoint);

	for (int i = 0; i < 20; i++)
	{
		bool bTouch = pair.at(i)->getBoundingBox().containsPoint(touchPoint);

		if (bTouch)
		{
			Discover(i);
		}
	}
	log("선택  x : %f  y : %f", touchPoint.x, touchPoint.y);
}

// 카드 뒤집기
void HelloWorld::Discover(int num)
{
	// 1번째 줄
	if (touchPoint.x < 64 && touchPoint.y < 64)
	{
		pair.at(0)->setVisible(false);
		count++;
	}
	else if (touchPoint.x > 64 && touchPoint.x < 128 && touchPoint.y < 64)
	{
		pair.at(1)->setVisible(false);
		count++;
	}
	else if (touchPoint.x > 128 && touchPoint.x < 192 && touchPoint.y < 64)
	{
		pair.at(2)->setVisible(false);
		count++;
	}
	else if (touchPoint.x > 192 && touchPoint.x < 256 && touchPoint.y < 64)
	{
		pair.at(3)->setVisible(false);
		count++;
	}

	// 2번째 줄
	else if (touchPoint.x < 64 && touchPoint.y > 64 && touchPoint.y < 128)
	{
		pair.at(4)->setVisible(false);
		count++;
	}
	else if (touchPoint.x > 64 && touchPoint.x < 128 && touchPoint.y > 64 && touchPoint.y < 128)
	{
		pair.at(5)->setVisible(false);
		count++;
	}
	else if (touchPoint.x > 128 && touchPoint.x < 192 && touchPoint.y > 64 && touchPoint.y < 128)
	{
		pair.at(6)->setVisible(false);
		count++;
	}
	else if (touchPoint.x > 192 && touchPoint.x < 256 && touchPoint.y > 64 && touchPoint.y < 128)
	{
		pair.at(7)->setVisible(false);
		count++;
	}

	// 3번째 줄
	else if (touchPoint.x < 64 && touchPoint.y > 128 && touchPoint.y < 192)
	{
		pair.at(8)->setVisible(false);
		count++;
	}
	else if (touchPoint.x > 64 && touchPoint.x < 128 && touchPoint.y > 128 && touchPoint.y < 192)
	{
		pair.at(9)->setVisible(false);
		count++;
	}
	else if (touchPoint.x > 128 && touchPoint.x < 192 && touchPoint.y > 128 && touchPoint.y < 192)
	{
		pair.at(10)->setVisible(false);
		count++;
	}
	else if (touchPoint.x > 192 && touchPoint.x < 256 && touchPoint.y > 128 && touchPoint.y < 192)
	{
		pair.at(11)->setVisible(false);
		count++;
	}

	// 4번째 줄
	else if (touchPoint.x < 64 && touchPoint.y > 192 && touchPoint.y < 256)
	{
		pair.at(12)->setVisible(false);
		count++;
	}
	else if (touchPoint.x > 64 && touchPoint.x < 128 && touchPoint.y > 192 && touchPoint.y < 256)
	{
		pair.at(13)->setVisible(false);
		count++;
	}
	else if (touchPoint.x > 128 && touchPoint.x < 192 && touchPoint.y > 192 && touchPoint.y < 256)
	{
		pair.at(14)->setVisible(false);
		count++;
	}
	else if (touchPoint.x > 192 && touchPoint.x < 256 && touchPoint.y > 192 && touchPoint.y < 256)
	{
		pair.at(15)->setVisible(false);
		count++;
	}

	//5번째 줄
	else if (touchPoint.x < 64 && touchPoint.y > 256 && touchPoint.y < 320)
	{
		pair.at(16)->setVisible(false);
		count++;
	}
	else if (touchPoint.x > 64 && touchPoint.x < 128 && touchPoint.y > 256 && touchPoint.y < 320)
	{
		pair.at(17)->setVisible(false);
		count++;
	}
	else if (touchPoint.x > 128 && touchPoint.x < 192 && touchPoint.y > 256 && touchPoint.y < 320)
	{
		pair.at(18)->setVisible(false);
		count++;
	}
	else if (touchPoint.x > 192 && touchPoint.x < 256 && touchPoint.y > 256 && touchPoint.y < 320)
	{
		pair.at(19)->setVisible(false);
		count++;
	}

	
}
