#include "GamePlay.h"
#include "GameMain.h"

USING_NS_CC;

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

	this->createGround();
	this->createBird();

	// ���� ����
	if (this->createBox2dWorld(true))
	{
		this->schedule(schedule_selector(GamePlay::tick));
	}


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
	groundEdge.Set(b2Vec2(0, 0), b2Vec2(winSize.width / PTM_RATIO, 0));
	groundBody->CreateFixture(&boxShapeDef);

	// ����
	groundEdge.Set(b2Vec2(0, 0), b2Vec2(0, winSize.height / PTM_RATIO));
	groundBody->CreateFixture(&boxShapeDef);

	// ����
	groundEdge.Set(b2Vec2(0, winSize.height / PTM_RATIO), b2Vec2(winSize.width / PTM_RATIO, winSize.height / PTM_RATIO));
	groundBody->CreateFixture(&boxShapeDef);

	// �Ʒ���
	groundEdge.Set(b2Vec2(winSize.width / PTM_RATIO, winSize.height / PTM_RATIO), b2Vec2(winSize.width / PTM_RATIO, 0));
	groundBody->CreateFixture(&boxShapeDef);

	// ���� ���� �� -----------------------------------------------------------
	//bDrag = false;

	//// ���콺 ����Ʈ �ٵ� �����ؼ� ���忡 �߰��Ѵ�.
	//gbody = this->addNewSprite(Vec2(0, 0), Size(0, 0), b2_staticBody, nullptr, 0);

	// �ٵ� �����ؼ� ���忡 �߰��Ѵ�.
	myBird = this->addNewSprite(Vec2(220, 620), Size(86, 60), b2_dynamicBody, "test", 0);

	return true;
}
GamePlay::~GamePlay()
{
	// ���带 C++�� new�� ���������Ƿ� ���⼭ �����ش�.
	delete _world;
	_world = nullptr;
}

void GamePlay::createBird()
{
	// Bird ���Ʒ��� Move
	auto bird = Sprite::create("bird1.png");
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
	bird->runAction(rep2->clone());
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

	this->addChild(ground);
	ground->runAction(rep);
}

bool GamePlay::onTouchBegan(Touch* touch, Event* event)
{
	auto touchPoint = touch->getLocation();

	auto readyRe = FadeOut::create(0.3f);
	auto howtoRe = FadeOut::create(0.3f);

	ready->runAction(readyRe);
	howto->runAction(howtoRe);

	return true;
}

void GamePlay::onTouchEnded(Touch* touch, Event* event)
{
	auto touchPoint = touch->getLocation();

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

void GamePlay::onDraw(const Mat4 &transform, uint32_t flags)
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