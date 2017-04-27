#ifndef __HELLOWORLD_SCENE_H__
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
	cocos2d::Sprite*      Sister;
	cocos2d::Vec2         SisterPosition;
	cocos2d::TMXTiledMap* tmap;
	cocos2d::TMXLayer*    background;
	cocos2d::TMXLayer*    Ground;

	void setSisterPosition(float f);
	void moveBackground(float f);
	void createSister();
	cocos2d::Vec2 tileCoordForPosition(cocos2d::Vec2 position);

	float xValue;
	int count;
	int num;
};

#endif // __HELLOWORLD_SCENE_H__
