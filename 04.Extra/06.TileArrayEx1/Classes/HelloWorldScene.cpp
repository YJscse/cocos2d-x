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

	// 타일맵에서 objects라고 지정한 오브젝트 레이어의 객체들 가져오기
	TMXObjectGroup* objects = tmap->getObjectGroup("Objects");

	// 오브젝트 레이어에서 SpawnPoint라고 지정한 속성값 읽어오기
	ValueMap spawnPoint = objects->getObject("SpawnPoint");

	int x = spawnPoint["x"].asInt();
	int y = spawnPoint["y"].asInt();

	// SpawnPoint의 위치로 생성할 드래곤의 위치 지정하기
	dragonPosition = Vec2(x, y);

	// 드래곤을 만든다.
	this->createDragon();

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

void HelloWorld::createDragon()
{
	// 움직이는 드래곤 넣기 시작
	auto texture = Director::getInstance()->getTextureCache()->addImage("Images/dragon_animation.png");

	auto animation = Animation::create();
	animation->setDelayPerUnit(0.1f);

	for (int i = 0; i < 6; i++)
	{
		// 첫째 줄에 4개의 프레임이 있고, 5번째부터는 두 번째 줄에 있으므로
		// 5번째(idx==4)부터는 y 좌표의 값을 증가시켜야 한다.
		int index = i % 4;
		int rowIndex = i / 4;

		animation->addSpriteFrameWithTexture(texture, Rect(index * 130, rowIndex * 140, 130, 140));
	}

	// 스프라이트 생성 및 초기화
	dragon = Sprite::createWithTexture(texture, Rect(0, 0, 130, 140));
	dragon->setPosition(dragonPosition);
	this->addChild(dragon);

	// 드래곤의 방향을 바꿔준다.
	dragon->setFlippedX(true);

	// 드래곤의 크기를 줄여준다. 맵에 비해 너무 크다.
	dragon->setScale(0.5);

	// 드래곤 애니메이션
	auto animate = Animate::create(animation);
	auto rep = RepeatForever::create(animate);
	dragon->runAction(rep);
}

bool HelloWorld::onTouchBegan(Touch* touch, Event* event)
{
	return true;
}

void HelloWorld::onTouchEnded(Touch* touch, Event* event)
{
	auto touchPoint = touch->getLocation();
	touchPoint = this->convertToNodeSpace(touchPoint);

	Vec2 playerPos = dragon->getPosition();

	Vec2 diff = touchPoint - playerPos;

	if (abs(diff.x) > abs(diff.y))
	{
		if (diff.x > 0)
		{
			playerPos.x = playerPos.x + tmap->getTileSize().width;

			// 드래곤의 방향을 바꿔준다.
			dragon->setFlippedX(true);
		}
		else
		{
			playerPos.x = playerPos.x - tmap->getTileSize().width;

			// 드래곤의 방향을 바꿔준다.
			dragon->setFlippedX(false);
		}
	}
	else
	{
		if (diff.y > 0)
		{
			playerPos.y = playerPos.y + tmap->getTileSize().height;
		}
		else
		{
			playerPos.y = playerPos.y - tmap->getTileSize().height;
		}
	}

	if (playerPos.x <= (tmap->getMapSize().width * tmap->getTileSize().width) &&
		playerPos.y <= (tmap->getMapSize().height * tmap->getTileSize().height) &&
		playerPos.y >= 0 &&
		playerPos.x >= 0)
	{
		// 드래곤의 새로운 위치 지정
		//dragon->setPosition(playerPos);
		this->setPlayerPosition(playerPos);
	}

	// 드래곤의 위치에 맞춰 화면 위치 조정
	this->setViewpointCenter(dragon->getPosition());

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
			//std::string wall = properties.asValueMap()["Wall"].asString();
			//if (wall == "YES")
			//{
			//	log("Wall...");
			//	return;
			//}
			std::string item1 = properties.asValueMap()["Items"].asString();
			if (item1 == "apple")
			{
				this->metainfo->removeTileAt(tileCoord);
				items->removeTileAt(tileCoord);

				// 먹은 수만큼 점수를 올려주는 코드가 추가적으로 필요하다.
				log("aplle 아이템 획득!!!");
			}
			if (item1 == "banana")
			{
				this->metainfo->removeTileAt(tileCoord);
				items->removeTileAt(tileCoord);

				// 먹은 수만큼 점수를 올려주는 코드가 추가적으로 필요하다.
				log("banana 아이템 획득!!! ");
			}
		}
	}

	// 파라미터로 들어온 위치에 드래곤 위치 조정하기
	dragon->setPosition(position);
}