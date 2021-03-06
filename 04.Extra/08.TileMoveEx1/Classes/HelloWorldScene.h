﻿#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__


#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)             // 한글나오게 하는 헤더
#pragma execution_character_set("utf-8")
#endif

#include "cocos2d.h"

class HelloWorld : public cocos2d::LayerColor
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
        
    CREATE_FUNC(HelloWorld);

	cocos2d::Size         winSize;
	cocos2d::Vec2         dragonPosition;
	cocos2d::Sprite*      dragon;
	cocos2d::TMXTiledMap* tmap;
	cocos2d::TMXLayer*    background;
	cocos2d::TMXLayer*    items;
	cocos2d::TMXLayer*    metainfo;

	virtual void onEnter();
	virtual void onExit();
	virtual bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
	virtual void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);

	void setViewpointCenter(cocos2d::Vec2 position);
	cocos2d::Vec2 tileCoordForPosition(cocos2d::Vec2 position);
	void setPlayerPosition(cocos2d::Vec2 position);
};

#endif // __HELLOWORLD_SCENE_H__
