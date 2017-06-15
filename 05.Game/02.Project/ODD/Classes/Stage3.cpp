#include "Stage2.h"
#include "Stage3.h"
#include "GameMain.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

using namespace CocosDenshion;

int bScore3 = 0;

Scene* Stage3::createScene()
{
	auto scene = Scene::create();
	auto layer = Stage3::create();
	scene->addChild(layer);
	return scene;
}

bool Stage3::init()
{
	if (!Layer::init())
	{
		return false;
	}
	/////////////////////////////////////////////
	winSize = Director::getInstance()->getWinSize();

	texture = Director::getInstance()->getTextureCache()->addImage("Images/Punk_Run.png");
	texture2 = Director::getInstance()->getTextureCache()->addImage("Images/fire_animation.png");
	texture3 = Director::getInstance()->getTextureCache()->addImage("Images/star.png");
	texture4 = Director::getInstance()->getTextureCache()->addImage("Images/meteor.png");
	texture5 = Director::getInstance()->getTextureCache()->addImage("Images/explosion.png");

	bg = Sprite::create("Images/stage3_background.png");
	bg->setScale(7);
	bg->setAnchorPoint(Vec2(0, 0));
	bg->setPosition(Vec2(0, 0));
	this->addChild(bg);

	ready = Sprite::create("Images/ready.png");
	ready->setPosition(Vec2(winSize.width * 0.5, winSize.height * 2 / 3));
	ready->setScale(2);
	this->addChild(ready, 3);

	auto action = FadeOut::create(1);
	auto action2 = FadeIn::create(1);
	auto seq = Sequence::create(action, action2, nullptr);
	auto rep = RepeatForever::create(seq);

	ready->runAction(rep);

	this->createPlayer();

	SimpleAudioEngine::getInstance()->playBackgroundMusic("sounds/Game_Play_BGM.mp3");

	delVec.clear();

	return true;
}

bool Stage3::createBox2dWorld(bool debug)
{
	// 월드 생성 시작 ----------------------------------------------------

	// 중력의 방향을 결정한다.
	b2Vec2 gravity = b2Vec2(0.0f, -30.0f);

	_world = new b2World(gravity);
	_world->SetAllowSleeping(true);
	_world->SetContinuousPhysics(true);

	// 디버그 드로잉 설정
	if (debug)
	{
		// 적색 : 현재 물리 운동을 하는 것
		// 회색 : 현재 물리 운동량이 없는 것
		m_debugDraw = new GLESDebugDraw(PTM_RATIO);
		_world->SetDebugDraw(m_debugDraw);

		uint32 flags = 0;
		flags += b2Draw::e_shapeBit;
		//flags += b2Draw::e_jointBit;
		//flags += b2Draw::e_aabbBit;
		//flags += b2Draw::e_pairBit;
		//flags += b2Draw::e_centerOfMassBit;
		m_debugDraw->SetFlags(flags);

	}

	// 가장자리(테두리)를 지정해 공간(Ground Box)을 만든다.

	// 바디데프에 좌표를 설정한다.
	b2BodyDef groundBodyDef;
	groundBodyDef.position.Set(0, 0);

	// 월드에 바디데프의 정보(좌표)로 바디를 만든다.
	b2Body *groundBody = _world->CreateBody(&groundBodyDef);

	// 가장자리(테두리) 경계선을 그릴 수 있는 모양의 객체를 만든다.
	b2EdgeShape groundEdge;
	b2FixtureDef boxShapeDef;
	boxShapeDef.shape = &groundEdge;

	// 에지 모양의 객체에 Set(점1, 점2)로 선을 만든다.
	// 그리고 바디(groundBody)에 모양(groundEdge)을 고정시킨다.

	// 아래쪽
	groundEdge.Set(b2Vec2(0, 0), b2Vec2(winSize.width * 7 / PTM_RATIO, 0));
	groundBody->CreateFixture(&boxShapeDef);

	// 왼쪽
	groundEdge.Set(b2Vec2(0, 0), b2Vec2(0, winSize.height * 7 / PTM_RATIO));
	groundBody->CreateFixture(&boxShapeDef);

	// 위쪽
	groundEdge.Set(b2Vec2(0, winSize.height * 7 / PTM_RATIO), b2Vec2(winSize.width * 7 / PTM_RATIO, winSize.height * 7 / PTM_RATIO));
	groundBody->CreateFixture(&boxShapeDef);

	// 오른쪽
	groundEdge.Set(b2Vec2(winSize.width * 7 / PTM_RATIO, winSize.height * 7 / PTM_RATIO), b2Vec2(winSize.width * 7 / PTM_RATIO, 0));
	groundBody->CreateFixture(&boxShapeDef);

	// 월드 생성 끝 -----------------------------------------------------------

	pManBody = this->addNewSprite(Vec2(winSize.width * 6.8, 60), Size(40, 80), b2_dynamicBody, "test", 0);

	this->createWall();
	this->waySwich();
	this->createItem();
	this->createStar();
	this->schedule(schedule_selector(Stage3::movePlayer));
	this->schedule(schedule_selector(Stage3::createFire));
	this->schedule(schedule_selector(Stage3::createMeteor));
	
	return true;
}

Stage3::~Stage3()
{
	// 월드를 C++의 new로 생성했으므로 여기서 지워준다.
	delete _world;
	_world = nullptr;
}

void Stage3::createStar()
{
	// 아래 별
	for (int i = 1; i < 10; i++)
	{
		this->addNewSprite(Vec2(winSize.width + (i * 64), 40), Size(32, 32), b2_staticBody, "star", 2);
	}

	for (int i = 1; i < 20; i++)
	{
		if (i < 10)
		{
			this->addNewSprite(Vec2(winSize.width * 5.2 + (i * 64), 40), Size(32, 32), b2_staticBody, "star", 2);
			this->addNewSprite(Vec2(winSize.width * 5.2 + (i * 64), 100), Size(32, 32), b2_staticBody, "star", 2);
		}
		else if (i >= 10 && i < 20)
		{
			this->addNewSprite(Vec2(winSize.width * 5.2 + (i * 64), 100), Size(32, 32), b2_staticBody, "-star", 2);
		}
	}

	// 오른쪽 별
	for (int i = 0; i < 5; i++)
	{
		this->addNewSprite(Vec2(winSize.width * 7 - 40, winSize.height * 0.4 + (i * 64)), Size(32, 32), b2_staticBody, "-star", 2);
	}

	this->addNewSprite(Vec2(winSize.width * 7 - 300, winSize.height * 0.6), Size(128, 128), b2_staticBody, "100star", 2);

	for (int i = 0; i < 30; i++)
	{
		this->addNewSprite(Vec2(winSize.width * 7 - 40, winSize.height * 1.9 + (i * 64)), Size(32, 32), b2_staticBody, "star", 2);

		if (i < 8)
		{
			this->addNewSprite(Vec2(winSize.width * 7 - 100, winSize.height * 1.9 + (i * 64)), Size(32, 32), b2_staticBody, "star", 2);
		}
		else if (i > 15)
		{
			this->addNewSprite(Vec2(winSize.width * 7 - 100, winSize.height * 1.9 + (i * 64)), Size(32, 32), b2_staticBody, "star", 2);
		}
	}

	this->addNewSprite(Vec2(winSize.width * 7 - 400, winSize.height * 4.5), Size(128, 128), b2_staticBody, "100star", 2);

	for (int i = -10; i < 17; i++)
	{
		if (i < -2)
		{
			this->addNewSprite(Vec2(winSize.width * 7 - 40, winSize.height * 6 + (i * 64)), Size(32, 32), b2_staticBody, "star", 2);
		}
		else if (i >=0 && i < 4)
		{
			this->addNewSprite(Vec2(winSize.width * 7 - 180, winSize.height * 6 + (i * 64)), Size(32, 32), b2_staticBody, "star", 2);
		}
		else if (i >= 8 && i < 17)
		{
			this->addNewSprite(Vec2(winSize.width * 7 - 40, winSize.height * 6 + (i * 64)), Size(32, 32), b2_staticBody, "star", 2);
		}
	}
}

