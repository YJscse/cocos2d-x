#include "HelloWorldScene.h"
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

    if ( !LayerColor::initWithColor(Color4B(255, 255,255,255) ))
    {
        return false;
    }

	/////////////////////////////////////////////

	// 화면 사이즈 구하기
	winSize = Director::getInstance()->getWinSize();

	// 타일맵 읽어 오기
	tmap = TMXTiledMap::create("TileMaps/TestDesert.tmx");
	background = tmap->getLayer("Background");
	items = tmap->getLayer("Items");
	metainfo = tmap->getLayer("MetaInfo");
	metainfo->setVisible(false);
	this->addChild(tmap, 0, 11); // z-order를 -1로 하면 화면이 나오지 않는다.

    return true;
}

void HelloWorld::onEnter()
{
	Layer::onEnter();

	// 싱글 터치 모드로 터치리스너 등록
	auto listener = EventListenerTouchOneByOne::create();

	listener->setSwallowTouches(true);

	listener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
	listener->onTouchEnded = CC_CALLBACK_2(HelloWorld::onTouchEnded, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void HelloWorld::onExit()
{
	_eventDispatcher->removeEventListenersForType(EventListener::Type::TOUCH_ONE_BY_ONE);

	Layer::onExit();
}



bool HelloWorld::onTouchBegan(Touch* touch, Event* event)
{

	return true;
}

void HelloWorld::onTouchEnded(Touch* touch, Event* event)
{

	auto touchPoint = touch->getLocation();
	touchPoint = this->convertToNodeSpace(touchPoint);

	this->setPlayerPosition(touchPoint);    // 내가 터치하는것을 불러온다.
}

void HelloWorld::setViewpointCenter(Vec2 position)
{
	// 파라미터로 들어오는 위치에 맞춰 화면을 움직인다.
	int x = MAX(position.x, winSize.width / 2);
	int y = MAX(position.y, winSize.height / 2);

	x = MIN(x, (tmap->getMapSize().width * tmap->getTileSize().width) - winSize.width / 2);
	y = MIN(y, (tmap->getMapSize().height * tmap->getTileSize().height) - winSize.height / 2);

	Vec2 actualPosition = Vec2(x, y);
	Vec2 centerOfView = Vec2(winSize.width / 2, winSize.height / 2);
	Vec2 viewPoint = centerOfView - actualPosition;

	this->setPosition(viewPoint);
}

// 현재 탭으로 선택된 타일의 위치를 가져온다.
Vec2 HelloWorld::tileCoordForPosition(Vec2 position)
{
	int x = position.x / tmap->getTileSize().width;
	int y = ((tmap->getMapSize().height * tmap->getTileSize().height) - position.y) / tmap->getTileSize().height;

	return Vec2(x, y);
}

void HelloWorld::setPlayerPosition(Vec2 position)
{
	// 탭된 위치 구하기
	Vec2 tileCoord = this->tileCoordForPosition(position);

	// 현재 위치의 Tile GID 구하기
	int tileGid = this->metainfo->getTileGIDAt(tileCoord);

	if (tileGid)
	{
		// 타일맵에서 GID에 해당하는 부분의 속성 읽어 오기
		Value properties = tmap->getPropertiesForGID(tileGid);

		if (!properties.isNull())
		{
			std::string item1 = properties.asValueMap()["Items"].asString();
			if (item1 == "apple")
			{

				//GID 이동
				int nVecX = rand() % 10;
				int nVecY = rand() % 10;

				int tileGid2 = this->items->getTileGIDAt(tileCoord);
				int tileGid3 = this->metainfo->getTileGIDAt(tileCoord);

				items->setTileGID(tileGid2, Vec2(nVecX, nVecY));
				metainfo->setTileGID(tileGid3, Vec2(nVecX, nVecY));

				log("apple 아이템 획득");

				this->metainfo->removeTileAt(tileCoord);
				this->items->removeTileAt(tileCoord);
			}

			if (item1 == "banana")
			{
			//	this->metainfo->removeTileAt(tileCoord);
			//	items->removeTileAt(tileCoord);

				// 먹은 수만큼 점수를 올려주는 코드가 추가적으로 필요하다.
				log("banana 아이템 터치");
			}
		}
	}

	// 파라미터로 들어온 위치에 드래곤 위치 조정하기
}