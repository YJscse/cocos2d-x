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

	///////////////////////////////////////////////////

	// MotionStreak를 만들
	// (float fade, float minSeg, float stroke, Color3B% color, const std::string& path)
	// (float fade, float minSeg, float stroke, Color3B% color, Texture2D* texture);
	m_pStreak = MotionStreak::create(1.0f, 1.0f, 10.0f, Color3B::GREEN, "streak.png");
	this->addChild(m_pStreak);

    return true;
}

void HelloWorld::onEnter()
{
	Layer::onEnter();

	// 싱글 터치 모드로 터치 리스너 등록
	auto listener = EventListenerTouchOneByOne::create();
	// Swallow touches only available in OneByOne mode.
	// 핸들링된 터치 이벤트 array에서 지우겠다는 의미이다.
	//listener->setSwallowTouches(true);

	listener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(HelloWorld::onTouchMoved, this);

	// The priority of the touch listener is based on the draw order of sprite
	// 터치 리스너의 우선순위를 (노드가) 화면에 그려진 순서대로 한다.
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);                 // 실제로 게임을 만들때는 onEnter에 넣지말고 init에 넣어라 onExit도 마찬가지!!!
}

void HelloWorld::onExit()
{
	//_eventDispatcher->removeEventListenersForType(EventListener::Type::TOUCH_ONE_BY_ONE);

	Layer::onExit();
}

bool HelloWorld::onTouchBegan(Touch* touch, Event* event)
{
	m_pStreak->reset();

	auto touchPoint = touch->getLocation();

	m_pStreak->setPosition(touchPoint);

	return true;
}

void HelloWorld::onTouchMoved(Touch* touch, Event* event)
{
	auto touchPoint = touch->getLocation();

	m_pStreak->setPosition(touchPoint);
}