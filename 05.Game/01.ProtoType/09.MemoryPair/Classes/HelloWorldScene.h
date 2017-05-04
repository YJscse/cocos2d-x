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

	Sprite* sprite1;
	Sprite* pair1;
	Sprite* pair2;
	Sprite* pair3;
	Sprite* pair4;
	Sprite* pair5;
	Sprite* pair6;
	Sprite* pair7;
	Sprite* pair8;
	Sprite* pair9;
	Sprite* pair10;
	Sprite* pair11;
	Sprite* pair12;
	Sprite* pair13;
	Sprite* pair14;
	Sprite* pair15;
	Sprite* pair16;
	Sprite* pair17;
	Sprite* pair18;
	Sprite* pair19;
	Sprite* pair20;
	Sprite * obj;

	Vec2 touchPoint;
	Vector<Sprite*> pair;
	Vector<Sprite*> card1;
	Vector<Sprite*> card2;

	void onEnter();
	void onExit();

	bool onTouchBegan(Touch *touch, Event *event);
	void onTouchEnded(Touch *touch, Event *event);
	void Discover(int num);
//	void createPair();
	//void Clear();

	int count;
	
};

#endif // __HELLOWORLD_SCENE_H__
