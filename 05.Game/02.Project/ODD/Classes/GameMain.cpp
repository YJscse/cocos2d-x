#include "GameMain.h"
#include "HelloWorldScene.h"
#include "Stage1.h"

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

	winSize = Director::getInstance()->getWinSize();


	auto sound = Sprite::create("Images/sound_on.png");
	sound->setScale(4.0f);
	sound->setPosition(Vec2(winSize.width - 200, winSize.height / 20));
	this->addChild(sound, 2);

	auto bgm = Sprite::create("Images/bgm_on.png");
	bgm->setScale(4.0f);
	bgm->setPosition(Vec2(winSize.width * 4/5, winSize.height / 20));
	this->addChild(bgm, 2);

	this->createBackground();
	this->createTitle();
	this->createPlay();
	this->createScore();

	return true;
}

void GameMain::createBackground()
{
	background = Sprite::create("Images/Main_background.png");
	background->setScale(5.0f);
	background->setPosition(Vec2(winSize.width / 2, winSize.height / 2));
	this->addChild(background);

	auto left = MoveBy::create(2, Vec2(-150, 0));
	auto down = MoveBy::create(2, Vec2(0, -150));
	auto right = MoveBy::create(2, Vec2(150, 0));
	auto up = MoveBy::create(2, Vec2(0, 150));

	auto seq = Sequence::create(left, down, right, up, nullptr);
	auto rep = RepeatForever::create(seq);

	background->runAction(rep);
}

void GameMain::createTitle()
{
	title = Sprite::create("Images/title.png");
	title->setPosition(Vec2(winSize.width / 2, winSize.height * 2 / 3 + 200));
	title->setScale(12.0f);
	title->setOpacity(0);
	this->addChild(title);

	auto move = MoveBy::create(3, Vec2(0, 40));
	auto moveDown = move->reverse();
	auto fade = FadeIn::create(3);
	
	auto seq = Sequence::create(move, moveDown, nullptr);
	auto rep = RepeatForever::create(seq);

	title->runAction(fade);
	title->runAction(rep);

}

void GameMain::createPlay()
{
	play = Sprite::create("Images/play.png");
	play->setPosition(Vec2(0, -200));
	play->setScale(6.0f);
	this->addChild(play);

	auto move = Place::create(Vec2(winSize.width / 2, winSize.height / 2));
	auto seq1 = Sequence::create(DelayTime::create(3.0f), move, nullptr);

	auto fadeOut = FadeOut::create(1.5f);
	auto fadeIn = FadeIn::create(1.5f);

	auto seq = Sequence::create(fadeOut, fadeIn, nullptr);
	auto rep = RepeatForever::create(seq);

	play->runAction(seq1);
	play->runAction(rep);

}

void GameMain::createScore()
{
	score = Sprite::create("Images/score.png");
	score->setPosition(Vec2(winSize.width / 2, winSize.height + 200));
	score->setScale(3.0f);
	this->addChild(score);

	auto move = MoveBy::create(0.5f, Vec2(0, -winSize.height * 3/4));
	auto seq = Sequence::create(DelayTime::create(3.0f), move, nullptr);

	score->runAction(seq);
}

bool GameMain::onTouchBegan(Touch* touch, Event* event)
{
	auto touchPoint = touch->getLocation();

	Rect rect = play->getBoundingBox();
	Rect rect2 = score->getBoundingBox();

	if (rect.containsPoint(touchPoint))
	{
		auto playMove = MoveBy::create(0.1f, Vec2(0, -10));
		play->runAction(playMove);

		this->selectStage();
	}
	else if (rect2.containsPoint(touchPoint))
	{
		auto scoreMove = MoveBy::create(0.1f, Vec2(0, -10));
		score->runAction(scoreMove);
	}

	return true;
}

void GameMain::onTouchEnded(Touch* touch, Event* event)
{
	auto touchPoint = touch->getLocation();

	Rect rect = play->getBoundingBox();
	Rect rect2 = score->getBoundingBox();

	if (rect.containsPoint(touchPoint))
	{
		auto playMove = MoveBy::create(0.1f, Vec2(0, 10));
		play->runAction(playMove);

		auto pScene = Stage1::createScene();
		Director::getInstance()->replaceScene(TransitionFade::create(0.5f, pScene));
	}
	else if (rect2.containsPoint(touchPoint))
	{
		auto scoreMove = MoveBy::create(0.1f, Vec2(0, 10));
		score->runAction(scoreMove);
	}


}

void GameMain::selectStage()
{
	
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