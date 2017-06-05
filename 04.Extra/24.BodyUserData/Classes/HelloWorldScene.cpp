#include "HelloWorldScene.h"

Scene* HelloWorld::createScene()
{
    auto scene = Scene::create();
    
    auto layer = HelloWorld::create();

    scene->addChild(layer);

    return scene;
}

bool HelloWorld::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    // 윈도우 크기를 구한다.
    winSize = Director::getInstance()->getWinSize();

    // 이미지의 텍스쳐를 구한다.
    texture = Director::getInstance()->getTextureCache()->addImage("blocks.png");

    // 월드 생성
    if (this->createBox2dWorld(true))
    {
        this->schedule(schedule_selector(HelloWorld::tick));
    }

//	delVec = new std::vector<b2Body*>;
	delVec.clear();

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



//    myContactListener = new ContactListener();

    _world->SetContactListener((b2ContactListener*)this);


    return true;
}

HelloWorld::~HelloWorld()
{
	//delete delVec;
    delete _world;
    _world = NULL;
}

void HelloWorld::draw(Renderer *renderer, const Mat4 &transform, uint32_t flags)
{
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

    // 싱글터치모드로 터치리스너 등록
    auto listener = EventListenerTouchOneByOne::create();
    // Swallow touches only available in OneByOne mode.
    // 핸들링된 터치 이벤트를 터치 이벤트 array에서 지우겠다는 의미이다.
    listener->setSwallowTouches(true);

    listener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
//    listener->onTouchMoved = CC_CALLBACK_2(HelloWorld::onTouchMoved, this);
//    listener->onTouchEnded = CC_CALLBACK_2(HelloWorld::onTouchEnded, this);

    // The priority of the touch listener is based on the draw order of sprite
    // 터치리스너의 우선순위를 (노드가) 화면에 그려진 순서대로 한다.
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void HelloWorld::onExit()
{
    _eventDispatcher->removeEventListenersForType(EventListener::Type::TOUCH_ONE_BY_ONE);

    Layer::onExit();
}

void HelloWorld::tick(float dt)
{
	// 객체 제거
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
}

b2Body* HelloWorld::addNewSprite(Vec2 point, Size size, b2BodyType bodytype,
    const char* spriteName, int type)
{
	// 바디데프 만들고 속성들을 지정한다.
    b2BodyDef bodyDef;
    bodyDef.type = bodytype;
    bodyDef.position.Set(point.x / PTM_RATIO, point.y / PTM_RATIO);

    if (spriteName){
        if (strcmp(spriteName, "test") == 0) {
            int idx = (CCRANDOM_0_1() > .5 ? 0 : 1);
            int idy = (CCRANDOM_0_1() > .5 ? 0 : 1);
            Sprite* sprite = Sprite::createWithTexture(texture,
                Rect(32 * idx, 32 * idy, 32, 32));
            sprite->setPosition(point);
            this->addChild(sprite);

			if (idx == 0 && idy == 0)
				sprite->setTag(1);
			else if (idx == 1 && idy == 0)
				sprite->setTag(2);
			else if (idx == 0 && idy == 1)
				sprite->setTag(3);
			else if (idx == 1 && idy == 1)
				sprite->setTag(4);

            bodyDef.userData = sprite;
        }
        else {
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

    if (type == 0) {
        dynamicBox.SetAsBox(size.width / 2 / PTM_RATIO, size.height / 2 / PTM_RATIO);

        fixtureDef.shape = &dynamicBox;
    }
    else {
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

bool HelloWorld::onTouchBegan(Touch *touch, Event * event)
{
    Point touchPoint = touch->getLocation();

    // 터치된 지점에 새로운 물리 객체의 바디와 해당 스프라이트를 추가한다.
    this->addNewSprite(touchPoint, Size(50, 50), b2_dynamicBody, "test", 0);

    return true;
}

void HelloWorld::PostSolve(b2Contact *contact, const b2ContactImpulse *impulse)
{
	//b2Fixture *fixA = contact->GetFixtureA();
	b2Fixture *fixB = contact->GetFixtureB();

	//b2Body *bodyA = fixA->GetBody();
	b2Body *bodyB = fixB->GetBody();

	if (bodyB->GetType() == b2_dynamicBody) 
	{	
		if (bodyB->GetUserData() != NULL) 
		{
			delVec.push_back(bodyB);
		}
	}

}