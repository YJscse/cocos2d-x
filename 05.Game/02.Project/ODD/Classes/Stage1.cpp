#include "Stage1.h"
#include "GameMain.h"
//#include "Stage2.h"

USING_NS_CC;

Scene* Stage1::createScene()
{
	auto scene = Scene::create();
	auto layer = Stage1::create();
	scene->addChild(layer);
	return scene;
}

bool Stage1::init()
{
	if (!Layer::init())
	{
		return false;
	}

	/////////////////////////////////////////////
	// 윈도우 크기를 구한다.
	winSize = Director::getInstance()->getWinSize();

	// 이미지의 텍스처를 구한다.
	texture = Director::getInstance()->getTextureCache()->addImage("Images/Punk_Run.png");
	texture2 = Director::getInstance()->getTextureCache()->addImage("Images/fire_animation.png");
	texture3 = Director::getInstance()->getTextureCache()->addImage("Images/Punk_Run_barrier.png");


	bg = Sprite::create("Images/stage1_background.png");
	bg->setScale(7);
	bg->setAnchorPoint(Vec2(0, 0));
	bg->setPosition(Vec2(0, 0));
	this->addChild(bg);

	this->createPlayer();

	delVec.clear();

	return true;
}

bool Stage1::createBox2dWorld(bool debug)
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
		flags += b2Draw::e_jointBit;
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
	bDrag = false;

	// 마우스 조인트 바디를 생성해서 월드에 추가한다.
	gbody = this->addNewSprite(Vec2(0, 0), Size(0, 0), b2_staticBody, nullptr, 0);

	pManBody = this->addNewSprite(Vec2(350, 40), Size(40, 80), b2_dynamicBody, "test", 0);

	this->createWall();
	this->createFire();
	this->waySwich();
	this->createItem();
	this->schedule(schedule_selector(Stage1::movePlayer));

	return true;
}

Stage1::~Stage1()
{
	// 월드를 C++의 new로 생성했으므로 여기서 지워준다.
	delete _world;
	_world = nullptr;
}

void Stage1::createFire()
{
	// 아래 장애물
	for (int i = 1; i < 3; i++)
	{
		this->addNewSprite(Vec2(winSize.width + 43.5 * i, 18), Size(43.5, 97.5), b2_staticBody, "fire", 0);
	}

	for (int i = 1; i < 7; i++)
	{
		this->addNewSprite(Vec2(winSize.width * 3 + 43.5 * i, 18), Size(43.5, 97.5), b2_staticBody, "fire", 0);
	}

	for (int i = 1; i < 5; i++)
	{
		this->addNewSprite(Vec2(winSize.width * 5 + 43.5 * i, 18), Size(43.5, 97.5), b2_staticBody, "fire", 0);
	}

	//오른쪽 장애물


	
}

void Stage1::createWall()
{
	// 아래 벽
	this->addNewSprite(Vec2(winSize.width * 4, 250), Size(50, 200), b2_staticBody, "wallV", 0);

	// 오른쪽 벽
}

void Stage1::createItem()
{
	shieldItem = this->addNewSprite(Vec2(winSize.width * 7 - 100, winSize.height / 5),
										Size(72, 72), b2_staticBody, "shield", 0);
	b2Vec2 pos = shieldItem->GetPosition();
	shieldItem->SetTransform(pos, 89.5f);
}

void Stage1::createPlayer()
{
	auto texture = Director::getInstance()->getTextureCache()->addImage("Images/Punk_Run.png");
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
	pMan->setPosition(Vec2(350, 45));
	this->addChild(pMan);

	auto animate = Animate::create(animation);
	auto rep = RepeatForever::create(animate);
	pMan->setScale(0.5f);
	pMan->runAction(rep);
}

