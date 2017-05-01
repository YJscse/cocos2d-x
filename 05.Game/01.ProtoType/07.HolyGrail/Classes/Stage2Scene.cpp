#include "Stage2Scene.h"
#include "Stage3Scene.h"
#include "HelloWorldScene.h"

using namespace cocos2d;

Scene* Stage2Scene::createScene()
{
	auto scene = Scene::create();
	auto layer = Stage2Scene::create();
	scene->addChild(layer);
	return scene;
}

bool Stage2Scene::init()
{
	if (!LayerColor::initWithColor(Color4B(0, 255, 0, 255)))
	{
		return false;
	}

	winSize = Director::getInstance()->getWinSize();

	// 타일맵 읽어 오기
	tmap = TMXTiledMap::create("Images/Stage2.tmx");
	background2 = tmap->getLayer("Background2");
	metainfo1 = tmap->getLayer("MetaInfo1");
	metainfo1->setVisible(false);
	metainfo2 = tmap->getLayer("MetaInfo2");
	metainfo2->setVisible(false);
	metainfo3 = tmap->getLayer("MetaInfo3");
	metainfo3->setVisible(false);
	this->addChild(tmap, 0, 11);

	cache2 = SpriteFrameCache::getInstance();
	cache2->addSpriteFramesWithFile("Images/Holygrail.plist");

	auto SideUi = Sprite::create("Images/menu_layer.png");
	auto SideB1 = Sprite::create("Images/SideB1.png");
	auto SideMenu = MenuItemImage::create(
		"Images/SideB2.png",
		"Images/SideB2S.png",
		CC_CALLBACK_1(Stage2Scene::Reset, this));
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

	// 타일맵에서 objects라고 지정한 오브젝트 레이어의 객체들 가져오기
	TMXObjectGroup* objects = tmap->getObjectGroup("Objects2");

	// 오브젝트 레이어에서 Hero라고 지정한 속성값 읽어오기
	ValueMap spawnPoint = objects->getObject("Hero");
	ValueMap spawnPoint11 = objects->getObject("Sword1");
	ValueMap spawnPoint12 = objects->getObject("Sword2");
	ValueMap spawnPoint13 = objects->getObject("Sword3");
	ValueMap spawnPoint21 = objects->getObject("Wand1");
	ValueMap spawnPoint22 = objects->getObject("Wand2");
	ValueMap spawnPoint23 = objects->getObject("Wand3");
	ValueMap spawnPoint24 = objects->getObject("Wand4");
	ValueMap spawnPoint31 = objects->getObject("HolySword1");
	ValueMap spawnPoint32 = objects->getObject("HolySword2");
	ValueMap spawnPoint4 = objects->getObject("Key");
	ValueMap spawnPoint51 = objects->getObject("Wolf1");
	ValueMap spawnPoint52 = objects->getObject("Wolf2");
	ValueMap spawnPoint53 = objects->getObject("Wolf3");
	ValueMap spawnPoint54 = objects->getObject("Wolf4");
	ValueMap spawnPoint61 = objects->getObject("Jelly1");
	ValueMap spawnPoint62 = objects->getObject("Jelly2");
	ValueMap spawnPoint63 = objects->getObject("Jelly3");
	ValueMap spawnPoint64 = objects->getObject("Jelly4");
	ValueMap spawnPoint65 = objects->getObject("Jelly5");
	ValueMap spawnPoint71 = objects->getObject("Demon1");
	ValueMap spawnPoint72 = objects->getObject("Demon2");
	ValueMap spawnPoint73 = objects->getObject("Demon3");
	ValueMap spawnPoint8 = objects->getObject("Door");

	int x = spawnPoint["x"].asInt();    // Hero
	int y = spawnPoint["y"].asInt();

	int x11 = spawnPoint11["x"].asInt();  // Sword1
	int y11 = spawnPoint11["y"].asInt();

	int x12 = spawnPoint12["x"].asInt();  // Sword2
	int y12 = spawnPoint12["y"].asInt();

	int x13 = spawnPoint13["x"].asInt();  // Sword3
	int y13 = spawnPoint13["y"].asInt();

	int x21 = spawnPoint21["x"].asInt();  // Wand1
	int y21 = spawnPoint21["y"].asInt();

	int x22 = spawnPoint22["x"].asInt();  // Wand2
	int y22 = spawnPoint22["y"].asInt();

	int x23 = spawnPoint23["x"].asInt();  // Wand3
	int y23 = spawnPoint23["y"].asInt();

	int x24 = spawnPoint24["x"].asInt();  // Wand4
	int y24 = spawnPoint24["y"].asInt();

	int x31 = spawnPoint31["x"].asInt();  // HolySword1
	int y31 = spawnPoint31["y"].asInt();

	int x32 = spawnPoint32["x"].asInt();  // HolySword2
	int y32 = spawnPoint32["y"].asInt();

	int x4 = spawnPoint4["x"].asInt();  // Key
	int y4 = spawnPoint4["y"].asInt();

	int x51 = spawnPoint51["x"].asInt();  // Wolf1
	int y51 = spawnPoint51["y"].asInt();

	int x52 = spawnPoint52["x"].asInt();  // Wolf2
	int y52 = spawnPoint52["y"].asInt();

	int x53 = spawnPoint53["x"].asInt();  // Wolf3
	int y53 = spawnPoint53["y"].asInt();

	int x54 = spawnPoint54["x"].asInt();  // Wolf4
	int y54 = spawnPoint54["y"].asInt();

	int x61 = spawnPoint61["x"].asInt();  // Jelly1
	int y61 = spawnPoint61["y"].asInt();

	int x62 = spawnPoint62["x"].asInt();  // Jelly2
	int y62 = spawnPoint62["y"].asInt();

	int x63 = spawnPoint63["x"].asInt();  // Jelly3
	int y63 = spawnPoint63["y"].asInt();

	int x64 = spawnPoint64["x"].asInt();  // Jelly4
	int y64 = spawnPoint64["y"].asInt();

	int x65 = spawnPoint65["x"].asInt();  // Jelly5
	int y65 = spawnPoint65["y"].asInt();

	int x71 = spawnPoint71["x"].asInt();  // Demon
	int y71 = spawnPoint71["y"].asInt();

	int x72 = spawnPoint72["x"].asInt();  // Demon
	int y72 = spawnPoint72["y"].asInt();

	int x73 = spawnPoint73["x"].asInt();  // Demon
	int y73 = spawnPoint73["y"].asInt();

	int x8 = spawnPoint8["x"].asInt();  // Door
	int y8 = spawnPoint8["y"].asInt();

	// SpawnPoint의 위치로 생성할 히어로, 무기, 몬스터들 위치 지정하기
	HeroPosition = Vec2(x, y);
	SwordPosition1 = Vec2(x11, y11);
	SwordPosition2 = Vec2(x12, y12);
	SwordPosition3 = Vec2(x13, y13);
	WandPosition1 = Vec2(x21, y21);
	WandPosition2 = Vec2(x22, y22);
	WandPosition3 = Vec2(x23, y23);
	WandPosition4 = Vec2(x24, y24);
	HolySwordPosition1 = Vec2(x31, y31);
	HolySwordPosition2 = Vec2(x32, y32);
	KeyPosition = Vec2(x4, y4);
	WolfPosition1 = Vec2(x51, y51);
	WolfPosition2 = Vec2(x52, y52);
	WolfPosition3 = Vec2(x53, y53);
	WolfPosition4 = Vec2(x54, y54);
	JellyPosition1 = Vec2(x61, y61);
	JellyPosition2 = Vec2(x62, y62);
	JellyPosition3 = Vec2(x63, y63);
	JellyPosition4 = Vec2(x64, y64);
	JellyPosition5 = Vec2(x65, y65);
	DemonPosition1 = Vec2(x71, y71);
	DemonPosition2 = Vec2(x72, y72);
	DemonPosition3 = Vec2(x73, y73);
	DoorPosition = Vec2(x8, y8);

	// 히어로, 무기, 몬스터를 만든다.
	this->createHero();
	this->createWeapons();
	this->createWolf();
	this->createJelly();
	this->createDemon();
	this->createDoor();

	//this->schedule(schedule_selector(Stage2Scene::wolfMove), 1.0f);

	return true;
}

