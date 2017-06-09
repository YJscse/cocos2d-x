#include "Stage1.h"
#include "Stage2.h"
#include "GameMain.h"

USING_NS_CC;

Scene* Stage2::createScene()
{
	auto scene = Scene::create();
	auto layer = Stage2::create();
	scene->addChild(layer);
	return scene;
}

bool Stage2::init()
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

	bg = Sprite::create("Images/stage2_background.png");
	bg->setScale(7);
	bg->setAnchorPoint(Vec2(0, 0));
	bg->setPosition(Vec2(0, 0));
	this->addChild(bg);

	cover = Sprite::create("Images/cover.png");
	cover->setAnchorPoint(Vec2(0, 1));
	cover->setPosition(Vec2(0, winSize.height));
	//this->addChild(cover, 2);

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

	delVec.clear();

	return true;
}

bool Stage2::createBox2dWorld(bool debug)
{
	// ���� ���� ���� ----------------------------------------------------

	// �߷��� ������ �����Ѵ�.
	b2Vec2 gravity = b2Vec2(0.0f, -30.0f);

	_world = new b2World(gravity);
	_world->SetAllowSleeping(true);
	_world->SetContinuousPhysics(true);

	// ����� ����� ����
	if (debug)
	{
		// ���� : ���� ���� ��� �ϴ� ��
		// ȸ�� : ���� ���� ����� ���� ��
		m_debugDraw = new GLESDebugDraw(PTM_RATIO);
		_world->SetDebugDraw(m_debugDraw);

		uint32 flags = 0;
		//flags += b2Draw::e_shapeBit;
		//flags += b2Draw::e_jointBit;
		//flags += b2Draw::e_aabbBit;
		//flags += b2Draw::e_pairBit;
		//flags += b2Draw::e_centerOfMassBit;
		m_debugDraw->SetFlags(flags);

	}

	// �����ڸ�(�׵θ�)�� ������ ����(Ground Box)�� �����.

	// �ٵ����� ��ǥ�� �����Ѵ�.
	b2BodyDef groundBodyDef;
	groundBodyDef.position.Set(0, 0);

	// ���忡 �ٵ����� ����(��ǥ)�� �ٵ� �����.
	b2Body *groundBody = _world->CreateBody(&groundBodyDef);

	// �����ڸ�(�׵θ�) ��輱�� �׸� �� �ִ� ����� ��ü�� �����.
	b2EdgeShape groundEdge;
	b2FixtureDef boxShapeDef;
	boxShapeDef.shape = &groundEdge;

	// ���� ����� ��ü�� Set(��1, ��2)�� ���� �����.
	// �׸��� �ٵ�(groundBody)�� ���(groundEdge)�� ������Ų��.

	// �Ʒ���
	groundEdge.Set(b2Vec2(0, 0), b2Vec2(winSize.width * 7 / PTM_RATIO, 0));
	groundBody->CreateFixture(&boxShapeDef);

	// ����
	groundEdge.Set(b2Vec2(0, 0), b2Vec2(0, winSize.height * 7 / PTM_RATIO));
	groundBody->CreateFixture(&boxShapeDef);

	// ����
	groundEdge.Set(b2Vec2(0, winSize.height * 7 / PTM_RATIO), b2Vec2(winSize.width * 7 / PTM_RATIO, winSize.height * 7 / PTM_RATIO));
	groundBody->CreateFixture(&boxShapeDef);

	// ������
	groundEdge.Set(b2Vec2(winSize.width * 7 / PTM_RATIO, winSize.height * 7 / PTM_RATIO), b2Vec2(winSize.width * 7 / PTM_RATIO, 0));
	groundBody->CreateFixture(&boxShapeDef);

	// ���� ���� �� -----------------------------------------------------------
	//bDrag = false;

	//// ���콺 ����Ʈ �ٵ� �����ؼ� ���忡 �߰��Ѵ�.
	//gbody = this->addNewSprite(Vec2(0, 0), Size(0, 0), b2_staticBody, nullptr, 0);

	pManBody = this->addNewSprite(Vec2(winSize.width * 6.8, 40), Size(40, 80), b2_dynamicBody, "test", 0);

	this->createWall();
	this->waySwich();
	this->createItem();
	this->createStar();
	this->schedule(schedule_selector(Stage2::movePlayer));
	this->schedule(schedule_selector(Stage2::createFire));

	return true;
}

