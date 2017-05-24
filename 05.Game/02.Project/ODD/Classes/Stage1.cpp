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
	// ������ ũ�⸦ ���Ѵ�.
	winSize = Director::getInstance()->getWinSize();

	// �̹����� �ؽ�ó�� ���Ѵ�.
	texture = Director::getInstance()->getTextureCache()->addImage("Images/Punk_Run.png");

	bg = Sprite::create("Images/stage1_background.png");
	bg->setScale(5);
	bg->setAnchorPoint(Vec2(0, 0));
	bg->setPosition(Vec2(0, 0));
	//this->addChild(bg);

	this->createPlayer();

	return true;
}

bool Stage1::createBox2dWorld(bool debug)
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
		flags += b2Draw::e_shapeBit;
		flags += b2Draw::e_jointBit;
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
	groundEdge.Set(b2Vec2(0, 0), b2Vec2(winSize.width * 5 / PTM_RATIO, 0));
	groundBody->CreateFixture(&boxShapeDef);

	// ����
	groundEdge.Set(b2Vec2(0, 0), b2Vec2(0, winSize.height * 5 / PTM_RATIO));
	groundBody->CreateFixture(&boxShapeDef);

	// ����
	groundEdge.Set(b2Vec2(0, winSize.height * 5 / PTM_RATIO), b2Vec2(winSize.width * 5 / PTM_RATIO, winSize.height * 5 / PTM_RATIO));
	groundBody->CreateFixture(&boxShapeDef);

	// �Ʒ���
	groundEdge.Set(b2Vec2(winSize.width * 5 / PTM_RATIO, winSize.height * 5 / PTM_RATIO), b2Vec2(winSize.width * 5 / PTM_RATIO, 0));
	groundBody->CreateFixture(&boxShapeDef);

	// ���� ���� �� -----------------------------------------------------------
	bDrag = false;

	// ���콺 ����Ʈ �ٵ� �����ؼ� ���忡 �߰��Ѵ�.
	gbody = this->addNewSprite(Vec2(0, 0), Size(0, 0), b2_staticBody, nullptr, 0);

	pManBody = this->addNewSprite(Vec2(350, 100), Size(40, 100), b2_dynamicBody, "test", 0);

	b2Body* rightWall = this->addNewSprite(Vec2(winSize.width * 5, 150), Size(1, 300), b2_kinematicBody, nullptr, 0);

	b2Body* upWall = this->addNewSprite(Vec2(winSize.width * 5 - 150, winSize.height * 5), Size(300, 1), b2_kinematicBody, nullptr, 0);

	b2Body* leftWall = this->addNewSprite(Vec2(0, winSize.height * 5 - 150), Size(1, 300), b2_kinematicBody, nullptr, 0);

	b2Body* downWall = this->addNewSprite(Vec2(150, 0), Size(300, 1), b2_kinematicBody, nullptr, 0);

	this->schedule(schedule_selector(Stage1::moveBackGround));

	return true;
}

Stage1::~Stage1()
{
	// ���带 C++�� new�� ���������Ƿ� ���⼭ �����ش�.
	delete _world;
	_world = nullptr;
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
	pMan->setPosition(Vec2(150, 300));
	this->addChild(pMan);

	auto animate = Animate::create(animation);
	auto rep = RepeatForever::create(animate);
	pMan->setScale(0.5f);
	pMan->runAction(rep);
}

void Stage1::draw(Renderer *renderer, const Mat4 &transform, uint32_t flags)
{
	Layer::draw(renderer, transform, flags);

	_customCmd.init(_globalZOrder, transform, flags);
	_customCmd.func = CC_CALLBACK_0(Stage1::onDraw, this, transform, flags);
	renderer->addCommand(&_customCmd);
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
	_eventDispatcher->removeAllEventListeners();
	Layer::onExit();
}

