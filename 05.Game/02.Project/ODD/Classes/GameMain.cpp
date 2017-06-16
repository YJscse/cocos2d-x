#include "GameMain.h"
#include "HelloWorldScene.h"
#include "Stage1.h"
#include "Stage2.h"
#include "Stage3.h"
#include "AudioEngine.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "platform/android/jni/JniHelper.h"

void callJavaMethod(std::string func, std::string arg0)
{
	JniMethodInfo t;

	/**
	JniHelper를 통해 org/cocos2dx/cpp/에 있는
	AppActivity class의 파라미터로 들어온 스트링 이름의 함수 정보를 가져온다.
	*/
	if (JniHelper::getStaticMethodInfo(t
		, "org/cocos2dx/cpp/AppActivity"
		, func.c_str()
		, "(Ljava/lang/String;)V"))
	{
		jstring stringArg0 = t.env->NewStringUTF(arg0.c_str());
		// 함수 호출
		t.env->CallStaticVoidMethod(t.classID, t.methodID, stringArg0);
		// Release
		t.env->DeleteLocalRef(t.classID);
	}
}
#endif

using namespace experimental;

const std::string BGM_1_PATH = "sounds/Game_Main_BGM.mp3";
const std::string BGM_2_PATH = "sounds/click.mp3";


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


	effect = Sprite::create("Images/sound_on.png");
	effect->setPosition(Vec2(winSize.width * 0.9, winSize.height * 0.9));
	this->addChild(effect, 2);

	bgm = Sprite::create("Images/bgm_on.png");
	bgm->setPosition(Vec2(winSize.width * 0.75, winSize.height * 0.9));
	this->addChild(bgm, 2);

	this->createBackground();
	this->createTitle();
	this->createPlay();
	this->createScore();

	auto wlayer = LayerColor::create(Color4B(255, 255, 255, 255));
	wlayer->setOpacity(0);
	this->addChild(wlayer);

	// 메뉴 아이템 생성 및 초기화

	MenuItemFont::setFontName("fonts/Marker Felt.ttf");
	MenuItemFont::setFontSize(20);
	auto pMenuItem1 = MenuItemFont::create(
		" 최고점수 ",
		CC_CALLBACK_1(GameMain::doSendScore, this));
	pMenuItem1->setColor(Color3B(0, 0, 0));
	pMenuItem1->setPosition(Vec2(180, 180));

	auto pMenuItem2 = MenuItemFont::create(
		" 소요시간 ",
		CC_CALLBACK_1(GameMain::doSendTime, this));
	pMenuItem2->setColor(Color3B(0, 0, 0));
	pMenuItem2->setPosition(Vec2(300, 180));

	auto pMenuItem3 = MenuItemFont::create(
		" 일회성 업적 ",
		CC_CALLBACK_1(GameMain::doSendOne, this));
	pMenuItem3->setColor(Color3B(0, 0, 0));
	pMenuItem3->setPosition(Vec2(180, 130));

	auto pMenuItem4 = MenuItemFont::create(
		" 단계별 업적 ",
		CC_CALLBACK_1(GameMain::doSendMulti, this));
	pMenuItem4->setColor(Color3B(0, 0, 0));
	pMenuItem4->setPosition(Vec2(300, 130));

	auto pMenuItem5 = MenuItemFont::create(
		" 리더보드 ",
		CC_CALLBACK_1(GameMain::doShowLeaderBoard, this));
	pMenuItem5->setColor(Color3B(0, 0, 0));
	pMenuItem5->setPosition(Vec2(180, 80));

	auto pMenuItem6 = MenuItemImage::create(
		"Images/score.png",
		"Images/score.png",
		CC_CALLBACK_1(GameMain::doShowAchivement, this));
	pMenuItem6->setColor(Color3B(0, 0, 0));
	pMenuItem6->setPosition(Vec2(300, 80));

	auto pMenu = Menu::create(
		pMenuItem1, pMenuItem2, pMenuItem3, pMenuItem4,
		pMenuItem5, pMenuItem6,
		nullptr);

	pMenu->setPosition(Vec2(0, 0));

	this->addChild(pMenu);

	//--------------------------------------------------------------
	Size editBoxSize = Size(100, 60);
	std::string pNormalSprite = "Images/go.png";
	_editNum = ui::EditBox::create(editBoxSize,
		ui::Scale9Sprite::create(pNormalSprite));
	_editNum->setPosition(Vec2(240, 250));
	_editNum->setFontName("Paint Boy");
	_editNum->setFontSize(25);
	_editNum->setFontColor(Color3B::RED);
	_editNum->setPlaceHolder("Score or Time :");
	_editNum->setPlaceholderFontColor(Color3B::WHITE);
	_editNum->setMaxLength(8);
	_editNum->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
	_editNum->setDelegate(this);
	this->addChild(_editNum);

	// 사운드

	AudioEngine::preload(BGM_1_PATH);
	AudioEngine::preload(BGM_2_PATH);

	if (AudioEngine::getState(audioId1) != AudioEngine::AudioState::PLAYING)
		audioId1 = AudioEngine::play2d(BGM_1_PATH, true);


	if (AudioEngine::getState(audioId2) != AudioEngine::AudioState::PLAYING)
		audioId2 = AudioEngine::play2d(BGM_2_PATH, true);


	return true;
}

