﻿#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__


#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)             // 한글나오게 하는 헤더
#pragma execution_character_set("utf-8")
#endif

#include "cocos2d.h"

USING_NS_CC;

class HelloWorld : public cocos2d::LayerColor
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
        
    CREATE_FUNC(HelloWorld);

	Size winSize;
	Sprite* player;
	Vector<Sprite*> projectiles;
	bool bProjectile;

	virtual bool onTouchBegan(Touch* touch, Event* event);
	void finishRotate(Ref* sender, Vec2);
	void spriteMoveFinished(Ref* sender);
};

#endif // __HELLOWORLD_SCENE_H__