void Stage1::onEnter()
{
	Layer::onEnter();

	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(Stage1::onTouchBegan, this);
//	listener->onTouchMoved = CC_CALLBACK_2(Stage1::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(Stage1::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void Stage1::onExit()
{
	//_eventDispatcher->removeAllEventListeners();
	Layer::onExit();
}

void Stage1::tick(float dt)
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
	}

	if (rBool)
	{

		b2Vec2 pos = pManBody->GetPosition();
		float angle = 0.0f;
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

		// 방향 전환할때 각 벽 끝에 있는 kinematicBody에 몇번을 부딪혀도
		// 중력이 계속해서 바뀌지않도록 하기위한 코드
		if (pManBody->GetPosition().x > 70)
		{
			swichNum = 1;
		}

		// 쉴드
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
	}
	else if (uBool)
	{

		b2Vec2 pos = pManBody->GetPosition();
		float angle = 89.53f;
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

		// 방향 전환할때 각 벽 끝에 있는 kinematicBody에 몇번을 부딪혀도
		// 중력이 계속해서 바뀌지않도록 하기위한 코드
		if (pManBody->GetPosition().y > 70)
		{
			swichNum = 2;
		}

		// 쉴드 
		if (sum == 1)
		{
			removeChild(barrier);
		}
		sum = 1;
		if (shield)
		{
			barrier = Sprite::create("Images/shieldWhite.png");
			barrier->setPosition(Vec2(pManBody->GetPosition().x * 32, pManBody->GetPosition().y * 32));
			barrier->setScale(0.5);
			if (shieldNum == 1)
			{
				barrier->setOpacity(50);
			}
			this->addChild(barrier);

		}
	}
	else if (lBool)
	{

		b2Vec2 pos = pManBody->GetPosition();
		float angle = 179.07f;
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

		// 방향 전환할때 각 벽 끝에 있는 kinematicBody에 몇번을 부딪혀도
		// 중력이 계속해서 바뀌지않도록 하기위한 코드
		if (pManBody->GetPosition().x < 70)
		{
			swichNum = 3;
		}

		// 쉴드
		if (sum == 1)
		{
			removeChild(barrier);
		}
		sum = 1;
		if (shield)
		{
			barrier = Sprite::create("Images/shieldWhite.png");
			barrier->setPosition(Vec2(pManBody->GetPosition().x * 32, pManBody->GetPosition().y * 32));
			barrier->setScale(0.5);
			if (shieldNum == 1)
			{
				barrier->setOpacity(50);
			}
			this->addChild(barrier);

		}
	}
	else if (dBool)
	{

		b2Vec2 pos = pManBody->GetPosition();
		float angle = 268.59f;
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

		// 방향 전환할때 각 벽 끝에 있는 kinematicBody에 몇번을 부딪혀도
		// 중력이 계속해서 바뀌지않도록 하기위한 코드
		if (pManBody->GetPosition().y < 70)
		{
			swichNum = 4;
		}

		// 쉴드
		if (sum == 1)
		{
			removeChild(barrier);
		}
		sum = 1;
		if (shield)
		{
			barrier = Sprite::create("Images/shieldWhite.png");
			barrier->setPosition(Vec2(pManBody->GetPosition().x * 32, pManBody->GetPosition().y * 32));
			barrier->setScale(0.5);
			if (shieldNum == 1)
			{
				barrier->setOpacity(50);
			}
			this->addChild(barrier);

		}
	}

	

}

b2Body* Stage1::addNewSprite(Vec2 point, Size size, b2BodyType bodytype, const char* spriteName, int type)
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
		else if (strcmp(spriteName, "test_barrier") == 0)
		{
			auto animation = Animation::create();
			animation->setDelayPerUnit(0.05f);

			for (int i = 0; i < 8; i++)
			{
				int column = i % 8;
				int row = i / 8;

				animation->addSpriteFrameWithTexture(
					texture3,
					Rect(column * 256, row * 256, 256, 256));
			}

			pMan = Sprite::createWithTexture(texture3, Rect(0, 0, 256, 256));
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
				int column = i % 8;
				int row = i / 8;

				fire_animation->addSpriteFrameWithTexture(
					texture2,
					Rect(i * 29, 0, 29, 65));
			}

			Sprite* fire = Sprite::createWithTexture(texture2, Rect(0, 0, 29, 65));

			auto fire_animate = Animate::create(fire_animation);
			auto rep = RepeatForever::create(fire_animate);
			fire->setScale(1.5f);
			fire->runAction(rep);
			fire->setAnchorPoint(Vec2(0.5, 0.2));
			fire->setPosition(Vec2(point));
			this->addChild(fire);

			fire->setTag(1);
			bodyDef.userData = fire;
		}
		else if (strcmp(spriteName, "wall") == 0)
		{
			Sprite* sprite = Sprite::create("Images/wallH.png");
			//sprite->setAnchorPoint(Vec2(0, 0.5));
			sprite->setPosition(Vec2(point));
			sprite->setScaleY(0.5f);
			this->addChild(sprite);

			sprite->setTag(2);
			bodyDef.userData = sprite;
		}
		else if (strcmp(spriteName, "wallV") == 0)
		{
			Sprite* sprite = Sprite::create("Images/wallV.png");
			//sprite->setAnchorPoint(Vec2(0.5, 0));
			sprite->setPosition(Vec2(point));
			this->addChild(sprite);

			sprite->setTag(2);
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
	else
	{
		circle.m_radius = (size.width / 2) / PTM_RATIO;

		fixtureDef.shape = &circle;
	}

	// Define the dynamic body fixture.
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.1f;
	fixtureDef.restitution = 0.0f;

	body->CreateFixture(&fixtureDef);

	return body;
}