void Stage3::createFire(float f)
{
	// 아래 장애물
	if (fireNum == 0 && pManBody->GetPosition().x * 32 > winSize.width * 2.3) // 스케쥴로 불렀을때 계속해서 만들어지기 때문에 조건을 담
	{
		for (int i = 0; i < 4; i++)
		{
			this->addNewSprite(Vec2(winSize.width * 2.5 + 40.8 * i, 43.5), Size(40, 87), b2_staticBody, "fire", 0);
		}

		for (int i = 0; i < 4; i++)
		{
			this->addNewSprite(Vec2(winSize.width * 4.8 + 40.8 * i, 43.5), Size(40, 87), b2_staticBody, "fire", 0);
		}

		fireNum = 1;
	}

	// 오른쪽 장애물
	if (fireNum == 1)
	{
		for (int i = 0; i < 4; i++)
		{
			b2Body* pFire = this->addNewSprite(Vec2(winSize.width * 7 - 43.5 , winSize.height * 1.5 + 40.8 * i), Size(40, 87), b2_staticBody, "fire", 0);

			b2Vec2 pos = pFire->GetPosition();
			double DEGREES_TO_RADIANS = (double)(3.141592 / 180);
			float angle = (float)(90 * DEGREES_TO_RADIANS);
			pFire->SetTransform(pos, angle);
		}

		for (int i = 0; i < 4; i++)
		{
			b2Body* pFire = this->addNewSprite(Vec2(winSize.width * 7 - 43.5, winSize.height * 4 + 40.8 * i), Size(40, 87), b2_staticBody, "fire", 0);

			b2Vec2 pos = pFire->GetPosition();
			double DEGREES_TO_RADIANS = (double)(3.141592 / 180);
			float angle = (float)(90 * DEGREES_TO_RADIANS);
			pFire->SetTransform(pos, angle);
		}

		for (int i = 0; i < 4; i++)
		{
			b2Body* pFire = this->addNewSprite(Vec2(winSize.width * 7 - 43.5, winSize.height * 6 + 40.8 * i), Size(40, 87), b2_staticBody, "fire", 0);

			b2Vec2 pos = pFire->GetPosition();
			double DEGREES_TO_RADIANS = (double)(3.141592 / 180);
			float angle = (float)(90 * DEGREES_TO_RADIANS);
			pFire->SetTransform(pos, angle);
		}

		fireNum = 2;
	}

}

void Stage3::createMeteor(float f)
{
	// 아래 메테오
	if (meteorNum == 0 && pManBody->GetPosition().x * 32 > winSize.width * 1.7)
	{
		this->addNewSprite(Vec2(winSize.width * 3.5, winSize.height), Size(60, 60), b2_dynamicBody, "meteor", 1);
		meteor = 1;
		meteorNum = 1;
	}

	// 오른쪽 메테오
	else if (meteorNum == 1 && uBool && pManBody->GetPosition().y * 32 > winSize.height * 0.4)
	{
		for (int i = 0; i < 1; i++)
		{
			b2Body* pMeteor = this->addNewSprite(Vec2(winSize.width * 6, winSize.height * 0.8), Size(60, 60), b2_dynamicBody, "meteor", 1);

			b2Vec2 pos = pMeteor->GetPosition();
			double DEGREES_TO_RADIANS = (double)(3.141592 / 180);
			float angle = (float)(90 * DEGREES_TO_RADIANS);
			pMeteor->SetTransform(pos, angle);
		}
		meteorNum = 2;
	}
	else if (meteorNum == 2 && uBool && pManBody->GetPosition().y * 32 > winSize.height * 3.5)
	{
		for (int i = 0; i < 1; i++)
		{
			b2Body* pMeteor = this->addNewSprite(Vec2(winSize.width * 5, winSize.height * 5), Size(60, 60), b2_dynamicBody, "meteor", 1);

			b2Vec2 pos = pMeteor->GetPosition();
			double DEGREES_TO_RADIANS = (double)(3.141592 / 180);
			float angle = (float)(90 * DEGREES_TO_RADIANS);
			pMeteor->SetTransform(pos, angle);
		}
		meteorNum = 3;
	}
}

void Stage3::createWall()
{
	// 아래 벽
	//this->addNewSprite(Vec2(winSize.width * 2.9, 20), Size(50, 40), b2_staticBody, "wallV", 0);

	// 오른쪽 벽
	for (int i = 0; i < 1; i++)
	{
		b2Body* pWall = this->addNewSprite(Vec2(winSize.width * 7 - 120, winSize.height * 0.5), Size(336, 50), b2_staticBody, "wall", 0);

		b2Vec2 pos = pWall->GetPosition();
		double DEGREES_TO_RADIANS = (double)(3.141592 / 180);
		float angle = (float)(90 * DEGREES_TO_RADIANS);
		pWall->SetTransform(pos, angle);
	}

	for (int i = 0; i < 1; i++)
	{
		b2Body* pWall = this->addNewSprite(Vec2(winSize.width * 7 - 200, winSize.height * 1.8), Size(50, 200), b2_staticBody, "wallV", 0);

		b2Vec2 pos = pWall->GetPosition();
		double DEGREES_TO_RADIANS = (double)(3.141592 / 180);
		float angle = (float)(90 * DEGREES_TO_RADIANS);
		pWall->SetTransform(pos, angle);
	}

	for (int i = 0; i < 1; i++)
	{
		b2Body* pWall = this->addNewSprite(Vec2(winSize.width * 7 - 120, winSize.height * 2.5), Size(336, 50), b2_staticBody, "wall", 0);

		b2Vec2 pos = pWall->GetPosition();
		double DEGREES_TO_RADIANS = (double)(3.141592 / 180);
		float angle = (float)(90 * DEGREES_TO_RADIANS);
		pWall->SetTransform(pos, angle);
	}

	for (int i = 0; i < 1; i++)
	{
		b2Body* pWall = this->addNewSprite(Vec2(winSize.width * 7 - 220, winSize.height * 3.1), Size(336, 50), b2_staticBody, "wall", 0);

		b2Vec2 pos = pWall->GetPosition();
		double DEGREES_TO_RADIANS = (double)(3.141592 / 180);
		float angle = (float)(90 * DEGREES_TO_RADIANS);
		pWall->SetTransform(pos, angle);
	}

	for (int i = 0; i < 1; i++)
	{
		b2Body* pWall = this->addNewSprite(Vec2(winSize.width * 7 - 320, winSize.height * 4), Size(336, 50), b2_staticBody, "wall", 0);

		b2Vec2 pos = pWall->GetPosition();
		double DEGREES_TO_RADIANS = (double)(3.141592 / 180);
		float angle = (float)(90 * DEGREES_TO_RADIANS);
		pWall->SetTransform(pos, angle);
	}
}

