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

	winSize = Director::getInstance()->getWinSize();

	// 타일맵 읽어 오기
	tmap = TMXTiledMap::create("Images/Tutorial.tmx");
	background = tmap->getLayer("Background1");
	metainfo = tmap->getLayer("MetaInfo");
	metainfo->setVisible(false);
	this->addChild(tmap, 0, 11); 

	cache = SpriteFrameCache::getInstance();
	cache->addSpriteFramesWithFile("Images/Holygrail.plist");

	// 타일맵에서 objects라고 지정한 오브젝트 레이어의 객체들 가져오기
	TMXObjectGroup* objects = tmap->getObjectGroup("Objects");

	// 오브젝트 레이어에서 Hero라고 지정한 속성값 읽어오기
	ValueMap spawnPoint = objects->getObject("Hero");
	ValueMap spawnPoint1 = objects->getObject("Sword");
	ValueMap spawnPoint2 = objects->getObject("Wand");
	ValueMap spawnPoint3 = objects->getObject("HolySword");
	ValueMap spawnPoint4 = objects->getObject("Key");
	ValueMap spawnPoint5 = objects->getObject("Wolf");

	int x = spawnPoint["x"].asInt();    // Hero
	int y = spawnPoint["y"].asInt();

	int x1 = spawnPoint1["x"].asInt();  // Sword
	int y1 = spawnPoint1["y"].asInt();

	int x2 = spawnPoint2["x"].asInt();  // Wand
	int y2 = spawnPoint2["y"].asInt();

	int x3 = spawnPoint3["x"].asInt();  // HolySword
	int y3 = spawnPoint3["y"].asInt();

	int x4 = spawnPoint4["x"].asInt();  // Key
	int y4 = spawnPoint4["y"].asInt();

	int x5 = spawnPoint5["x"].asInt();  // Wolf
	int y5 = spawnPoint5["y"].asInt();

	// SpawnPoint의 위치로 생성할 히어로, 무기, 몬스터들 위치 지정하기
	HeroPosition = Vec2(x, y);
	SwordPosition = Vec2(x1, y1);
	WandPosition = Vec2(x2, y2);
	HolySwordPosition = Vec2(x3, y3);
	KeyPosition = Vec2(x4, y4);
	WolfPosition = Vec2(x5, y5);

	// 히어로, 무기, 몬스터를 만든다.
	this->createHero();
	this->createWeapons();
	this->createWolf();

	


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
	//_eventDispatcher->removeEventListenersForType(EventListener::Type::TOUCH_ONE_BY_ONE);

	Layer::onExit();
}

void HelloWorld::createHero()
{
	Hero = Sprite::createWithSpriteFrameName("Hero02.png"); //정면 서있는모습
	Hero->setPosition(HeroPosition);
	Hero->setScale(0.8f);
	this->addChild(Hero);

}

void HelloWorld::createWolf()
{
	Wolf = Sprite::createWithSpriteFrameName("Wolf1.png");

	char str1[50] = { 0 };

	for (int i = 1; i < 4; i++)
	{
		sprintf(str1, "Wolf%d.png", i);
		SpriteFrame* wframe = cache->getSpriteFrameByName(str1);
		MonsterFrames.pushBack(wframe);
	}

	auto wolfAnimation = Animation::createWithSpriteFrames(MonsterFrames, 0.5f);
	auto wolfAnimate = Animate::create(wolfAnimation);
	auto rep1 = RepeatForever::create(wolfAnimate);
	Wolf->setPosition(WolfPosition);
	this->addChild(Wolf);
	Wolf->runAction(rep1);

}

void HelloWorld::createWeapons()
{
	Sword = Sprite::createWithSpriteFrameName("W_Sword001.png");
	Wand = Sprite::createWithSpriteFrameName("W_Mace008.png");
	HolySword = Sprite::createWithSpriteFrameName("W_Sword015.png");
	Key = Sprite::createWithSpriteFrameName("I_Key02.png");

	Sword->setPosition(SwordPosition);
	Wand->setPosition(WandPosition);
	HolySword->setPosition(HolySwordPosition);
	Key->setPosition(KeyPosition);

	this->addChild(Sword);
	this->addChild(Wand);
	this->addChild(HolySword);
	this->addChild(Key);


}

bool HelloWorld::onTouchBegan(Touch* touch, Event* event)
{
	return true;
}

