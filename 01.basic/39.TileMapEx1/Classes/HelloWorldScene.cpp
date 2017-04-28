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

	//////////////////////////////////

	// 타일맵 읽어 오기
	auto tmap = TMXTiledMap::create("TileMaps/TestDesert.tmx");
	this->addChild(tmap, 0, 11);         // z-order를 -1로 지정하면 화면이 나오지 않는다.

	Size s = tmap->getContentSize();
	log("ContentSize: %f, %f", s.width, s.height);

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

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);                 // 실제로 게임을 만들때는 onEnter에 넣지말고 init에 넣어라 onExit도 마찬가지!!!
}

void HelloWorld::onExit()
{
	_eventDispatcher->removeEventListenersForType(EventListener::Type::TOUCH_ONE_BY_ONE);

	Layer::onExit();
}

bool HelloWorld::onTouchBegan(Touch* touch, Event* event)
{
	auto touchPoint = touch->getLocation();

	return true;
}

void HelloWorld::onTouchMoved(Touch* touch, Event* event)
{
	Vec2 diff = touch->getDelta();

	Node* node = this->getChildByTag(11);
	Vec2 currentPos = node->getPosition();

	Vec2 newPos = currentPos + diff;

	if (newPos.x < -160)
	{
		newPos.x = -160;
	}
	if (newPos.x > 0)
	{
		newPos.x = 0;
	}
	newPos.y = 0;

	node->setPosition(newPos);
}