Stage2::~Stage2()
{
	// ���带 C++�� new�� ���������Ƿ� ���⼭ �����ش�.
	delete _world;
	_world = nullptr;
}

void Stage2::createStar()
{
	// �Ʒ� ��
	for (int i = 1; i < 14; i++)
	{
		this->addNewSprite(Vec2(winSize.width * 1.5 + (i * 64), 180), Size(32, 32), b2_staticBody, "star", 2);
		this->addNewSprite(Vec2(winSize.width * 1.5 + (i * 64), 250), Size(32, 32), b2_staticBody, "star", 2);
	}

	for (int i = -2; i < 8; i++)
	{
		this->addNewSprite(Vec2(winSize.width * 4 + (i * 64), 40), Size(32, 32), b2_staticBody, "star", 2);
		this->addNewSprite(Vec2(winSize.width * 4 + (i * 64), 220), Size(32, 32), b2_staticBody, "star", 2);
	}

	for (int i = 0; i < 10; i++)
	{
		this->addNewSprite(Vec2(winSize.width * 5 + (i * 64), 40), Size(32, 32), b2_staticBody, "star", 2);
	}

	// ������ ��
	for (int i = 0; i < 27; i++)
	{
		if (i < 4)
		{
			this->addNewSprite(Vec2(winSize.width * 7 - 180, winSize.height * 0.5 + (i * 64)), Size(32, 32), b2_staticBody, "star", 2);
		}
		else if (i >= 10 && i < 14)
		{
			this->addNewSprite(Vec2(winSize.width * 7 - 180, winSize.height * 0.5 + (i * 64)), Size(32, 32), b2_staticBody, "star", 2);
		}
		else if (i >= 22 && i < 27)
		{
			this->addNewSprite(Vec2(winSize.width * 7 - 220, winSize.height * 0.5 + (i * 64)), Size(32, 32), b2_staticBody, "star", 2);
		}
	}

	for (int i = 0; i < 10; i++)
	{
		this->addNewSprite(Vec2(winSize.width * 7 - 300, winSize.height * 2 + (i * 64)), Size(32, 32), b2_staticBody, "star", 2);
		this->addNewSprite(Vec2(winSize.width * 7 - 350, winSize.height * 2 + (i * 64)), Size(32, 32), b2_staticBody, "star", 2);
	}


	
}