void Stage3::createItem()
{
	// 아래 아이템
	shieldItem = this->addNewSprite(Vec2(winSize.width * 2.6, 180),
		Size(72, 72), b2_staticBody, "shield", 2);

	// 오른쪽 아이템
	for (int i = 0; i < 1; i++)
	{
		jumpItem = this->addNewSprite(Vec2(winSize.width * 7 - 100, winSize.height * 0.2),
			Size(72, 72), b2_staticBody, "jump", 2);

		b2Vec2 pos = jumpItem->GetPosition();
		double DEGREES_TO_RADIANS = (double)(3.141592 / 180);
		float angle = (float)(90 * DEGREES_TO_RADIANS);
		jumpItem->SetTransform(pos, angle);
	}

	for (int i = 0; i < 1; i++)
	{
		shieldItem = this->addNewSprite(Vec2(winSize.width * 7 - 180, winSize.height * 4.1),
			Size(72, 72), b2_staticBody, "shield", 2);

		b2Vec2 pos = shieldItem->GetPosition();
		double DEGREES_TO_RADIANS = (double)(3.141592 / 180);
		float angle = (float)(90 * DEGREES_TO_RADIANS);
		shieldItem->SetTransform(pos, angle);
	}
}

void Stage3::createPlayer()
{
	auto texture = Director::getInstance()->getTextureCache()->addImage("Images/Punk_Run.png");
	pMan = Sprite::createWithTexture(texture, Rect(512, 0, 256, 256));
	pMan->setPosition(Vec2(100, 60));
	pMan->setScale(0.5f);
	this->addChild(pMan);
}

void Stage3::createScore()
{
	if (rBool)
	{
		if (pManBody->GetPosition().x * 32 < winSize.width * 0.5)
		{
			removeChild(Score);

			sprintf(str, "%d", nowScore);
			Score = LabelAtlas::create(str, "Images/score_number.png", 24, 34, '0');
			Score->setScale(3);
			Score->setPosition(Vec2(winSize.width * 0.5, winSize.height * 0.7));
			this->addChild(Score, 3);

		}
		else if (pManBody->GetPosition().x * 32 >= winSize.width * 0.5 &&
			pManBody->GetPosition().x * 32 < winSize.width * 6.3)
		{
			removeChild(Score);

			sprintf(str, "%d", nowScore);
			Score = LabelAtlas::create(str, "Images/score_number.png", 24, 34, '0');
			Score->setScale(3);
			Score->setPosition(Vec2(pManBody->GetPosition().x * 32, winSize.height * 0.7));
			this->addChild(Score, 3);

		}

		else if (pManBody->GetPosition().x * 32 >= winSize.width * 6.3)
		{
			removeChild(Score);

			sprintf(str, "%d", nowScore);
			Score = LabelAtlas::create(str, "Images/score_number.png", 24, 34, '0');
			Score->setScale(3);
			Score->setPosition(Vec2(winSize.width * 6.3, winSize.height * 0.7));
			this->addChild(Score, 3);
		}
	}

	else if (uBool)
	{
		if (pManBody->GetPosition().y * 32 < winSize.height * 0.7)
		{
			removeChild(Score);

			sprintf(str, "%d", nowScore);
			Score = LabelAtlas::create(str, "Images/score_number.png", 24, 34, '0');
			Score->setScale(3);
			Score->setPosition(Vec2(winSize.width * 6.3, winSize.height * 0.7));
			this->addChild(Score, 3);
		}
		else if (pManBody->GetPosition().y * 32 >= winSize.height * 0.5 &&
			pManBody->GetPosition().y * 32 < winSize.height * 6.3)
		{
			removeChild(Score);

			sprintf(str, "%d", nowScore);
			Score = LabelAtlas::create(str, "Images/score_number.png", 24, 34, '0');
			Score->setScale(3);
			Score->setPosition(Vec2(winSize.width * 6.3, pManBody->GetPosition().y * 32));
			this->addChild(Score, 3);
		}

		else if (pManBody->GetPosition().y * 32 >= winSize.height * 6.3)
		{
			removeChild(Score);

			sprintf(str, "%d", nowScore);
			Score = LabelAtlas::create(str, "Images/score_number.png", 24, 34, '0');
			Score->setScale(3);
			Score->setPosition(Vec2(winSize.width * 6.3, winSize.height * 6.3));
			this->addChild(Score, 3);
		}

	}

	else if (lBool)
	{
		if (pManBody->GetPosition().x * 32 > winSize.width * 6.3)
		{
			removeChild(Score);

			sprintf(str, "%d", nowScore);
			Score = LabelAtlas::create(str, "Images/score_number.png", 24, 34, '0');
			Score->setScale(3);
			Score->setPosition(Vec2(winSize.width * 6.3, winSize.height * 6.3));
			this->addChild(Score, 3);
		}

		else if (pManBody->GetPosition().x * 32 <= winSize.width * 6.3 &&
			pManBody->GetPosition().x * 32 > winSize.width * 0.7)
		{
			removeChild(Score);

			sprintf(str, "%d", nowScore);
			Score = LabelAtlas::create(str, "Images/score_number.png", 24, 34, '0');
			Score->setScale(3);
			Score->setPosition(Vec2(pManBody->GetPosition().x * 32, winSize.height * 6.3));
			this->addChild(Score, 3);
		}
		else if (pManBody->GetPosition().x * 32 <= winSize.width * 0.7)
		{
			removeChild(Score);

			sprintf(str, "%d", nowScore);
			Score = LabelAtlas::create(str, "Images/score_number.png", 24, 34, '0');
			Score->setScale(3);
			Score->setPosition(Vec2(winSize.width * 0.7, winSize.height * 6.3));
			this->addChild(Score, 3);
		}
	}

	else if (dBool)
	{
		if (pManBody->GetPosition().y * 32 > winSize.height * 6.3)
		{
			removeChild(Score);

			sprintf(str, "%d", nowScore);
			Score = LabelAtlas::create(str, "Images/score_number.png", 24, 34, '0');
			Score->setScale(3);
			Score->setPosition(Vec2(winSize.width * 0.7, winSize.height * 6.3));
			this->addChild(Score, 3);
		}
		else if (pManBody->GetPosition().y * 32 <= winSize.height * 6.3 &&
			pManBody->GetPosition().y * 32 > winSize.height * 0.7)
		{
			removeChild(Score);

			sprintf(str, "%d", nowScore);
			Score = LabelAtlas::create(str, "Images/score_number.png", 24, 34, '0');
			Score->setScale(3);
			Score->setPosition(Vec2(winSize.width * 0.7, pManBody->GetPosition().y * 32));
			this->addChild(Score, 3);
		}
		else if (pManBody->GetPosition().y * 32 < winSize.height * 0.7)
		{
			removeChild(Score);

			sprintf(str, "%d", nowScore);
			Score = LabelAtlas::create(str, "Images/score_number.png", 24, 34, '0');
			Score->setScale(3);
			Score->setPosition(Vec2(winSize.width * 0.7, winSize.height * 0.7));
			this->addChild(Score, 3);
		}
	}
}