b2Body* Stage1::getBodyAtTab(Vec2 p)
{
	b2Fixture *fix;
	for (b2Body *b = _world->GetBodyList(); b; b = b->GetNext())
	{
		if (b->GetUserData() != nullptr)
		{
			if (b->GetType() == b2_staticBody)   continue;
			fix = b->GetFixtureList();
			if (fix->TestPoint(b2Vec2(p.x / PTM_RATIO, p.y / PTM_RATIO)))
			{
				return b;
			}
		}
	}
	return nullptr;
}

bool Stage1::onTouchBegan(Touch* touch, Event* event)
{
	auto touchPoint = touch->getLocation();

	if (!_world)
	{
		removeChild(pMan);
		playerVelocity = 0.0f;
		// 월드 생성
		if (this->createBox2dWorld(true))
		{
			_world->SetContactListener((b2ContactListener*)this);
			this->schedule(schedule_selector(Stage1::tick));
			log("%f .. %f", winSize.width, winSize.height);
		}
	}
	else if (jumpBool)  // 점프 한번만 가능하게끔
	{
		if (rBool && pManBody->GetPosition().y < 1.3f)
		{
			playerVelocity = 29.9f;
			playerIsFlying = true;
		}
		else if (uBool && pManBody->GetPosition().x > 156.0f)
		{
			playerVelocity = 29.9f;
			playerIsFlying = true;
		}
		else if (lBool && pManBody->GetPosition().y > 278.0f)
		{
			playerVelocity = 29.9f;
			playerIsFlying = true;
		}
		else if (dBool && pManBody->GetPosition().x < 1.5f)
		{
			playerVelocity = 29.9f;
			playerIsFlying = true;
		}
	}

	jumpBool = true;
	return true;
}

void Stage1::onTouchEnded(Touch *touch, Event *event)
{
	playerIsFlying = false;
	playerVelocity = 0.0f;

	auto touchPoint = touch->getLocation();

}

void Stage1::onDraw(const Mat4 &transform, uint32_t flags)
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

void Stage1::draw(Renderer *renderer, const Mat4 &transform, uint32_t flags)
{
	if (!_world)
	{
		return;
	}

	Layer::draw(renderer, transform, flags);

	_customCmd.init(_globalZOrder, transform, flags);
	_customCmd.func = CC_CALLBACK_0(Stage1::onDraw, this, transform, flags);
	renderer->addCommand(&_customCmd);
}

void Stage1::movePlayer(float f)
{
	this->runAction(Follow::create(pMan, Rect(0, 0, winSize.width * 7, winSize.height * 7)));
}

void Stage1::BeginContact(b2Contact *contact)
{
	b2Fixture *fixA = contact->GetFixtureA();
	b2Fixture *fixB = contact->GetFixtureB();

	b2Body *bodyA = fixA->GetBody();
	b2Body *bodyB = fixB->GetBody();


	if (bodyA->GetType() == b2_dynamicBody && bodyB->GetType() == b2_kinematicBody)
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
		}
	}
	else if (bodyA->GetType() == b2_dynamicBody && bodyB->GetType() == b2_staticBody)
	{
		if (bodyB->GetUserData() != nullptr)
		{
			Sprite* spriteData = (Sprite *)bodyB->GetUserData();
			int nTag = spriteData->getTag();

			if (nTag == 1)
			{
				if (shield == false)
				{
					// 애니메이션 액션 멈추기
					Director::getInstance()->getActionManager()->pauseAllRunningActions(); 
					this->unschedule(schedule_selector(Stage1::tick));
					this->gameOver();
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
					}
					else
					{
						this->removeChild(barrier, true);
						shield = false;
					}
				}
			}
			else if (nTag == 2)
			{

			}
			else if (nTag == 3)
			{
				b2Vec2 pos = pManBody->GetPosition();
				delVec.push_back(bodyB);
				shieldNum = 2;
				shield = true;
			}
		}

	}
}

