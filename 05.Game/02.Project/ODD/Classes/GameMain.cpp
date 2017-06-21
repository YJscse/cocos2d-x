#include "GameMain.h"
#include "HelloWorldScene.h"
#include "Stage1.h"
#include "Stage2.h"
#include "Stage3.h"
#include "AudioEngine.h"
#include "soundManager.h"

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

void callJavaMethod1(std::string func)
{
	JniMethodInfo t;

	/**
	JniHelper를 통해 org/cocos2dx/cpp/에 있는
	AppActivity class의 파라미터로 들어온 스트링 이름의 함수 정보를 가져온다.
	*/
	if (JniHelper::getStaticMethodInfo(t
		, "org/cocos2dx/cpp/AppActivity"
		, func.c_str()
		, "()V"))
	{
		// 함수 호출
		t.env->CallStaticVoidMethod(t.classID, t.methodID);
		// Release
		t.env->DeleteLocalRef(t.classID);
	}

}
#endif

using namespace experimental;

const std::string BGM_1_PATH = "sounds/Game_Main_BGM.mp3";

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

	this->createBackground();
	this->createTitle();
	this->createPlay();
	this->createSound();

	auto wlayer = LayerColor::create(Color4B(255, 255, 255, 255));
	wlayer->setOpacity(0);
	this->addChild(wlayer);

	this->createMenu();
	//--------------------------------------------------------------
	//Size editBoxSize = Size(100, 60);
	//std::string pNormalSprite = "Images/go.png";
	//_editNum = ui::EditBox::create(editBoxSize,
	//	ui::Scale9Sprite::create(pNormalSprite));
	//_editNum->setPosition(Vec2(240, 250));
	//_editNum->setFontName("Paint Boy");
	//_editNum->setFontSize(25);
	//_editNum->setFontColor(Color3B::RED);
	//_editNum->setPlaceHolder("Score or Time :");
	//_editNum->setPlaceholderFontColor(Color3B::WHITE);
	//_editNum->setMaxLength(8);
	//_editNum->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
	//_editNum->setDelegate(this);
	//this->addChild(_editNum);

	// 사운드

	bool b = UserDefault::getInstance()->getBoolForKey("bgm");

	if (b)
	{
		if (AudioEngine::getState(audioId1) != AudioEngine::AudioState::PLAYING)
			audioId1 = AudioEngine::play2d(BGM_1_PATH, true);

		UserDefault::getInstance()->setBoolForKey("bgm", true);
	}
	else
	{
		if (AudioEngine::getState(audioId1) == AudioEngine::AudioState::PLAYING)
			AudioEngine::pause(audioId1);
	}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	callJavaMethod1("ShowAdPopup");
#else
//	ShowAdmobAds();
#endif
		
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
	title->setPosition(Vec2(winSize.width / 2, winSize.height * 3 / 4));
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
	play->setScale(1.2f);
	this->addChild(play);

	auto move = Place::create(Vec2(winSize.width / 2, winSize.height / 2));
	auto seq1 = Sequence::create(DelayTime::create(3.0f), move, nullptr);

	auto fadeOut = FadeOut::create(1.5f);
	auto fadeIn = FadeIn::create(0.5f);

	auto seq = Sequence::create(fadeOut, fadeIn, nullptr);
	auto rep = RepeatForever::create(seq);

	play->runAction(seq1);
	play->runAction(rep);

}

