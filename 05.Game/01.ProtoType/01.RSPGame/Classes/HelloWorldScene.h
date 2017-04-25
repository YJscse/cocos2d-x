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

	cocos2d::Sprite* Total;
	cocos2d::Sprite* Base;
	cocos2d::Sprite* CreateBase;
	cocos2d::Sprite* Title;
	cocos2d::Sprite* TitleLight;
	cocos2d::Sprite* MainLight;
	cocos2d::Sprite* Light1;
	cocos2d::Sprite* Light2;
	cocos2d::Sprite* Light3;
	cocos2d::Sprite* Light4;
	cocos2d::Sprite* Light5;
	cocos2d::Sprite* Fish1;
	cocos2d::Sprite* Fish2;
	cocos2d::Sprite* Bubble1;
	cocos2d::Sprite* Bubble2;
	cocos2d::Sprite* Bubble3;
	cocos2d::Sprite* Bubble4;
	cocos2d::Sprite* Star1;
	cocos2d::Sprite* Star2;
	cocos2d::Sprite* Star3;

	
	void doClick1(Ref* pSender);
	void doClick2(Ref* pSender);
	void doAction(Ref* pSender);
	void ActionRepeatForever(Ref* pSender);

};

#endif // __HELLOWORLD_SCENE_H__
