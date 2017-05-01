#include "HelloWorldScene.h"
#include "Stage2Scene.h"


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

    if ( !LayerColor::initWithColor(Color4B(255, 255, 255, 255) ))
    {
        return false;
    }

	winSize = Director::getInstance()->getWinSize();

	// 타일맵 읽어 오기
	tmap = TMXTiledMap::create("Images/Tutorial.tmx");
	background = tmap->getLayer("Background1");
	metainfo = tmap->getLayer("MetaInfo1");
	metainfo->setVisible(false);
	this->addChild(tmap, 0, 11); 

	cache = SpriteFrameCache::getInstance();
	cache->addSpriteFramesWithFile("Images/Holygrail.plist");

	auto SideUi = Sprite::create("Images/menu_layer.png");
	auto SideB1 = Sprite::create("Images/SideB1.png");
	auto SideMenu = MenuItemImage::create(
		"Images/SideB2.png",
		"Images/SideB2S.png",
		CC_CALLBACK_1(HelloWorld::Reset, this));
	auto ResetMenu = Menu::create(SideMenu, nullptr);

	SideUi->setAnchorPoint(Vec2(1.0f, 1.0f));
	SideUi->setPosition(Vec2(480, 320));
	SideB1->setPosition(Vec2(448, 270));
	ResetMenu->setPosition(Vec2(448, 40));

	SideUi->setOpacity(150);
	SideB1->setOpacity(200);

	this->addChild(SideUi, 2);
	this->addChild(SideB1, 3);
	this->addChild(ResetMenu, 3);

	Manual = Label::create(
		"칼은 늑대를 처치할 수 있으며, 메이스는 젤리를 처치할 수 있고, 성검으로 악마를 처치할 수 있습니다. 열쇠로 상자를 열면 클리어입니다.단, 한번에 하나의 아이템만을 가질 수 있습니다.",
		"Arial",
		20);
	Manual->setColor(Color3B::BLACK);
	Manual->setAnchorPoint(Vec2(0, 0));
	Manual->setPosition(Vec2(480, 5));
	this->addChild(Manual, 5);

	auto move = MoveBy::create(12, Vec2(-2500, 0));
	auto replace = Place::create(Vec2(480, 10));
	auto seq = Sequence::create(move, replace, nullptr);
	Manual->runAction(RepeatForever::create(seq));

	// 타일맵에서 objects라고 지정한 오브젝트 레이어의 객체들 가져오기
	TMXObjectGroup* objects = tmap->getObjectGroup("Objects1");

	// 오브젝트 레이어에서 Hero라고 지정한 속성값 읽어오기
	ValueMap spawnPoint = objects->getObject("Hero");
	ValueMap spawnPoint1 = objects->getObject("Sword");
	ValueMap spawnPoint2 = objects->getObject("Wand");
	ValueMap spawnPoint3 = objects->getObject("HolySword");
	ValueMap spawnPoint4 = objects->getObject("Key");
	ValueMap spawnPoint5 = objects->getObject("Wolf");
	ValueMap spawnPoint6 = objects->getObject("Jelly");
	ValueMap spawnPoint7 = objects->getObject("Demon");
	ValueMap spawnPoint8 = objects->getObject("Door");

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

	int x6 = spawnPoint6["x"].asInt();  // Jelly
	int y6 = spawnPoint6["y"].asInt();

	int x7 = spawnPoint7["x"].asInt();  // Demon
	int y7 = spawnPoint7["y"].asInt();

	int x8 = spawnPoint8["x"].asInt();  // Door
	int y8 = spawnPoint8["y"].asInt();

	// SpawnPoint의 위치로 생성할 히어로, 무기, 몬스터들 위치 지정하기
	HeroPosition = Vec2(x, y);
	SwordPosition = Vec2(x1, y1);
	WandPosition = Vec2(x2, y2);
	HolySwordPosition = Vec2(x3, y3);
	KeyPosition = Vec2(x4, y4);
	WolfPosition = Vec2(x5, y5);
	JellyPosition = Vec2(x6, y6);
	DemonPosition = Vec2(x7, y7);
	DoorPosition = Vec2(x8, y8);

	// 히어로, 무기, 몬스터를 만든다.
	this->createHero();
	this->createWeapons();
	this->createWolf();
	this->createJelly();
	this->createDemon();
	this->createDoor();

	


    return true;
}

