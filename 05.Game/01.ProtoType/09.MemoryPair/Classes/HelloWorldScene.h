#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__


#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)             // 한글나오게 하는 헤더
#pragma execution_character_set("utf-8")
#endif

#include "cocos2d.h"

USING_NS_CC;

using namespace cocos2d;

class HelloWorld : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
        
    CREATE_FUNC(HelloWorld);

	Sprite* cover[20];
	Sprite* sprCard[20];
	Sprite* obj;

	Vec2 pos[20];
	Vector<Sprite*> pair;
	Vector<Sprite*> card1;
	Vector<Sprite*> card2;

	void onEnter();
	void onExit();

	bool onTouchBegan(Touch *touch, Event *event);
	void onTouchEnded(Touch *touch, Event *event);
	//void Discover(int num);
	void good(float f);
	void bad(float fa);

	int count;
	int pair1;
	int pair2;
	int over;
	int num1;
	int num2;
};

#endif // __HELLOWORLD_SCENE_H__