void GameMain::createMenu()
{
	auto score = MenuItemImage::create(
		"Images/score.png",
		"Images/score_click.png",
		CC_CALLBACK_1(GameMain::doShowLeaderBoard, this));
	score->setPosition(Vec2(winSize.width * 0.25, winSize.height * 0.35));
	score->setScale(0.5f);
	score->setOpacity(0);

	auto scoreMenu = Menu::create(score, nullptr);
	scoreMenu->setPosition(Vec2::ZERO);

	this->addChild(scoreMenu);

	auto move = FadeIn::create(0.5f);
	auto seq = Sequence::create(DelayTime::create(3.0f), move, nullptr);

	score->runAction(seq);

	auto mission = MenuItemImage::create(
		"Images/mission.png",
		"Images/mission_click.png",
		CC_CALLBACK_1(GameMain::doShowAchivement, this));
	mission->setScale(0.5f);
	mission->setPosition(Vec2(winSize.width * 0.5, winSize.height * 0.25));
	mission->setOpacity(0);

	auto missionMenu = Menu::create(mission, nullptr);
	missionMenu->setPosition(Vec2::ZERO);

	this->addChild(missionMenu);

	auto move2 = FadeIn::create(0.5f);
	auto seq2 = Sequence::create(DelayTime::create(3.0f), move2, nullptr);

	mission->runAction(seq2);

}

void GameMain::createSound()
{
	sound = Sprite::create("Images/sound.png");
	sound->setPosition(Vec2(winSize.width * 0.75, winSize.height * 0.35));
	sound->setScale(0.5f);
	sound->setOpacity(0);
	this->addChild(sound);

	auto move = FadeIn::create(0.5f);
	auto seq = Sequence::create(DelayTime::create(3.0f), move, nullptr);

	sound->runAction(seq);

}

void GameMain::soundOnoff()
{
	soundBord = Sprite::create("Images/sound_bord.png");
	soundBord->setPosition(Vec2(winSize.width * 0.5, winSize.height * 0.5));
	this->addChild(soundBord);

	effect_bgm = Sprite::create("Images/bgm_effect.png");
	effect_bgm->setAnchorPoint(Vec2(0, 0.5));
	effect_bgm->setPosition(Vec2(20, 75));
	soundBord->addChild(effect_bgm);

	effect_on = Sprite::create("Images/sound_on.png");
	effect_on->setPosition(Vec2(winSize.width * 0.55, winSize.height * 0.48));
	this->addChild(effect_on);

	effect_off = Sprite::create("Images/sound_off.png");
	effect_off->setPosition(Vec2(winSize.width * 0.7, winSize.height * 0.482));
	this->addChild(effect_off);

	bgm_on = Sprite::create("Images/sound_on.png");
	bgm_on->setPosition(Vec2(winSize.width * 0.55, winSize.height * 0.52));
	this->addChild(bgm_on);

	bgm_off = Sprite::create("Images/sound_off.png");
	bgm_off->setPosition(Vec2(winSize.width * 0.7, winSize.height * 0.522));
	this->addChild(bgm_off);

	soundBack = Sprite::create("Images/back.png");
	soundBack->setScale(1.5f);
	soundBack->setPosition(Vec2(winSize.width * 0.5, winSize.height * 0.4));
	this->addChild(soundBack);
}

void GameMain::selectStage()
{
	stageBord = Sprite::create("Images/stage_bord.png");
	stageBord->setPosition(Vec2(winSize.width / 2, winSize.height / 2));
	this->addChild(stageBord);

	auto stageSelect = Sprite::create("stageImages3/select.png");
	stageSelect->setPosition(Vec2(200, 350));
	stageSelect->setScale(0.6f);
	stageBord->addChild(stageSelect);

	auto move = MoveBy::create(2, Vec2(0, 20));
	auto back = move->reverse();
	auto seq = Sequence::create(move, back, nullptr);
	auto rep = RepeatForever::create(seq);
	stageSelect->runAction(rep);

	int g = UserDefault::getInstance()->getIntegerForKey("stage");

	for (int j = 0; j < 9; j++)
	{
		int column = j % 3 + 1;
		int row = j / 3;
		char str[50] = { 0 };
		sprintf(str, "stageImages3/stage_clear%d.png", j + 1);
		stage_clear[j] = Sprite::create(str);
		stage_clear[j]->setPosition(Vec2(winSize.width * 2 / 9 + (column * 100), winSize.height * 4 / 7 - (row * 100)));
		stage_clear[j]->setVisible(false);
		this->addChild(stage_clear[j]);
	}

	for (int j = 0; j < 9; j++)
	{
		int column = j % 3 + 1;
		int row = j / 3;
		char str[50] = { 0 };
		sprintf(str, "stageImages3/stage%d.png", j + 1);
		stage[j] = Sprite::create(str);
		stage[j]->setPosition(Vec2(winSize.width * 2 / 9 + (column * 100), winSize.height * 4 / 7 - (row * 100)));
		this->addChild(stage[j]);
	}

	stage[0]->setVisible(false);
	stage_clear[0]->setVisible(true);

	if (g > 2)
	{
		for (int i = 0; i < 2; i++)
		{
			stage[i]->setVisible(false);
			stage_clear[i]->setVisible(true);
		}
	}
	else if (g > 3)
	{
		for (int i = 0; i < 3; i++)
		{
			stage[i]->setVisible(false);
			stage_clear[i]->setVisible(true);
		}
	}

	home = Sprite::create("Images/back.png");
	home->setPosition(Vec2(winSize.width * 0.5, winSize.height * 0.3));
	home->setScale(1.5);
	this->addChild(home);
}

