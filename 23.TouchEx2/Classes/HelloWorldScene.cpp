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
	pMan = Sprite::create("Images/grossini.png");
	pMan->setPosition(Vec2(240, 160));
	this->addChild(pMan);

	return true;
}

void HelloWorld::onEnter()
{
	Layer::onEnter();

	// 멀티 터치 모드로 터치 리스너 등록
	auto listener = EventListenerTouchAllAtOnce::create();

	listener->onTouchesBegan = CC_CALLBACK_2(HelloWorld::onTouchesBegan, this);
	listener->onTouchesMoved = CC_CALLBACK_2(HelloWorld::onTouchesMoved, this);
	listener->onTouchesEnded = CC_CALLBACK_2(HelloWorld::onTouchesEnded, this);

	// The priority of the touch listener is based on the draw order of sprite
	// 터치 리스너의 우선순위를 (노드가) 화면에 그려진 순서대로 한다.
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);                 // 실제로 게임을 만들때는 onEnter에 넣지말고 init에 넣어라 onExit도 마찬가지!!!
}

void HelloWorld::onExit()
{
	_eventDispatcher->removeEventListenersForType(EventListener::Type::TOUCH_ALL_AT_ONCE);

	Layer::onExit();
}

// 처음 손가락이 화면에 닿는 순간 호출된다.
void HelloWorld::onTouchesBegan(const std::vector<Touch*>& touches, Event *event)
{
	auto touch = touches[0];
	auto touchPoint = touch->getLocation();
	touchPoint = this->convertToWorldSpace(touchPoint);

	log("onTouchesBeagan id = %d, x = %f, y = %f",
		touch->getID(),
		touchPoint.x,
		touchPoint.y);

	// touch check-------------------------------------------------------
	bool bTouch = pMan->getBoundingBox().containsPoint(touchPoint);

	if (bTouch)
	{
		log("Sprite clicked...");
	}

	/*for (auto &item : touches)
	{
		auto touch = item;
		auto touchPoint = touch->getLocation();

		log("onTouchesBegan id = %d, x = %f, y = %f",
			touch->getID(),
			touchPoint.x,
			touchPoint.y)
	}*/
	
}

// 손가락을 화면에서 뗴지 않고 이리저리 움질일 때 계속해서 호출된다.
// 얼마나 자주 호출되느냐는 전적으로
// 이벤트를 핸들링하는 애플리케이션의 Run Loop에 달렸다.
void HelloWorld::onTouchesMoved(const std::vector<Touch*>& touches, Event* event)
{
	auto touch = touches[0];
	auto touchPoint = touch->getLocation();
	touchPoint = this->convertToWorldSpace(touchPoint);

	log("onTouchesMoved id = %d, x = %f, y = %f",
		touch->getID(),
		touchPoint.x,
		touchPoint.y);

	/*for (auto &item : touches)
	{
	auto touch = item;
	auto touchPoint = touch->getLocation();

	log("onTouchesBegan id = %d, x = %f, y = %f",
	touch->getID(),
	touchPoint.x,
	touchPoint.y)
	}*/
}

//손가락을 화면에서 떼는 순간 호출된다.
void HelloWorld::onTouchesEnded(const std::vector<Touch*>& touches, Event* event)
{
	auto touch = touches[0];
	auto touchPoint = touch->getLocation();
	touchPoint = this->convertToWorldSpace(touchPoint);

	log("onTouchesEnded id = %d, x = %f, y = %f",
		touch->getID(),
		touchPoint.x,
		touchPoint.y);

	/*for (auto &item : touches)
	{
	auto touch = item;
	auto touchPoint = touch->getLocation();

	log("onTouchesBegan id = %d, x = %f, y = %f",
	touch->getID(),
	touchPoint.x,
	touchPoint.y)
	}*/
}

// 시스템이 터치를 중지시키는 경우에 호출된다. ex) 전화가 오는 경우
void HelloWorld::onTouchesCancelled(const std::vector<Touch*>& touches, Event* event)
{
	onTouchesEnded(touches, event);
}