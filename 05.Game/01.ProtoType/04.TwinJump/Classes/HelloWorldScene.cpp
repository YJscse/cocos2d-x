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
	// ----------------------- 애니메이션 생성
	auto texture = Director::getInstance()->getTextureCache()->addImage("Images/player2.png");
	auto animation = Animation::create();
	animation->setDelayPerUnit(0.02f);
	
	for (int i = 0; i < 8; i++)
	{
		int column = i % 8;
		int row = i / 8;

		animation->addSpriteFrameWithTexture(
			texture,
			Rect(column * 30, row * 30, 30, 30));
	}

	pMan = Sprite::createWithTexture(texture, Rect(0, 0, 30, 30));
	pMan->setPosition(Vec2(100, 100));
	this->addChild(pMan);

	auto animate = Animate::create(animation);
	auto rep = RepeatForever::create(animate);
	pMan->setScale(1.5f);
	pMan->setPosition(Vec2(100, 100));
	pMan->runAction(rep);

	// -------------------------------------- 점프
	Layer::onEnter();

	auto listener = EventListenerTouchOneByOne::create();

	listener->setSwallowTouches(true);
	yValue = 0;
	num = 0;

	listener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(HelloWorld::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(HelloWorld::onTouchEnded, this);
	this->schedule(schedule_selector(HelloWorld::myTick), 1.0f / 60);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}

bool HelloWorld::onTouchBegan(Touch* touch, Event* event)
{
	if (yValue < 0)
	{
		log("떨어지는중임 yValue: %f", yValue);
		return false;
	}
	this->Jump();

	return true;
}

void HelloWorld::Jump()
{
	if (num < 2)
	{
		yValue = 10;
	}
	if (num >= 2)
	{
		log("3단 점프는 욕심");
	}
	num++;
	//log("num: %d", num);
}

void HelloWorld::myTick(float f)                // 스케쥴 계속 반복
{
	if (num == 0)
	{
		return;
	}

	pMan->setPosition(Vec2(pMan->getPosition().x, pMan->getPosition().y + yValue));           // 점프 코드         yValue는 최고점에서 0이 된다

	if (pMan->getPosition().y > 100)
	{
		yValue = yValue - 0.5f;        // 점프하고 내려올때 중력을 표현하기위한 코드
		//log("yValue: %f", yValue);
		if (yValue == 0)
		{
			log("3. %f", pMan->getPosition().y);
		
		}
	}
	else
	{
			yValue = 0;
			num = 0;
			pMan->setPosition(Vec2(pMan->getPosition().x, 100));        //점프하고 나서 계속해서 내려가면 안되니까 초기 위치값으로 위치하게 하는 코드
	}
	
}

void HelloWorld::onTouchMoved(Touch* touch, Event* event)
{

}


void HelloWorld::onTouchEnded(Touch* touch, Event* event)
{
	
}