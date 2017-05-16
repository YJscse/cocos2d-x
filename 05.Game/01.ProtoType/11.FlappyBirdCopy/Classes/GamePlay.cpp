#include "GamePlay.h"
#include "GameMain.h"

USING_NS_CC;

int bScore = 0;

Scene* GamePlay::createScene()
{
	auto scene = Scene::create();
	auto layer = GamePlay::create();
	scene->addChild(layer);
	return scene;
}

bool GamePlay::init()
{
	if (!Layer::init())
	{
		return false;
	}

	///////////////////////////////////////////////

	// ������ ũ�⸦ ���Ѵ�.
	winSize = Director::getInstance()->getWinSize();

	//texture = Director::getInstance()->getTextureCache()->addImage("number.png");

	// ���
	Sprite* bg = Sprite::create("bg.png");
	bg->setPosition(Vec2(360, 640));
	this->addChild(bg);

	// Get Ready! �߰�
	ready = Sprite::create("getready.png");
	ready->setPosition(Vec2(360, 850));
	this->addChild(ready);
	
	// How To �߰�
	howto = Sprite::create("click.png");
	howto->setPosition(Vec2(360, 600));
	this->addChild(howto);

	Score = LabelAtlas::create("0", "number.png", 68, 91, '0');
	Score->setPosition(Vec2(360, 950));
	this->addChild(Score, 4);


	this->createGround();
	this->moveBird();

	srand((int)time(NULL));

	// ���� ����


	return true;
}

bool GamePlay::createBox2dWorld(bool debug)
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
	groundEdge.Set(b2Vec2(0, (winSize.height / 5 + 16) / PTM_RATIO), b2Vec2(winSize.width / PTM_RATIO, (winSize.height / 5 + 16) / PTM_RATIO));
	groundBody->CreateFixture(&boxShapeDef);

	// ����
	groundEdge.Set(b2Vec2(0, 0), b2Vec2(0, winSize.height / PTM_RATIO));
	groundBody->CreateFixture(&boxShapeDef);

	// ����
	groundEdge.Set(b2Vec2(0, (winSize.height / 5 * 4 + 160) / PTM_RATIO), b2Vec2(winSize.width / PTM_RATIO, (winSize.height / 5 * 4 + 160)/ PTM_RATIO));
	groundBody->CreateFixture(&boxShapeDef);

	// ������
	groundEdge.Set(b2Vec2(winSize.width / PTM_RATIO, winSize.height / PTM_RATIO), b2Vec2(winSize.width / PTM_RATIO, 600));
	groundBody->CreateFixture(&boxShapeDef);

	// ���� ���� �� -----------------------------------------------------------
	bDrag = false;

	// ���콺 ����Ʈ �ٵ� �����ؼ� ���忡 �߰��Ѵ�.
	gbody = this->addNewSprite(Vec2(0, 0), Size(0, 0), b2_staticBody, nullptr, 0);

	_world->SetContactListener((b2ContactListener*)this);

	// �ٴڹٵ� ����� �浹�˻�
	b2Body* ground1 = this->addNewSprite(Vec2(220, 250), Size(60, 60), b2_staticBody, nullptr, 1);

	// bird �ٵ�
	myBird = this->addNewSprite(Vec2(220, 620), Size(35, 35), b2_dynamicBody, "test", 0);

	this->schedule(schedule_selector(GamePlay::createPipe), 2.0f);
	

	return true;
}

GamePlay::~GamePlay()
{
	// ���带 C++�� new�� ���������Ƿ� ���⼭ �����ش�.
	delete _world;
	_world = nullptr;
}

void GamePlay::draw(Renderer *renderer, const Mat4 &transform, uint32_t flags)
{
	if (!_world)
	{
		return;
	}
	Layer::draw(renderer, transform, flags);

	_customCmd.init(_globalZOrder, transform, flags);
	_customCmd.func = CC_CALLBACK_0(GamePlay::onDraw, this, transform, flags);
	renderer->addCommand(&_customCmd);
}

void GamePlay::moveBird()
{
	bird = Sprite::create("bird1.png");
	bird->setPosition(Vec2(220, 620));
	this->addChild(bird);

	auto birdMove = MoveBy::create(0.4f, Vec2(0, 30));
	auto birdMoveBack = birdMove->reverse();
	auto seq1 = Sequence::create(birdMove, birdMoveBack, nullptr);
	auto rep1 = RepeatForever::create(seq1);
	bird->runAction(rep1);

	// Bird �ִϸ��̼� �߰�
	auto animation = Animation::create();
	animation->setDelayPerUnit(0.1f);

	animation->addSpriteFrameWithFile("bird1.png");
	animation->addSpriteFrameWithFile("bird2.png");
	animation->addSpriteFrameWithFile("bird3.png");

	auto animate = Animate::create(animation);
	auto rep2 = RepeatForever::create(animate);

	bird->runAction(rep2);
}