void HelloWorld::onTouchEnded(Touch* touch, Event* event)
{
	auto touchPoint = touch->getLocation();
	touchPoint = this->convertToNodeSpace(touchPoint);

	Vec2 playerPos = Hero->getPosition();

	Vec2 diff = touchPoint - playerPos;

	if (abs(diff.x) > abs(diff.y))
	{
		if (diff.x > 0)
		{
			playerPos.x = playerPos.x + tmap->getTileSize().width;
			this->heroMove(1);

		}
		else
		{
			playerPos.x = playerPos.x - tmap->getTileSize().width;
			this->heroMove(2);
		}
	}
	else
	{
		if (diff.y > 0)
		{
			playerPos.y = playerPos.y + tmap->getTileSize().height;
			this->heroMove(3);
		}
		else
		{
			playerPos.y = playerPos.y - tmap->getTileSize().height;
			this->heroMove(4);
		}
	}

	if (playerPos.x <= (tmap->getMapSize().width * tmap->getTileSize().width) &&
		playerPos.y <= (tmap->getMapSize().height * tmap->getTileSize().height) &&
		playerPos.y >= 0 &&
		playerPos.x >= 0)
	{
		// 드래곤의 새로운 위치 지정
		//dragon->setPosition(playerPos);
		this->setHeroPosition(playerPos);

	}

	// 드래곤의 위치에 맞춰 화면 위치 조정
	this->setViewpointCenter(Hero->getPosition());

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

void HelloWorld::setHeroPosition(Vec2 position)
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
			std::string item = properties.asValueMap()["Weapon"].asString();
			if (item == "Sword")
			{
				this->metainfo->removeTileAt(tileCoord);
				this->removeChild(Sword); //weapons->removeTileAt(tileCoord);

				log("Sword 아이템 획득");
			}

			else if (item == "Wand")
			{
				this->metainfo->removeTileAt(tileCoord);
				this->removeChild(Wand);

				log("Wand 아이템 획득");
			}

			else if (item == "HolySword")
			{
				this->metainfo->removeTileAt(tileCoord);
				this->removeChild(HolySword);

				log("HolySword 아이템 획득");
			}
			else if (item == "Key")
			{
				this->metainfo->removeTileAt(tileCoord);
				this->removeChild(Key);

				log("Key 아이템 획득");
			}

			if (item == "YES")
			{
				log("Wall...");
				return;
			}
		}
	}
	// 애니메이션 만들기
	auto animation = Animation::createWithSpriteFrames(HeroFrames, 0.5f);
	auto animate = Animate::create(animation);

	Hero->runAction(animate);
	Hero->runAction(pMove);

	HeroFrames.clear();
}

void HelloWorld::setMonsterPosition(Vec2 position)
{

}

void HelloWorld::heroMove(int num)
{
	char str[100] = { 0 };

	if (num == 1) // 우측 이동
	{
		for (int i = 7; i < 10; i++)
		{
			sprintf(str, "Hero%02d.png", i);
			SpriteFrame* frame = cache->getSpriteFrameByName(str);
			HeroFrames.pushBack(frame);
		}

		pMove = MoveBy::create(0.3, Vec2(32, 0));
	}
	else if (num == 2) // 좌측 이동
	{
		for (int i = 4; i < 7; i++)
		{
			sprintf(str, "Hero%02d.png", i);
			SpriteFrame* frame = cache->getSpriteFrameByName(str);
			HeroFrames.pushBack(frame);
		}

		pMove = MoveBy::create(0.3, Vec2(-32, 0));
	}
	else if (num == 3) // 위로 이동
	{
		for (int i = 10; i < 13; i++)
		{
			sprintf(str, "Hero%02d.png", i);
			SpriteFrame* frame = cache->getSpriteFrameByName(str);
			HeroFrames.pushBack(frame);
		}

		pMove = MoveBy::create(0.3, Vec2(0, 32));
	}
	else if (num == 4) // 아래로 이동
	{
		for (int i = 1; i < 4; i++)
		{
			sprintf(str, "Hero%02d.png", i);
			SpriteFrame* frame = cache->getSpriteFrameByName(str);
			HeroFrames.pushBack(frame);
		}

		pMove = MoveBy::create(0.3, Vec2(0, -32));
	}

}