void HelloWorld::LabelMove(float f)
{
	int MoveAdd = -1;
	Vec2 ManualPos = Manual->getPosition();

	Manual->setPosition(Vec2(ManualPos.x + MoveAdd, ManualPos.y));

	if (ManualPos.x == -1280)
	{
		Manual->setPosition(Vec2(480, 5));
	}
}

void HelloWorld::Reset(Ref *pSender)
{
	auto pScene = HelloWorld::createScene();
	Director::getInstance()->pushScene(pScene);
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

void HelloWorld::createJelly()
{
	Jelly = Sprite::createWithSpriteFrameName("Jelly1.png");

	char str2[50] = { 0 };

	for (int i = 1; i < 4; i++)
	{
		sprintf(str2, "Jelly%d.png", i);
		SpriteFrame* jframe = cache->getSpriteFrameByName(str2);
		JellyFrames.pushBack(jframe);
	}

	auto JellyAnimation = Animation::createWithSpriteFrames(JellyFrames, 0.5f);
	auto JellyAnimate = Animate::create(JellyAnimation);
	auto rep2 = RepeatForever::create(JellyAnimate);

	Jelly->setPosition(JellyPosition);
	this->addChild(Jelly);
	Jelly->runAction(rep2);

}

void HelloWorld::createDemon()
{
	Demon = Sprite::createWithSpriteFrameName("Demon1.png");

	char str3[50] = { 0 };

	for (int i = 1; i < 4; i++)
	{
		sprintf(str3, "Demon%d.png", i);
		SpriteFrame* dframe = cache->getSpriteFrameByName(str3);
		DemonFrames.pushBack(dframe);
	}

	auto DemonAnimation = Animation::createWithSpriteFrames(DemonFrames, 0.5f);
	auto DemonAnimate = Animate::create(DemonAnimation);
	auto rep3 = RepeatForever::create(DemonAnimate);
	Demon->setPosition(DemonPosition);
	this->addChild(Demon);
	Demon->runAction(rep3);

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

	/********************** 파티클 **********************/
	const char* Holly = "Particles/holly.plist";

	emitter = ParticleSystemQuad::create(Holly);
	emitter->setPosition(KeyPosition);
	emitter->setScale(0.7f);
	emitter->setDuration(-1.0f);
	emitter->setAutoRemoveOnFinish(true);

	this->addChild(emitter);
	/********************** 파티클 끝 **********************/


}

void HelloWorld::createDoor()
{
	Door = Sprite::createWithSpriteFrameName("I_Chest01.png");
	Door->setPosition(DoorPosition);
	this->addChild(Door);
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

	auto RemoveAction = FadeOut::create(1);

	if (tileGid)
	{
		// 타일맵에서 GID에 해당하는 부분의 속성 읽어 오기
		Value properties = tmap->getPropertiesForGID(tileGid);

		if (!properties.isNull())
		{
	

			std::string item = properties.asValueMap()["Weapon"].asString();
			if (item == "YES")
			{
				log("Wall...");
				HeroFrames.clear();
				return;
			}
			std::string monster = properties.asValueMap()["Monster"].asString();
			std::string door = properties.asValueMap()["Door"].asString();
			if (item == "Sword")
			{
				swordcount++;
				this->metainfo->removeTileAt(tileCoord);
				Sword->runAction(RemoveAction);

				boxSword = Sprite::createWithSpriteFrameName("W_Sword001.png");
				boxSword->setPosition(Vec2(450, 270));
				boxSword->setScale(1.2);
				this->addChild(boxSword, 4);

				log("Sword 아이템 획득, sword :%d", swordcount);
			}
			else if (item == "Wand")
			{
				if (swordcount == 1 || holyswordcount == 1 || keycount == 1)
				{
					log("이미 아이템을 가지고 있습니다");
					HeroFrames.clear();
					return;
				}
				wandcount++;
				this->metainfo->removeTileAt(tileCoord);
				Wand->runAction(RemoveAction);

				boxWand = Sprite::createWithSpriteFrameName("W_Mace008.png");
				boxWand->setPosition(Vec2(450, 270));
				boxWand->setScale(1.2);
				this->addChild(boxWand, 4);

				log("Wand 아이템 획득, wand : %d", wandcount);
			}
			else if (item == "HolySword")
			{
				if (swordcount == 1 || wandcount == 1 || keycount == 1)
				{
					log("이미 아이템을 가지고 있습니다");
					HeroFrames.clear();
					return;
				}

				holyswordcount++;
				this->metainfo->removeTileAt(tileCoord);
				HolySword->runAction(RemoveAction);

				boxHolySword = Sprite::createWithSpriteFrameName("W_Sword015.png");
				boxHolySword->setPosition(Vec2(450, 270));
				boxHolySword->setScale(1.2);
				this->addChild(boxHolySword, 4);

				log("HolySword 아이템 획득 holyswrd : %d", holyswordcount);
	
			}
			else if (item == "Key")
			{
				if (swordcount == 1 || wandcount == 1 || holyswordcount == 1)
				{
					log("이미 아이템을 가지고 있습니다");
					HeroFrames.clear();
					return;
				}

				keycount++;
				this->metainfo->removeTileAt(tileCoord);
				Key->runAction(RemoveAction);
				this->removeChild(emitter);

				boxKey = Sprite::createWithSpriteFrameName("I_Key02.png");
				boxKey->setPosition(Vec2(450, 270));
				boxKey->setScale(1.2);
				this->addChild(boxKey, 4);

				log("Key 아이템 획득, key : %d", keycount);
			}
	
			else if (door == "Door")
			{
				if (keycount == 1)
				{

					Door1 = Sprite::createWithSpriteFrameName("I_Chest02.png");
					Door1->setPosition(DoorPosition);
					Door1->setScale(0.8);
					this->addChild(Door1);

					log("Stage2로 이동!!!");
					auto pScene = Stage2Scene::createScene();
					Director::getInstance()->pushScene(TransitionProgressRadialCW::create(1, pScene));
					this->removeChild(boxKey);
					Door->setVisible(false);
					keycount = 0;

				}
				else
				{
					log("Key를 가지고 오세요!!");
					HeroFrames.clear();
					return;
				}
			}

			if (monster == "Wolf")
			{
				if (swordcount == 1)
				{
					this->metainfo->removeTileAt(tileCoord);
					Wolf->runAction(RemoveAction);
					this->removeChild(boxSword);

					log("늑대 사냥!!");
					swordcount = 0;
				}
				else
				{
					log("Sword로만 처치할 수 있어요 ㅜㅜ");
					HeroFrames.clear();
					return;
				}
			}
	
			else if (monster == "Jelly")
			{
				if (wandcount == 1)
				{
					this->metainfo->removeTileAt(tileCoord);
					Jelly->runAction(RemoveAction);
					this->removeChild(boxWand);

					log("슬라임 사냥!!");
					wandcount = 0;
				}
				else
				{
					log("Wand로만 처치할 수 있어요 ㅜㅜ");
					HeroFrames.clear();
					return;
				}
			}
			else if (monster == "Demon")
			{
				if (holyswordcount == 1)
				{
					this->metainfo->removeTileAt(tileCoord);
					Demon->runAction(RemoveAction);
					this->removeChild(boxHolySword);

					log("악마 사냥!!");
					holyswordcount = 0;
				}
				else
				{
					log("HolySword로만 처치할 수 있어요 ㅜㅜ");
					HeroFrames.clear();
					return;
				}
			}
			
		}
	}
	// 애니메이션 만들기
	auto animation = Animation::createWithSpriteFrames(HeroFrames, 0.15f);
	auto animate = Animate::create(animation);

	Hero->runAction(animate);
	Hero->runAction(pMove);

	HeroFrames.clear();
}

// 움직일때 구질구질하게 자꾸 뒤돌아보는것은 ↑ 애니메이션이 끝나기전에 움직여서 그런거임 그러니까 애니메이션이 작동하는 시간을 줄여주면
// 쿨하게 앞만보고 걸어다닌다.
void HelloWorld::heroMove(int num)
{
	char str[50] = { 0 };

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
