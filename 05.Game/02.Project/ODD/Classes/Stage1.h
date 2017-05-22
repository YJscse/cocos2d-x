#pragma once
#ifndef __STAGE1__
#define __STAGE1__

#include "cocos2d.h"

using namespace cocos2d;

class Stage1 : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	CREATE_FUNC(Stage1);

	Size winSize;

	Sprite* title;
	Sprite* play;
	Sprite* score;
	Sprite* setting;
	Sprite* background;

	//virtual void onEnter();
	//virtual void onExit();
	//virtual bool onTouchBegan(Touch* touch, Event* event);
	//virtual void onTouchEnded(Touch* touch, Event* event);

	//void SceneTrans(float dt);
	
};
#endif __STAGE1__ // __SELECT_STAGE__
