#include "HelloWorldScene.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    return HelloWorld::create();
}

bool HelloWorld::init()
{
    if ( !Scene::init() )
    {
        return false;
    }
	
    /////////////////////////////
    
	// 윈도우 크기를 구한다.
	winSize = Director::getInstance()->getWinSize();

	// 이미지의 텍스쳐를 구한다.
	texture = Director::getInstance()->getTextureCache()->addImage("blocks.png");

	// 월드 생성
	if (this->createBox2dWorld(true))
	{
		playerIsFlying = false;
		this->setWorld();

		this->schedule(schedule_selector(HelloWorld::tick));
	}


    return true;
}

bool HelloWorld::createBox2dWorld(bool debug)
{
	// 월드 생성 시작 --------------------------------------------------------------------------

	// 중력의 방향을 결정한다.
	b2Vec2 gravity = b2Vec2(0.0f, -30.0f);

	_world = new b2World(gravity);
	_world->SetAllowSleeping(true);
	_world->SetContinuousPhysics(true);

	// 디버그 드로잉 설정
	if (debug) {
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

	b2BodyDef groundBodyDef;
	groundBodyDef.position.Set(0, 0);
	b2Body *groundBody = _world->CreateBody(&groundBodyDef);

	b2EdgeShape groundEdge;
	b2FixtureDef boxShapeDef;
	boxShapeDef.shape = &groundEdge;

	// 아래쪽.
	groundEdge.Set(b2Vec2(0, 0), b2Vec2(winSize.width / PTM_RATIO, 0));
	groundBody->CreateFixture(&boxShapeDef);
	// 왼쪽
	groundEdge.Set(b2Vec2(0, 0), b2Vec2(0, winSize.height / PTM_RATIO));
	groundBody->CreateFixture(&boxShapeDef);
	// 위쪽
	groundEdge.Set(b2Vec2(0, winSize.height / PTM_RATIO),
		b2Vec2(winSize.width / PTM_RATIO, winSize.height / PTM_RATIO));
	groundBody->CreateFixture(&boxShapeDef);
	// 오른쪽
	groundEdge.Set(b2Vec2(winSize.width / PTM_RATIO, winSize.height / PTM_RATIO),
		b2Vec2(winSize.width / PTM_RATIO, 0));
	groundBody->CreateFixture(&boxShapeDef);

	// 월드 생성 끝   --------------------------------------------------------------------------

	_world->SetContactListener((b2ContactListener*)this);

	return true;
}

HelloWorld::~HelloWorld()
{
	delete _world;
	_world = NULL;
}

void HelloWorld::draw(Renderer *renderer, const Mat4 &transform, uint32_t flags)
{
	Scene::draw(renderer, transform, flags);

	_customCmd.init(_globalZOrder, transform, flags);
	_customCmd.func = CC_CALLBACK_0(HelloWorld::onDraw, this, transform, flags);
	renderer->addCommand(&_customCmd);
}

void HelloWorld::onDraw(const Mat4 &transform, uint32_t flags)
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

void HelloWorld::onEnter()
{
	Scene::onEnter();

	// 싱글터치모드로 터치리스너 등록
	auto listener = EventListenerTouchOneByOne::create();
	// Swallow touches only available in OneByOne mode.
	// 핸들링된 터치 이벤트를 터치 이벤트 array에서 지우겠다는 의미이다.
	listener->setSwallowTouches(true);

	listener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
	listener->onTouchEnded = CC_CALLBACK_2(HelloWorld::onTouchEnded, this);

	// The priority of the touch listener is based on the draw order of sprite
	// 터치리스너의 우선순위를 (노드가) 화면에 그려진 순서대로 한다.
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void HelloWorld::onExit()
{
	Scene::onExit();
}

void HelloWorld::tick(float dt)
{
	int velocityIterations = 8;
	int positionIterations = 3;

	// Step : 물리 세계를 시뮬레이션한다.
	_world->Step(dt, velocityIterations, positionIterations);

	// 만들어진 객체 만큼 루프를 돌리면서 바디에 붙인 스프라이트를 여기서 제어한다.
	for (b2Body *b = _world->GetBodyList(); b; b = b->GetNext())
	{
		if (b->GetUserData() != nullptr) {
			Sprite* spriteData = (Sprite *)b->GetUserData();

			spriteData->setPosition(Vec2(b->GetPosition().x * PTM_RATIO,
				b->GetPosition().y * PTM_RATIO));
			spriteData->setRotation(-1 * CC_RADIANS_TO_DEGREES(b->GetAngle()));
		}
	}

	if (playerIsFlying)
	{
		myBall->ApplyLinearImpulse(b2Vec2(0, playerVelocity),
			myBall->GetWorldCenter(),
			true);

		playerVelocity += 0.01f;

		if (playerVelocity > 1.5f)
			playerVelocity = 1.5f;
	}
}

bool HelloWorld::onTouchBegan(Touch *touch, Event * event)
{
	playerVelocity = 0.5f;
	playerIsFlying = true;

	return true;
}

void HelloWorld::onTouchEnded(Touch *touch, Event * event)
{
	playerIsFlying = false;
	playerVelocity = 0.0f;
}

void HelloWorld::addNewSpriteAtPosition(Vec2 location)
{
	// 바디데프 만들고 속성들을 지정한다.
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(location.x / PTM_RATIO, location.y / PTM_RATIO);

	// 보통은 유져데이터에 스프라이트를 붙이는데
	// 여기서는 아무 데이타를 넣지 않고 디버그 드로잉만 수행한다.
	Sprite* sprite = Sprite::create("CloseNormal.png");
	sprite->setPosition(location);
	this->addChild(sprite);

	bodyDef.userData = sprite;
//	bodyDef.userData = NULL;

	// 월드에 바디데프의 정보로 바디를 만든다.
	myBall = _world->CreateBody(&bodyDef);


	// 바디에 적용할 물리 속성용 바디의 모양을 만든다. 여기서는 원을 만든다.
	b2CircleShape circle;
	// 바디의 크기 지정 - 원의 경우엔 반지름
	circle.m_radius = 0.45f;

	b2FixtureDef fixtureDef;
	// 모양
	fixtureDef.shape = &circle;
	// 밀도
	fixtureDef.density = 1.0f;
	// 마찰력
	fixtureDef.friction = 0.2f;
	// 반발력
	fixtureDef.restitution = 0.6f;

	myBall->CreateFixture(&fixtureDef);
}

void HelloWorld::setWorld()
{
	// staticBody 스프라이트를 추가한다.
	Sprite* pSprite1 = Sprite::createWithTexture(texture, Rect(0, 0, 64, 64));
	pSprite1->setPosition(Vec2(winSize.width / 2, winSize.height / 2));
	this->addChild(pSprite1);

	// 바디데프 만들고 속성들을 지정한다.
	b2BodyDef bodyDef1;
	bodyDef1.type = b2_staticBody;
	bodyDef1.position.Set(winSize.width / 2 / PTM_RATIO,
		winSize.height / 2 / PTM_RATIO);
	bodyDef1.userData = pSprite1;

	b2Body* body1 = _world->CreateBody(&bodyDef1);

	// 바디에 적용할 물리 속성용 바디의 모양을 만든다.
	b2PolygonShape staticBox;
	// 바디의 크기 지정 - 상자의 크기에서 가운데 위치를 지정한다.
	staticBox.SetAsBox((pSprite1->getContentSize().width / 2) / PTM_RATIO,
		(pSprite1->getContentSize().height / 2) / PTM_RATIO);

	b2FixtureDef fixtureDef1;
	fixtureDef1.shape = &staticBox;
	fixtureDef1.density = 1.0f;

	body1->CreateFixture(&fixtureDef1);
	//-------------------------------------------------------------------------------

	// 새로운 물리 객체의 바디와 해당 스프라이트를 추가한다.
	addNewSpriteAtPosition(Vec2(240, 100));
}

void HelloWorld::BeginContact(b2Contact *contact)
{
}

void HelloWorld::PreSolve(b2Contact *contact, const b2Manifold *oldManifold)
{
	b2Fixture* fixtureA = contact->GetFixtureA();
	b2Fixture* fixtureB = contact->GetFixtureB();

	b2Body* bodyA = fixtureA->GetBody();
	b2Body* bodyB = fixtureB->GetBody();

	if (bodyA->GetUserData() == nullptr || bodyB->GetUserData() == nullptr) {
		return;
	}

	Sprite* spr1 = (Sprite *)bodyA->GetUserData();
	Sprite* spr2 = (Sprite *)bodyB->GetUserData();

	float pos1 = spr1->getPosition().y + (spr1->getContentSize().height / 2);
	//float pos2 = spr2->getPosition().y - (spr2->getContentSize().height / 2);
	float pos2 = spr2->getPosition().y;

	if (pos2 < pos1)
	{
		contact->SetEnabled(false);
	}
}

void HelloWorld::PostSolve(b2Contact *contact, const b2ContactImpulse *impulse)
{
}

void HelloWorld::EndContact(b2Contact *contact)
{
	contact->SetEnabled(true);
}
