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
	/////////////////////////////////////

	map = TMXTiledMap::create("TileMaps/iso-test-touch.tmx");
	this->addChild(map, 0, 1);

	Size s = map->getContentSize();
	log("ContentSize: %f, %f", s.width, s.height);
	map->setPosition(Vec2(0, 0));

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

bool HelloWorld::onTouchBegan(Touch* touch, Event* event)
{
	auto touchPoint = touch->getLocation();
	touchPoint = map->convertToNodeSpace(touchPoint);

	Vec2 pos = touchToXY(touchPoint);

	log("gid.x = %f, gid.y = %f", pos.x, pos.y);

	return true;
}

Vec2 HelloWorld::touchToXY(Vec2 touch)
{
	Size maps = map->getContentSize();
	Size ts = map->getTileSize();
	int mapsize = maps.width / ts.width;

	int isoy = (
				(
					(touch.y / ts.height) + 
					(touch.x - (mapsize * ts.width / 2)) / ts.width
					) - mapsize
				) * -1;
	int isox = (
				(
					(touch.y / ts.height) - 
					(touch.x - (mapsize * ts.width / 2)) / ts.width
					) - mapsize
				) * -1;

	return Vec2(isox, isoy);

}