void Stage2::createFire(float f)
{
	// �Ʒ� ��ֹ�
	if (fireNum == 0) // ������� �ҷ����� ����ؼ� ��������� ������ ������ ��
	{
		for (int i = 1; i < 20; i++)
		{
			this->addNewSprite(Vec2(winSize.width * 1.5 + 40.8 * i, 43.5), Size(40.8, 87), b2_staticBody, "fire", 0);
		}
		fireNum = 1;
	}

	else if (pManBody->GetPosition().x * 32 > winSize.width * 3.7 && fireNum == 1) // ���ڱ� ��ֹ� Ƣ���
	{
		for (int i = 0; i < 17; i++)
		{
			this->addNewSprite(Vec2(winSize.width * 3.8 + 40.8 * i, 43.5), Size(40.8, 87), b2_staticBody, "fire", 0);
		}
		fireNum = 2;
	}

	// ������ ��ֹ�
	if (fireNum == 2)
	{
		for (int i = 1; i < 5; i++)
		{
			b2Body* pFire = this->addNewSprite(Vec2(winSize.width * 7 - 43.5, winSize.height * 0.5 + 40.8 * i), Size(40.8, 87), b2_staticBody, "fire", 0);
			b2Vec2 pos = pFire->GetPosition();
			double DEGREES_TO_RADIANS = (double)(3.141592 / 180);
			float angle = (float)(90 * DEGREES_TO_RADIANS);
			pFire->SetTransform(pos, angle);
		}

		for (int i = 0; i < 7; i++)
		{
			b2Body* pFire = this->addNewSprite(Vec2(winSize.width * 7 - 43.5, winSize.height * 1.6 + 40.8 * i), Size(40.8, 87), b2_staticBody, "fire", 0);
			b2Vec2 pos = pFire->GetPosition();
			double DEGREES_TO_RADIANS = (double)(3.141592 / 180);
			float angle = (float)(90 * DEGREES_TO_RADIANS);
			pFire->SetTransform(pos, angle);
		}
		fireNum = 3;
	}

	else if (pManBody->GetPosition().y * 32 > winSize.height * 0.9 && fireNum == 3)
	{
		for (int i = 1; i < 5; i++)
		{
			b2Body* pFire = this->addNewSprite(Vec2(winSize.width * 7 - 43.5, winSize.height + 40.8 * i), Size(40.8, 87), b2_staticBody, "fire", 0);
			b2Vec2 pos = pFire->GetPosition();
			double DEGREES_TO_RADIANS = (double)(3.141592 / 180);
			float angle = (float)(90 * DEGREES_TO_RADIANS);
			pFire->SetTransform(pos, angle);
		}
		fireNum = 4;
	}
}

void Stage2::createWall()
{
	// �Ʒ� ��
	this->addNewSprite(Vec2(winSize.width * 3, 250), Size(50, 200), b2_staticBody, "wallV", 0);

	for (int i = 0; i < 2; i++)
	{
		this->addNewSprite(Vec2(winSize.width * 4 + i * 336, 150), Size(336, 50), b2_staticBody, "wall", 0);
	}

	// ������ ��
	for (int i = 0; i < 1; i++)
	{
		b2Body* pWall = this->addNewSprite(Vec2(winSize.width * 7 - 120, winSize.height * 1.7), Size(336, 50), b2_staticBody, "wall", 0);

		b2Vec2 pos = pWall->GetPosition();
		double DEGREES_TO_RADIANS = (double)(3.141592 / 180);
		float angle = (float)(90 * DEGREES_TO_RADIANS);
		pWall->SetTransform(pos, angle);
	}
}

void Stage2::createItem()
{	
	// �Ʒ� ������
	jumpItem = this->addNewSprite(Vec2(winSize.width * 1.2, 100),
		Size(72, 72), b2_staticBody, "jump", 2);

	// ������ ������
	for (int i = 0; i < 1; i++)
	{
		jumpItem = this->addNewSprite(Vec2(winSize.width * 7 - 100, winSize.height * 0.2),
			Size(72, 72), b2_staticBody, "jump", 2);

		b2Vec2 pos = jumpItem->GetPosition();
		double DEGREES_TO_RADIANS = (double)(3.141592 / 180);
		float angle = (float)(90 * DEGREES_TO_RADIANS);
		jumpItem->SetTransform(pos, angle);
	}

//	shieldItem = this->addNewSprite(Vec2(winSize.width * 7 - 50, winSize.height / 5),
//		Size(72, 72), b2_staticBody, "shield", 2);

	//b2Vec2 pos = shieldItem->GetPosition();
	//double DEGREES_TO_RADIANS = (double)(3.141592 / 180);
	//float angle = (float)(90 * DEGREES_TO_RADIANS);
	//shieldItem->SetTransform(pos, angle);



}

void Stage2::createPlayer()
{
	auto texture = Director::getInstance()->getTextureCache()->addImage("Images/Punk_Run.png");
	pMan = Sprite::createWithTexture(texture, Rect(512, 0, 256, 256));
	pMan->setPosition(Vec2(100, 60));
	pMan->setScale(0.5f);
	this->addChild(pMan);
}