void Stage3::overScore(float f)
{
	float ni = 0;

	if (nowScore != 0)
	{
		ni = 1 / nowScore;
	}

	NowScore = LabelAtlas::create("0", "Images/score_number.png", 24, 34, '0');
	NowScore->setPosition(Vec2(180, 50));
	NowScore->setScale(2);
	bord->addChild(NowScore, 3);

	this->schedule(schedule_selector(Stage3::overBestScore), ni);

	sprintf(str, "%d", bScore3);
	BestScore = LabelAtlas::create(str, "Images/score_number.png", 24, 34, '0');
	BestScore->setPosition(Vec2(180, 250));
	BestScore->setScale(2);
	bord->addChild(BestScore, 3);
}

void Stage3::overBestScore(float f)
{
	bord->removeChild(NowScore, true);
	sprintf(str, "%d", nScore);
	NowScore = LabelAtlas::create(str, "Images/score_number.png", 24, 34, '0');
	NowScore->setScale(2);
	NowScore->setPosition(Vec2(180, 50));
	bord->addChild(NowScore, 3);

	if (nowScore == nScore)
	{
		if (nowScore > bScore3)
		{
			removeChild(BestScore);

			bScore3 = nowScore;
			sprintf(str, "%d", bScore3);
			BestScore = LabelAtlas::create(str, "Images/score_number.png", 24, 34, '0');
			BestScore->setPosition(Vec2(180, 250));
			BestScore->setScale(2);
			bord->addChild(BestScore, 3);

		}
		this->unschedule(schedule_selector(Stage3::overBestScore));

	}

	nScore++;
}