bool GameMain::onTouchBegan(Touch* touch, Event* event)
{
	auto touchPoint = touch->getLocation();

	Rect rect = play->getBoundingBox();
	Rect rect2 = sound->getBoundingBox();

	if (rect.containsPoint(touchPoint) && sselect == false && soundSetting == false)
	{
		play->setOpacity(50);
	}
	else if (rect2.containsPoint(touchPoint) && sselect == false && soundSetting == false)
	{
		sound->setScale(0.3f);
	}
	else if (sselect && soundSetting == false)
	{
		int g = UserDefault::getInstance()->getIntegerForKey("stage");

		Rect homeButton = home->getBoundingBox();

		Rect stage1 = stage[0]->getBoundingBox();
		Rect stage2 = stage[1]->getBoundingBox();
		Rect stage3 = stage[2]->getBoundingBox();
		Rect stage4 = stage[3]->getBoundingBox();
		Rect stage5 = stage[4]->getBoundingBox();
		Rect stage6 = stage[5]->getBoundingBox();
		Rect stage7 = stage[6]->getBoundingBox();
		Rect stage8 = stage[7]->getBoundingBox();
		Rect stage9 = stage[8]->getBoundingBox();

		Rect stage_claer1 = stage_clear[0]->getBoundingBox();
		Rect stage_claer2 = stage_clear[1]->getBoundingBox();
		Rect stage_claer3 = stage_clear[2]->getBoundingBox();
		Rect stage_claer4 = stage_clear[3]->getBoundingBox();
		Rect stage_claer5 = stage_clear[4]->getBoundingBox();
		Rect stage_claer6 = stage_clear[5]->getBoundingBox();
		Rect stage_claer7 = stage_clear[6]->getBoundingBox();
		Rect stage_claer8 = stage_clear[7]->getBoundingBox();
		Rect stage_claer9 = stage_clear[8]->getBoundingBox();

		if (stage1.containsPoint(touchPoint))
		{
			stage[0]->setOpacity(50);
			stage_clear[0]->setOpacity(0);
			log("Stage1 began");	
		}
		else if (stage2.containsPoint(touchPoint))
		{
			stage[1]->setOpacity(50);
			stage_clear[1]->setOpacity(0);
			log("Stage2 began");
		}
		else if (stage3.containsPoint(touchPoint))
		{
			stage[2]->setOpacity(50);
			stage_clear[2]->setOpacity(0);
			log("Stage3 began");
		}
		else if (stage4.containsPoint(touchPoint))
		{
			stage[3]->setOpacity(50);
			stage_clear[3]->setOpacity(0);
			log("Stage4 began");
		}
		else if (stage5.containsPoint(touchPoint))
		{
			stage[4]->setOpacity(50);
			log("stage5 began");
		}
		else if (stage6.containsPoint(touchPoint))
		{
			stage[5]->setOpacity(50);
			log("stage6 began");
		}
		else if (stage7.containsPoint(touchPoint))
		{
			stage[6]->setOpacity(50);
			log("stage7 began");
		}
		else if (stage8.containsPoint(touchPoint))
		{
			stage[7]->setOpacity(50);
			log("stage8 began");
		}
		else if (stage9.containsPoint(touchPoint))
		{
			stage[8]->setOpacity(50);
			log("stage9 began");
		}
		else if (homeButton.containsPoint(touchPoint))
		{
			home->setOpacity(50);
		}

	}

	else if (soundSetting && sselect == false)
	{
		Rect backButton = soundBack->getBoundingBox();
		Rect eOn = effect_on->getBoundingBox();
		Rect eOff = effect_off->getBoundingBox();
		Rect bOn = bgm_on->getBoundingBox();
		Rect bOff = bgm_off->getBoundingBox();

		if (backButton.containsPoint(touchPoint))
		{
			soundBack->setOpacity(50);
		}
		else if (eOn.containsPoint(touchPoint))
		{
			UserDefault::getInstance()->setBoolForKey("effect", true);

			effect_on->setOpacity(50);
		}
		else if (eOff.containsPoint(touchPoint))
		{
			UserDefault::getInstance()->setBoolForKey("effect", false);

			effect_off->setOpacity(50);
		}
		else if (bOn.containsPoint(touchPoint))
		{
			if (AudioEngine::getState(audioId1) != AudioEngine::AudioState::PLAYING)
				audioId1 = AudioEngine::play2d(BGM_1_PATH, true);

			UserDefault::getInstance()->setBoolForKey("bgm", true);
			bgm_on->setOpacity(50);
		}
		else if (bOff.containsPoint(touchPoint))
		{
			if (AudioEngine::getState(audioId1) == AudioEngine::AudioState::PLAYING)
				AudioEngine::stop(audioId1);

			UserDefault::getInstance()->setBoolForKey("bgm", false);
			bgm_off->setOpacity(50);
		}
		
	}

	return true;
}

