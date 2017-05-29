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
	bg->setScale(7);
	bg->setAnchorPoint(Vec2(0, 0));
	bg->setPosition(Vec2(0, 0));
	this->addChild(bg);

	//LayerColor* mapLayer = LayerColor::create(Color4B(0, 0, 0, 0), winSize.width, winSize.height);
	//mapLayer->setAnchorPoint(Vec2(0, 0));
	//mapLayer->setPosition(Vec2(0, 0));
	//this->addChild(mapLayer);

	//auto pBack = Sprite::create("Images/minimap_back.png");
	//pBack->setColor(Color3B::BLACK);
	//pBack->setOpacity(50);
	//pBack->setPosition(Vec2(400, 260));
	//mapLayer->addChild(pBack);

	//miniMap = RenderTexture::create(720, 1280, Texture2D::PixelFormat::RGBA8888);
	//miniMap->retain();
	//miniMap->setPosition(400, 260);
	//miniMap->getSprite()->setScale(0.22f);
	//mapLayer->addChild(miniMap);
	this->createPlayer();

	delVec.clear();

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
	bDrag = false;

	// ���콺 ����Ʈ �ٵ� �����ؼ� ���忡 �߰��Ѵ�.
	gbody = this->addNewSprite(Vec2(0, 0), Size(0, 0), b2_staticBody, nullptr, 0);

	pManBody = this->addNewSprite(Vec2(350, 40), Size(40, 80), b2_dynamicBody, "test", 0);

	this->createWall();
	this->createSpine();
	this->waySwich();
	this->schedule(schedule_selector(Stage1::movePlayer));

	return true;
}

Stage1::~Stage1()
{
	// ���带 C++�� new�� ���������Ƿ� ���⼭ �����ش�.
	delete _world;
	_world = nullptr;
}

void Stage1::createSpine()
{

	for (int i = 1; i < 3; i++)
	{
		this->addNewSprite(Vec2(winSize.width + 24 * i, 18), Size(24, 36), b2_staticBody, "barrier", 0);
	}

	for (int i = 1; i < 15; i++)
	{
		this->addNewSprite(Vec2(winSize.width * 3 + 24 * i, 18), Size(24, 36), b2_staticBody, "barrier", 0);
	}

	for (int i = 1; i < 5; i++)
	{
		this->addNewSprite(Vec2(winSize.width * 4 + 24 * i, 18), Size(24, 36), b2_staticBody, "barrier", 0);
	}

	for (int i = 1; i < 5; i++)
	{
		this->addNewSprite(Vec2(winSize.width * 5 + 24 * i, 18), Size(24, 36), b2_staticBody, "barrier", 0);
	}
}