void Stage3::onEnter()
{
	Layer::onEnter();

	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(Stage3::onTouchBegan, this);
	//	listener->onTouchMoved = CC_CALLBACK_2(Stage3::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(Stage3::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void Stage3::onExit()
{
	//_eventDispatcher->removeAllEventListeners();
	Layer::onExit();
}

void Stage3::tick(float dt)
{
	// 객체 제거
	//*************************************************

	std::vector<b2Body*>::iterator it = delVec.begin();
	// loop through, increasing to next element until the end is reached
	for (; it != delVec.end(); ++it) {
		auto obj = (b2Body*)(*it);

		Sprite* spriteData = (Sprite *)obj->GetUserData();
		int nTag = spriteData->getTag();

		log("Tag .. %d", nTag);

		// 스프라이트 삭제
		//obj->SetUserData(nullptr);
		this->removeChild(spriteData, true);

		// 물리객체 삭제
		_world->DestroyBody(obj);
	}
	delVec.clear();

	//*******************************************

	int velocityIterations = 8;
	int positionIterations = 3;

	// Step : 물리 세계를 시뮬레이션한다.
	_world->Step(dt, velocityIterations, positionIterations);

	// 모든 물리 객체들은 링크드 리스트에 저장되어 참조해 볼 수 있게 구현돼 있다.
	// 만들어진 객체만큼 루프를 돌리면서 바디에 붙인 스프라이트를 여기서 제어한다.
	for (b2Body *b = _world->GetBodyList(); b; b = b->GetNext())
	{
		if (b->GetUserData() != nullptr)
		{
			Sprite* spriteData = (Sprite *)b->GetUserData();
			spriteData->setPosition(Vec2(b->GetPosition().x * PTM_RATIO, b->GetPosition().y * PTM_RATIO));
			spriteData->setRotation(-1 * CC_RADIANS_TO_DEGREES(b->GetAngle()));
		}

		//if (b->GetType() == b2_kinematicBody)
		//{
		//	if (b->GetPosition().y * 32 > winSize.height * 6.5)
		//	{
		//		b->SetLinearVelocity(b2Vec2(0, -10.0f));
		//	}
		//	else if (b->GetPosition().y * 32 < winSize.height * 6)
		//	{
		//		b->SetLinearVelocity(b2Vec2(0, 10.0f));
		//	}
		//}
	}

	// 쉴드
	if (sum == 1)
	{
		removeChild(barrier);
	}
	sum = 1;
	if (shield && shieldNum > 0)
	{
		barrier = Sprite::create("Images/shieldWhite.png");
		barrier->setPosition(Vec2(pManBody->GetPosition().x * 32, pManBody->GetPosition().y * 32));

		if (shieldNum == 1)
		{
			barrier->setOpacity(50);
		}
		barrier->setScale(0.5);
		this->addChild(barrier);

	}


	if (rBool)
	{
		b2Vec2 pos = pManBody->GetPosition();
		double DEGREES_TO_RADIANS = (double)(3.141592 / 180); // 회전 공식
		float angle = (float)(360 * DEGREES_TO_RADIANS);
		pManBody->SetTransform(pos, angle);

		pManBody->ApplyLinearImpulse(b2Vec2(0.5f, 0), pManBody->GetWorldCenter(), true);

		if (playerIsFlying)
		{
			pManBody->ApplyLinearImpulse(b2Vec2(0, playerVelocity), pManBody->GetWorldCenter(), true);

			playerVelocity = playerVelocity + 0.09f;

			if (playerVelocity > 30.0f)
			{
				playerIsFlying = false;
				playerVelocity = 0.0f;
			}

		}

		// 방향 전환할때 각 벽 끝에 있는 staticBody에 몇번을 부딪혀도
		// 중력이 계속해서 바뀌지않도록 하기위한 코드
		if (pManBody->GetPosition().x > 70)
		{
			swichNum = 1;
		}

		// 2단 점프
		if (jSum == 1)
		{
			removeChild(jumpjump);
		}
		jSum = 1;
		if (doubleJump > 0)
		{
			jumpjump = Sprite::create("Images/jump.png");
			jumpjump->setPosition(Vec2(pManBody->GetPosition().x * 32, pManBody->GetPosition().y * 32 + 100));
			jumpjump->setScale(0.5);
			this->addChild(jumpjump);

		}

	}
	else if (uBool)
	{

		b2Vec2 pos = pManBody->GetPosition();
		double DEGREES_TO_RADIANS = (double)(3.141592 / 180);
		float angle = (float)(90 * DEGREES_TO_RADIANS);
		pManBody->SetTransform(pos, angle);

		pManBody->ApplyLinearImpulse(b2Vec2(0, 0.5f), pManBody->GetWorldCenter(), true);

		if (playerIsFlying)
		{
			pManBody->ApplyLinearImpulse(b2Vec2(-playerVelocity, 0), pManBody->GetWorldCenter(), true);

			playerVelocity = playerVelocity + 0.09f;

			if (playerVelocity > 30.0f)
			{
				playerIsFlying = false;
				playerVelocity = 0.0f;
			}
		}

		// 방향 전환할때 각 벽 끝에 있는 staticBody에 몇번을 부딪혀도
		// 중력이 계속해서 바뀌지않도록 하기위한 코드
		if (pManBody->GetPosition().y > 70)
		{
			swichNum = 2;
		}

		// 2단 점프
		if (jSum == 1)
		{
			removeChild(jumpjump);
		}
		jSum = 1;
		if (dJump)
		{
			jumpjump = Sprite::create("Images/jump.png");
			jumpjump->setPosition(Vec2(pManBody->GetPosition().x * 32 - 100, pManBody->GetPosition().y * 32));
			jumpjump->setScale(0.5);
			jumpjump->setRotation(270);
			this->addChild(jumpjump);

		}


	}
	else if (lBool)
	{

		b2Vec2 pos = pManBody->GetPosition();
		double DEGREES_TO_RADIANS = (double)(3.141592 / 180);
		float angle = (float)(180 * DEGREES_TO_RADIANS);
		pManBody->SetTransform(pos, angle);

		pManBody->ApplyLinearImpulse(b2Vec2(-0.5f, 0), pManBody->GetWorldCenter(), true);

		if (playerIsFlying)
		{
			pManBody->ApplyLinearImpulse(b2Vec2(0, -playerVelocity), pManBody->GetWorldCenter(), true);

			playerVelocity = playerVelocity + 0.09f;

			if (playerVelocity > 30.0f)
			{
				playerIsFlying = false;
				playerVelocity = 0.0f;
			}
		}

		// 방향 전환할때 각 벽 끝에 있는 staticBody에 몇번을 부딪혀도
		// 중력이 계속해서 바뀌지않도록 하기위한 코드
		if (pManBody->GetPosition().x < 70)
		{
			swichNum = 3;
		}

		// 2단 점프
		if (jSum == 1)
		{
			removeChild(jumpjump);
		}
		jSum = 1;
		if (doubleJump > 0)
		{
			jumpjump = Sprite::create("Images/jump.png");
			jumpjump->setPosition(Vec2(pManBody->GetPosition().x * 32, pManBody->GetPosition().y * 32 - 100));
			jumpjump->setScale(0.5);
			jumpjump->setRotation(180);
			this->addChild(jumpjump);
		}


	}
	else if (dBool)
	{

		b2Vec2 pos = pManBody->GetPosition();
		double DEGREES_TO_RADIANS = (double)(3.141592 / 180);
		float angle = (float)(270 * DEGREES_TO_RADIANS);
		pManBody->SetTransform(pos, angle);

		pManBody->ApplyLinearImpulse(b2Vec2(0, -0.5f), pManBody->GetWorldCenter(), true);

		if (playerIsFlying)
		{
			pManBody->ApplyLinearImpulse(b2Vec2(playerVelocity, 0), pManBody->GetWorldCenter(), true);

			playerVelocity = playerVelocity + 0.09f;

			if (playerVelocity > 30.0f)
			{
				playerIsFlying = false;
				playerVelocity = 0.0f;
			}
		}

		// 방향 전환할때 각 벽 끝에 있는 staticBody에 몇번을 부딪혀도
		// 중력이 계속해서 바뀌지않도록 하기위한 코드
		if (pManBody->GetPosition().y < 70)
		{
			swichNum = 4;
		}

		// 2단 점프
		if (jSum == 1)
		{
			removeChild(jumpjump);
		}
		jSum = 1;
		if (doubleJump > 0)
		{
			jumpjump = Sprite::create("Images/jump.png");
			jumpjump->setPosition(Vec2(pManBody->GetPosition().x * 32 + 100, pManBody->GetPosition().y * 32));
			jumpjump->setScale(0.5);
			jumpjump->setRotation(90);
			this->addChild(jumpjump);
		}
	}

	if (pManBody->GetPosition().x * 32 > winSize.width * 2)
	{
		//removeChild(cover);
	}

	this->createScore();
}

b2Body* Stage3::addNewSprite(Vec2 point, Size size, b2BodyType bodytype, const char* spriteName, int type)
{
	// 바디데프를 만들고 속성들을 지정한다.
	b2BodyDef bodyDef;
	bodyDef.type = bodytype;
	bodyDef.position.Set(point.x / PTM_RATIO, point.y / PTM_RATIO);

	if (spriteName)
	{
		if (strcmp(spriteName, "test") == 0)
		{
			auto animation = Animation::create();
			animation->setDelayPerUnit(0.05f);

			for (int i = 0; i < 8; i++)
			{
				animation->addSpriteFrameWithTexture(
					texture,
					Rect(i * 256, 0, 256, 256));
			}

			pMan = Sprite::createWithTexture(texture, Rect(0, 0, 256, 256));
			pMan->setPosition(Vec2(point));
			this->addChild(pMan);

			auto animate = Animate::create(animation);
			auto rep = RepeatForever::create(animate);
			pMan->setAnchorPoint(Vec2(0.5f, 0.4f));
			pMan->setScale(0.5f);
			pMan->runAction(rep);

			pMan->setTag(0);
			bodyDef.userData = pMan;
		}
		else if (strcmp(spriteName, "fire") == 0)
		{
			auto fire_animation = Animation::create();
			fire_animation->setDelayPerUnit(0.07f);

			for (int i = 0; i < 5; i++)
			{
				fire_animation->addSpriteFrameWithTexture(
					texture2,
					Rect(i * 27.2, 0, 27.2, 65));
			}

			Sprite* fire = Sprite::createWithTexture(texture2, Rect(0, 0, 27.2, 58));

			auto fire_animate = Animate::create(fire_animation);
			auto rep = RepeatForever::create(fire_animate);
			fire->setScale(1.5f);
			fire->runAction(rep);
			fire->setPosition(Vec2(point));
			this->addChild(fire);

			fire->setTag(1);
			bodyDef.userData = fire;
		}
		else if (strcmp(spriteName, "wall") == 0)
		{
			Sprite* sprite = Sprite::create("Images/wallH.png");
			sprite->setPosition(Vec2(point));
			this->addChild(sprite);

			sprite->setTag(4);
			bodyDef.userData = sprite;
		}
		else if (strcmp(spriteName, "smallWall") == 0)
		{
			Sprite* sprite = Sprite::create("Images/wallH.png");
			sprite->setScaleX(0.5);
			sprite->setPosition(Vec2(point));
			this->addChild(sprite);

			sprite->setTag(4);
			bodyDef.linearVelocity = b2Vec2(0, 10.0f);
			bodyDef.userData = sprite;
		}
		else if (strcmp(spriteName, "wallV") == 0)
		{
			Sprite* sprite = Sprite::create("Images/wallV.png");
			sprite->setPosition(Vec2(point));
			this->addChild(sprite);

			bodyDef.userData = sprite;
		}
		else if (strcmp(spriteName, "smallWallV") == 0)
		{
			Sprite* sprite = Sprite::create("Images/wallV.png");
			sprite->setScaleY(0.5);
			sprite->setPosition(Vec2(point));
			this->addChild(sprite);

			sprite->setTag(4);
			bodyDef.userData = sprite;
		}
		else if (strcmp(spriteName, "shield") == 0)
		{
			Sprite* sprite = Sprite::create("Images/shield.png");
			sprite->setScale(3);
			sprite->setPosition(Vec2(point));
			this->addChild(sprite);

			sprite->setTag(3);
			bodyDef.userData = sprite;
		}
		else if (strcmp(spriteName, "jump") == 0)
		{
			Sprite* sprite = Sprite::create("Images/jump.png");
			sprite->setPosition(Vec2(point));
			this->addChild(sprite);

			sprite->setTag(5);
			bodyDef.userData = sprite;
		}
		else if (strcmp(spriteName, "star") == 0)
		{
			auto star_animation = Animation::create();
			star_animation->setDelayPerUnit(0.05f);

			for (int i = 0; i < 8; i++)
			{
				star_animation->addSpriteFrameWithTexture(
					texture3,
					Rect(i * 32, 0, 32, 32));
			}

			Sprite* star = Sprite::createWithTexture(texture3, Rect(0, 0, 32, 32));

			auto star_animate = Animate::create(star_animation);
			auto rep = RepeatForever::create(star_animate);
			star->setScale(1.5f);
			star->runAction(rep);
			star->setPosition(Vec2(point));
			this->addChild(star);

			star->setTag(2);
			bodyDef.userData = star;
		}
		else if (strcmp(spriteName, "-star") == 0)
		{
			auto star_animation = Animation::create();
			star_animation->setDelayPerUnit(0.05f);

			for (int i = 0; i < 8; i++)
			{
				star_animation->addSpriteFrameWithTexture(
					texture3,
					Rect(i * 32, 0, 32, 32));
			}

			Sprite* star = Sprite::createWithTexture(texture3, Rect(0, 0, 32, 32));

			auto star_animate = Animate::create(star_animation);
			auto rep = RepeatForever::create(star_animate);
			star->setColor(Color3B::RED);
			star->setScale(1.5f);
			star->runAction(rep);
			star->setPosition(Vec2(point));
			this->addChild(star);

			star->setTag(9);
			bodyDef.userData = star;
		}
		else if (strcmp(spriteName, "100star") == 0)
		{
			auto star_animation = Animation::create();
			star_animation->setDelayPerUnit(0.05f);

			for (int i = 0; i < 8; i++)
			{
				star_animation->addSpriteFrameWithTexture(
					texture3,
					Rect(i * 32, 0, 32, 32));
			}

			Sprite* star = Sprite::createWithTexture(texture3, Rect(0, 0, 32, 32));

			auto star_animate = Animate::create(star_animation);
			auto rep = RepeatForever::create(star_animate);
			star->setScale(4);
			star->runAction(rep);
			star->setPosition(Vec2(point));
			this->addChild(star);

			star->setTag(7);
			bodyDef.userData = star;
		}
		else if (strcmp(spriteName, "way") == 0)
		{
			Sprite* sprite = Sprite::create();
			sprite->setPosition(Vec2(point));
			this->addChild(sprite);

			sprite->setTag(6);
			bodyDef.userData = sprite;
		}
		else if (strcmp(spriteName, "meteor") == 0)
		{
			auto meteor_animation = Animation::create();
			meteor_animation->setDelayPerUnit(0.05f);

			for (int i = 0; i < 11; i++)
			{
				int column = i % 4;
				int row = i / 4;

				meteor_animation->addSpriteFrameWithTexture(
					texture4,
					Rect(column * 232.25, row * 396.6, 232.25, 396.6));
			}

			Sprite* meteor = Sprite::createWithTexture(texture3, Rect(0, 0, 232.25, 396.6));

			auto meteor_animate = Animate::create(meteor_animation);
			auto rep = RepeatForever::create(meteor_animate);
			meteor->setAnchorPoint(Vec2(0.5, 0.3));
			meteor->setScale(0.5);
			meteor->runAction(rep);
			meteor->setPosition(Vec2(point));
			this->addChild(meteor);

			meteor->setTag(8);
			bodyDef.userData = meteor;
		}
	}
	// 월드에 바디데프의 정보로 바디를 만든다.
	b2Body *body = _world->CreateBody(&bodyDef);

	// 바디에 적용할 물리 속성용 바디의 모양을 만든다.
	b2FixtureDef fixtureDef;
	b2PolygonShape dynamicBox;
	b2CircleShape circle;

	if (type == 0)
	{
		dynamicBox.SetAsBox(size.width / 2 / PTM_RATIO, size.height / 2 / PTM_RATIO);

		fixtureDef.shape = &dynamicBox;
	}
	else if (type == 1)
	{
		circle.m_radius = (size.width / 2) / PTM_RATIO;

		fixtureDef.shape = &circle;
	}
	else if (type == 2)
	{
		dynamicBox.SetAsBox(size.width / 2 / PTM_RATIO, size.height / 2 / PTM_RATIO);

		fixtureDef.shape = &dynamicBox;
		fixtureDef.isSensor = true;
	}

	// Define the dynamic body fixture.
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.1f;
	fixtureDef.restitution = 0.0f;

	body->CreateFixture(&fixtureDef);

	return body;
}

bool Stage3::onTouchBegan(Touch* touch, Event* event)
{
	auto touchPoint = touch->getLocation();

	if (!_world)
	{
		removeChild(ready);
		removeChild(pMan);

		go = Sprite::create("Images/go.png");
		go->setPosition(Vec2(winSize.width * 0.5, winSize.height * 2 / 3));
		go->setScale(4);
		this->addChild(go, 3);

		auto action = FadeOut::create(1);

		go->runAction(action);

		playerVelocity = 0.0f;
		// 월드 생성
		if (this->createBox2dWorld(true))
		{
			_world->SetContactListener((b2ContactListener*)this);
			this->schedule(schedule_selector(Stage3::tick));
			log("%f .. %f", winSize.width, winSize.height);
		}
	}
	else if (jumpBool)  // 점프 한번만 가능하게끔
	{
		if (rBool && (pManBody->GetPosition().y * 32 < 50 || jump || dJump))
		{
			playerVelocity = 29.9f;
			playerIsFlying = true;
			jump = false;

			m_nSoundId = SimpleAudioEngine::getInstance()->playEffect("sounds/Jump.mp3");

			if (pManBody->GetPosition().y * 32 > 50 && dJump)
			{
				doubleJump--;
				if (doubleJump == 0)
				{
					dJump = false;
				}
			}
		}
		else if (uBool && (pManBody->GetPosition().x * 32 > winSize.width * 7 - 50 || jump || dJump))
		{
			playerVelocity = 29.9f;
			playerIsFlying = true;
			jump = false;

			m_nSoundId = SimpleAudioEngine::getInstance()->playEffect("sounds/Jump.mp3");

			if (pManBody->GetPosition().x * 32 < winSize.width * 7 - 50 && dJump)
			{
				doubleJump--;
				if (doubleJump == 0)
				{
					dJump = false;
				}
			}
		}
		else if (lBool && (pManBody->GetPosition().y * 32 > winSize.height * 7 - 50 || jump || dJump))
		{
			playerVelocity = 29.9f;
			playerIsFlying = true;
			jump = false;

			m_nSoundId = SimpleAudioEngine::getInstance()->playEffect("sounds/Jump.mp3");

			if (pManBody->GetPosition().y * 32 < winSize.height * 7 - 50 && dJump)
			{
				doubleJump--;
				if (doubleJump == 0)
				{
					dJump = false;
				}
			}
		}
		else if (dBool && (pManBody->GetPosition().x * 32 < 50 || jump || dJump)) // jump = 발판위에서도 점프가 가능하게끔 설정
		{
			playerVelocity = 29.9f;
			playerIsFlying = true;
			jump = false;

			m_nSoundId = SimpleAudioEngine::getInstance()->playEffect("sounds/Jump.mp3");

			if (pManBody->GetPosition().x * 32 > 50 && dJump)
			{
				doubleJump--;
				if (doubleJump == 0)
				{
					dJump = false;
				}
			}
		}
	}

	jumpBool = true;
	return true;
}

void Stage3::onTouchEnded(Touch *touch, Event *event)
{
	playerIsFlying = false;
	playerVelocity = 0.0f;

	auto touchPoint = touch->getLocation();

}

void Stage3::onDraw(const Mat4 &transform, uint32_t flags)
{
	if (!_world)
	{
		return;
	}

	Director* director = Director::getInstance();
	CCASSERT(nullptr != director, "Director is null when seting matrix stack");
	director->pushMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
	director->loadMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW, transform);

	GL::enableVertexAttribs(cocos2d::GL::VERTEX_ATTRIB_FLAG_POSITION);
	_world->DrawDebugData();
	CHECK_GL_ERROR_DEBUG();

	director->popMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
}