void GameMain::onTouchEnded(Touch* touch, Event* event)
{
	auto touchPoint = touch->getLocation();

	Rect rect = play->getBoundingBox();
	Rect rect2 = sound->getBoundingBox();

	if (rect.containsPoint(touchPoint) && sselect == false && soundSetting == false)
	{
		sselect = true;
		this->selectStage();
	}
	else if (rect2.containsPoint(touchPoint) && sselect == false && soundSetting == false)
	{
		sound->setScale(0.5f);

		soundSetting = true;
		this->soundOnoff();
	}

	else if (sselect && soundSetting == false)
	{
		int g = UserDefault::getInstance()->getIntegerForKey("stage");

		Rect homeButton = home->getBoundingBox();

		Rect stage1 = stage[0]->getBoundingBox();
		Rect stage2 = stage[1]->getBoundingBox();
		Rect stage3 = stage[2]->getBoundingBox();
		Rect stage4 = stage[3]->getBoundingBox();
		Rect stage5 = stage[4]->getBoundingBox();
		Rect stage6 = stage[5]->getBoundingBox();
		Rect stage7 = stage[6]->getBoundingBox();
		Rect stage8 = stage[7]->getBoundingBox();
		Rect stage9 = stage[8]->getBoundingBox();

		Rect stage_claer1 = stage_clear[0]->getBoundingBox();
		Rect stage_claer2 = stage_clear[1]->getBoundingBox();
		Rect stage_claer3 = stage_clear[2]->getBoundingBox();
		Rect stage_claer4 = stage_clear[3]->getBoundingBox();
		Rect stage_claer5 = stage_clear[4]->getBoundingBox();
		Rect stage_claer6 = stage_clear[5]->getBoundingBox();
		Rect stage_claer7 = stage_clear[6]->getBoundingBox();
		Rect stage_claer8 = stage_clear[7]->getBoundingBox();
		Rect stage_claer9 = stage_clear[8]->getBoundingBox();

		if (stage1.containsPoint(touchPoint))
		{
			auto action = FadeIn::create(0.01f);
			stage[0]->runAction(action);
			stage_clear[0]->runAction(action->clone());

			auto pScene = Stage1::createScene();
			Director::getInstance()->replaceScene(TransitionFade::create(0.5f, pScene));
			
			
		}
		else if (stage2.containsPoint(touchPoint))
		{
			auto action = FadeIn::create(0.01f);
			stage[1]->runAction(action);
			stage_clear[1]->runAction(action->clone());

			if (g > 1)
			{
				auto pScene = Stage2::createScene();
				Director::getInstance()->replaceScene(TransitionFade::create(0.5f, pScene));
			
			}
		}
		else if (stage3.containsPoint(touchPoint))
		{
			auto action = FadeIn::create(0.01f);
			stage[2]->runAction(action);
			stage_clear[2]->runAction(action->clone());

			if (g > 2)
			{
				auto pScene = Stage3::createScene();
				Director::getInstance()->replaceScene(TransitionFade::create(0.5f, pScene));
			}			
		}
		else if (stage4.containsPoint(touchPoint))
		{
			auto action = FadeIn::create(0.01f);
			stage[3]->runAction(action);	
		}
		else if (stage5.containsPoint(touchPoint))
		{
			auto action = FadeIn::create(0.01f);
			stage[4]->runAction(action);		
		}
		else if (stage6.containsPoint(touchPoint))
		{
			auto action = FadeIn::create(0.01f);
			stage[5]->runAction(action);		
		}
		else if (stage7.containsPoint(touchPoint))
		{
			auto action = FadeIn::create(0.01f);
			stage[6]->runAction(action);
		}
		else if (stage8.containsPoint(touchPoint))
		{
			auto action = FadeIn::create(0.01f);
			stage[7]->runAction(action);	
		}
		else if (stage9.containsPoint(touchPoint))
		{
			auto action = FadeIn::create(0.01f);
			stage[8]->runAction(action);
		}
		else if (homeButton.containsPoint(touchPoint))
		{
			auto action = FadeIn::create(0.01f);
			home->runAction(action);

			removeChild(stageBord);
			for (int i = 0; i < 9; i++)
			{
				removeChild(stage[i]);
			}
			for (int i = 0; i < 9; i++)
			{
				removeChild(stage_clear[i]);
			}
			removeChild(home);
			sselect = false;
		}
	}
	else if (soundSetting && sselect == false)
	{
		auto action = FadeIn::create(0.01f);

		Rect backButton = soundBack->getBoundingBox();
		Rect eOn = effect_on->getBoundingBox();
		Rect eOff = effect_off->getBoundingBox();
		Rect bOn = bgm_on->getBoundingBox();
		Rect bOff = bgm_off->getBoundingBox();

		if (backButton.containsPoint(touchPoint))
		{
			soundBack->runAction(action);
			removeChild(soundBord);
			removeChild(effect_bgm);
			removeChild(effect_on);
			removeChild(effect_off);
			removeChild(bgm_on);
			removeChild(bgm_off);
			removeChild(soundBack);
			soundSetting = false;
		}
		else if (eOn.containsPoint(touchPoint))
		{
			effect_on->runAction(action);
		}
		else if (eOff.containsPoint(touchPoint))
		{
			effect_off->runAction(action);
		}
		else if (bOn.containsPoint(touchPoint))
		{
			bgm_on->runAction(action);
		}
		else if (bOff.containsPoint(touchPoint))
		{
			bgm_off->runAction(action);

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

	if (AudioEngine::getState(audioId1) == AudioEngine::AudioState::PLAYING)
		AudioEngine::stop(audioId1);

	#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
		callJavaMethod1("HideAdPopup");
	#else
		//	ShowAdmobAds();
	#endif

	Layer::onExit();
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