void Stage2::onEnter()
{
	Layer::onEnter();

	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(Stage2::onTouchBegan, this);
	//	listener->onTouchMoved = CC_CALLBACK_2(Stage2::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(Stage2::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void Stage2::onExit()
{
	//_eventDispatcher->removeAllEventListeners();
	Layer::onExit();
}

void Stage2::tick(float dt)
{
	// ��ü ����
	//*************************************************

	std::vector<b2Body*>::iterator it = delVec.begin();
	// loop through, increasing to next element until the end is reached
	for (; it != delVec.end(); ++it) {
		auto obj = (b2Body*)(*it);

		Sprite* spriteData = (Sprite *)obj->GetUserData();
		int nTag = spriteData->getTag();

		log("Tag .. %d", nTag);

		// ��������Ʈ ����
		//obj->SetUserData(nullptr);
		this->removeChild(spriteData, true);

		// ������ü ����
		_world->DestroyBody(obj);
	}
	delVec.clear();

	//*******************************************

	int velocityIterations = 8;
	int positionIterations = 3;

	// Step : ���� ���踦 �ùķ��̼��Ѵ�.
	_world->Step(dt, velocityIterations, positionIterations);

	// ��� ���� ��ü���� ��ũ�� ����Ʈ�� ����Ǿ� ������ �� �� �ְ� ������ �ִ�.
	// ������� ��ü��ŭ ������ �����鼭 �ٵ� ���� ��������Ʈ�� ���⼭ �����Ѵ�.
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

	// ����
	if (sum == 1)
	{
		removeChild(barrier);
	}
	sum = 1;
	if (shield)
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
		double DEGREES_TO_RADIANS = (double)(3.141592 / 180); // ȸ�� ����
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

		// ���� ��ȯ�Ҷ� �� �� ���� �ִ� kinematicBody�� ����� �ε�����
		// �߷��� ����ؼ� �ٲ����ʵ��� �ϱ����� �ڵ�
		if (pManBody->GetPosition().x > 70)
		{
			swichNum = 1;
		}

		// 2�� ����
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
		//float angle = 89.53f;
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

		// ���� ��ȯ�Ҷ� �� �� ���� �ִ� kinematicBody�� ����� �ε�����
		// �߷��� ����ؼ� �ٲ����ʵ��� �ϱ����� �ڵ�
		if (pManBody->GetPosition().y > 70)
		{
			swichNum = 2;
		}

		// 2�� ����
		if (jSum == 1)
		{
			removeChild(jumpjump);
		}
		jSum = 1;
		if (doubleJump > 0)
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
		//float angle = 179.07f;
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

		// ���� ��ȯ�Ҷ� �� �� ���� �ִ� kinematicBody�� ����� �ε�����
		// �߷��� ����ؼ� �ٲ����ʵ��� �ϱ����� �ڵ�
		if (pManBody->GetPosition().x < 70)
		{
			swichNum = 3;
		}

		// 2�� ����
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
		//float angle = 268.59f;
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

		// ���� ��ȯ�Ҷ� �� �� ���� �ִ� kinematicBody�� ����� �ε�����
		// �߷��� ����ؼ� �ٲ����ʵ��� �ϱ����� �ڵ�
		if (pManBody->GetPosition().y < 70)
		{
			swichNum = 4;
		}

		// 2�� ����
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

}

b2Body* Stage2::addNewSprite(Vec2 point, Size size, b2BodyType bodytype, const char* spriteName, int type)
{
	// �ٵ����� ����� �Ӽ����� �����Ѵ�.
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
				int column = i % 8;
				int row = i / 8;

				animation->addSpriteFrameWithTexture(
					texture,
					Rect(column * 256, row * 256, 256, 256));
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
				int column = i % 8;
				int row = i / 8;

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
		else if (strcmp(spriteName, "way") == 0)
		{
			Sprite* sprite = Sprite::create();
			sprite->setPosition(Vec2(point));
			this->addChild(sprite);

			sprite->setTag(6);
			bodyDef.userData = sprite;
		}
	}
	// ���忡 �ٵ����� ������ �ٵ� �����.
	b2Body *body = _world->CreateBody(&bodyDef);

	// �ٵ� ������ ���� �Ӽ��� �ٵ��� ����� �����.
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

//b2Body* Stage2::getBodyAtTab(Vec2 p)
//{
//	b2Fixture *fix;
//	for (b2Body *b = _world->GetBodyList(); b; b = b->GetNext())
//	{
//		if (b->GetUserData() != nullptr)
//		{
//			if (b->GetType() == b2_staticBody)   continue;
//			fix = b->GetFixtureList();
//			if (fix->TestPoint(b2Vec2(p.x / PTM_RATIO, p.y / PTM_RATIO)))
//			{
//				return b;
//			}
//		}
//	}
//	return nullptr;
//}

bool Stage2::onTouchBegan(Touch* touch, Event* event)
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
		// ���� ����
		if (this->createBox2dWorld(true))
		{
			_world->SetContactListener((b2ContactListener*)this);
			this->schedule(schedule_selector(Stage2::tick));
			log("%f .. %f", winSize.width, winSize.height);
		}
	}
	else if (jumpBool && doubleJump == 0)  // ���� �ѹ��� �����ϰԲ�
	{
		if (rBool && (pManBody->GetPosition().y < 1.3f || jump)) // pManBody->GetPosition().y < 1.3f
		{
			playerVelocity = 29.9f;
			playerIsFlying = true;
			jump = false;
		}
		else if (uBool && (pManBody->GetPosition().x > 156.0f || jump)) // pManBody->GetPosition().x > 156.0f
		{
			playerVelocity = 29.9f;
			playerIsFlying = true;
			jump = false;
		}
		else if (lBool && (pManBody->GetPosition().y > 278.0f || jump)) // pManBody->GetPosition().y > 278.0f
		{
			playerVelocity = 29.9f;
			playerIsFlying = true;
			jump = false;
		}
		else if (dBool && (pManBody->GetPosition().x < 1.5f || jump)) // pManBody->GetPosition().x < 1.5f
		{
			playerVelocity = 29.9f;
			playerIsFlying = true;
			jump = false;
		}
	}
	else if (jumpBool && doubleJump > 0)
	{
		playerVelocity = 29.9f;
		playerIsFlying = true;
		doubleJump--;
		log("double jump");
	}

	jumpBool = true;
	return true;
}

