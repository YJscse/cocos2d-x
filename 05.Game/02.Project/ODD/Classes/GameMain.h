#pragma once
#ifndef __GAMEMAIN__
#define __GAMEMAIN__

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)             // 한글나오게 하는 헤더
#pragma execution_character_set("utf-8")
#endif

#include "cocos2d.h"
#include "extensions/cocos-ext.h" 
// 프로젝트 -> NULL 속성 -> C/C++ -> 일반 -> 추가 포함 디렉터리-> $(EngineRoot) 추가 하시오

using namespace cocos2d;
using namespace cocos2d::extension;

class GameMain 
	: public cocos2d::Layer
	, public cocos2d::ui::EditBoxDelegate
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	CREATE_FUNC(GameMain);

	Size winSize;

	Sprite* title;
	Sprite* play;
	Sprite* score;
	Sprite* background;
	Sprite* home;
	Sprite* soundBack;
	Sprite* effect;
	Sprite* bgm;
	Sprite* sound;
	Sprite* stageBord;
	Sprite* stage[9];
	Sprite* stage_clear[9];
	Sprite* soundBord;
	Sprite* effect_bgm;
	Sprite* effect_on;
	Sprite* effect_off;
	Sprite* bgm_on;
	Sprite* bgm_off;

	bool sselect = false;
	bool soundSetting = false;

	virtual void onEnter();
	virtual void onExit();
	virtual bool onTouchBegan(Touch* touch, Event* event);
	virtual void onTouchEnded(Touch* touch, Event* event);

	void createBackground();
	void createTitle();
	void createPlay();
	void createMenu();
	void selectStage();
	void createSound();
	void soundOnoff();

	// 리더보드
	cocos2d::ui::EditBox* _editNum;
	std::string txtNum;

	void doSendScore(Ref* pSender);
	void doSendTime(Ref* pSender);

	void doSendOne(Ref* pSender);
	void doSendMulti(Ref* pSender);

	void doShowLeaderBoard(Ref* pSender);
	void doShowAchivement(Ref* pSender);

	void editBoxEditingDidBegin(cocos2d::ui::EditBox* editBox);
	void editBoxEditingDidEnd(cocos2d::ui::EditBox* editBox);
	void editBoxTextChanged(cocos2d::ui::EditBox* editBox, const std::string& text);
	void editBoxReturn(cocos2d::ui::EditBox* editBox);

	// 사운드
	float _soundVolume;

	int audioId1;
	int audioId2;


};

#endif  //  __GAMEMAIN__