void GameMain::editBoxEditingDidBegin(cocos2d::ui::EditBox* editBox)
{
}

void GameMain::editBoxEditingDidEnd(cocos2d::ui::EditBox* editBox)
{
}

void GameMain::editBoxTextChanged(cocos2d::ui::EditBox* editBox, const std::string& text)
{
	txtNum = text;
}

void GameMain::editBoxReturn(ui::EditBox* editBox)
{
}


void GameMain::doSendScore(Ref* pSender)
{
	log("%s", txtNum.c_str());
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	callJavaMethod("SendScore", txtNum);
#endif
}

void GameMain::doSendTime(Ref* pSender)
{
	log("%s", txtNum.c_str());
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	callJavaMethod("SendTime", txtNum);
#endif
}

void GameMain::doSendOne(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	callJavaMethod("SendOne", "");
#endif
}

void GameMain::doSendMulti(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	callJavaMethod("SendMulti", "");
#endif
}

void GameMain::doShowLeaderBoard(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	callJavaMethod("ShowLeaderBoard", "");
#endif
}

void GameMain::doShowAchivement(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	callJavaMethod("ShowAchivement", "");
#endif
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
	Rect rect2 = effect->getBoundingBox();
	Rect rect3 = bgm->getBoundingBox();

	if (rect.containsPoint(touchPoint) && sselect == false)
	{
		if (effectBool)
		{
			if (AudioEngine::getState(audioId2) != AudioEngine::AudioState::PLAYING)
				audioId2 = AudioEngine::play2d(BGM_2_PATH, true);
		}
		auto playMove = MoveBy::create(0.1f, Vec2(0, -10));
		play->runAction(playMove);	
		
	}

	else if (rect2.containsPoint(touchPoint))
	{
		effect->setOpacity(50);

		if (effectBool)
		{
			if (AudioEngine::getState(audioId2) != AudioEngine::AudioState::PLAYING)
				audioId2 = AudioEngine::play2d(BGM_2_PATH, true);
		}
	}

	else if (rect3.containsPoint(touchPoint))
	{
		bgm->setOpacity(50);

		if (effectBool)
		{
			if (AudioEngine::getState(audioId2) != AudioEngine::AudioState::PLAYING)
				audioId2 = AudioEngine::play2d(BGM_2_PATH, true);
		}
	}

	if (sselect)
	{
		if (effectBool)
		{
			if (AudioEngine::getState(audioId2) != AudioEngine::AudioState::PLAYING)
				audioId2 = AudioEngine::play2d(BGM_2_PATH, true);
		}

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
	}

	return true;
}