void Stage2::onTouchEnded(Touch *touch, Event *event)
{
	playerIsFlying = false;
	playerVelocity = 0.0f;

	auto touchPoint = touch->getLocation();

}

void Stage2::onDraw(const Mat4 &transform, uint32_t flags)
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

void Stage2::draw(Renderer *renderer, const Mat4 &transform, uint32_t flags)
{
	if (!_world)
	{
		return;
	}

	Layer::draw(renderer, transform, flags);

	_customCmd.init(_globalZOrder, transform, flags);
	_customCmd.func = CC_CALLBACK_0(Stage2::onDraw, this, transform, flags);
	renderer->addCommand(&_customCmd);
}

void Stage2::movePlayer(float f)
{
	this->runAction(Follow::create(pMan, Rect(0, 0, winSize.width * 7, winSize.height * 7)));
}

void Stage2::BeginContact(b2Contact *contact)
{
	b2Fixture *fixA = contact->GetFixtureA();
	b2Fixture *fixB = contact->GetFixtureB();

	b2Body *bodyA = fixA->GetBody();
	b2Body *bodyB = fixB->GetBody();

	Sprite* spriteData = (Sprite *)bodyB->GetUserData();
	int nTag = spriteData->getTag();

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
			this->unschedule(schedule_selector(Stage2::tick));
			this->scheduleOnce(schedule_selector(Stage2::gameOver), 1);
		}

	}
	else if (bodyA->GetType() == b2_dynamicBody && (bodyB->GetType() == b2_staticBody || bodyB->GetType() == b2_kinematicBody))
	{
		if (bodyB->GetUserData() != nullptr)
		{
			Sprite* spriteData = (Sprite *)bodyB->GetUserData();
			int nTag = spriteData->getTag();

			if (nTag == 1)
			{
				if (shield == false)
				{
					this->scheduleOnce(schedule_selector(Stage2::gameOver), 1);
					// �ִϸ��̼� �׼� ���߱�
					Director::getInstance()->getActionManager()->pauseAllRunningActions();
					this->unschedule(schedule_selector(Stage2::tick));

					log("game over");

				}
				else
				{
					if (shieldNum == 2)
					{
						shieldNum--;

					}
					else if (shieldNum == 1)
					{
						shieldNum--;
						this->removeChild(barrier, true);
						shield = false;
					}
					else
					{
						shield = false;
					}
				}
			}
			else if (nTag == 2)
			{
				delVec.push_back(bodyB);
			}
			else if (nTag == 3)
			{
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
				delVec.push_back(bodyB);
				doubleJump = 2;
			}
		}

	}
}

