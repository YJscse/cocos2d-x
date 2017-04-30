#pragma once
#ifndef __SceneTrans__Stage3Scene__
#define __SceneTrans__Stage3Scene__

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)             // 한글나오게 하는 헤더
#pragma execution_character_set("utf-8")
#endif

#include "cocos2d.h"

USING_NS_CC;

class Stage3Scene : public cocos2d::LayerColor
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	CREATE_FUNC(Stage3Scene);

	SpriteFrameCache* cache2;
	Size              winSize;
	TMXTiledMap*      tmap;
	TMXLayer*         background3;
	TMXLayer*         metainfo1;
	TMXLayer*         metainfo2;
	TMXLayer*         metainfo3;

	Sprite*           Hero;
	Sprite*           Sword1;
	Sprite*           Sword2;
	Sprite*           Sword3;
	Sprite*           Sword4;
	Sprite*           Sword5;
	Sprite*           Sword6;

	Sprite*           Wand1;
	Sprite*           Wand2;
	Sprite*           Wand3;
	Sprite*           HolySword1;
	Sprite*           HolySword2;
	Sprite*           HolySword3;
	Sprite*           HolySword4;
	Sprite*           Key;
	Sprite*           Wolf1;
	Sprite*           Wolf2;
	Sprite*           Wolf3;
	Sprite*           Wolf4;
	Sprite*           Wolf5;
	Sprite*           Wolf6;
	Sprite*           Wolf7;
	Sprite*           Jelly1;
	Sprite*           Jelly2;
	Sprite*           Jelly3;
	Sprite*           Jelly4;
	Sprite*           Demon1;
	Sprite*           Demon2;
	Sprite*           Demon3;
	Sprite*           Door;
	Sprite*           boxSword;
	Sprite*           boxWand;
	Sprite*           boxHolySword;
	Sprite*           boxKey;

	Vec2		      HeroPosition;
	Vec2              SwordPosition1;
	Vec2              SwordPosition2;
	Vec2              SwordPosition3;
	Vec2              SwordPosition4;
	Vec2              SwordPosition5;
	Vec2              SwordPosition6;
	Vec2              WandPosition1;
	Vec2              WandPosition2;
	Vec2              WandPosition3;
	Vec2              HolySwordPosition1;
	Vec2              HolySwordPosition2;
	Vec2              HolySwordPosition3;
	Vec2              HolySwordPosition4;
	Vec2              KeyPosition;
	Vec2              WolfPosition1;
	Vec2              WolfPosition2;
	Vec2              WolfPosition3;
	Vec2              WolfPosition4;
	Vec2              WolfPosition5;
	Vec2              WolfPosition6;
	Vec2              WolfPosition7;
	Vec2              JellyPosition1;
	Vec2              JellyPosition2;
	Vec2              JellyPosition3;
	Vec2              JellyPosition4;
	Vec2              DemonPosition1;
	Vec2              DemonPosition2;
	Vec2              DemonPosition3;
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

	int swordcount;
	int wandcount;
	int holyswordcount;
	int keycount;
};

#endif // defined(__SceneTrans__Stage2Scene__)

#pragma once