void Stage3::draw(Renderer *renderer, const Mat4 &transform, uint32_t flags)
{
	if (!_world)
	{
		return;
	}

	Layer::draw(renderer, transform, flags);

	_customCmd.init(_globalZOrder, transform, flags);
	_customCmd.func = CC_CALLBACK_0(Stage3::onDraw, this, transform, flags);
	renderer->addCommand(&_customCmd);
}

void Stage3::movePlayer(float f)
{
	this->runAction(Follow::create(pMan, Rect(0, 0, winSize.width * 7, winSize.height * 7)));
}

void Stage3::BeginContact(b2Contact *contact)
{
	b2Fixture *fixA = contact->GetFixtureA();
	b2Fixture *fixB = contact->GetFixtureB();

	b2Body *bodyA = fixA->GetBody();
	b2Body *bodyB = fixB->GetBody();

	Sprite* spriteData = (Sprite *)bodyB->GetUserData();
	int nTag = spriteData->getTag();

	// ------- 중력 변경 ----------
	if (bodyA->GetType() == b2_dynamicBody && bodyB->GetType() == b2_staticBody && nTag == 6)
	{
		if (rBool && swichNum == 1)
		{
			_world->SetGravity(b2Vec2(30.0f, 0));
			rBool = false;
			uBool = true;
		}
		else if (uBool && swichNum == 2)
		{
			_world->SetGravity(b2Vec2(0, 30.0f));
			uBool = false;
			lBool = true;
		}
		else if (lBool && swichNum == 3)
		{
			_world->SetGravity(b2Vec2(-30.0f, 0));
			lBool = false;
			dBool = true;
		}
		else if (dBool && swichNum == 4)
		{
			_world->SetGravity(b2Vec2(0, -30.0f));
			dBool = false;
			rBool = true;

			gameClear = 1;
			Director::getInstance()->getActionManager()->pauseAllRunningActions();
			this->unschedule(schedule_selector(Stage3::tick));
			this->scheduleOnce(schedule_selector(Stage3::gameOver), 1);
		}

	}
	// ------- 중력 변경끝 --------

	else if (bodyA->GetType() == b2_dynamicBody && (bodyB->GetType() == b2_staticBody || bodyB->GetType() == b2_kinematicBody || bodyB->GetType() == b2_dynamicBody))
	{
		if (bodyB->GetUserData() != nullptr)
		{
			if (nTag == 1)
			{
				if (shield == false)
				{
					m_nSoundId = SimpleAudioEngine::getInstance()->playEffect("sounds/dead.wav");
					SimpleAudioEngine::getInstance()->stopBackgroundMusic(true);

					this->scheduleOnce(schedule_selector(Stage3::gameOver), 1);
					// 애니메이션 액션 멈추기
					Director::getInstance()->getActionManager()->pauseAllRunningActions();
					this->unschedule(schedule_selector(Stage3::tick));

					log("game over");

				}
				else
				{
					m_nSoundId = SimpleAudioEngine::getInstance()->playEffect("sounds/Shield.wav");

					if (shieldNum == 2)
					{
						shieldNum--;

					}
					else if (shieldNum == 1)
					{
						shieldNum--;
						this->removeChild(barrier, true);
					}
					else if(shieldNum == 0)
					{
						shield = false;
					}
				}
			}
			else if (nTag == 2)
			{
				m_nSoundId = SimpleAudioEngine::getInstance()->playEffect("sounds/Coin1.wav");

				SimpleAudioEngine::getInstance()->setEffectsVolume(2.0f);

				delVec.push_back(bodyB);
				nowScore++;
			}
			else if (nTag == 3)
			{
				m_nSoundId = SimpleAudioEngine::getInstance()->playEffect("sounds/Item.wav");
				b2Vec2 pos = pManBody->GetPosition();
				delVec.push_back(bodyB);
				shieldNum = 2;
				shield = true;
			}
			else if (nTag == 4)
			{
				jump = true;
			}
			else if (nTag == 5)
			{
				m_nSoundId = SimpleAudioEngine::getInstance()->playEffect("sounds/Item.wav");

				delVec.push_back(bodyB);
				doubleJump = 1;
				dJump = true;
			}
			else if (nTag == 7)
			{
				m_nSoundId = SimpleAudioEngine::getInstance()->playEffect("sounds/Coin1.wav");

				SimpleAudioEngine::getInstance()->setEffectsVolume(2.0f);

				delVec.push_back(bodyB);
				nowScore = nowScore + 100;
			}
			else if (nTag == 8)
			{
				if (shield == false)
				{
					auto explosion_animation = Animation::create();
					explosion_animation->setDelayPerUnit(0.1f);

					for (int i = 0; i < 24; i++)
					{
						int column = i % 8;
						int row = i / 3;

						explosion_animation->addSpriteFrameWithTexture(
							texture5,
							Rect(column * 64, row * 85.3, 64, 85.3));
					}

					Sprite* explosion = Sprite::createWithTexture(texture5, Rect(0, 0, 64, 85.3));

					auto explosion_animate = Animate::create(explosion_animation);
					explosion->setScale(3);
					explosion->runAction(explosion_animate);
					explosion->setPosition(Vec2(bodyB->GetPosition().x * 32, bodyB->GetPosition().y * 32));
					this->addChild(explosion);

					delVec.push_back(bodyA);
					delVec.push_back(bodyB);

					m_nSoundId = SimpleAudioEngine::getInstance()->playEffect("sounds/dead.wav");
					m_nSoundId = SimpleAudioEngine::getInstance()->playEffect("sounds/explosion.mp3");
					SimpleAudioEngine::getInstance()->stopBackgroundMusic(true);

					this->scheduleOnce(schedule_selector(Stage3::gameOver), 1);
					// 애니메이션 액션 멈추기
					Director::getInstance()->getActionManager()->pauseAllRunningActions();
					this->unschedule(schedule_selector(Stage3::tick));

					log("game over");

				}
				else
				{
					// 폭발 애니메이션
					auto explosion_animation = Animation::create();
					explosion_animation->setDelayPerUnit(0.1f);

					for (int i = 0; i < 24; i++)
					{
						int column = i % 8;
						int row = i / 3;

						explosion_animation->addSpriteFrameWithTexture(
							texture5,
							Rect(column * 64, row * 85.3, 64, 85.3));
					}

					Sprite* explosion = Sprite::createWithTexture(texture5, Rect(0, 0, 64, 85.3));

					auto explosion_animate = Animate::create(explosion_animation);
					explosion->setScale(3);
					explosion->runAction(explosion_animate);
					explosion->setPosition(Vec2(bodyB->GetPosition().x * 32, bodyB->GetPosition().y * 32));
					this->addChild(explosion);

					m_nSoundId = SimpleAudioEngine::getInstance()->playEffect("sounds/Shield.wav");
					m_nSoundId = SimpleAudioEngine::getInstance()->playEffect("sounds/explosion.mp3");

					delVec.push_back(bodyB);

					if (shieldNum == 2)
					{
						shieldNum--;

					}
					else if (shieldNum == 1)
					{
						shieldNum--;
						this->removeChild(barrier, true);
					}
					else
					{
						shield = false;
					}
				}
			}
			else if (nTag == 9)
			{
				m_nSoundId = SimpleAudioEngine::getInstance()->playEffect("sounds/Coin1.wav");

				SimpleAudioEngine::getInstance()->setEffectsVolume(2.0f);

				delVec.push_back(bodyB);
				nowScore = nowScore--;
			}
		}

	}

	else if (bodyB->GetType() == b2_dynamicBody && meteor == 1)
	{
		if (nTag == 8)
		{
			m_nSoundId = SimpleAudioEngine::getInstance()->playEffect("sounds/explosion.mp3");

			// 폭발 애니메이션
			auto explosion_animation = Animation::create();
			explosion_animation->setDelayPerUnit(0.1f);

			for (int i = 0; i < 24; i++)
			{
				int column = i % 8;
				int row = i / 3;

				explosion_animation->addSpriteFrameWithTexture(
					texture5,
					Rect(column * 64, row * 85.3, 64, 85.3));
			}

			Sprite* explosion = Sprite::createWithTexture(texture5, Rect(0, 0, 64, 85.3));

			auto explosion_animate = Animate::create(explosion_animation);
			explosion->setScale(3);
			explosion->runAction(explosion_animate);
			explosion->setPosition(Vec2(bodyB->GetPosition().x * 32, bodyB->GetPosition().y * 32));
			this->addChild(explosion);

			delVec.push_back(bodyB);
			log("meteor");
		}	
	}
}