void GameMain::onTouchEnded(Touch* touch, Event* event)
{
	auto touchPoint = touch->getLocation();

	Rect rect = play->getBoundingBox();
	Rect rect2 = score->getBoundingBox();
	Rect rect3 = effect->getBoundingBox();
	Rect rect4 = bgm->getBoundingBox();
	

	if (rect.containsPoint(touchPoint) && sselect == false)
	{
		auto playMove = MoveBy::create(0.1f, Vec2(0, 10));
		play->runAction(playMove);

		sselect = true;
		this->selectStage();
	}
	else if (rect3.containsPoint(touchPoint))
	{
		if (effectBool)
		{
			effectBool = false;
			//removeChild(effect);	

			//effect = Sprite::create("Images/sound_off.png");
			//effect->setPosition(Vec2(winSize.width * 0.9, winSize.height * 0.9));
			//this->addChild(effect, 2);

		}
		else if (effectBool == false)
		{
			effectBool = true;
			//removeChild(effect);

		//	effect = Sprite::create("Images/sound_on.png");
		//	effect->setPosition(Vec2(winSize.width * 0.9, winSize.height * 0.9));
		//	this->addChild(effect, 2);
		}
	}
	else if (rect4.containsPoint(touchPoint))
	{
		if (bgmBool)
		{
			if (AudioEngine::getState(audioId1) == AudioEngine::AudioState::PLAYING)
				AudioEngine::pause(audioId1);
			bgmBool = false;

			bgm = Sprite::create("Images/bgm_off.png");
			bgm->setPosition(Vec2(winSize.width * 0.75, winSize.height * 0.9));
			this->addChild(bgm, 2);
		}
		else if (bgmBool == false)
		{
			if (AudioEngine::getState(audioId1) != AudioEngine::AudioState::PLAYING)
				audioId1 = AudioEngine::play2d(BGM_1_PATH, true);
			bgmBool = true;

			bgm = Sprite::create("Images/bgm_on.png");
			bgm->setPosition(Vec2(winSize.width * 0.75, winSize.height * 0.9));
			this->addChild(bgm, 2);
		}
	}
	else if (sselect)
	{
		if (effectBool)
		{
			if (AudioEngine::getState(audioId2) != AudioEngine::AudioState::PLAYING)
				audioId2 = AudioEngine::play2d(BGM_2_PATH, true);
		}

		Rect stage1 = stage[0]->getBoundingBox();
		Rect stage2 = stage[1]->getBoundingBox();
		Rect stage3 = stage[2]->getBoundingBox();
		Rect stage4 = stage[3]->getBoundingBox();
		Rect stage5 = stage[4]->getBoundingBox();
		Rect stage6 = stage[5]->getBoundingBox();
		Rect stage7 = stage[6]->getBoundingBox();
		Rect stage8 = stage[7]->getBoundingBox();
		Rect stage9 = stage[8]->getBoundingBox();

		if (stage1.containsPoint(touchPoint))
		{
			auto pScene = Stage1::createScene();
			Director::getInstance()->replaceScene(TransitionZoomFlipAngular::create(1, pScene, TransitionScene::Orientation::RIGHT_OVER));
			log("Stage1");
		}
		else if (stage2.containsPoint(touchPoint))
		{
			auto pScene = Stage2::createScene();
			Director::getInstance()->replaceScene(TransitionZoomFlipAngular::create(1, pScene, TransitionScene::Orientation::RIGHT_OVER));
			log("Stage2");
		}
		else if (stage3.containsPoint(touchPoint))
		{
			auto pScene = Stage3::createScene();
			Director::getInstance()->replaceScene(TransitionZoomFlipAngular::create(1, pScene, TransitionScene::Orientation::RIGHT_OVER));
			log("Stage3");
		}
		else if (stage4.containsPoint(touchPoint))
		{
			log("Stage4");
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