void Stage1::tick(float dt)
{
	// ������ ��ġ�� �̿��� �׷��� ��ġ�� �����Ѵ�.

	// velocityIterations : �ٵ���� ���������� �̵���Ű�� ���� �ʿ��� �浹����
	// �ݺ������� ���
	// positionIterations : ����Ʈ �и��� ��ħ ������ ���̱� ���� �ٵ��� ��ġ��
	// �ݺ������� ����
	// ���� Ŭ���� ��Ȯ�� ������ ���������� ������ ��������.

	// ������Ʈ ���� �� �⺻��
	//   int velocityIterations = 8;
	//   int positionIterations = 1;

	// �Ŵ��� ���� ���尪
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
	}
	else if (uBool)
	{

		b2Vec2 pos = pManBody->GetPosition();
		float angle = 89.53f;
		pManBody->SetTransform(pos, angle);

		pManBody->ApplyLinearImpulse(b2Vec2(0, 0.5f), pManBody->GetWorldCenter(), true);

		if (playerIsFlying)
		{
			pManBody->ApplyLinearImpulse(b2Vec2(-playerVelocity, 0.3f), pManBody->GetWorldCenter(), true);

			playerVelocity = playerVelocity + 0.09f;

			if (playerVelocity > 30.0f)
			{
				playerIsFlying = false;
				playerVelocity = 0.0f;
			}
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
	}
	else if (dBool)
	{

		b2Vec2 pos = pManBody->GetPosition();
		float angle = 268.59f;
		pManBody->SetTransform(pos, angle);

		pManBody->ApplyLinearImpulse(b2Vec2(0, -0.5f), pManBody->GetWorldCenter(), true);

		if (playerIsFlying)
		{
			pManBody->ApplyLinearImpulse(b2Vec2(playerVelocity, -0.3f), pManBody->GetWorldCenter(), true);

			playerVelocity = playerVelocity + 0.09f;

			if (playerVelocity > 30.0f)
			{
				playerIsFlying = false;
				playerVelocity = 0.0f;
			}
		}
	}
}

b2Body* Stage1::addNewSprite(Vec2 point, Size size, b2BodyType bodytype, const char* spriteName, int type)
{
	// �ٵ����� ����� �Ӽ����� �����Ѵ�.
	b2BodyDef bodyDef;
	bodyDef.type = bodytype;
	bodyDef.position.Set(point.x / PTM_RATIO, point.y / PTM_RATIO);

	if (spriteName)
	{
		if (strcmp(spriteName, "test") == 0)
		{
			int idx = (CCRANDOM_0_1() > .5 ? 0 : 1);
			int idy = (CCRANDOM_0_1() > .5 ? 0 : 1);
			//auto texture = Director::getInstance()->getTextureCache()->addImage("Images/Punk_Run.png");
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
			pMan->setScale(0.5f);
			pMan->runAction(rep);
			
			//bodyDef.linearVelocity = b2Vec2(10.0f, 0);
			bodyDef.userData = pMan;
		}
		else
		{
			Sprite* sprite = Sprite::create(spriteName);
			sprite->setPosition(point);
			this->addChild(sprite);

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
	else
	{
		circle.m_radius = (size.width / 2) / PTM_RATIO;

		fixtureDef.shape = &circle;
	}

	// Define the dynamic body fixture.
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.0f;
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
	Vec2 touchPoint = touch->getLocation();
	if (!_world)
	{
		removeChild(pMan);
		// ���� ����
		if (this->createBox2dWorld(true))
		{
			_world->SetContactListener((b2ContactListener*)this);
			this->schedule(schedule_selector(Stage1::tick));
			log("%f .. %f", winSize.width, winSize.height);
		}
	}
	playerVelocity = 29.9f;
	playerIsFlying = true;

	return true;
}

void Stage1::onTouchEnded(Touch *touch, Event *event)
{
	playerIsFlying = false;
	playerVelocity = -20.0f;
}

void Stage1::onDraw(const Mat4 &transform, uint32_t flags)
{
	Director* director = Director::getInstance();
	CCASSERT(nullptr != director, "Director is null when seting matrix stack");
	director->pushMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
	director->loadMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW, transform);

	GL::enableVertexAttribs(cocos2d::GL::VERTEX_ATTRIB_FLAG_POSITION);
	_world->DrawDebugData();
	CHECK_GL_ERROR_DEBUG();

	director->popMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
}

void Stage1::moveBackGround(float f)
{
	this->runAction(Follow::create(pMan, 
		Rect(-winSize.width * 1/5, -winSize.height * 1/5, 
		720 * 5 + winSize.width * 2/5, 1280 * 5 + winSize.height * 2/5)));
}

void Stage1::BeginContact(b2Contact *contact)
{
	b2Fixture *fixA = contact->GetFixtureA();
	b2Fixture *fixB = contact->GetFixtureB();

	b2Body *bodyA = fixA->GetBody();
	b2Body *bodyB = fixB->GetBody();


	if (bodyA->GetType() == b2_dynamicBody && bodyB->GetType() == b2_kinematicBody)
	{

		if (rBool)
		{
			_world->SetGravity(b2Vec2(30.0f, 0));
			rBool = false;
			uBool = true;

		}
		else if (uBool)
		{
			_world->SetGravity(b2Vec2(0, 30.0f));
			uBool = false;
			lBool = true;
		}
		else if (lBool)
		{
			_world->SetGravity(b2Vec2(-30.0f, 0));
			lBool = false;
			dBool = true;
		}
		else if (dBool)
		{
			_world->SetGravity(b2Vec2(0, -30.0f));
			dBool = false;
			rBool = true;
		}
		log("contact");
	}
}
