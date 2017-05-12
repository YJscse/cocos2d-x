#include "HelloWorldScene.h"
#include "vrope-x/vrope.h"

USING_NS_CC;

using namespace cocos2d;

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

	// 데이터 초기화
	ropes = new std::vector<VRope*>;

	// 윈도우 크기를 구한다.
	winSize = Director::getInstance()->getWinSize();

	// 이미지의 텍스처를 구한다.
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Images/CutTheVerlet.plist");
	Sprite* bg = Sprite::createWithSpriteFrameName("bg.png");
	bg->setPosition(Vec2(160, 240));
	//bg->setVisible(false);
	this->addChild(bg);

	srand((int)time(nullptr));

	// 월드 생성
	if (this->createBox2dWorld(true))
	{
		_world->SetContactListener((b2ContactListener*)this);
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
		//flags += b2Draw::e_shapeBit;
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
	groundBody = _world->CreateBody(&groundBodyDef);

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
	bDrag = false;

	// 마우스 조인트 바디를 생성해서 월드에 추가한다.
	gbody = this->addNewSprite(Vec2(0, 0), Size(0, 0), b2_staticBody, nullptr, 0);

	// 밧줄 이미지 로드
	ropeSpriteSheet = SpriteBatchNode::create("Images/rope_texture.png");
	this->addChild(ropeSpriteSheet);

	

	// 아이템 추가
	b2Body* body1 = this->addNewSpriteAt(Vec2(winSize.width / 2, winSize.height * 2 / 3));


	croc_ = Sprite::createWithSpriteFrameName("croc_front_mouthclosed.png");
	//croc_->setAnchorPoint(Vec2(0.5, 0.5));
	croc_->setPosition(Vec2(270, 90));
	this->addChild(croc_);

	// 바디데프를 만들고 속성들을 지정한다.
	b2BodyDef bodyDef1;
	bodyDef1.type = b2_staticBody;
	bodyDef1.position.Set((winSize.width / 2 + 128) / PTM_RATIO, 
						  (winSize.height / 2 - 150)/ PTM_RATIO);
	bodyDef1.userData = croc_;

	b2Body* body2 = _world->CreateBody(&bodyDef1);

	// 바디에 적용할 물리 속성용 바디의 모양을 만든다.
	b2PolygonShape staticBox;
	// 바디의 크기 지정 - 상자의 크기에서 가운데 위치를 지정한다.
	staticBox.SetAsBox((croc_->getContentSize().width / 10) / PTM_RATIO,
		(croc_->getContentSize().height / 10) / PTM_RATIO);

	b2FixtureDef fixtureDef1;
	fixtureDef1.shape = &staticBox;
	fixtureDef1.density = 1.0f;

	body2->CreateFixture(&fixtureDef1);

	// Add a bunch of ropes (파인애플과 나무에 매달려있는 로프 3개 소환)
	this->createRope(groundBody,
					 b2Vec2((winSize.width / 2 - 100) / PTM_RATIO, 350 / PTM_RATIO),
					 body1, body1->GetLocalCenter(), 1.1f);
	this->createRope(groundBody,
					 b2Vec2((winSize.width / 2 + 120) / PTM_RATIO, 400 / PTM_RATIO),
					 body1, body1->GetLocalCenter(), 1.1f);
	this->createRope(groundBody,
					 b2Vec2((winSize.width / 2 + 120) / PTM_RATIO, 320 / PTM_RATIO),
					 body1, body1->GetLocalCenter(), 1.1f);
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

void HelloWorld::onEnter()
{
	Layer::onEnter();

	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(HelloWorld::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(HelloWorld::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void HelloWorld::onExit()
{
	_eventDispatcher->removeAllEventListeners();
	Layer::onExit();
}

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
	int positionIterations = 1;

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

	// 밧줄 시뮬레이션
	std::vector<VRope*>::iterator rope;
	for (rope = ropes->begin(); rope != ropes->end(); ++rope)
	{
		(*rope)->update(dt);
		(*rope)->updateSprites();
	}
}

b2Body* HelloWorld::addNewSpriteAt(Vec2 point)
{
	// Get the sprite from the sprite sheet
	sprite = Sprite::createWithSpriteFrameName("pineapple.png");
	//sprite->setVisible(false);
	this->addChild(sprite);

	// Defines the body of your candy
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position = b2Vec2(point.x / PTM_RATIO, point.y / PTM_RATIO);
	bodyDef.userData = sprite;
	bodyDef.linearDamping = 0.3f;
	b2Body* body = _world->CreateBody(&bodyDef);

	// Define the fixture as a polygon
	b2FixtureDef fixtureDef;
	b2PolygonShape spriteShape;

	b2Vec2 verts[] =
	{
		b2Vec2(-7.6f / PTM_RATIO, -34.4f / PTM_RATIO),
		b2Vec2(8.3f / PTM_RATIO, -34.4f / PTM_RATIO),
		b2Vec2(15.55f / PTM_RATIO, -27.15f / PTM_RATIO),
		b2Vec2(13.8f / PTM_RATIO, 23.05f / PTM_RATIO),
		b2Vec2(-3.35f / PTM_RATIO, 35.25f / PTM_RATIO),
		b2Vec2(-16.25f / PTM_RATIO, 25.55f / PTM_RATIO),
		b2Vec2(-15.55f / PTM_RATIO, -23.95f / PTM_RATIO),
	};

	spriteShape.Set(verts, 7);

	fixtureDef.shape = &spriteShape;
	fixtureDef.density = 30.0f;
	fixtureDef.filter.categoryBits = 0x01;
	fixtureDef.filter.maskBits = 0x01;

	body->CreateFixture(&fixtureDef);

	return body;

}

void HelloWorld::createRope(b2Body* bodyA, b2Vec2 anchorA, b2Body* bodyB, b2Vec2 anchorB, float32 sag)
{
	b2RopeJointDef jd;
	jd.bodyA = bodyA;
	jd.bodyB = bodyB;
	jd.localAnchorA = anchorA;
	jd.localAnchorB = anchorB;

	// Max length of joint = current distance between bodies * sag
	float32 ropeLength = (bodyA->GetWorldPoint(anchorA) - bodyB->GetWorldPoint(anchorB)).Length()*sag;
	jd.maxLength = ropeLength;


	// Create joints..
	b2RopeJoint* ropeJoint = (b2RopeJoint*)_world->CreateJoint(&jd);

	VRope* newRope = new VRope(ropeJoint, ropeSpriteSheet);

	ropes->push_back(newRope);
}

// 밧줄에 터치가 되엇는지 조사
bool HelloWorld::checkLineIntersection(Vec2 p1, Vec2 p2, Vec2 p3, Vec2 p4)
{
	float denominator = (p4.y - p3.y) * (p2.x - p1.x) - (p4.x - p3.x) * (p2.y - p1.y);

	// In this case the lines are parallel so you assume they don't intersect
	if (denominator == 0.0f)
		return false;
	float ua = ((p4.x - p3.x) * (p1.y - p3.y) - (p4.y - p3.y) * (p1.x - p3.x)) / denominator;
	float ub = ((p2.x - p1.x) * (p1.y - p3.y) - (p2.y - p1.y) * (p1.x - p3.x)) / denominator;

	if (ua >= 0.0 && ua <= 1.0 && ub >= 0.0 && ub <= 1.0)
	{
		return true;
	}
	return false;
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

bool HelloWorld::onTouchBegan(Touch* touch, Event* event)
{
	//Vec2 touchPoint = touch->getLocation();

	//// 아이템 추가
	//b2Body* body1 = this->addNewSpriteAt(touchPoint);

	//// Add a bunch of ropes
	//this->createRope(groundBody,
	//	b2Vec2((winSize.width / 2) / PTM_RATIO, winSize.height / PTM_RATIO),
	//	body1, body1->GetLocalCenter(), 1.1f);

	return true;
}

void HelloWorld::onTouchMoved(Touch *touch, Event *event)
{
	Vec2 pt0 = touch->getPreviousLocation();
	Vec2 pt1 = touch->getLocation();

	//log("previousLocation : %f .. location : %f", pt0, pt1);

	std::vector<VRope*>::iterator rope;
	for (rope = ropes->begin(); rope != ropes->end(); ++rope)
	{
		std::vector<VStick*>::iterator stick;
		for (stick = (*rope)->getSticks().begin(); stick != (*rope)->getSticks().end(); ++stick)
		{
			Vec2 pa = (*stick)->getPointA()->point();
			Vec2 pb = (*stick)->getPointB()->point();
			if (this->checkLineIntersection(pt0, pt1, pa, pb))
			{
				// Cut the rope here
				b2Body *newBodyA = createRopeTipBody();
				b2Body *newBodyB = createRopeTipBody();

				VRope *newRope = (*rope)->cutRopeInStick((*stick), newBodyA, newBodyB);
				ropes->push_back(newRope);

				log("닿음");
				return;
			}

			//log("안닿음");
		}
	}

}

b2Body* HelloWorld::createRopeTipBody()
{
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.linearDamping = 0.5f;
	b2Body *body = _world->CreateBody(&bodyDef);

	b2FixtureDef circleDef;
	b2CircleShape circle;
	circle.m_radius = 1.0f / PTM_RATIO;
	circleDef.shape = &circle;
	circleDef.density = 10.0f;

	// Since these tips don`t have to collide with anything
	// set the mask bits to zero
	circleDef.filter.maskBits = 0;
	body->CreateFixture(&circleDef);

	return body;
}

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

void HelloWorld::BeginContact(b2Contact *contact)
{
	b2Fixture *fixA = contact->GetFixtureA();
	b2Fixture *fixB = contact->GetFixtureB();

	b2Body *bodyA = fixA->GetBody();
	b2Body *bodyB = fixB->GetBody();

	if (bodyA->GetType() == b2_dynamicBody && bodyB->GetType() == b2_staticBody)
	{
		sprite->setVisible(false);
		croc_->setVisible(false);
		Sprite* crocMouthopen = Sprite::createWithSpriteFrameName("croc_front_mouthopen.png");
		crocMouthopen->setPosition(croc_->getPosition());
		this->addChild(crocMouthopen);

	}

}