void GamePlay::createPipe(float df)
{
	wallCheck = true;

	int toto = rand() % 500 + 300;

	upPipe = this->addNewSprite(Vec2(winSize.width + 140, (winSize.height / 2 + toto) + 150), Size(50, 820 ), b2_kinematicBody, "pipe2", 2);
	downPipe = this->addNewSprite(Vec2(winSize.width + 140, (winSize.height / 2 + toto) -950), Size(50, 780 ), b2_kinematicBody, "pipe1", 2);
	log("winsize.width : %f .. winsize.height : %f", winSize.width, winSize.height);
}

b2Body* GamePlay::addNewSprite(Vec2 point, Size size, b2BodyType bodytype, const char* spriteName, int type)
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

			bird = Sprite::create("bird1.png");
			bird->setPosition(Vec2(point));
			this->addChild(bird);
			bodyDef.userData = bird;
		}
		else if (strcmp(spriteName, "pipe1") == 0)
		{
			Sprite* pipe1 = Sprite::create("holdback1.png");  // �Ʒ�
			pipe1->setPosition(point);
			this->addChild(pipe1);

			bodyDef.linearVelocity = b2Vec2(-9.0f, 0);
			bodyDef.userData = pipe1;
			lowPipe.pushBack(pipe1);
		}
		else if (strcmp(spriteName, "pipe2") == 0)
		{
			Sprite* pipe2 = Sprite::create("holdback2.png");  // ��
			pipe2->setPosition(point);
			this->addChild(pipe2);

			bodyDef.linearVelocity = b2Vec2(-9.0f, 0);
			bodyDef.userData = pipe2;
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
	b2PolygonShape kinematicBox;
	
	if (type == 0)
	{
		dynamicBox.SetAsBox(size.width / PTM_RATIO, size.height / PTM_RATIO);

		fixtureDef.shape = &dynamicBox;
	}
	else if(type == 1)
	{
		circle.m_radius = (size.width / 2) / PTM_RATIO;

		fixtureDef.shape = &circle;
	}
	else
	{
		kinematicBox.SetAsBox(size.width / PTM_RATIO, size.height / 2 / PTM_RATIO);

		fixtureDef.shape = &kinematicBox;
	}
	

	// Define the dynamic body fixture.
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.3f;
	fixtureDef.restitution = 0.0f;

	body->CreateFixture(&fixtureDef);

	return body;
}

void GamePlay::createGround()
{
	// �����̴� ��
	Sprite* ground = Sprite::create("ground.png");
	ground->setAnchorPoint(Vec2(0.5, 0));
	ground->setPosition(Vec2(360, 0));

	auto scroll = MoveBy::create(0.2, Vec2(-60, 0));
	auto scrollBack = Place::create(Vec2(360, 0));
	auto seq = Sequence::create(scroll, scrollBack, nullptr);
	auto rep = RepeatForever::create(seq);

	this->addChild(ground, 2);
	ground->runAction(rep);
}

void GamePlay::tick(float dt)
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

	if (wallCheck)
	{
		if (bird->getPosition().x > lowPipe.at(nowScore)->getPosition().x)
		{
			removeChild(Score);
			nowScore++;

			sprintf(str, "%d", nowScore);
			Score = LabelAtlas::create(str, "number.png", 68, 91, '0');
			Score->setPosition(Vec2(360, 950));
			this->addChild(Score, 3);
			//scoreVector.pushBack(Score);
		} 

	}

	

	if (playerIsFlying)
	{
		myBird->ApplyLinearImpulse(b2Vec2(0, playerVelocity), myBird->GetWorldCenter(), true);

		playerVelocity = playerVelocity + 0.09f;

		if (playerVelocity > 30.0f)
		{
			playerIsFlying = false;
			playerVelocity = 0.0f;

			log("velocity : %f", playerVelocity);
		}

	}	


}