void Stage1::createWall()
{
	this->addNewSprite(Vec2(winSize.width * 3 + 168, 150), Size(336, 25), b2_staticBody, "wall", 0);

	this->addNewSprite(Vec2(winSize.width * 2, 250), Size(50, 200), b2_staticBody, "wallV", 0);

	this->addNewSprite(Vec2(winSize.width * 4, 250), Size(50, 200), b2_staticBody, "wallV", 0);

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
	pMan->setPosition(Vec2(350, 40));
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
			pManBody->ApplyLinearImpulse(b2Vec2(-playerVelocity, 0), pManBody->GetWorldCenter(), true);

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

		gBool = true;

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
		else if (strcmp(spriteName, "barrier") == 0)
		{
			Sprite* sprite = Sprite::create("Images/spine.png");
			sprite->setPosition(point);
			sprite->setScale(1.5f);
			this->addChild(sprite);

			sprite->setTag(1);
			bodyDef.userData = sprite;
		}
		else if (strcmp(spriteName, "wall") == 0)
		{
			Sprite* sprite = Sprite::create("Images/wallH.png");
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
	auto touchPoint = touch->getLocation();

	if (!_world)
	{
		removeChild(pMan);
		playerVelocity = 0.0f;
		// ���� ����
		if (this->createBox2dWorld(true))
		{
			_world->SetContactListener((b2ContactListener*)this);
			this->schedule(schedule_selector(Stage1::tick));
			log("%f .. %f", winSize.width, winSize.height);
		}
	}
	else if (jumpBool)
	{
		playerVelocity = 29.9f;
		playerIsFlying = true;
	}
	log("num1 : %d", num);
	if (num == 1)
	{
		log("num2: %d", num);
		Rect rect = home->getBoundingBox();
		Rect rect2 = replay->getBoundingBox();

		if (rect.containsPoint(touchPoint))
		{
			auto homeMove = MoveBy::create(0.1f, Vec2(0, -10));
			home->runAction(homeMove);
			log("aa");
		}
		else if (rect2.containsPoint(touchPoint))
		{
			auto replayMove = MoveBy::create(0.1f, Vec2(0, -10));
			replay->runAction(replayMove);
		}
	}

	jumpBool = true;
	return true;
}

void Stage1::onTouchEnded(Touch *touch, Event *event)
{
	playerIsFlying = false;
	playerVelocity = -60.0f;

	auto touchPoint = touch->getLocation();

	if (num == 1)
	{
		log("num3: %d", num);
		Rect rect = home->getBoundingBox();
		Rect rect2 = replay->getBoundingBox();

		if (rect.containsPoint(touchPoint))
		{
			auto homeMove = MoveBy::create(0.1f, Vec2(0, 10));
			home->runAction(homeMove);

			auto pScene = GameMain::createScene();
			Director::getInstance()->pushScene(TransitionProgressRadialCW::create(1, pScene));

		}
		else if (rect2.containsPoint(touchPoint))
		{
			auto replayMove = MoveBy::create(0.1f, Vec2(0, 10));
			replay->runAction(replayMove);

			auto pScene = Stage1::createScene();
			Director::getInstance()->pushScene(TransitionProgressRadialCW::create(1, pScene));
		}
	}

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

		if (rBool)
		{
			_world->SetGravity(b2Vec2(30.0f, 0));
			rBool = false;
			uBool = true;
			if (gBool)
			{
				this->unschedule(schedule_selector(Stage1::tick));
				log("clear");
			}
			
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
	else if (bodyA->GetType() == b2_dynamicBody && bodyB->GetType() == b2_staticBody)
	{
		if (bodyB->GetUserData() != nullptr)
		{
			Sprite* spriteData = (Sprite *)bodyB->GetUserData();
			int nTag = spriteData->getTag();

			if (nTag == 1)
			{
				this->unschedule(schedule_selector(Stage1::tick));
				this->gameOver();
				//log("contact %d", nTag);
			}
			else if (nTag == 2)
			{
				log("contact2 %d", nTag);
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
	// ��¦�̴� ȿ��
	Sprite* sprite = Sprite::create("Images/base2.png");
	sprite->setPosition(Vec2(360, 640));
	sprite->setOpacity(0);
	sprite->setScale(16.0f);
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

	bord->runAction(fadeIn1);
	over->runAction(fadeIn1->clone());
	score->runAction(fadeIn2->clone());
	bestScore->runAction(fadeIn2->clone());
	
	this->scheduleOnce(schedule_selector(Stage1::createMenu), 1);

}

void Stage1::createMenu(float f)
{
	replay = Sprite::create("gameOverImages/replay.png");
	//replay->setPosition(Vec2(-100, 0));
	replay->setOpacity(0);
	this->addChild(replay);

	home = Sprite::create("Images/home.png");
	//home->setPosition(Vec2(-100, 0));
	home->setOpacity(0);
	this->addChild(home);

	auto fadeIn2 = FadeIn::create(2.5f);

	if (rBool)
	{
		replay->setPosition(Vec2(pManBody->GetPosition().x * 32 + 100, 170));
		home->setPosition(Vec2(pManBody->GetPosition().x * 32 - 100, 170));
	}

	replay->runAction(fadeIn2);
	home->runAction(fadeIn2);

	num++;
}
