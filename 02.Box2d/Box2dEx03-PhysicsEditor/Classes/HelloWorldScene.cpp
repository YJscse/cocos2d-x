#include "HelloWorldScene.h"
#include "MyBodyParser.h"


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

	// 윈도우 크기를 구한다.
	winSize = Director::getInstance()->getWinSize();

	// 이미지의 텍스쳐를 구한다
	//texture = Director::getInstance()->getTextureCache()->addImage("CloseNormal.png");
	texture = Director::getInstance()->getTextureCache()->addImage("CloseNormal.png");

	if (this->createBox2dWorld(true))
	{
		this->schedule(schedule_selector(HelloWorld::tick));
	}

	

	return true;
}

bool HelloWorld::createBox2dWorld(bool debug)
{
	//------------- 월드 생성시작 ----------------------------------------

	// 중력의 방향을 결정한다.
	b2Vec2 gravity = b2Vec2(0.0f, -30.0f);

	// 월드를 생성한다.
	_world = new b2World(gravity);

	// 휴식 상태일 때 포함된 바디들을 멈추게(sleep)할 것인지 결정한다.
	_world->SetAllowSleeping(true);

	// 지속적인 물리작용을 할 것인지 결정한다. : 테스트
	_world->SetContinuousPhysics(true);

	// 디버그 드로잉 설정
	if (debug)
	{
		// 적색 : 현재 물리운동을 하는것
		// 회색 : 현재 물리 운동량이 없는것
		m_debugDraw = new GLESDebugDraw(PTM_RATIO);
		_world->SetDebugDraw(m_debugDraw);

		uint32 flags = 0;
		flags += b2Draw::e_shapeBit;
		//flags = flags + b2Draw::e_jointBit;
		//flags = flags + b2Draw::e_aabbBit;
		//flags = flags + b2Draw::e_pairBit;
		//flags = flags + b2Draw::e_centerOfMassBit;
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

	// 에지 모양의 객체에 Set( 점1, 점2 )로 선을 만든다.
	// 그리고 바디(groundBody)에 모양(groundEdge)을 고정시킨다.

	// 아래쪽
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

	// -------------------------- 월드 생성 끝 ---------------------------

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

	// IMPORTANT:
	// This is only for debug purposes
	// It is recommend to disable it
	// 

	Layer::draw(renderer, transform, flags);

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
	Layer::onEnter();

	// 싱글 터치 모드로 터치리스너 등록
	auto listener = EventListenerTouchOneByOne::create();

	//listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);

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
	// int velocityIterations = 8;
	// int positionIterations = 1;

	// 메뉴얼 상의 권장값
	int velocityIterations = 8;
	int positionIterations = 3;

	// Step : 물리 세계를 시뮬레이션 한다.
	_world->Step(dt, velocityIterations, positionIterations);

	// 모든 물리 객체들은 링크드 리스트에 저장되어 참조해 볼 수 있게 구현돼 있다.
	// 만들어진 객체만큼 루프를 돌리면서 바디에 붙인 스프라이트를 여기서 제어한다.
	for (b2Body *b = _world->GetBodyList(); b; b = b->GetNext())
	{
		if (b->GetUserData() != NULL)
		{
			Sprite* spriteData = (Sprite*)b->GetUserData();
			spriteData->setPosition(Vec2(b->GetPosition().x * PTM_RATIO,
				b->GetPosition().y * PTM_RATIO));
			spriteData->setRotation(-1 * CC_RADIANS_TO_DEGREES(b->GetAngle()));
		}
	}
}

bool HelloWorld::onTouchBegan(Touch* touch, Event* event)
{
	auto touchPoint = touch->getLocation();

	// 터치된 지점에 새로운 물리 객체의 바디와 해당 스프라이트를 추가한다.
	addNewSpriteAtPosition(touchPoint);

	return true;
}

void HelloWorld::addNewSpriteAtPosition(Vec2 location)
{
	// 스프라이트를 파라미터로 넘어온 위치에 만든다.
	//	Sprite* pSprite = Sprite::createWithTexture(texture, Rect(0, 0, 37, 37));
	auto polygon = Sprite::create("f1.png");
	polygon->setPosition(Vec2(location.x, location.y));
//	this->addChild(polygon);

	// 바디데프를 만들고 속성들을 지정한다.
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(location.x / PTM_RATIO, location.y / PTM_RATIO);

	// 유저 데이터에 스프라이트를 붙인다.
	//bodyDef.userData = polygon;
	bodyDef.userData = nullptr;

	// 월드에 바디데프의 정보로 바디를 만든다.
	b2Body* body = _world->CreateBody(&bodyDef);

	MyBodyParser::getInstance()->parseJsonFile("exam1.json");
	MyBodyParser::getInstance()->b2BodyJson(polygon, "forward", body);

}