bool GamePlay::onTouchBegan(Touch* touch, Event* event)
{
	auto touchPoint = touch->getLocation();

	auto readyRe = FadeOut::create(0.3f);
	auto howtoRe = FadeOut::create(0.3f);

	ready->runAction(readyRe);
	howto->runAction(howtoRe);

	playerVelocity = 29.9f;
	playerIsFlying = true;

	if (!_world)
	{
		removeChild(bird);

		if (this->createBox2dWorld(true))
		{
			this->schedule(schedule_selector(GamePlay::tick));
		}
	}

	auto animation = Animation::create();
	animation->setDelayPerUnit(0.1f);

	animation->addSpriteFrameWithFile("bird1.png");
	animation->addSpriteFrameWithFile("bird2.png");
	animation->addSpriteFrameWithFile("bird3.png");
	animation->addSpriteFrameWithFile("bird1.png");
	animation->addSpriteFrameWithFile("bird2.png");
	animation->addSpriteFrameWithFile("bird3.png");

	auto animate = Animate::create(animation);

	bird->runAction(animate);

	if (gameover == 1)
	{
		Rect rect = startButton->getBoundingBox();
		Rect rect2 = gradeButton->getBoundingBox();

		if (rect.containsPoint(touchPoint))
		{
			auto playMove = MoveBy::create(0.1f, Vec2(0, -10));
			startButton->runAction(playMove);
		}
		else if (rect2.containsPoint(touchPoint))
		{
			auto gardeMove = MoveBy::create(0.1f, Vec2(0, -10));
			gradeButton->runAction(gardeMove);
		}
	}

	if (bBool)
	{
		bRotate = bRotate - 29;
		bird->setRotation(bRotate);

		if (bRotate < -30)
		{
			bBool = false;
			
		}
		return true;
	}

	return true;
}

void GamePlay::onTouchEnded(Touch* touch, Event* event)
{
	auto touchPoint = touch->getLocation();

	playerIsFlying = false;
	playerVelocity = -20.0f;
	
	if (gameover == 1)
	{
		Rect rect = startButton->getBoundingBox();
		Rect rect2 = gradeButton->getBoundingBox();

		if (rect.containsPoint(touchPoint))
		{
			auto playMove = MoveBy::create(0.1f, Vec2(0, 10));
			startButton->runAction(playMove);

			auto pScene = GamePlay::createScene();
			Director::getInstance()->replaceScene(TransitionFade::create(0.5f, pScene));
		}
		else if (rect2.containsPoint(touchPoint))
		{
			auto gardeMove = MoveBy::create(0.1f, Vec2(0, 10));
			gradeButton->runAction(gardeMove);
		}
	}

	if(bBool = false)
	{
		if (bRotate < 90)
		{
			bRotate = bRotate + 89;
			bird->setRotation(bRotate);
		}
		else if (bRotate >= 90)
		{
			bird->setRotation(90);
		}
	}

}

void GamePlay::onEnter()
{
	Layer::onEnter();

	// �̱� ��ġ ���� ��ġ ������ ���
	auto listener = EventListenerTouchOneByOne::create();
	// Swallow touches only available in OneByOne mode.
	// �ڵ鸵�� ��ġ �̺�Ʈ array���� ����ڴٴ� �ǹ��̴�.
	listener->setSwallowTouches(true);

	listener->onTouchBegan = CC_CALLBACK_2(GamePlay::onTouchBegan, this);
	listener->onTouchEnded = CC_CALLBACK_2(GamePlay::onTouchEnded, this);

	// The priority of the touch listener is based on the draw order of sprite
	// ��ġ �������� �켱������ (��尡) ȭ�鿡 �׷��� ������� �Ѵ�.
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);                 // ������ ������ ���鶧�� onEnter�� �������� init�� �־�� onExit�� ��������!!!
}

void GamePlay::onExit()
{
	//_eventDispatcher->removeAllEventListeners();
	Layer::onExit();
}

void GamePlay::onDraw(const Mat4 &transform, uint32_t flags)
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

void GamePlay::BeginContact(b2Contact *contact)
{
	b2Fixture *fixA = contact->GetFixtureA();
	b2Fixture *fixB = contact->GetFixtureB();

	b2Body *bodyA = fixA->GetBody();
	b2Body *bodyB = fixB->GetBody();

	
	if (bodyA->GetType() == b2_dynamicBody && bodyB->GetType() == b2_staticBody )
	{
		Director::getInstance()->getActionManager()->pauseAllRunningActions();
		this->unschedule(schedule_selector(GamePlay::createPipe));
		this->unschedule(schedule_selector(GamePlay::tick));

		this->gameOver();
		log("aaa");

	}
	else if (bodyA->GetType() == b2_dynamicBody && bodyB->GetType() == b2_kinematicBody)
	{
		Director::getInstance()->getActionManager()->pauseAllRunningActions();
		this->unschedule(schedule_selector(GamePlay::createPipe));
		this->unschedule(schedule_selector(GamePlay::tick));

		this->gameOver();
		log("bbb");
	}
}

