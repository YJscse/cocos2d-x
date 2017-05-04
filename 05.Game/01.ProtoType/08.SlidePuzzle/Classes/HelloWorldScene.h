#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#pragma execution_character_set("utf-8")
#endif

#include "cocos2d.h"

USING_NS_CC;

class HelloWorld : public Layer
{
public:
	static Scene* createScene();

	virtual bool init();

	CREATE_FUNC(HelloWorld);

	char str[10] = { 0 };

	void onEnter();
	void onExit();
	void Change(int num);
	void Clear();

	cocos2d::Vec2 ClearPos0;
	cocos2d::Vec2 ClearPos1;
	cocos2d::Vec2 ClearPos2;
	cocos2d::Vec2 ClearPos3;
	cocos2d::Vec2 ClearPos4;
	cocos2d::Vec2 ClearPos5;
	cocos2d::Vec2 ClearPos6;
	cocos2d::Vec2 ClearPos7;

	Vector<Sprite*> Puzzle;

	bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event);
};

#endif // __HELLOWORLD_SCENE_H__
