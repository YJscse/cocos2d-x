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

	SpriteFrameCache* cache;
	Size              winSize;
	TMXTiledMap*      tmap;
	TMXLayer*         background;
	TMXLayer*         metainfo;
	Label*            Manual;
	Sprite*           Hero;
	Sprite*           Sword;
	Sprite*           Wand;
	Sprite*           HolySword;
	Sprite*           Key;
	Sprite*           Wolf;
	Sprite*           Jelly;
	Sprite*           Demon;
	Sprite*           Door;

	Sprite*           boxSword;
	Sprite*           boxWand;
	Sprite*           boxHolySword;
	Sprite*           boxKey;

	Vec2		      HeroPosition;
	Vec2              SwordPosition;
	Vec2              WandPosition;
	Vec2              HolySwordPosition;
	Vec2              KeyPosition;
	Vec2              WolfPosition;
	Vec2              JellyPosition;
	Vec2              DemonPosition;
	Vec2              DoorPosition;

	MoveBy*           pMove;
	Vector<SpriteFrame*> HeroFrames;
	Vector<SpriteFrame*> MonsterFrames;
	Vector<SpriteFrame*> JellyFrames;
	Vector<SpriteFrame*> DemonFrames;
	Vector<SpriteFrame*> DoorFrames;
	ParticleSystem *emitter;
	
	virtual void onEnter();
	virtual void onExit();
	virtual bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
	virtual void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);

	void createHero();
	void createWeapons();
	void createWolf();
	void createJelly();
	void createDemon();
	void createDoor();
	void setViewpointCenter(Vec2 position);
	Vec2 tileCoordForPosition(Vec2 position);
	void setHeroPosition(Vec2 position);
	void heroMove(int num);
	void Reset(Ref *pSender);
	void LabelMove(float f);

	int swordcount;
	int wandcount;
	int holyswordcount;
	int keycount;
	
};

#endif // __HELLOWORLD_SCENE_H__
