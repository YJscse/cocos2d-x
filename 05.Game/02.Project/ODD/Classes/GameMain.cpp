#include "GameMain.h"
#include "HelloWorldScene.h"
#include "Stage1.h"
#include "Stage2.h"

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
	sound->setScale(0.7f);
	sound->setPosition(Vec2(winSize.width * 14/15, winSize.height * 14/15));
	this->addChild(sound, 2);

	auto bgm = Sprite::create("Images/bgm_on.png");
	bgm->setScale(0.7f);
	bgm->setPosition(Vec2(winSize.width * 12/15, winSize.height * 14/15));
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
	background->setScale(1.3f);
	background->setPosition(Vec2(winSize.width / 2, winSize.height / 2));
	this->addChild(background);

	auto left = MoveBy::create(2, Vec2(-(winSize.width / 50), 0));
	auto down = MoveBy::create(2, Vec2(0, -(winSize.height / 50)));
	auto right = MoveBy::create(2, Vec2(winSize.width / 50, 0));
	auto up = MoveBy::create(2, Vec2(0, winSize.height / 50));

	auto seq = Sequence::create(left, down, right, up, nullptr);
	auto rep = RepeatForever::create(seq);

	background->runAction(rep);
}

void GameMain::createTitle()
{
	title = Sprite::create("Images/title.png");
	title->setPosition(Vec2(winSize.width / 2, winSize.height * 3/4));
	title->setScale(2.5f);
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
	//play->setScale(6.0f);
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
	score->setPosition(Vec2(winSize.width / 2, winSize.height + 100));
	score->setScale(0.5f);
	this->addChild(score);

	auto move = MoveBy::create(0.5f, Vec2(0, -winSize.height * 3/4));
	auto seq = Sequence::create(DelayTime::create(3.0f), move, nullptr);

	score->runAction(seq);
}

void GameMain::selectStage()
{
	stageBord = Sprite::create("Images/stage_bord.png");
	stageBord->setPosition(Vec2(winSize.width / 2, winSize.height / 2));
	//stageBord->setScale(3.0f);
	this->addChild(stageBord);

	auto stageSelect = Sprite::create("stageImages/select.png");
	stageSelect->setPosition(Vec2(200, 350));
	stageSelect->setScale(0.6f);
	stageBord->addChild(stageSelect);

	auto move = MoveBy::create(2, Vec2(0, 20));
	auto back = move->reverse();
	auto seq = Sequence::create(move, back, nullptr);
	auto rep = RepeatForever::create(seq);
	stageSelect->runAction(rep);

	for (int j = 0; j < 9; j++)
	{
		int column = j % 3 + 1;
		int row = j / 3;
		char str[50] = { 0 };
		sprintf(str, "stageImages/stage%d.png", j + 1);
		stage[j] = Sprite::create(str);
		stage[j]->setPosition(Vec2(winSize.width * 2/9 + (column * 100), winSize.height * 4/7 - (row * 100)));
		//stage[j]->setScale(0.8f);
		this->addChild(stage[j]);
		//log("column : %d, row : %d", column, row);
	}

	home = Sprite::create("Images/home.png");
	home->setPosition(Vec2(winSize.width / 4, winSize.height / 3));
	this->addChild(home);
}

bool GameMain::onTouchBegan(Touch* touch, Event* event)
{
	auto touchPoint = touch->getLocation();

	Rect rect = play->getBoundingBox();
	Rect rect2 = score->getBoundingBox();

	if (rect.containsPoint(touchPoint) && sselect == false)
	{
		auto playMove = MoveBy::create(0.1f, Vec2(0, -10));
		play->runAction(playMove);	
		
	}
	else if (rect2.containsPoint(touchPoint) && sselect == false)
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
	

	if (rect.containsPoint(touchPoint) && sselect == false)
	{
		auto playMove = MoveBy::create(0.1f, Vec2(0, 10));
		play->runAction(playMove);

		sselect = true;
		this->selectStage();

//		auto pScene = StageSelect::createScene();
//		Director::getInstance()->pushScene(TransitionProgressRadialCW::create(1 ,pScene));
	}
	else if (rect2.containsPoint(touchPoint) && sselect == false)
	{
		auto scoreMove = MoveBy::create(0.1f, Vec2(0, 10));
		score->runAction(scoreMove);
	}
	else if (sselect)
	{

		Rect stage1 = stage[0]->getBoundingBox();
		Rect stage2 = stage[1]->getBoundingBox();
		Rect stage3 = stage[2]->getBoundingBox();
		Rect stage4 = stage[3]->getBoundingBox();
		Rect stage5 = stage[4]->getBoundingBox();
		Rect stage6 = stage[5]->getBoundingBox();
		Rect stage7 = stage[6]->getBoundingBox();
		Rect stage8 = stage[7]->getBoundingBox();
		Rect stage9 = stage[8]->getBoundingBox();
		Rect homeButton = home->getBoundingBox();

		if (homeButton.containsPoint(touchPoint))
		{
			removeChild(stageBord);
			for (int i = 0; i < 9; i++)
			{
				removeChild(stage[i]);
			}
			removeChild(home);
			sselect = false;
		}
		else if (stage1.containsPoint(touchPoint))
		{
			auto pScene = Stage1::createScene();
			Director::getInstance()->replaceScene(TransitionZoomFlipAngular::create(1, pScene, TransitionScene::Orientation::RIGHT_OVER));
			log("aaa");
		}
		else if (stage2.containsPoint(touchPoint))
		{
			auto pScene = Stage2::createScene();
			Director::getInstance()->replaceScene(TransitionZoomFlipAngular::create(1, pScene, TransitionScene::Orientation::RIGHT_OVER));
			log("aaa");
		}
		else if (stage3.containsPoint(touchPoint))
		{
			auto pScene = Stage1::createScene();
			Director::getInstance()->replaceScene(TransitionZoomFlipAngular::create(1, pScene, TransitionScene::Orientation::RIGHT_OVER));
			log("aaa");
		}
		else if (stage4.containsPoint(touchPoint))
		{
			log("aaa");
		}
		else if (stage5.containsPoint(touchPoint))
		{
			log("stage5");
		}
		else if (stage6.containsPoint(touchPoint))
		{
			log("stage6");
		}
		else if (stage7.containsPoint(touchPoint))
		{
			log("stage7");
		}
		else if (stage8.containsPoint(touchPoint))
		{
			log("stage8");
		}
		else if (stage9.containsPoint(touchPoint))
		{
			log("stage9");
		}
			
	}
	
}

void GameMain::onEnter()
{
	Layer::onEnter();

	// 싱글 터치 모드로 터치 리스너 등록
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);

	listener->onTouchBegan = CC_CALLBACK_2(GameMain::onTouchBegan, this);
	listener->onTouchEnded = CC_CALLBACK_2(GameMain::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);        
}

void GameMain::onExit()
{
	//_eventDispatcher->removeEventListenersForType(EventListener::Type::TOUCH_ONE_BY_ONE);

	Layer::onExit();
}