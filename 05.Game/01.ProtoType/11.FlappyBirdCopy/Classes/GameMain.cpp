#include "GameMain.h"
#include "HelloWorldScene.h"
#include "GamePlay.h"

USING_NS_CC;

Scene* GameMain::createScene()
{
	auto scene = Scene::create();
	auto layer = GameMain::create();
	scene->addChild(layer);
	return scene;
}

bool GameMain::init()
{
	if (!Layer::init())
	{
		return false;
	}
	
	///////////////////////////////////////////////
	// 배경
	Sprite* bg = Sprite::create("bg.png");
	bg->setPosition(Vec2(360, 640));
	this->addChild(bg);

	// 게임 제목
	Sprite* name = Sprite::create("flappybird.png");
	name->setPosition(Vec2(360, 850));
	this->addChild(name);

	// RATE 버튼
	rate = Sprite::create("rate.png");
	rate->setPosition(Vec2(360, 550));
	this->addChild(rate);

	// PLAY 버튼
	play = Sprite::create("start.png");
	play->setPosition(Vec2(180, 340));
	this->addChild(play);
	
	// GRADE 버튼
	grade = Sprite::create("grade.png");
	grade->setPosition(Vec2(540, 340));
	this->addChild(grade);

	this->createGround();
	this->createBird();
	
	return true;
}

void GameMain::createBird()
{
	// Bird 위아래로 Move
	auto bird = Sprite::create("bird1.png");
	bird->setPosition(Vec2(360, 700));
	this->addChild(bird);

	auto birdMove = MoveBy::create(0.4f, Vec2(0, 30));
	auto birdMoveBack = birdMove->reverse();
	auto seq1 = Sequence::create(birdMove, birdMoveBack, nullptr);
	auto rep1 = RepeatForever::create(seq1);
	bird->runAction(rep1);

	// Bird 애니메이션 추가
	auto animation = Animation::create();
	animation->setDelayPerUnit(0.1f);

	animation->addSpriteFrameWithFile("bird1.png");
	animation->addSpriteFrameWithFile("bird2.png");
	animation->addSpriteFrameWithFile("bird3.png");

	auto animate = Animate::create(animation);

	auto rep2 = RepeatForever::create(animate);
	bird->runAction(rep2->clone());
}

void GameMain::createGround()
{
	// 움직이는 땅
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

bool GameMain::onTouchBegan(Touch* touch, Event* event)
{
	auto touchPoint = touch->getLocation();

	Rect rect = rate->getBoundingBox();
	Rect rect2 = play->getBoundingBox();
	Rect rect3 = grade->getBoundingBox();

	if (rect.containsPoint(touchPoint))
	{
		auto rateMove = MoveBy::create(0.1f, Vec2(0, -10));
		rate->runAction(rateMove);
	}
	else if (rect2.containsPoint(touchPoint))
	{
		auto playMove = MoveBy::create(0.1f, Vec2(0, -10));
		play->runAction(playMove);
	}
	else if (rect3.containsPoint(touchPoint))
	{
		auto gradeMove = MoveBy::create(0.1f, Vec2(0, -10));
		grade->runAction(gradeMove);
	}

	return true;
}

void GameMain::onTouchEnded(Touch* touch, Event* event)
{
	auto touchPoint = touch->getLocation();

	Rect rect = rate->getBoundingBox();
	Rect rect2 = play->getBoundingBox();
	Rect rect3 = grade->getBoundingBox();

	if (rect.containsPoint(touchPoint))
	{
		auto rateMove = MoveBy::create(0.1f, Vec2(0, 10));
		rate->runAction(rateMove);
	}
	else if (rect2.containsPoint(touchPoint))
	{
		auto playMove = MoveBy::create(0.1f, Vec2(0, 10));
		play->runAction(playMove);

		auto pScene = GamePlay::createScene();
		Director::getInstance()->replaceScene(TransitionFade::create(0.5f, pScene));
	}
	else if (rect3.containsPoint(touchPoint))
	{
		auto gradeMove = MoveBy::create(0.1f, Vec2(0, 10));
		grade->runAction(gradeMove);
	}
	
	
}

void GameMain::onEnter()
{
	Layer::onEnter();

	// 싱글 터치 모드로 터치 리스너 등록
	auto listener = EventListenerTouchOneByOne::create();
	// Swallow touches only available in OneByOne mode.
	// 핸들링된 터치 이벤트 array에서 지우겠다는 의미이다.
	listener->setSwallowTouches(true);

	listener->onTouchBegan = CC_CALLBACK_2(GameMain::onTouchBegan, this);
	listener->onTouchEnded = CC_CALLBACK_2(GameMain::onTouchEnded, this);

	// The priority of the touch listener is based on the draw order of sprite
	// 터치 리스너의 우선순위를 (노드가) 화면에 그려진 순서대로 한다.
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);                 // 실제로 게임을 만들때는 onEnter에 넣지말고 init에 넣어라 onExit도 마찬가지!!!
}

void GameMain::onExit()
{
	//_eventDispatcher->removeEventListenersForType(EventListener::Type::TOUCH_ONE_BY_ONE);

	Layer::onExit();
}