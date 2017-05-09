#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

using namespace CocosDenshion;

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

		pos[i] = (Vec2(64 * column, 64 * row));
	}
	//------------------------------------------------------
	//                        랜덤
	srand((int)time(NULL));

	for (int i = 0; i < 200; i++)
	{
		int r1 = rand() % 20;
		int r2 = rand() % 20;

		if (r1 != r2)
		{
			auto pos1 = pos[r1];
			auto pos2 = pos[r2];

			pos[r1] = pos2;
			pos[r2] = pos1;
		}

	}	
	//                         랜덤
	//--------------------------------------------------------

	for (int i = 0; i < 20; i++)
	{
		boxCover[i] = Sprite::create("Images/box_normal.png");
		boxCover[i]->setAnchorPoint(Vec2(0, 0));
		boxCover[i]->setPosition(Vec2(pos[i]));
		boxCover[i]->setScale(0.8f);
		boxCover[i]->setVisible(false);
		this->addChild(boxCover[i]);
	}

	for (int i = 0; i < 20; i++)
	{
		cover[i] = Sprite::create("Images/box_normal.png");
		cover[i]->setAnchorPoint(Vec2(0, 0));
		cover[i]->setPosition(Vec2(pos[i]));
		cover[i]->setScale(0.8f);
		this->addChild(cover[i], 2);
	}
	
	for (int j = 0; j < 20; j++)
	{
		char str[50] = { 0 };
		sprintf(str, "Images/card%02d.png", j / 2 + 1);
		sprCard[j] = Sprite::create(str);
		sprCard[j]->setAnchorPoint(Vec2(0, 0));
		sprCard[j]->setPosition(Vec2(pos[j]));
		sprCard[j]->setScale(0.8f);
		//log("column : %d row : %d", column, row);
		this->addChild(sprCard[j]);

		//card1.pushBack(sprCard[j]);
	}

    return true;
}

void HelloWorld::onEnter()
{
	Layer::onEnter();

	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
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
	if (count == 2)
	{
		log("기다려");
		return true;
	}
	
	auto touchPoint = touch->getLocation();


	for (int i = 0; i < 20; i++)
	{
		bool bTouch = sprCard[i]->getBoundingBox().containsPoint(touchPoint);

		if (bTouch)
		{
			if (boxCover[i] == 0)
			{
				log("빈칸 누르지 마시게나");
				return true;
			}
			count++;

			SimpleAudioEngine::getInstance()->playEffect("Sounds/ding.wav");

			cover[i]->setVisible(false);

			if (count == 1)
			{
				pair1 = i / 2 + 1;
				num1 = i;
				log("pair1 : %d num1 : %d count : %d", pair1, num1, count);
			}
			if (count == 2)
			{
				pair2 = i / 2 + 1;
				num2 = i;
				log("pair2 : %d num2 : %d count : %d", pair2, num2, count);
			}

			if (num1 == num2)
			{
				count--;
				log("금지");
			}

			if (sprCard[i]->getNumberOfRunningActions() == 0)
			{
				auto move = RotateBy::create(0.1f, -5);
				auto back = move->reverse();
				auto seq = Sequence::create(move, back, nullptr);
				auto rep = Repeat::create(seq, 2);
				sprCard[i]->runAction(rep);
			}

			if (pair1 == pair2 && count == 2)
			{
				schedule(schedule_selector(HelloWorld::good), 1.0f);
				over = over + 2;

				return true;
			}
			if (pair1 != pair2 && count == 2)
			{
				schedule(schedule_selector(HelloWorld::bad), 1.0f);

				return true;

			}
		}
	}

	return true;
}

void HelloWorld::onTouchEnded(Touch *touch, Event *event)
{
	//log("좌표 x : %f  y : %f pair1 : %d pair2 : %d", touchPoint.x, touchPoint.y, pair1, pair2);
}

void HelloWorld::good(float f)
{
	auto pos1 = sprCard[num1]->getPosition();
	auto pos2 = sprCard[num2]->getPosition();

	sprCard[num1]->setVisible(false);
	sprCard[num2]->setVisible(false);

	cover[num1]->setVisible(false);
	cover[num2]->setVisible(false);

	boxCover[num1] = 0;
	boxCover[num2] = 0;

	ParticleSystem* emitter1 = ParticleSystemQuad::create("particle/Flower.plist");

	emitter1 = ParticleSystemQuad::create();
	emitter1->setPosition(pos1);
	emitter1->setScale(0.3f);
	emitter1->setDuration(1.5f);
	emitter1->setAutoRemoveOnFinish(true);

	this->addChild(emitter1);

	ParticleSystem* emitter2 = ParticleSystemQuad::create("particle/Flower.plist");

	emitter2 = ParticleSystemQuad::create();
	emitter2->setPosition(pos2);
	emitter2->setScale(0.3f);
	emitter2->setDuration(1.5f);
	emitter2->setAutoRemoveOnFinish(true);

	this->addChild(emitter2);

	if (over == 20)
	{
		SimpleAudioEngine::getInstance()->playEffect("Sounds/result.wav");

		unschedule(schedule_selector(HelloWorld::good));
	}

	SimpleAudioEngine::getInstance()->playEffect("Sounds/del_block.wav");


	count = 0;
	unschedule(schedule_selector(HelloWorld::good));
}

void HelloWorld::bad(float fa)
{
	cover[num1]->setVisible(true);
	cover[num2]->setVisible(true);


	count = 0;
	unschedule(schedule_selector(HelloWorld::bad));
}
