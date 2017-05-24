#pragma once
#ifndef __GAMEMAIN__
#define __GAMEMAIN__

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)             // 한글나오게 하는 헤더
#pragma execution_character_set("utf-8")
#endif

#include "cocos2d.h"

using namespace cocos2d;


class GameMain : public cocos2d::Layer
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
	Sprite* sound;
	Sprite* bgm;
	Sprite* home;

	Sprite* stageBord;
	Sprite* stage[9];

	bool sselect = false;

	virtual void onEnter();
	virtual void onExit();
	virtual bool onTouchBegan(Touch* touch, Event* event);
	virtual void onTouchEnded(Touch* touch, Event* event);

	//void SceneTrans(float dt);
	void createBackground();
	void createTitle();
	void createPlay();
	void createScore();
	void selectStage();

};

#endif  __GAMEMAIN__ //  __GAMEMAIN__