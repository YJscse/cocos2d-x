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

	// 싱글 터치 모드로 터치 리스너 등록
	auto listener = EventListenerTouchOneByOne::create();
	// Swallow touches only available in OneByOne mode.
	// 핸들링된 터치 이벤트 array에서 지우겠다는 의미이다.
	listener->setSwallowTouches(true);

	listener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(HelloWorld::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(HelloWorld::onTouchEnded, this);

	// The priority of the touch listener is based on the draw order of sprite
	// 터치 리스너의 우선순위를 (노드가) 화면에 그려진 순서대로 한다.
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);                 // 실제로 게임을 만들때는 onEnter에 넣지말고 init에 넣어라 onExit도 마찬가지!!!
}

void HelloWorld::onExit()
{
	_eventDispatcher->removeEventListenersForType(EventListener::Type::TOUCH_ONE_BY_ONE);

	Layer::onExit();
}

// 처음 손가락이 화면에 닿는 순간 호출된다.
bool HelloWorld::onTouchBegan(Touch* touch, Event* event)
{
	auto touchPoint = touch->getLocation();

	log("onTouchBeagan id = %d, x = %f, y = %f",
		touch->getID(),
		touchPoint.x,
		touchPoint.y);

	// touch check-------------------------------------------------------
	bool bTouch = pMan->getBoundingBox().containsPoint(touchPoint);

	if (bTouch)
	{
		log("Sprite clicked...");
	}

	return true;
}

// 손가락을 화면에서 뗴지 않고 이리저리 움질일 때 계속해서 호출된다.
// 얼마나 자주 호출되느냐는 전적으로
// 이벤트를 핸들링하는 애플리케이션의 Run Loop에 달렸다.
void HelloWorld::onTouchMoved(Touch* touch, Event* event)
{
	auto touchPoint = touch->getLocation();

	log("onTouchMoved id = %d, x = %f, y = %f",
		touch->getID(),
		touchPoint.x,
		touchPoint.y);
}

//손가락을 화면에서 떼는 순간 호출된다.
void HelloWorld::onTouchEnded(Touch* touch, Event* event)
{
	auto touchPoint = touch->getLocation();

	log("onTouchEnded id = %d, x = %f, y = %f",
		touch->getID(),
		touchPoint.x,
		touchPoint.y);
}

// 시스템이 터치를 중지시키는 경우에 호출된다. ex) 전화가 오는 경우
void HelloWorld::onTouchCancelled(Touch* touch, Event* event)
{
}