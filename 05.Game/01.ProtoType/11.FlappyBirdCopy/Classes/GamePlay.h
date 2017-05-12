#pragma once
#ifndef __GAMEPLAY__
#define __GAMEPLAY__

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)             // 한글나오게 하는 헤더
#pragma execution_character_set("utf-8")
#endif

#include "cocos2d.h"
#include "Box2D/Box2D.h"
#include <GLES-Render.h>

#define PTM_RATIO 32

using namespace cocos2d;

class GamePlay : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	CREATE_FUNC(GamePlay);

	bool createBox2dWorld(bool debug);
	~GamePlay();
	virtual void draw(Renderer* renderer, const Mat4& transform, 
					  uint32_t flags) override;
	b2Body* addNewSprite(Vec2 point, Size size,
						 b2BodyType bodytype, 
						 const char* spriteName, 
						 int type);
	b2Body* getBodyAtTab(Vec2 p);

	Size winSize;
	Texture2D* texture;
	b2World* _world;
	GLESDebugDraw* m_debugDraw;

	bool playerIsFlying;
	bool bDrag;
	bool bBallTouch;
	b2Body* dragBody;
	b2Body* myBird;
	b2MouseJoint* mouseJoint;
	b2Body* gbody;
	float playerVelocity;

	virtual void onEnter();
//	virtual void onExit();
	virtual bool onTouchBegan(Touch* touch, Event* event);
	virtual void onTouchEnded(Touch* touch, Event* event);

//	void SceneTrans(float dt);
	void createBird();
	void createGround();
	Sprite* ready;
	Sprite* howto;

protected:
	void onDraw(const Mat4& transform, uint32_t flags);

	CustomCommand _customCmd;
};
#endif // __GAMEPLAY__