void Stage1::waySwich()
{
	b2Body* rightWall = this->addNewSprite(Vec2(winSize.width * 7, 150), Size(1, 300), b2_kinematicBody, nullptr, 0);

	b2Body* upWall = this->addNewSprite(Vec2(winSize.width * 7 - 150, winSize.height * 7), Size(300, 1), b2_kinematicBody, nullptr, 0);

	b2Body* leftWall = this->addNewSprite(Vec2(0, winSize.height * 7 - 150), Size(1, 300), b2_kinematicBody, nullptr, 0);

	b2Body* downWall = this->addNewSprite(Vec2(150, 0), Size(300, 1), b2_kinematicBody, nullptr, 0);	
}

void Stage1::gameOver()
{
	// 반짝이는 효과
	Sprite* sprite = Sprite::create("Images/base2.png");
	sprite->setPosition(Vec2(winSize.width * 4, winSize.height * 4));
	sprite->setOpacity(0);
	sprite->setScale(1000.0f);
	this->addChild(sprite);

	auto bord = Sprite::create("gameOverImages/score_bord.png");
	bord->setOpacity(0);
	this->addChild(bord);

	auto over = Sprite::create("gameOverImages/game_over.png");
	over->setOpacity(0);
	over->setPosition(Vec2(200, 600));
	over->setScale(2);
	bord->addChild(over);

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

	if (rBool)
	{
		bord->setPosition(Vec2(pManBody->GetPosition().x * 32, 400));
	}
	else if (uBool)
	{
		bord->setPosition(Vec2(pManBody->GetPosition().x * 31 , pManBody->GetPosition().y * 32));
	}
	else if (lBool)
	{
		bord->setPosition(Vec2(pManBody->GetPosition().x * 32, pManBody->GetPosition().y * 30));
	}
	else if (dBool)
	{
		bord->setPosition(Vec2(pManBody->GetPosition().x * 4, pManBody->GetPosition().y * 32));
	}

	bord->runAction(fadeIn1);
	over->runAction(fadeIn1->clone());
	score->runAction(fadeIn2->clone());
	bestScore->runAction(fadeIn2->clone());
	
	num++;

	auto homeButton = MenuItemImage::create(
		"Images/home.png",
		"Images/home.png",
		CC_CALLBACK_1(Stage1::createHome, this));

	auto replayButton = MenuItemImage::create(
		"gameOverImages/replay.png",
		"gameOverImages/replay.png",
		CC_CALLBACK_1(Stage1::createReplay, this));

	// 메뉴 생성
	auto pReplay = Menu::create(replayButton, nullptr);
	auto pHome = Menu::create(homeButton, nullptr);

	replayButton->setPosition(Vec2(350, -40));
	homeButton->setPosition(Vec2(50, -40));

	pReplay->setPosition(Vec2::ZERO);
	pHome->setPosition(Vec2::ZERO);

	bord->addChild(pReplay);
	bord->addChild(pHome);

	//log("replay position().x : %f, position().y : %f, home position().x : %f, position().y : %f", replayButton->getPosition().x, replayButton->getPosition().y, homeButton->getPosition().x, homeButton->getPosition().y);
	//log("%f .. %f   %f .. %f", pReplay->getPosition().x, pReplay->getPosition().y, pHome->getPosition().x, pHome->getPosition().y);
}

void Stage1::createReplay(Ref* pSender)
{
	auto pScene = Stage1::createScene();
	Director::getInstance()->pushScene(TransitionProgressRadialCW::create(1, pScene));
}

void Stage1::createHome(Ref* pSender)
{
	auto pScene = GameMain::createScene();
	Director::getInstance()->pushScene(TransitionProgressRadialCW::create(1, pScene));
}

void Stage1::createBarrier(float f)
{
	
}