void Stage2::waySwich()
{
	b2Body* rightWall = this->addNewSprite(Vec2(winSize.width * 7, 150), Size(1, 300), b2_staticBody, "way", 0);

	b2Body* upWall = this->addNewSprite(Vec2(winSize.width * 7 - 150, winSize.height * 7), Size(300, 1), b2_staticBody, "way", 0);

	b2Body* leftWall = this->addNewSprite(Vec2(0, winSize.height * 7 - 150), Size(1, 300), b2_staticBody, "way", 0);

	b2Body* downWall = this->addNewSprite(Vec2(150, 0), Size(300, 1), b2_staticBody, "way", 0);
}

void Stage2::gameOver(float f)
{
	auto bord = Sprite::create("gameOverImages/score_bord.png");
	bord->setOpacity(0);

	auto score = Sprite::create("gameOverImages/score.png");
	score->setPosition(Vec2(200, 200));
	score->setScale(0.3f);
	score->setOpacity(0);
	bord->addChild(score);

	auto bestScore = Sprite::create("gameOverImages/best_score.png");
	bestScore->setPosition(Vec2(200, 350));
	bestScore->setScale(0.5f);
	bestScore->setOpacity(0);
	bord->addChild(bestScore);

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
			CC_CALLBACK_1(Stage2::createNext, this));
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
	bestScore->runAction(fadeIn2->clone());

	num++;

	auto homeButton = MenuItemImage::create(
		"Images/home.png",
		"Images/home.png",
		CC_CALLBACK_1(Stage2::createHome, this));

	auto replayButton = MenuItemImage::create(
		"gameOverImages/replay.png",
		"gameOverImages/replay.png",
		CC_CALLBACK_1(Stage2::createReplay, this));

	// �޴� ����
	auto pReplay = Menu::create(replayButton, nullptr);
	auto pHome = Menu::create(homeButton, nullptr);

	replayButton->setPosition(Vec2(350, -40));
	homeButton->setPosition(Vec2(50, -40));

	pReplay->setPosition(Vec2::ZERO);
	pHome->setPosition(Vec2::ZERO);

	this->addChild(bord);
	bord->addChild(pReplay);
	bord->addChild(pHome);
}

void Stage2::createReplay(Ref* pSender)
{
	auto pScene = Stage2::createScene();
	Director::getInstance()->pushScene(TransitionProgressRadialCW::create(1, pScene));
}

void Stage2::createHome(Ref* pSender)
{
	auto pScene = GameMain::createScene();
	Director::getInstance()->pushScene(TransitionProgressRadialCW::create(1, pScene));
}

void Stage2::createNext(Ref* pSender)
{
	auto pScene = GameMain::createScene();
	Director::getInstance()->pushScene(TransitionProgressRadialCW::create(1, pScene));
}