void GamePlay::gameOver()
{
	// ��¦�̴� ȿ��
	Sprite* sprite = Sprite::create("base2.png");
	sprite->setPosition(Vec2(360, 640));
	sprite->setOpacity(0);
	sprite->setScale(16.0f);
	this->addChild(sprite);

	auto overIn = FadeIn::create(0.05f);
	auto overOut = FadeOut::create(0.05f);
	auto seq = Sequence::create(overIn, overOut, nullptr);
	sprite->runAction(seq);
	auto scoreOut = FadeOut::create(0.05f);
	Score->runAction(scoreOut);

	// ���ӿ��� ��������Ʈ
	Sprite* overSprite = Sprite::create("gameover.png");
	overSprite->setPosition(Vec2(360, 850));
	this->addChild(overSprite);
	auto overAction = FadeIn::create(2.0f);
	overSprite->runAction(overAction);

	// ������ 
	Sprite* baseSprite = Sprite::create("base.png");
	baseSprite->setPosition(Vec2(360, 0));
	this->addChild(baseSprite);
	auto baseAction = MoveTo::create(0.6, Vec2(360, 640));
	baseSprite->runAction(baseAction);

	this->scheduleOnce(schedule_selector(GamePlay::baseScore), 1);

	this->scheduleOnce(schedule_selector(GamePlay::createMenu), 1);
	
}
void GamePlay::createMenu(float f)
{
	startButton = Sprite::create("start.png");
	startButton->setPosition(Vec2(180, 340));
	this->addChild(startButton);

	gradeButton = Sprite::create("grade.png");
	gradeButton->setPosition(Vec2(540, 340));
	this->addChild(gradeButton);
	
	gameover++;

}
void GamePlay::baseScore(float f)
{
	float ni = 0;

	if (nowScore != 0)
	{
		ni = 1 / nowScore;
	}

	smallScore = LabelAtlas::create("0", "number.png", 68, 91, '0');
	smallScore->setPosition(Vec2(570, 650));
	smallScore->setScale(0.5);
	this->addChild(smallScore, 3);
	this->schedule(schedule_selector(GamePlay::baseNowScore), ni);


	sprintf(str, "%d", bScore);
	bestScore = LabelAtlas::create(str, "number.png", 68, 91, '0');
	bestScore->setPosition(Vec2(570, 650));
	bestScore->setScale(0.5);
	this->addChild(bestScore, 3);

	if (nowScore > 20)
	{
		auto Gold = Sprite::create("yellow.png");
		Gold->setPosition(Vec2(350, 600));
		this->addChild(Gold, 3);
	}
	else if (nowScore > 10)
	{
		auto Silver = Sprite::create("gray.png");
		Silver->setPosition(Vec2(350, 600));
		this->addChild(Silver, 3);
	}
}

void GamePlay::baseNowScore(float f)
{
	removeChild(smallScore);
	sprintf(str, "%d", nScore);
	smallScore = LabelAtlas::create(str, "number.png", 68, 91, '0');
	smallScore->setPosition(Vec2(570, 650));
	smallScore->setScale(0.5);

	sprintf(str, "%d", bScore);
	bestScore = LabelAtlas::create(str, "number.png", 68, 91, '0');
	bestScore->setPosition(Vec2(570, 540));
	bestScore->setScale(0.5);
	this->addChild(bestScore, 3);

	this->addChild(smallScore, 3);

	if (nowScore == nScore)
	{
		if (nowScore > bScore)
		{
			removeChild(bestScore);

			bScore = nowScore;
			sprintf(str, "%d", bScore);
			bestScore = LabelAtlas::create(str, "number.png", 68, 91, '0');
			bestScore->setPosition(Vec2(570, 540));
			bestScore->setScale(0.5);
			this->addChild(bestScore, 3);

			auto newBest = Sprite::create("new.png");
			newBest->setPosition(Vec2(460, 720));
			this->addChild(newBest, 3);
		}

		this->unschedule(schedule_selector(GamePlay::baseNowScore));

	}

	nScore++;
}

