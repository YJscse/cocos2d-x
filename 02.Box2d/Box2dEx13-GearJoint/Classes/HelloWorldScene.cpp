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
	if (!Layer::init())
	{
		return false;
	}
	/////////////////////////////////////////////
	// 윈도우 크기를 구한다.
	winSize = Director::getInstance()->getWinSize();

	// 이미지의 텍스처를 구한다.
	texture = Director::getInstance()->getTextureCache()->addImage("blocks.png");

	// 월드 생성
	if (this->createBox2dWorld(true))
	{
		this->schedule(schedule_selector(HelloWorld::tick));
	}
	return true;
}

bool HelloWorld::createBox2dWorld(bool debug)
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
	groundEdge.Set(b2Vec2(0, 0), b2Vec2(winSize.width / PTM_RATIO, 0));
	groundBody->CreateFixture(&boxShapeDef);

	// 왼쪽
	groundEdge.Set(b2Vec2(0, 0), b2Vec2(0, winSize.height / PTM_RATIO));
	groundBody->CreateFixture(&boxShapeDef);

	// 위쪽
	groundEdge.Set(b2Vec2(0, winSize.height / PTM_RATIO), b2Vec2(winSize.width / PTM_RATIO, winSize.height / PTM_RATIO));
	groundBody->CreateFixture(&boxShapeDef);

	// 아래쪽
	groundEdge.Set(b2Vec2(winSize.width / PTM_RATIO, winSize.height / PTM_RATIO), b2Vec2(winSize.width / PTM_RATIO, 0));
	groundBody->CreateFixture(&boxShapeDef);

	// 월드 생성 끝 -----------------------------------------------------------

	b2GearJointDef gearJointDef;
	b2RevoluteJointDef jointDef1;
	b2RevoluteJointDef jointDef2;
	b2RevoluteJoint *joint1;
	b2RevoluteJoint *joint2;

	b2Body *body0 = this->addNewSprite(Vec2(240, 160), Size(160, 60), b2_staticBody, nullptr, 0);

	b2Body *body1 = this->addNewSprite(Vec2(240 - 40, 160), Size(40, 40), b2_dynamicBody, nullptr, 1);

	b2Body *body2 = this->addNewSprite(Vec2(240 + 20, 160), Size(80, 80), b2_dynamicBody, nullptr, 1);

	jointDef1.Initialize(body0, body1, body1->GetPosition());
	jointDef2.Initialize(body0, body2, body2->GetPosition());

	jointDef1.enableMotor = true;
	jointDef1.motorSpeed = 1;
	jointDef1.maxMotorTorque = 10;

	// joint1를 생성하기 전에 위 모터의 속성들을 정의해야 한다.
	joint1 = (b2RevoluteJoint*)_world->CreateJoint(&jointDef1);
	joint2 = (b2RevoluteJoint*)_world->CreateJoint(&jointDef2);

	gearJointDef.bodyA = body1;
	gearJointDef.bodyB = body2;
	gearJointDef.joint1 = (b2Joint*)joint1;
	gearJointDef.joint2 = (b2Joint*)joint2;
	gearJointDef.ratio = 2.0f;

	_world->CreateJoint(&gearJointDef);

	return true;
}

HelloWorld::~HelloWorld()
{
	// 월드를 C++의 new로 생성했으므로 여기서 지워준다.
	delete _world;
	_world = nullptr;
}

void HelloWorld::draw(Renderer *renderer, const Mat4 &transform, uint32_t flags)
{
	Layer::draw(renderer, transform, flags);

	_customCmd.init(_globalZOrder, transform, flags);
	_customCmd.func = CC_CALLBACK_0(HelloWorld::onDraw, this, transform, flags);
	renderer->addCommand(&_customCmd);
}

//void HelloWorld::onEnter()
//{
//	Layer::onEnter();
//
//	auto listener = EventListenerTouchOneByOne::create();
//	listener->setSwallowTouches(true);
//	listener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
//	listener->onTouchMoved = CC_CALLBACK_2(HelloWorld::onTouchMoved, this);
//	listener->onTouchEnded = CC_CALLBACK_2(HelloWorld::onTouchEnded, this);
//	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
//}
//
//void HelloWorld::onExit()
//{
//	_eventDispatcher->removeAllEventListeners();
//	Layer::onExit();
//}

void HelloWorld::tick(float dt)
{
	// 물리적 위치를 이용해 그래픽 위치를 갱신한다.

	// velocityIterations : 바디들을 정상적으로 이동시키기 위해 필요한 충돌들을
	// 반복적으로 계산
	// positionIterations : 조인트 분리와 겹침 현상을 줄이기 위해 바디의 위치를
	// 반복적으로 적용
	// 값이 클수록 정확한 연산이 가능하지만 성능이 떨어진다.

	// 프로젝트 생성 시 기본값
	//   int velocityIterations = 8;
	//   int positionIterations = 1;

	// 매뉴얼 상의 권장값
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
}

b2Body* HelloWorld::addNewSprite(Vec2 point, Size size, b2BodyType bodytype, const char* spriteName, int type)
{
	// 바디데프를 만들고 속성들을 지정한다.
	b2BodyDef bodyDef;
	bodyDef.type = bodytype;
	bodyDef.position.Set(point.x / PTM_RATIO, point.y / PTM_RATIO);

	if (spriteName)
	{
		if (strcmp(spriteName, "test") == 0)
		{
			int idx = (CCRANDOM_0_1() > .5 ? 0 : 1);
			int idy = (CCRANDOM_0_1() > .5 ? 0 : 1);
			Sprite* sprite = Sprite::createWithTexture(texture, Rect(32 * idx, 32 * idy, 32, 32));
			sprite->setPosition(point);
			this->addChild(sprite);

			bodyDef.userData = sprite;
		}
		else
		{
			Sprite* sprite = Sprite::create(spriteName);
			sprite->setPosition(point);
			this->addChild(sprite);

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
	fixtureDef.friction = 0.3f;
	fixtureDef.restitution = 0.0f;

	body->CreateFixture(&fixtureDef);

	return body;
}

//b2Body* HelloWorld::getBodyAtTab(Vec2 p)
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

//bool HelloWorld::onTouchBegan(Touch* touch, Event* event)
//{
//	Vec2 touchPoint = touch->getLocation();
//
//	if (b2Body *b = this->getBodyAtTab(touchPoint))
//	{
//		dragBody = b;
//		bDrag = true;
//
//		b2MouseJointDef md;
//		md.bodyA = gbody;
//		md.bodyB = dragBody;
//		md.target.Set(dragBody->GetPosition().x, dragBody->GetPosition().y);
//		md.maxForce = 300.0 * dragBody->GetMass();
//
//		mouseJoint = (b2MouseJoint *)_world->CreateJoint(&md);
//	}
//
//	return true;
//}
//
//void HelloWorld::onTouchMoved(Touch *touch, Event *event)
//{
//	Vec2 touchPoint = touch->getLocation();
//	if (bDrag)
//	{
//		mouseJoint->SetTarget(b2Vec2(touchPoint.x / PTM_RATIO, touchPoint.y / PTM_RATIO));
//	}
//}
//
//void HelloWorld::onTouchEnded(Touch *touch, Event *event)
//{
//	if (bDrag)
//	{
//		_world->DestroyJoint(mouseJoint);
//		mouseJoint = nullptr;
//
//		dragBody->SetAwake(true);
//	}
//	bDrag = false;
//}

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