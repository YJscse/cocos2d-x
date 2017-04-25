#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__


#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)             // 한글나오게 하는 헤더
#pragma execution_character_set("utf-8")
#endif

#include "cocos2d.h"

class HelloWorld : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
        
    CREATE_FUNC(HelloWorld);

	cocos2d::Sprite* Digda1;
	cocos2d::Sprite* Digda2;
	cocos2d::Sprite* Digda3;

	//void ActionRepeatForever(Ref* pSender);
	void Digda1Action(float f);
	void Digda2Action(float f);
	void Digda3Action(float f);
	void tick(Ref* pSender);

	int dig1;
	int dig2;
	int dig3;
	
};

#endif // __HELLOWORLD_SCENE_H__