void Stage3::createExplosion()
{
	auto meteor_animation = Animation::create();
	meteor_animation->setDelayPerUnit(0.05f);

	for (int i = 0; i < 11; i++)
	{
		int column = i % 4;
		int row = i / 4;

		meteor_animation->addSpriteFrameWithTexture(
			texture4,
			Rect(column * 232.25, row * 396.6, 232.25, 396.6));
	}

	Sprite* meteor = Sprite::createWithTexture(texture3, Rect(0, 0, 232.25, 396.6));

	auto meteor_animate = Animate::create(meteor_animation);
	auto rep = RepeatForever::create(meteor_animate);
	meteor->setAnchorPoint(Vec2(0.5, 0.3));
	meteor->setScale(0.5);
	meteor->runAction(rep);
	meteor->setPosition(Vec2());
	this->addChild(meteor);
}

void Stage3::waySwich()
{
	b2Body* rightWall = this->addNewSprite(Vec2(winSize.width * 7, 150), Size(1, 300), b2_staticBody, "way", 0);

	b2Body* upWall = this->addNewSprite(Vec2(winSize.width * 7 - 150, winSize.height * 7), Size(300, 1), b2_staticBody, "way", 0);

	b2Body* leftWall = this->addNewSprite(Vec2(0, winSize.height * 7 - 150), Size(1, 300), b2_staticBody, "way", 0);

	b2Body* downWall = this->addNewSprite(Vec2(150, 0), Size(300, 1), b2_staticBody, "way", 0);
}