void Stage2Scene::Reset(Ref *pSender)
{
	auto pScene = Stage2Scene::createScene();
	Director::getInstance()->pushScene(pScene);
}

void Stage2Scene::onEnter()
{
	Layer::onEnter();

	// 싱글 터치 모드로 터치리스너 등록
	auto listener = EventListenerTouchOneByOne::create();

	listener->setSwallowTouches(true);

	listener->onTouchBegan = CC_CALLBACK_2(Stage2Scene::onTouchBegan, this);
	listener->onTouchEnded = CC_CALLBACK_2(Stage2Scene::onTouchEnded, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void Stage2Scene::onExit()
{
	//_eventDispatcher->removeEventListenersForType(EventListener::Type::TOUCH_ONE_BY_ONE);

	Layer::onExit();
}

void Stage2Scene::createHero()
{
	Hero = Sprite::createWithSpriteFrameName("Hero02.png"); //정면 서있는모습
	Hero->setPosition(HeroPosition);
	Hero->setScale(0.8f);
	this->addChild(Hero);

}

void Stage2Scene::createWolf()
{
	Wolf1 = Sprite::createWithSpriteFrameName("Wolf1.png");
	Wolf2 = Sprite::createWithSpriteFrameName("Wolf1.png");
	Wolf3 = Sprite::createWithSpriteFrameName("Wolf1.png");
	Wolf4 = Sprite::createWithSpriteFrameName("Wolf1.png");

	char str1[50] = { 0 };

	for (int i = 1; i < 4; i++)
	{
		sprintf(str1, "Wolf%d.png", i);
		SpriteFrame* wframe = cache2->getSpriteFrameByName(str1);
		MonsterFrames.pushBack(wframe);
	}

	auto wolfAnimation = Animation::createWithSpriteFrames(MonsterFrames, 0.7f);
	auto wolfAnimate = Animate::create(wolfAnimation);
	auto rep1 = RepeatForever::create(wolfAnimate);

	Wolf1->setPosition(WolfPosition1);
	Wolf2->setPosition(WolfPosition2);
	Wolf3->setPosition(WolfPosition3);
	Wolf4->setPosition(WolfPosition4);

	this->addChild(Wolf1);
	this->addChild(Wolf2);
	this->addChild(Wolf3);
	this->addChild(Wolf4);

	Wolf1->runAction(rep1);
	Wolf2->runAction(rep1->clone());
	Wolf3->runAction(rep1->clone());
	Wolf4->runAction(rep1->clone());
}

void Stage2Scene::createJelly()
{
	Jelly1 = Sprite::createWithSpriteFrameName("Jelly1.png");
	Jelly2 = Sprite::createWithSpriteFrameName("Jelly1.png");
	Jelly3 = Sprite::createWithSpriteFrameName("Jelly1.png");
	Jelly4 = Sprite::createWithSpriteFrameName("Jelly1.png");
	Jelly5 = Sprite::createWithSpriteFrameName("Jelly1.png");

	char str2[50] = { 0 };

	for (int i = 1; i < 4; i++)
	{
		sprintf(str2, "Jelly%d.png", i);
		SpriteFrame* jframe = cache2->getSpriteFrameByName(str2);
		JellyFrames.pushBack(jframe);
	}

	auto JellyAnimation = Animation::createWithSpriteFrames(JellyFrames, 0.5f);
	auto JellyAnimate = Animate::create(JellyAnimation);
	auto rep2 = RepeatForever::create(JellyAnimate);

	Jelly1->setPosition(JellyPosition1);
	Jelly2->setPosition(JellyPosition2);
	Jelly3->setPosition(JellyPosition3);
	Jelly4->setPosition(JellyPosition4);
	Jelly5->setPosition(JellyPosition5);

	this->addChild(Jelly1);
	this->addChild(Jelly2);
	this->addChild(Jelly3);
	this->addChild(Jelly4);
	this->addChild(Jelly5);

	Jelly1->runAction(rep2);
	Jelly2->runAction(rep2->clone());
	Jelly3->runAction(rep2->clone());
	Jelly4->runAction(rep2->clone());
	Jelly5->runAction(rep2->clone());

}

void Stage2Scene::createDemon()
{
	Demon1 = Sprite::createWithSpriteFrameName("Demon1.png");
	Demon2 = Sprite::createWithSpriteFrameName("Demon1.png");
	Demon3 = Sprite::createWithSpriteFrameName("Demon1.png");

	char str3[50] = { 0 };

	for (int i = 1; i < 4; i++)
	{
		sprintf(str3, "Demon%d.png", i);
		SpriteFrame* dframe = cache2->getSpriteFrameByName(str3);
		DemonFrames.pushBack(dframe);
	}

	auto DemonAnimation = Animation::createWithSpriteFrames(DemonFrames, 0.5f);
	auto DemonAnimate = Animate::create(DemonAnimation);
	auto rep3 = RepeatForever::create(DemonAnimate);

	Demon1->setPosition(DemonPosition1);
	Demon2->setPosition(DemonPosition2);
	Demon3->setPosition(DemonPosition3);

	this->addChild(Demon1);
	this->addChild(Demon2);
	this->addChild(Demon3);

	Demon1->runAction(rep3);
	Demon2->runAction(rep3->clone());
	Demon3->runAction(rep3->clone());

}

void Stage2Scene::createWeapons()
{
	Sword1 = Sprite::createWithSpriteFrameName("W_Sword001.png");
	Sword2 = Sprite::createWithSpriteFrameName("W_Sword001.png");
	Sword3 = Sprite::createWithSpriteFrameName("W_Sword001.png");

	Wand1 = Sprite::createWithSpriteFrameName("W_Mace008.png");
	Wand2 = Sprite::createWithSpriteFrameName("W_Mace008.png");
	Wand3 = Sprite::createWithSpriteFrameName("W_Mace008.png");
	Wand4 = Sprite::createWithSpriteFrameName("W_Mace008.png");

	HolySword1 = Sprite::createWithSpriteFrameName("W_Sword015.png");
	HolySword2 = Sprite::createWithSpriteFrameName("W_Sword015.png");

	Key = Sprite::createWithSpriteFrameName("I_Key02.png");

	Sword1->setPosition(SwordPosition1);
	Sword2->setPosition(SwordPosition2);
	Sword3->setPosition(SwordPosition3);
	Wand1->setPosition(WandPosition1);
	Wand2->setPosition(WandPosition2);
	Wand3->setPosition(WandPosition3);
	Wand4->setPosition(WandPosition4);
	HolySword1->setPosition(HolySwordPosition1);
	HolySword2->setPosition(HolySwordPosition2);
	Key->setPosition(KeyPosition);

	this->addChild(Sword1);
	this->addChild(Sword2);
	this->addChild(Sword3);
	this->addChild(Wand1);
	this->addChild(Wand2);
	this->addChild(Wand3);
	this->addChild(Wand4);
	this->addChild(HolySword1);
	this->addChild(HolySword2);
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

void Stage2Scene::createDoor()
{
	Door = Sprite::createWithSpriteFrameName("I_Chest01.png");
	Door->setPosition(DoorPosition);
	this->addChild(Door);
}

bool Stage2Scene::onTouchBegan(Touch* touch, Event* event)
{
	return true;
}

void Stage2Scene::onTouchEnded(Touch* touch, Event* event)
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

void Stage2Scene::setViewpointCenter(Vec2 position)
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
Vec2 Stage2Scene::tileCoordForPosition(Vec2 position)
{
	int x = position.x / tmap->getTileSize().width;
	int y = ((tmap->getMapSize().height * tmap->getTileSize().height) - position.y) / tmap->getTileSize().height;

	return Vec2(x, y);
}

void Stage2Scene::setHeroPosition(Vec2 position)
{
	// 탭된 위치 구하기
	Vec2 tileCoord = this->tileCoordForPosition(position);
	Vec2 tileCoord2 = this->tileCoordForPosition(position);
	Vec2 tileCoord3 = this->tileCoordForPosition(position);

	// 현재 위치의 Tile GID 구하기
	int tileGid = this->metainfo1->getTileGIDAt(tileCoord);
	int tileGid2 = this->metainfo2->getTileGIDAt(tileCoord2);
	int tileGid3 = this->metainfo3->getTileGIDAt(tileCoord3);

	auto RemoveAction = FadeOut::create(1);

	// meta1
	if (tileGid)
	{
		// 타일맵에서 GID에 해당하는 부분의 속성 읽어 오기
		Value properties = tmap->getPropertiesForGID(tileGid);

		if (!properties.isNull())
		{
			std::string item = properties.asValueMap()["Weapon"].asString();
			// sword
			if (item == "Sword1")
			{
				if (swordcount == 1 || wandcount == 1 || holyswordcount == 1 || keycount == 1)
				{
					log("이미 아이템을 가지고 있습니다");
					return;
				}
				swordcount++;
				this->metainfo1->removeTileAt(tileCoord);
				Sword1->runAction(RemoveAction);

				boxSword = Sprite::createWithSpriteFrameName("W_Sword001.png");
				boxSword->setPosition(Vec2(450, 270));
				boxSword->setScale(1.2);
				this->addChild(boxSword, 4);

				log("Sword 아이템 획득, sword :%d", swordcount);
			}
			else if (item == "Sword2")
			{
				if (swordcount == 1 || wandcount == 1 || holyswordcount == 1 || keycount == 1)
				{
					log("이미 아이템을 가지고 있습니다");
					return;
				}
				swordcount++;
				this->metainfo1->removeTileAt(tileCoord);
				Sword2->runAction(RemoveAction);

				boxSword = Sprite::createWithSpriteFrameName("W_Sword001.png");
				boxSword->setPosition(Vec2(450, 270));
				boxSword->setScale(1.2);
				this->addChild(boxSword, 4);

				log("Sword 아이템 획득, sword :%d", swordcount);
			}
			else if (item == "Sword3")
			{
				if (swordcount == 1 || wandcount == 1 || holyswordcount == 1 || keycount == 1)
				{
					log("이미 아이템을 가지고 있습니다");
					return;
				}
				swordcount++;
				this->metainfo1->removeTileAt(tileCoord);
				Sword3->runAction(RemoveAction);

				boxSword = Sprite::createWithSpriteFrameName("W_Sword001.png");
				boxSword->setPosition(Vec2(450, 270));
				boxSword->setScale(1.2);
				this->addChild(boxSword, 4);

				log("Sword 아이템 획득, sword :%d", swordcount);
			}
			// wand
			else if (item == "Wand1")
			{
				if (swordcount == 1 || wandcount == 1 || holyswordcount == 1 || keycount == 1)
				{
					log("이미 아이템을 가지고 있습니다");
					return;
				}
				wandcount++;
				this->metainfo1->removeTileAt(tileCoord);
				Wand1->runAction(RemoveAction);

				boxWand = Sprite::createWithSpriteFrameName("W_Mace008.png");
				boxWand->setPosition(Vec2(450, 270));
				boxWand->setScale(1.2);
				this->addChild(boxWand, 4);

				log("Wand 아이템 획득, wand : %d", wandcount);
			}
			else if (item == "Wand2")
			{
				if (swordcount == 1 || wandcount == 1 || holyswordcount == 1 || keycount == 1)
				{
					log("이미 아이템을 가지고 있습니다");
					return;
				}
				wandcount++;
				this->metainfo1->removeTileAt(tileCoord);
				Wand2->runAction(RemoveAction);

				boxWand = Sprite::createWithSpriteFrameName("W_Mace008.png");
				boxWand->setPosition(Vec2(450, 270));
				boxWand->setScale(1.2);
				this->addChild(boxWand, 4);

				log("Wand 아이템 획득, wand : %d", wandcount);
			}
			else if (item == "Wand3")
			{
				if (swordcount == 1 || wandcount == 1 || holyswordcount == 1 || keycount == 1)
				{
					log("이미 아이템을 가지고 있습니다");
					return;
				}
				wandcount++;
				this->metainfo1->removeTileAt(tileCoord);
				Wand3->runAction(RemoveAction);

				boxWand = Sprite::createWithSpriteFrameName("W_Mace008.png");
				boxWand->setPosition(Vec2(450, 270));
				boxWand->setScale(1.2);
				this->addChild(boxWand, 4);

				log("Wand 아이템 획득, wand : %d", wandcount);
			}
			else if (item == "Wand4")
			{
				if (swordcount == 1 || wandcount == 1 || holyswordcount == 1 || keycount == 1)
				{
					log("이미 아이템을 가지고 있습니다");
					return;
				}
				wandcount++;
				this->metainfo1->removeTileAt(tileCoord);
				Wand4->runAction(RemoveAction);

				boxWand = Sprite::createWithSpriteFrameName("W_Mace008.png");
				boxWand->setPosition(Vec2(450, 270));
				boxWand->setScale(1.2);
				this->addChild(boxWand, 4);

				log("Wand 아이템 획득, wand : %d", wandcount);
			}
			// holysword
			else if (item == "HolySword1")
			{
				if (swordcount == 1 || wandcount == 1 || holyswordcount == 1 || keycount == 1)
				{
					log("이미 아이템을 가지고 있습니다");
					return;
				}

				holyswordcount++;
				this->metainfo1->removeTileAt(tileCoord);
				HolySword1->runAction(RemoveAction);

				boxHolySword = Sprite::createWithSpriteFrameName("W_Sword015.png");
				boxHolySword->setPosition(Vec2(450, 270));
				boxHolySword->setScale(1.2);
				this->addChild(boxHolySword, 4);

				log("HolySword 아이템 획득 holyswrd : %d", holyswordcount);
			}
			else if (item == "HolySword2")
			{
				if (swordcount == 1 || wandcount == 1 || holyswordcount == 1 || keycount == 1)
				{
					log("이미 아이템을 가지고 있습니다");
					return;
				}

				holyswordcount++;
				this->metainfo1->removeTileAt(tileCoord);
				HolySword2->runAction(RemoveAction);

				boxHolySword = Sprite::createWithSpriteFrameName("W_Sword015.png");
				boxHolySword->setPosition(Vec2(450, 270));
				boxHolySword->setScale(1.2);
				this->addChild(boxHolySword, 4);

				log("HolySword 아이템 획득 holyswrd : %d", holyswordcount);
			}
			// key
			else if (item == "Key")
			{
				if (swordcount == 1 || wandcount == 1 || holyswordcount == 1)
				{
					log("이미 아이템을 가지고 있습니다");
					return;
				}

				keycount++;
				this->metainfo1->removeTileAt(tileCoord);
				Key->runAction(RemoveAction);
				emitter->runAction(RemoveAction->clone());

				boxKey = Sprite::createWithSpriteFrameName("I_Key02.png");
				boxKey->setPosition(Vec2(450, 270));
				boxKey->setScale(1.2);
				this->addChild(boxKey, 4);

				log("Key 아이템 획득, key : %d", keycount);

			}


		}
	}
	// meta2
	if (tileGid2)
	{
		Value properties2 = tmap->getPropertiesForGID(tileGid2);

		if (!properties2.isNull())
		{
			std::string monster = properties2.asValueMap()["Monster"].asString();
			if (monster == "Wolf1")
			{
				if (swordcount == 1)
				{
					this->metainfo2->removeTileAt(tileCoord);
					Wolf1->runAction(RemoveAction);
					this->removeChild(boxSword);

					log("늑대 사냥!!");
					swordcount = 0;
				}
				else
				{
					log("Sword로만 처치할 수 있어요 ㅜㅜ");
					return;
				}
			}
			else if (monster == "Wolf2")
			{
				if (swordcount == 1)
				{
					this->metainfo2->removeTileAt(tileCoord);
					Wolf2->runAction(RemoveAction);
					this->removeChild(boxSword);

					log("늑대 사냥!!");
					swordcount = 0;
				}
				else
				{
					log("Sword로만 처치할 수 있어요 ㅜㅜ");
					return;
				}
			}
			else if (monster == "Wolf3")
			{
				if (swordcount == 1)
				{
					this->metainfo2->removeTileAt(tileCoord);
					Wolf3->runAction(RemoveAction);
					this->removeChild(boxSword);

					log("늑대 사냥!!");
					swordcount = 0;
				}
				else
				{
					log("Sword로만 처치할 수 있어요 ㅜㅜ");
					return;
				}
			}
			else if (monster == "Wolf4")
			{
				if (swordcount == 1)
				{
					this->metainfo2->removeTileAt(tileCoord);
					Wolf4->runAction(RemoveAction);
					this->removeChild(boxSword);

					log("늑대 사냥!!");
					swordcount = 0;
				}
				else
				{
					log("Sword로만 처치할 수 있어요 ㅜㅜ");
					return;
				}
			}

			else if (monster == "Jelly1")
			{
				if (wandcount == 1)
				{
					this->metainfo2->removeTileAt(tileCoord);
					Jelly1->runAction(RemoveAction);
					this->removeChild(boxWand);

					log("슬라임 사냥!!");
					wandcount = 0;
				}
				else
				{
					log("Wand로만 처치할 수 있어요 ㅜㅜ");
					return;
				}
			}
			else if (monster == "Jelly2")
			{
				if (wandcount == 1)
				{
					this->metainfo2->removeTileAt(tileCoord);
					Jelly2->runAction(RemoveAction);
					this->removeChild(boxWand);

					log("슬라임 사냥!!");
					wandcount = 0;
				}
				else
				{
					log("Wand로만 처치할 수 있어요 ㅜㅜ");
					return;
				}
			}
			else if (monster == "Jelly3")
			{
				if (wandcount == 1)
				{
					this->metainfo2->removeTileAt(tileCoord);
					Jelly3->runAction(RemoveAction);
					this->removeChild(boxWand);

					log("슬라임 사냥!!");
					wandcount = 0;
				}
				else
				{
					log("Wand로만 처치할 수 있어요 ㅜㅜ");
					return;
				}
			}
			else if (monster == "Jelly4")
			{
				if (wandcount == 1)
				{
					this->metainfo2->removeTileAt(tileCoord);
					Jelly4->runAction(RemoveAction);
					this->removeChild(boxWand);

					log("슬라임 사냥!!");
					wandcount = 0;
				}
				else
				{
					log("Wand로만 처치할 수 있어요 ㅜㅜ");
					return;
				}
			}
			else if (monster == "Jelly5")
			{
				if (wandcount == 1)
				{
					this->metainfo2->removeTileAt(tileCoord);
					Jelly5->runAction(RemoveAction);
					this->removeChild(boxWand);

					log("슬라임 사냥!!");
					wandcount = 0;
				}
				else
				{
					log("Wand로만 처치할 수 있어요 ㅜㅜ");
					return;
				}
			}
			//door
			else if (monster == "Door")
			{
				Door1 = Sprite::createWithSpriteFrameName("I_Chest02.png");
				Door1->setPosition(DoorPosition);
				Door1->setScale(0.8);
				this->addChild(Door1);

				log("Stage3!!!");
				auto pScene = Stage3Scene::createScene();
				Director::getInstance()->pushScene(TransitionProgressRadialCW::create(1, pScene));
				Door->setVisible(false);
				keycount = 0;

			}

		}
	}
	// meta3
	if (tileGid3)
	{
		Value properties3 = tmap->getPropertiesForGID(tileGid3);

		if (!properties3.isNull())
		{
			std::string monster = properties3.asValueMap()["Monster"].asString();
			if (monster == "Demon1")
			{
				if (holyswordcount == 1)
				{
					this->metainfo3->removeTileAt(tileCoord);
					Demon1->runAction(RemoveAction);
					this->removeChild(boxHolySword);

					log("악마 사냥!!");
					holyswordcount = 0;
				}
				else
				{
					log("HolySword로만 처치할 수 있어요 ㅜㅜ");
					return;
				}
			}
			else if (monster == "Demon2")
			{
				if (holyswordcount == 1)
				{
					this->metainfo3->removeTileAt(tileCoord);
					Demon2->runAction(RemoveAction);
					this->removeChild(boxHolySword);

					log("악마 사냥!!");
					holyswordcount = 0;
				}
				else
				{
					log("HolySword로만 처치할 수 있어요 ㅜㅜ");
					return;
				}
			}
			else if (monster == "Demon3")
			{
				if (holyswordcount == 1)
				{
					this->metainfo3->removeTileAt(tileCoord);
					Demon3->runAction(RemoveAction);
					this->removeChild(boxHolySword);

					log("악마 사냥!!");
					holyswordcount = 0;
				}
				else
				{
					log("HolySword로만 처치할 수 있어요 ㅜㅜ");
					return;
				}
			}

			if (monster == "Wall")
			{
				log("wall...");
				HeroFrames.clear();
				return;
			}
		}
	}
	// 애니메이션 만들기
	auto animation = Animation::createWithSpriteFrames(HeroFrames, 0.2f);                    
	auto animate = Animate::create(animation);

	Hero->runAction(animate);
	Hero->runAction(pMove);

	HeroFrames.clear();
}

// 움직일때 구질구질하게 자꾸 뒤돌아보는것은 ↑ 애니메이션이 끝나기전에 움직여서 그런거임 그러니까 애니메이션이 작동하는 시간을 줄여주면
// 쿨하게 앞만보고 걸어다닌다.
void Stage2Scene::heroMove(int num)
{
	char str[100] = { 0 };

	if (num == 1) // 우측 이동
	{
		for (int i = 7; i < 10; i++)
		{
			sprintf(str, "Hero%02d.png", i);
			SpriteFrame* frame = cache2->getSpriteFrameByName(str);
			HeroFrames.pushBack(frame);
		}

		pMove = MoveBy::create(0.3f, Vec2(32, 0));
	}
	else if (num == 2) // 좌측 이동
	{
		for (int i = 4; i < 7; i++)
		{
			sprintf(str, "Hero%02d.png", i);
			SpriteFrame* frame = cache2->getSpriteFrameByName(str);
			HeroFrames.pushBack(frame);
		}

		pMove = MoveBy::create(0.3f, Vec2(-32, 0));
	}
	else if (num == 3) // 위로 이동
	{
		for (int i = 10; i < 13; i++)
		{
			sprintf(str, "Hero%02d.png", i);
			SpriteFrame* frame = cache2->getSpriteFrameByName(str);
			HeroFrames.pushBack(frame);
		}

		pMove = MoveBy::create(0.3f, Vec2(0, 32));
	}
	else if (num == 4) // 아래로 이동
	{
		for (int i = 1; i < 4; i++)
		{
			sprintf(str, "Hero%02d.png", i);
			SpriteFrame* frame = cache2->getSpriteFrameByName(str);
			HeroFrames.pushBack(frame);
		}

		pMove = MoveBy::create(0.3f, Vec2(0, -32));
	}

}