void Stage3::gameOver(float f)
{
	removeChild(Score);

	bord = Sprite::create("gameOverImages/score_bord.png");
	bord->setOpacity(0);

	score = Sprite::create("gameOverImages/score.png");
	score->setPosition(Vec2(200, 200));
	score->setScale(0.3f);
	score->setOpacity(0);
	bord->addChild(score);

	best = Sprite::create("gameOverImages/best_score.png");
	best->setPosition(Vec2(200, 350));
	best->setScale(0.5f);
	best->setOpacity(0);
	bord->addChild(best);

	auto fadeIn1 = FadeIn::create(1.5f);
	auto fadeIn2 = FadeIn::create(2.5f);

	if (rBool && gameClear == 0)
	{
		bord->setPosition(Vec2(pManBody->GetPosition().x * 32, winSize.height * 0.5));
	}
	else if (uBool)
	{
		bord->setPosition(Vec2(winSize.width * 6.5, pManBody->GetPosition().y * 31));
	}
	else if (lBool)
	{
		bord->setPosition(Vec2(pManBody->GetPosition().x * 32, winSize.height * 6.35));
	}
	else if (dBool)
	{
		bord->setPosition(Vec2(winSize.width * 0.5, pManBody->GetPosition().y * 31));
	}
	else if (rBool && gameClear == 1)
	{
		bord->setPosition(Vec2(winSize.width * 0.5, winSize.height * 0.4));
	}

	if (gameClear == 1)
	{
		clear = Sprite::create("gameOverImages/clear.png");
		clear->setOpacity(0);
		clear->setPosition(Vec2(200, 600));
		clear->setScale(2);
		bord->addChild(clear);
		clear->runAction(fadeIn1);

		auto nextButton = MenuItemImage::create(
			"Images/next.png",
			"Images/next.png",
			CC_CALLBACK_1(Stage3::createNext, this));
		auto pNext = Menu::create(nextButton, nullptr);
		nextButton->setPosition(Vec2(200, -40));
		pNext->setPosition(Vec2::ZERO);
		bord->addChild(pNext);

	}
	else
	{
		over = Sprite::create("gameOverImages/game_over.png");
		over->setOpacity(0);
		over->setPosition(Vec2(200, 600));
		over->setScale(2);
		bord->addChild(over);
		over->runAction(fadeIn1);
	}

	bord->runAction(fadeIn1->clone());
	score->runAction(fadeIn2->clone());
	best->runAction(fadeIn2->clone());

	num++;

	auto homeButton = MenuItemImage::create(
		"Images/home.png",
		"Images/home.png",
		CC_CALLBACK_1(Stage3::createHome, this));

	auto replayButton = MenuItemImage::create(
		"gameOverImages/replay.png",
		"gameOverImages/replay.png",
		CC_CALLBACK_1(Stage3::createReplay, this));

	// 메뉴 생성
	auto pReplay = Menu::create(replayButton, nullptr);
	auto pHome = Menu::create(homeButton, nullptr);

	replayButton->setPosition(Vec2(350, -40));
	homeButton->setPosition(Vec2(50, -40));

	pReplay->setPosition(Vec2::ZERO);
	pHome->setPosition(Vec2::ZERO);

	this->addChild(bord);
	bord->addChild(pReplay);
	bord->addChild(pHome);

	this->scheduleOnce(schedule_selector(Stage3::overScore), 1);

	num = 1; // 게임이 끝났는데도 클릭하면 점프효과음이 계속나서 num에게 1을 줌
	SimpleAudioEngine::getInstance()->playBackgroundMusic("sounds/Game_Over.mp3");
}

void Stage3::createReplay(Ref* pSender)
{
	auto pScene = Stage3::createScene();
	Director::getInstance()->pushScene(TransitionProgressRadialCW::create(1, pScene));
}

void Stage3::createHome(Ref* pSender)
{
	auto pScene = GameMain::createScene();
	Director::getInstance()->pushScene(TransitionProgressRadialCW::create(1, pScene));
}

void Stage3::createNext(Ref* pSender)
{
	//auto pScene = GameMain::createScene();
	//Director::getInstance()->pushScene(TransitionProgressRadialCW::create(1, pScene));
}

